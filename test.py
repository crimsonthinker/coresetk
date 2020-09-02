from sampling.protras import ProTraS
import pandas as pd
test = ProTraS()
test.set_epsilon(0.3)
df = pd.read_csv("test.csv")
test.fit_for_protras(df)