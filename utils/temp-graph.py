import json
import math
import sys
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

volLength = 25
min_timestep = 0
max_timestep = 2000

timesteps = []
temperatures = []
max_average = 0

print("Loading velocities")
# For each timestep
for timestep in range(min_timestep, max_timestep + 1):
    timesteps.append(timestep)
    print("Timestep " + str(timestep), end = "\r") # Print timestep to give an idea of progress
    # JSON filepath filled with bead info
    filepath = "../25_bond_frames/state_" + str(timestep) + ".json"
    # Total number of beads in this timestep
    beads = 0
    # Velocity magnitude accumulator
    velocity = 0
    # Get beads from file and add the velocity magnitude
    with open(filepath) as json_file:
        data = json.load(json_file) # Load JSON file
        for b in data["beads"]: # For each bead in the file
            # Calc velocity magnitude and add it to the accumulator
            velocity += (b["vx"] ** 2) + (b["vy"] ** 2) + (b["vz"] ** 2)
            beads += 1

    # Calculate average bead velocity
    average_velocity = velocity / (beads * 3) # Divide by 3 for the 3 freedoms of movement
    if average_velocity > max_average:
        max_average = average_velocity

    temperatures.append(average_velocity)

print("              ",end = "\r")
print("Complete")
print("Preparing data frame")

# Create a data_frame containing the timesteps
data_frame = pd.DataFrame()
# Column 1 is Timestep
data_frame["Timestep"] = timesteps
# Column 2 is Temperature
data_frame["Temperature"] = temperatures

# Change timestep data type
data_frame = data_frame.astype({'Timestep': int})

print("Complete")
print("Drawing plot")

# View as a graph
ax = plt.gca() # Get current axis

# Draw line
ax = data_frame.plot(kind = 'line', x = 'Timestep', y = "Temperature", ax = ax, linewidth=1, figsize=(8, 5))

print("Complete")
# Plot configuration
plt.xticks(np.arange(0, max_timestep + 1, 1000), rotation=90)
# plt.yticks(np.arange(0, max_average + 1, 1))
plt.grid(b=True, which='major', color='#cccccccc', linestyle='-')

# Save the figure
print("Saving the figure")
plt.savefig("temp.pdf")
print("Complete")


