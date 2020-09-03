from sampling.protras import ProTraS
import pandas as pd
from time import time
test = ProTraS()
test.set_epsilon(0.015)
df = pd.read_csv("test.csv")
a = time()
dump = test.fit_for_protras(df)
b = time()
print(f"Running time: {b - a}")
import pdb ; pdb.set_trace()