from sampling.sampling_algorithm import SamplingAlgorithm
import pandas as pd

class ProTraS(SamplingAlgorithm):

    def __init__(self,rep_col = 'rep',
                dis_to_rep_col = 'dis_to_rep',
                cluster_col = 'cluster',
                coord_col = 'coord',
                *args):
        super(SamplingAlgorithm,self).__init__(*args)
        self._rep_col = rep_col
        self._dis_to_rep_col = dis_to_rep_col
        self._cluster_col = cluster_col
        self._coord_col = coord_col

    def set_epsilon(self, new_eps):
        self._epsilon = new_eps

    def fit(self,dataset : Dataset):
        pass

    def fit_for_protras(self,d : pd.DataFrame) -> list:
        """
        lscm sdlcm
        """
        #deepcopy
        dataset = d.copy()
        #add pandas column index
        dataset['index'] = dataset.index
        #int
        dataset[[self._rep_col, self._cluster_col]] = dataset[[self._rep_col, self._cluster_col]].astype(int)
        #float
        dataset[self._dis_to_rep_col] = dataset[self._dis_to_rep_col].astype(float)
        #convert to dict
        data_dict = {}
        attribute_dict = vars(self)
        for col_name in attribute_dict:
            name = attribute_dict[col_name]
            data_dict[name] = dataset[name].to_dict(orient='index')
        coreset_indices = self.do_something(data_dict[self._rep_col], 
                                    data_dict[self._dis_to_rep_col], 
                                    data_dict[self._cluster_col], 
                                    data_dict[self._coord_col])

        #update gained information to pandas

        return coreset_indices