import time
import pandas as pd
import sys

if __name__ == "main":
    id = int(sys.argv[1])
    print(id)
    if id != 1:
        exit(0)

    start = time.time()
    df = pd.read_orc("/scratch03.local/smohr/dask/orc")
    end = time.time()

    print(end - start)
