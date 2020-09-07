from sampling.sampling_algorithm import ProTraS
from sampling.__c__.core import _ModificationProTraS
import pandas as pd
import ast

class ModificationProTraS(ProTraS):

    def __init__(self, coord_col = 'coord',index_col = 'index', *args):
        super().__init__(coord_col, index_col, args)

    def set_distance_function(self, distance_function):
        self._distance_function = distance_function
        return self

    def set_percentage(self, new_percentage):
        if new_percentage < 0.0 or new_percentage > 1.0:
            print("Error setting percentage for the modified ProTraS: Please choose percentage that is between 0 and 1!")

        self._percentage = new_percentage
        return self

    def fit(self, dataset: pd.DataFrame):
        print("Start ProTraS")
        coreset_index = None
        if self._sampling_phase == "default":
            coreset_index = self.fit_for_protras(dataset)
        elif self._sampling_phase == "modification":
            coreset_index = list(set(dataset["rep"].tolist()))
            self._representative = self.read_representative(dataset)

        print("Start MP")

        # otsu-based noise filter
        filtered_coreset_index = []
        mean = 0.0
        if self._representative:
            for _, v in self._representative.items():
                mean = mean + len(v)

        mean = self._percentage * (mean / len(self._representative))
        for i in coreset_index:
            if len(self._representative[i] >= mean):
                filtered_coreset_index.append(i)

        # overlapped filtering
        # check if the point is substituted
        # call C++ code for this operation

        # deep copy
        dataset_x = dataset.copy()

        # convert phase
        matrix_dataset = dataset_x[self._coord_col].apply(lambda x : ast.literal_eval(x))

        # calling C++ service
        c_mp = _ModificationProTraS()

        shifted_list = []
        c_mp.run_modification_protras(filtered_coreset_index, shifted_list, matrix_dataset)

        # TODO: handle matrix_dataset modification after calling C++ service

        sampling = None
        if shifted_list:
            sampling = dataset.at[shifted_list[0]]            
        for i in shifted_list[1:]:
            sampling = pd.concat([sampling, dataset[i]])

        return sampling