from abc import ABC
from abc import abstractmethod

class SamplingAlgorithm(PipelineOperator, ABC):
    def __init__(self, plot_enable=False,
                log_enable=False,
                distance_function='euclidean',
                cal_mode='ram-based',
                percentage=0.5,
                epsilon=1.0,
                sampling_mode='default',
                sampling_phase='default'):
        self._plot_enable = plot_enable
        self._log_enable = log_enable
        self._distance_function = distance_function
        self._cal_mode = cal_mode
        self._percentage = percentage
        self._epsilon = epsilon
        self._sampling_mode = sampling_mode

    def set_distance_function(self, new_dis_func):
        pass

    def set_sampling_mode(self, mode):
        pass

    def set_sampling_phase(self, mode):
        pass

    def set_cal_mode(self, mode):
        pass

    def _get_distance(self):
        pass

    def _get_initial_point(self):
        pass

    def enable_plot(self,is_enable):
        self._plot_enable = is_enable

    def enable_log(self, is_enable):
        self._log_enable = is_enable

    def save_scatter_chart(self):
        pass

    def write_to_file(self):
        pass

    def write_representative(self):
        pass

    def _fit(self):
        pass

    @abstractmethod
    def fit(self, dataset):
        pass

    def process(self, dataset):
        self._duration = 0
        sampling = self._fit(dataset)

        if self._plot_enable:
            self.save_scatter_chart(dataset, "origin")
            self.save_scatter_chart(sampling, "coreset")
        
        if self._log_enable:
            self.write_to_file(dataset, "origin")
            self.write_to_file(sampling, "coreset")

        return sampling
        