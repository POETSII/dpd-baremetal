import json
import math
import sys
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

volLength = 25
bond_length_threshold = 0.0
min_timestep = 0
max_timestep = 2000

class Bond:
    def __init__(self, bead1, bead2):
        self.bead1 = bead1
        self.bead2 = bead2
        self.length = self.bond_length()
        self.id = str(bead1["id"]) + "-" + str(bead2["id"])

    def smallestDiff(p1, p2):
        d1 = p1 - p2
        if (p1 > p2):
            d2 = (p1 - volLength) - p2
        else:
            d2 = p1 - (p2 - volLength)

        d1_2 = d1**2
        d2_2 = d2**2
        if (d1_2 < d2_2):
            return d1
        else:
            return d2

    def bond_length(self):
        x_diff = smallestDiff(self.bead1["x"], self.bead2["x"])
        y_diff = smallestDiff(self.bead1["y"], self.bead2["y"])
        z_diff = smallestDiff(self.bead1["z"], self.bead2["z"])

        return math.sqrt((x_diff ** 2) + (y_diff**2) + (z_diff ** 2))

def smallestDiff(p1, p2):
    d1 = p1 - p2
    if (p1 > p2):
        d2 = (p1 - volLength) - p2
    else:
        d2 = p1 - (p2 - volLength)

    d1_2 = d1**2
    d2_2 = d2**2
    if (d1_2 < d2_2):
        return d1
    else:
        return d2

def is_end_of_chain(id, n_id, beads):
    if (n_id + 1) not in beads:
        return True
    if (id - 1) not in beads:
        return True
    return False

bonds = [] # Dictionary of beads that are bonded
all_bonds = False
bonds_of_interest = []

if (len(sys.argv) > 1):
    if int(sys.argv[1]) >= 1:
        all_bonds = True

print("Loading bonds")
# For each timestep
for timestep in range(min_timestep, max_timestep + 1):
    bonds.append({}) # Do bonds via dictionary to keep an order
    print("Timestep " + str(timestep), end = "\r") # Print timestep to keep an idea of progress
    # JSON filepath filled with bond info
    filepath = "../25_bond_frames/state_" + str(timestep) + ".json"
    # Store beads to then be worked on below
    beads = {}
    # Get beads from file and add to dictionary
    with open(filepath) as json_file:
        data = json.load(json_file) # Load JSON file
        for b in data["beads"]: # For each bead in the file
            # File should have only bonded beads in, but double check
            if b["id"] == (0x80000000 + 473) or b["id"] == (0x80000000 + 474):
                b["id"] = b["id"] - 0x80000000;
                beads[b["id"]] = b

    # Go through each bead and generate a bond
    for b in beads.values():
        bead_id = b["id"]
        nbor_id = bead_id + 1
        if (bead_id + 1) not in beads: # If bead has a next bonded neighbour, it'll have the next id
            continue # If not, then we can move onto the next bead
        n = beads[nbor_id]
        # Create the bond object
        # This automatically calculates the bond length and generates the bond id
        bond = Bond(b, n)
        # Add the bond object to the list of bonds for this timestep
        bonds[timestep][bond.id] = bond
        if not all_bonds: # If we only want to see the bonds that become particularly long
            # The threshold is set at the top. Any bonds that ever go above this length should be included
            # This threshold may be breached at any point in time
            # We want to see how only the bonds which breaches this and how it reacts both before and after
            if bond.length > bond_length_threshold:
                bonds_of_interest.append(bond.id) # Add this bond id so we can find it later
                bonds_of_interest = list(set(bonds_of_interest)) # Remove duplicates

# If we want to see all bonds, the bonds_of interest become all bonds
if all_bonds:
    bonds_of_interest = bonds[0].keys()

print("                                                                        ",end = "\r")
print("Complete")
print("Preparing data frame")

# Create a data_frame containing the timesteps
data_frame = pd.DataFrame()
data_frame["Timestep"] = []

# For each bond of interest
for bond_id in bonds_of_interest:
    data_frame[bond_id] = [] # Add the bond ID as a column to the data frame

# For each timestep
for timestep in range(min_timestep, max_timestep + 1):
    print("Timestep " + str(timestep), end = "\r")
    # Add the timestep to the data frame
    row = { "Timestep": timestep }
    # For each bond of interest
    for bond_id in bonds_of_interest:
        length = bonds[timestep][bond_id].length # Get bond length at this timestep
        row[bond_id] = length # Add the length to this row
    # Add the row to the data frame
    data_frame = data_frame.append(pd.Series(row, index = data_frame.columns), ignore_index = True)

# Change timestep data type
data_frame = data_frame.astype({'Timestep': int})

print("                                                                        ",end = "\r")
print("Complete")
print("Drawing plot")

# View as a graph
ax = plt.gca() # Get current axis
# For each bond of interest, plot this line
c = 0
for bond_id in bonds_of_interest:
    ax = data_frame.plot(kind = 'line', x = 'Timestep', y = bond_id, ax = ax, linewidth=1, figsize=(8, 5))
    c += 1
    print("Bond " + str(c) + " / " + str(len(bonds_of_interest)), end = "\r")

print("                                                                        ",end = "\r")
print("Complete")
# Plot configuration
lgd = plt.legend(loc = (1.04, 0.5), ncol=1)
ax.set_ylabel('Bond length')
plt.xticks(np.arange(0, max_timestep + 1, 1000), rotation=90)
plt.yticks(np.arange(0, 20, 1.0))
plt.grid(b=True, which='major', color='#cccccccc', linestyle='-')

# Save the figure
print("Saving the figure")
plt.savefig("test.pdf", bbox_extra_artists=(lgd,), bbox_inches='tight')
print("Complete")


