class Pipeline:
    def __init__(self, operator_list):
        self._operator_list = operator_list
        self._obj = None
        self._duration = 0

    def fit(self, dataset):
        self._obj = dataset

    def transform(self):
        if len(self._operator_list) == 0:
            #raise warning
            return

        for operator in self._operator_list:
            self._obj = operator.process(self._obj)
            self._duration = self._duration + operator.time_elapsed()

    def fit_and_transform(self, dataset):
        self.fit(dataset)
        self.transform()
        return self._obj
    
    def time_elapsed(self):
        return self._duration

    def get_output(self):
        return self._obj

    def add_operator(self, operator):
        self._operator_list.append(operator)

    def __getitem__(self, key):
        return self._obj[key]

    def __len__(self):
        return len(self._obj)
            