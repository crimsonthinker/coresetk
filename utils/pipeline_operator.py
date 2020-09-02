from abc import ABC
from abc import abstractmethod

class PipelineOperator(ABC):
    def __init__(self):
        self._duration = 0

    def time_elapsed(self):
        return self._duration

    @abstractmethod
    def process(self, dataset):
        pass

    