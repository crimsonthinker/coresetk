from sampling.sampling_algorithm import ProTraS
from sampling.__c__.core import _ProTraS
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

    