from sampling.protras import ProTraS
from utils.pipeline import Pipeline
import pandas as pd
from time import time

def main():
    #defined ProTras
    p = ProTraS()
    p.set_epsilon(0.3)
    p.set_mode("mp")
    p.set_cal_mode("memory-based")
    pipeline = Pipeline([p])
    df = pd.read_csv("test.csv")
    new_df = pipeline.fit_and_transform(df)
    import pdb ; pdb.set_trace()

if __name__ == '__main__':
    main()