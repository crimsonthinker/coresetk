from sampling.sampling_algorithm import SamplingAlgorithm
from sampling.__c__ import _ProTraS, _ModificationProTraS
import pandas as pd
import ast
import numpy as np


class ProTraS(SamplingAlgorithm):

    def __init__(self,
                mode = 'default', #default, mp
                epsilon = 1.0,
                percentage = 0.5,
                **kwargs):
        super().__init__(**kwargs)
        self._mode = mode
        self._epsilon = epsilon
        self._percentage = percentage

    def set_epsilon(self, new_eps):
        self._epsilon = new_eps

    def set_mode(self,new_mode):
        if new_mode != "default" and new_mode != "mp":
            print(f'Invalid sampling mode: {new_mode}. Setting default')
            self._mode = 'mp'
        else:
            self._mode = new_mode

    def set_percentage(self, new_percentage):
        if new_percentage > 1.0 or new_percentage < 0.0:
            print(f"Invalid percentage value: {new_percentage}. Setting default.")
            self._percentage = 0.5
        else:
            self._percentage = new_percentage

    def fit(self,dataset : pd.DataFrame):
        #convert coord column to a (dxn) matrix
        raw_coord_data = np.array(dataset[self._coord_col].apply(lambda x : ast.literal_eval(x)).tolist())
        #create C++ _ProTraS
        if self._mode == 'default':
            protras = _ProTraS()
            protras.set_epsilon(self._epsilon)
            protras.set_cal_mode(self._cal_mode)
            protras.run(raw_coord_data)
        else:
            mp = _ModificationProTraS()
            mp.set_epsilon(self._epsilon)
            mp.set_percentage(self._percentage)
            mp.run(raw_coord_data)
        dataset['rep'] = protras.get_rep_of_point_list()
        dataset['dis_to_rep'] = protras.get_rep_dist_list()
        return dataset

            
