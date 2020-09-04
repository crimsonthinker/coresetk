from metrics.index import Index

class DunnIndex(Index):
    def __init__(self, inter_cluster_dist_type, intra_cluster_dist_type):
        self._inter_cluster_dist_type = inter_cluster_dist_type
        self._intra_cluster_dist_type = intra_cluster_dist_type

    def set_inter_cluster_dist_type(self, tp):
        if tp != "max" and tp != "min" and tp != "cen" and tp != "avg1" and tp != "avg2"
            print("Unrecognized distance type {0}. Using default: min".format(tp))
            self._inter_cluster_dist_type = "min"
        else:
            self._inter_cluster_dist_type = tp
            
        return self

    def set_intra_cluster_dist_type(self, tp):
        if tp != "max" and tp != "avg1" and tp != "avg2"
            print("Unrecognized distance type {0}. Using default: max".format(tp))
            self.intra_cluster_dist_type = "max"
        else:
            self.intra_cluster_dist_type = tp
            
        return self

    def get_inter_cluster_dist_type(self):
        return self._inter_cluster_dist_type

    def get_intra_cluster_dist_type(self):
        return self._intra_cluster_dist_type

    def find_centroid(self, index):
        pass

    