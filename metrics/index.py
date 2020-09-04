from abc import ABC
from abc import abstractmethod
from utils.pipeline_operator import PipelineOperator

class Index(PipelineOperator, ABC):
    def __init__(self, distance_function):
        self._distance_function = distance_function
        self._value = 0.0

    def set_distance_function(self, new_dist_func):
        self._distance_function = new_dist_func
        return self

    def get_value(self):
        return self._value
        