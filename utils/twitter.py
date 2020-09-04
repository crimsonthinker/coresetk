class TwitterConfiguration:
    def __init__(self, configuration):
        self.api_key = configuration["twitter_key"]["api_key"]
        self.api_key_secret = configuration["twitter_key"]["api_key_secret"]
        self.access_token = configuration["twitter_key"]["access_token"]
        self.access_token_secret = configuration["twitter_key"]["access_token_secret"]