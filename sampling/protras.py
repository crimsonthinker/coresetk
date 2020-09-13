from sampling.sampling_algorithm import SamplingAlgorithm
from sampling.__c__ import _ProTraS
import pandas as pd
import ast
import numpy as np


class ProTraS(SamplingAlgorithm):

    def __init__(self,
                coord_col = 'coord',
                index_col = 'index',
                mode = 'default', #default, mp
                epsilon = 1.0,
                percentage=0.5,
                *args):
        super(SamplingAlgorithm,self).__init__(*args)
        self._coord_col = coord_col
        self._index_col = index_col
        self._mode = mode
        self._epsilon = epsilon

    def set_epsilon(self, new_eps):
        self._epsilon = new_eps

    def fit(self,dataset : pd.DataFrame):
        pass

    def fit_for_protras(self,dataset : pd.DataFrame):
        """
        lscm sdlcm
        """
        #convert coord column to a (dxn) matrix
        matrix = np.array(dataset[self._coord_col].apply(lambda x : ast.literal_eval(x)).tolist())
        #create C++ _ProTraS
        c_protras = _ProTraS()
        c_protras.set_eps(self._epsilon)
        c_protras.set_cal_mode("memory-based")
        c_protras.run(matrix)
        import pdb ; pdb.set_trace()
        return c_protras