from streaming.base_context import BaseContext
from utils.twitter import TwitterConfiguration

from pyspark import SparkContext, SQLContext, StreamingContext
from pyspark.sql.functions import *
from pyspark.sql import LongType
import yaml
from datetime import datetime

import re
import nltk
from nltk.corpus import stopwords
from nltk.tokenize import word_tokenize

import tweepy

def normalize_datetime(input_time, input_format, output_format):
    date_object = datetime.strptime(input_time, input_format)
    return datetime.strftime(date_object, output_format)

def text_processing(text):
    processed_text = text.lower()

    # 1.remove emails
    simple_email_regex = r"([^.@\\s]+)(\\.[^.@\\s]+)*@([^.@\\s]+\\.)+([^.@\\s]+)"
    processed_text = re.sub(simple_email_regex, " ", processed_text)

    # 2.remove urls
    complex_URL_regex = r"""(?:(?:https?|ftp):\/\/|\b(?:[a-z\d]+\.))(?:(?:[^\s()<>]+|\((?:[^\s()<>]+|(?:\([^\s()<>]+\)))?\))+(?:\((?:[^\s()<>]+|(?:\(?:[^\s()<>]+\)))?\)|[^\s`!()\[\]{};:'".,<>?«»“”‘’]))?"""
    processed_text = re.sub(complex_URL_regex, " ", processed_text)

    # 3.remove mentions and hashtag
    simple_mention_hashtag_regex = r"""\s([@#][\w_-]+)"""
    processed_text = re.sub(simple_mention_hashtag_regex, " ", processed_text)

    # 4.remove non-alphabet characters (which are special character, emojis, punctuations, ...)
    non_alphabet_regex = r"[^A-Za-z0-9 ]"
    processed_text = re.sub(non_alphabet_regex, " ", processed_text)
    processed_text = re.sub(r" +", " ", processed_text)

    # 5.remove stopword
    nltk.download("stopwords")
    nltk.download("punkt")

    stop_words = set(stopwords.words('english')) 
    word_tokens = word_tokenize(processed_text)
    tmp_text = ""

    for w in word_tokens: 
        if w not in stop_words: 
            tmp_text + w + " "

    processed_text = tmp_text

    return processed_text


class MyStreamListener(tweepy.StreamListener):
    def __init__(self, api):
        self.api = api
        self.me = api.me()

    def on_status(self, tweet):
        # TODO: need processing
        print(tweet)
        return tweet

    def on_error(self, status):
        print("Error detected")


class Producer(BaseContext):
    def __init__(self, app_name):
        super(app_name)
        self.conf.set("spark.ui.port", "9050")
        self.conf.set("spark.executor.memory", "2g")

        self.sc = SparkContext(self.conf)
        self.sc.setLogLevel("ERROR")

        self.sql_context = SQLContext(self.sc)
        self.ssc = StreamingContext(self.sc,2)

class _TweetProducer(Producer):
    def __init__(self, app_name, file_core):
        super(app_name)
        self._file_core = file_core
        self._norm_time = udf(normalize_datetime, StringType())
        self._text_processing = udf(text_processing, StringType())

    def read_yaml_configuration(self):
        with open("./resources/coresetk.yaml") as f:
            configuration = yaml.load(f, Loader=yaml.FullLoader)
            self.twitter_config = TwitterConfiguration(configuration)

    def pre_processing(self, rdd):
        if rdd.head(1) != None:

            input_time_format = "MMM dd, yyyy h:mm:ss a"
            output_time_format = "yyyy-MM-dd HH:mm:ss"
            current_time = datetime.now().strftime(input_time_format)
            
            data_raw = rdd.withColumn("createdAt", when(col("createdAt").isNotNull(), col("createdAt")).otherwise(lit(current_time)))\
                .withColumn("tweetTime", self._norm_time(col("createdAt"),lit(input_time_format),lit(output_time_format)))\
                .withColumn("ymd", substring(col("tweetTime"), 0, 10))\
                .withColumn("tweetID", col("id").cast(LongType()))\
                .drop("id").drop("tweetTime")

    def further_etl(self, data, datatype, mode = "test"):
        consider_attribute = ["tweetID","text","createdAt","user.id","user.name","user.friendsCount","user.followersCount","user.favouritesCount"]
        final_consider_attribute = ["tweetID","text","createdAt","userID","userName","friends","followers","favourites"]

        input_time_format = "MMM dd, yyyy h:mm:ss a"
        output_time_format = "yyyy-MM-dd HH:mm:ss"

        store_data = data.select(consider_attribute).distinct()\
            .withColumn("preContent", self._text_processing(col("text")))\
            .withColumn("contentWords", split(col("preContent"), " "))

        store_data = store_data.withColumn("tweetTime", self._norm_time(col("createdAt"),lit(input_time_format),lit(output_time_format)))\
            .withColumn("timeStamp", unix_timestamp(col("tweetTime"), output_time_format))\
            .withColumn("userID", col("id").cast(LongType()))\
            .withColumn("userName", col("name"))

        if "friendsCount" in store_data.columns:
            store_data = store_data.withColumn("friends", col("friendsCount").cast(LongType()))
        else:
            store_data = store_data.withColumn("friendsCount", lit(""))
        if "followersCount" in store_data.columns:
            store_data = store_data.withColumn("followers", col("followersCount").cast(LongType()))
        else:
            store_data = store_data.withColumn("followersCount", lit(""))
        if "favouritesCount" in store_data.columns:
            store_data = store_data.withColumn("favourites", col("favouritesCount").cast(LongType()))
        else:
            store_data = store_data.withColumn("favouritesCount", lit(""))

        store_data = store_data.withColumn("ymd", substring(col("tweetTime"), 0, 10))\
            .select(final_consider_attribute)

        final_data = store_data.select(sorted(store_data.columns))
        final_data.show(2, False)

        return final_data
        

    def main_streaming(self):
        self.read_yaml_configuration()

        auth = tweepy.OAuthHandler(self.twitter_config.api_key,self.twitter_config.api_key_secret)
        auth.set_access_token(self.twitter_config.access_token, self.twitter_config.access_token_secret)
        api = tweepy.API(auth, wait_on_rate_limit=True, wait_on_rate_limit_notify=True)

        tweets_listener = MyStreamListener(api)
        tweet_stream = tweepy.Stream(api.auth, tweets_listener)
        
        print("========== START GETTING DATA AT " + datetime.now() + " ==========")
        while True:
            new_tweet = tweet_stream.filter(languages=["en"])
            if new_tweet is not None:
                # do streaming process
                pass
        

if __name__ == "__main__":
    tweet_streaming = _TweetProducer("Tweet Streaming Process", "twitter_streaming")
    tweet_streaming.main_streaming()