import json
import math
import sys
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

bars = {}
range_width = 0.01
max_dist = 1.0

c = 0
while c < max_dist:
    bars[c + range_width] = 0
    c += range_width

print("Loading initial distances")

with open("../init_dist.json") as json_file:
    data = json.load(json_file) # Load JSON file
    total = 0
    for d in data["min_dists"]: # For each minimum distance
        c = range_width
        while c < max_dist:
            if d < c:
                bars[c] += 1
                total += 1
                break
            c += range_width
    remainder = len(data["min_dists"]) - total

print("Complete")
print("Preparing data frame")

# Create a data_frame containing the timesteps
data_frame = pd.DataFrame()
# Column 1 is Max of each range
data_frame["Max of Range"] = bars.keys()
# Column 2 is Numbers in each range
data_frame["Numbers"] = bars.values()

print("Complete")
print("Drawing plot")

print(data_frame)

# View as a graph
ax = plt.gca() # Get current axis

# Draw plot
ax = data_frame.plot(kind = 'bar', x = 'Max of Range', y = "Numbers", ax = ax)

print("Complete")

# Save the figure
print("Saving the figure")
plt.savefig("min-dists.pdf")
print("Complete")


