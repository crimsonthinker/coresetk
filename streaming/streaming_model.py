from sampling.modification_protras import ModificationProTraS
from utils.pipeline import Pipeline
import os

import numpy as np
import pandas as pd

class StreamingModel:
    
    def __init__(self, distance_function):
        self.rank_list = []
        self.coreset0 = None
        self.coreset1 = None
        self.cumulative_dataset = None
        self.merged_group_coreset = []

        self.coordinate = 100
        self.remaining_rank_zero_size = self.coordinate
        self.epsilon = 0.7
        self.percentage = 0.8
        self.file_core = ""
        self.global_order = 0

        self.avat_mode = "avatlite_idsa_streaming"
        self.distance_function = distance_function

        self.num_cluster = []
        self.ratio = 0.000800000001
        self.threshold_level = 2
        self.num_gen = 200
        self.population = 50

        self.original_index = {}
        tmp_arg = [False, False, "euclidean","memory-based", self.percentage, self.epsilon, "default", "default"]
        self.mp = ModificationProTraS("coord","index", tmp_arg)
        # TODO: vat

    def delete_initial_cluster_file(self):
        os.remove("./result/initial_cluster/data/" + self.file_core + "/" + self.file_core + "_v1_origin.csv")

    def set_target_file(self, new_file_core):
        print("Target Streaming file set")
        self.file_core = new_file_core
        self.globalOrder = 0

        self.delete_initial_cluster_file()    

    def check_consecutive_rank(self, start_rank):
        sort_rank_list = list(set(sorted(self.rank_list)))
        end_rank = start_rank
        stop = False
        for j in range(sort_rank_list.index(start_rank), len(sort_rank_list) - 1):
            if sort_rank_list[j + 1] != sort_rank_list[j + 1]:
                end_rank = sort_rank_list[j]
                stop = True
                break
            
        if not stop:
            end_rank = sort_rank_list[-1]
        
        return end_rank

    def set_batch_size(self, new_coordinate):
        self.coordinate = new_coordinate
        self.remaining_rank_zero_size = new_coordinate

    def parse_data_streaming(self, dataset: pd.DataFrame):
        print(self.remaining_rank_zero_size)
        self._parse_data_streaming(dataset)

    def _parse_data_streaming(self, dataset: pd.DataFrame):
        self.file_core = ""

        batch_data_size = len(dataset)

        # 1st circumstance: real-time batch < remaining buffer
        if batch_data_size < self.remaining_rank_zero_size:
            if self.coreset0 is None:
                self.coreset0 = dataset
            else:
                self.coreset0 = pd.concat([self.coreset0, dataset])
        elif batch_data_size == self.remaining_rank_zero_size:
            if self.coreset0 is None:
                self.coreset0 = dataset
            else:
                self.coreset0 = pd.concat([self.coreset0, dataset])

            prepare_coreset = self.coreset0
            self.coreset0 = None
            self.remaining_rank_zero_size = self.coordinate

            self.coreset1 = self.create_coreset(prepare_coreset)
            self.rank_list += 1
            self.merge_coreset()
            self.clustering()
        else:
            prepare_coreset = self.coreset0
            prepare_coreset = pd.concat(prepare_coreset, dataset.loc[0:self.remaining_rank_zero_size - 1])

            next_dataset = dataset.loc[self.remaining_rank_zero_size:]
            self.remaining_rank_zero_size = self.coordinate

            self.coreset1 = self.create_coreset(prepare_coreset)
            self.rank_list += 1
            self.merge_coreset()
            self.clustering()

            self._parse_data_streaming(next_dataset)

    def create_coreset(self, dataset: pd.DataFrame):
        dataset_size = dataset.size()
        new_dataset = dataset

        for x in range(dataset_size):
            self.original_index[x] = new_dataset.at[x, "index"]
            new_dataset.at[x, "index"] = x

        pipeline = Pipeline(self.mp)
        pipeline.fit(new_dataset)
        pipeline.transform()

        coreset_generated = pipeline.get_output()

        coreset_size = coreset_generated.size()
        for x in range(0, coreset_size):
            coreset_generated.at[x, "index"] = self.original_index[x]

        self.original_index = {}

        return coreset_generated

    def clustering(self):
        pass

    def merge_coreset(self):
        duplicate_rank = list(set([x for x in self.rank_list if self.rank_list.count(x) > 1]))

        if duplicate_rank:
            for i in range(duplicate_rank):
                end_rank = self.check_consecutive_rank(duplicate_rank[i])
                all_coreset_merge = self.coreset1

                if self.merged_group_coreset:
                    for x in range(end_rank):
                        all_coreset_merge = pd.concat(all_coreset_merge, self.merged_group_coreset[x])
                        self.merged_group_coreset[x] = pd.DataFrame()

                if len(self.merged_group_coreset) == end_rank:
                    self.rank_list = list(end_rank + 1)
                    self.merged_group_coreset.append(all_coreset_merge)
                elif len(self.merged_group_coreset) > end_rank:
                    self.rank_list.append(end_rank + 1)
                    self.rank_list = list(filter(lambda x: x > end_rank, self.rank_list))
                    self.merged_group_coreset[end_rank] = all_coreset_merge
        elif len(self.rank_list) == 1 and self.rank_list[0] == 1:
            self.merged_group_coreset = list(self.coreset1)
        else:
            self.merged_group_coreset[0] = self.coreset1

        cumulative_coreset = self.merged_group_coreset[0]
        for i in range(1, len(self.merged_group_coreset)):
            cumulative_coreset = pd.concat([cumulative_coreset, self.merged_group_coreset[i]])
        cumulative_coreset = pd.concat([cumulative_coreset, self.coreset0])

        self.cumulative_dataset = cumulative_coreset