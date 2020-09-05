import pyspark.SparkContext, SparkConf

class BaseContext:
    def __init__(self, app_name):
        self.conf = buildSparkConf(appName)
    
    def get_spark_context(self):
        return SparkContext(self.conf)

  def buildSparkConf(appName: String): SparkConf = {
    tmp_conf = SparkConf(true)
    tmp_conf.setAppName(appName)
    tmp_conf.setMaster("local[*]")
    tmp_conf.set("spark.streaming.kafka.consumer.cache.initialCapacity", "64")
    tmp_conf.set("spark.streaming.kafka.consumer.cache.maxCapacity", "64")
    tmp_conf.set("spark.streaming.kafka.consumer.poll.ms", "75000")
    tmp_conf.set("spark.streaming.unpersist", "true")
    tmp_conf.set("spark.cleaner.ttl", "600")
    tmp_conf.set("spark.driver.extraJavaOptions", "-XX:+UseG1GC")
    tmp_conf.set("spark.executor.extraJavaOptions", "-XX:+UseG1GC")
    tmp_conf.set("spark.streaming.backpressure.enabled", "true")
    tmp_conf.set("spark.streaming.receiver.maxRate", "100")
    tmp_conf.set("spark.streaming.kafka.maxRatePerPartition", "100")
    tmp_conf.set("spark.streaming.stopGracefullyOnShutdown", "true")
    tmp_conf.set("spark.shuffle.reduceLocality.enabled", "false")

    return tmp_conf
  }