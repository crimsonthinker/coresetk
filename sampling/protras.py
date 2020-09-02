from sampling.sampling_algorithm import SamplingAlgorithm
from sampling.__c__.core import _ProTraS
import pandas as pd
import ast


class ProTraS(SamplingAlgorithm):

    def __init__(self,
                coord_col = 'coord',
                index_col = 'index',
                *args):
        super(SamplingAlgorithm,self).__init__(*args)
        self._coord_col = coord_col
        self._index_col = index_col

    def set_epsilon(self, new_eps):
        self._epsilon = new_eps

    def fit(self,dataset : pd.DataFrame):
        pass

    def fit_for_protras(self,d : pd.DataFrame):
        """
        lscm sdlcm
        """
        #deepcopy
        dataset = d.copy()
        #convert coord column to list
        dataset[self._coord_col] = dataset[self._coord_col].apply(lambda x : ast.literal_eval(x))
        #extract dictionary with key = index, coord = list
        data = dict(zip(dataset[self._index_col],dataset[self._coord_col]))
        #create C++ _ProTraS
        c_protras = _ProTraS()
        c_protras.run_protras(data, self._epsilon, "memory-based")
        import pdb ; pdb.set_trace()
        #update gained information to pandas