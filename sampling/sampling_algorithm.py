from abc import ABC
from abc import abstractmethod
from utils.pipeline_operator import PipelineOperator

class SamplingAlgorithm(PipelineOperator, ABC):
    def __init__(self, plot_enable : bool = False,
                log_enable : bool = False,
                coord_col : str = 'coord',
                distance_function : str = 'euclidean', #manhattan
                cal_mode : str = 'ram-based' #ram-based, memory-based
        ):
        self._plot_enable = plot_enable
        self._log_enable = log_enable
        self._coord_col = coord_col
        self._distance_function = distance_function
        self._cal_mode = cal_mode
        

    def set_distance_function(self, new_dis_func):
        if new_dis_func != 'euclidean' and new_dis_func != 'manhattan':
            print(f"Unrecognized distance function: {new_dis_func}. Using default.")
            self._distance_function = 'euclidean'
        else:
            self._distance_function = new_dis_func

    def set_cal_mode(self, mode):
        if mode != 'memory-based' and mode != 'ram-based':
            print(f"Unrecognized calculation mode: {mode}. Using default.")
            self._cal_mode = 'ram-based'
        else:
            self._cal_mode = mode

    def enable_plot(self,is_enable : bool):
        self._plot_enable = is_enable

    def enable_log(self, is_enable : bool):
        self._log_enable = is_enable

    def save_scatter_chart(self):
        pass

    def write_to_file(self):
        pass

    @abstractmethod
    def fit(self, dataset):
        pass

    def read_representative(self, dataset):
        pass

    def process(self, dataset):
        self._duration = 0
        sampling = self.fit(dataset)

        if self._plot_enable:
            self.save_scatter_chart(dataset, "origin")
            self.save_scatter_chart(sampling, "coreset")
        
        if self._log_enable:
            self.write_to_file(dataset, "origin")
            self.write_to_file(sampling, "coreset")

        return sampling
        