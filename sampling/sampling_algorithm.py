from abc import ABC
from abc import abstractmethod
from utils.pipeline_operator import PipelineOperator

class SamplingAlgorithm(PipelineOperator, ABC):
    def __init__(self, plot_enable=False,
                log_enable=False,
                distance_function='euclidean', #manhattan
                cal_mode='ram-based'):
        self._plot_enable = plot_enable
        self._log_enable = log_enable
        self._distance_function = distance_function
        self._cal_mode = cal_mode
        self._sampling_mode = sampling_mode

    def set_distance_function(self, new_dis_func):
        if new_dis_func != 'euclidean' and new_dis_func != 'manhattan':
            print(f"Unrecognized distance function: {new_dis_func}. Using default.")
            self._distance_function = 'euclidean'
        else:
            self._distance_function = new_dis_func

    def set_percentage(self, new_percentage):
        if new_percentage > 1.0 or new_percentage < 0.0:
            print(f"Invalid percentage value: {}. Setting default.")

    def set_sampling_mode(self, mode):
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
        