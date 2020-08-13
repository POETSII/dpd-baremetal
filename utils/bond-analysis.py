import json
import math


bonded_beads = {} # Dictionary of beads that are bonded
all_bonded_beads = 0 # Total number of bonded beads across all timesteps
all_bonds = 0 # Total number of bonds across all timesteps
full_average = 0.0 # Average of all bonds across all timesteps
timestep = 0
# For each timestep
while timestep <= 10000:
    print("Timestep " + str(timestep))
    filepath = "../25_bond_frames/state_" + str(timestep) + ".json"
    with open(filepath) as json_file:
        data = json.load(json_file) # Load JSON file
        total_bonded = 0 # Total bonded beads for this timestep
        for b in data["beads"]: # For each bead in the file
            bead_id = b["id"] # Store the ID
            if (bead_id >= 0x80000000): # This is the lowest ID value for a bonded bead
                bonded_beads[bead_id] = b # Store the bead
                total_bonded += 1 # Increment the total for this timestep
        print(str(total_bonded) + " total bonded beads in this timestep")
        all_bonded_beads += total_bonded # Increase total number of beads for all timesteps

        timestep_average = 0.0 # Average for this timestep
        timestep_bonds = 0 # Total number of bonds for this timestep
        for k in bonded_beads.keys(): # For each stored bead
            n = k + 1 # One of the two potential bonded beads
            if n  not in bonded_beads: # If this bead doesn't exist just move on
                continue
            all_bonds += 1 # Increment number of bonds in all timesteps
            timestep_bonds += 1 # Increment number of bonds in this timestep

            # Get bead positions
            k_x = bonded_beads[k]["x"]
            k_y = bonded_beads[k]["y"]
            k_z = bonded_beads[k]["z"]

            n_x = bonded_beads[n]["x"]
            n_y = bonded_beads[n]["y"]
            n_z = bonded_beads[n]["z"]

            # Calculate Euclidean distance
            euc = math.sqrt((k_x - n_x) * (k_x - n_x) + (k_y - n_y) * (k_y - n_y) + (k_z - n_z) * (k_z - n_z))

            # Add to average
            timestep_average += euc
            full_average += euc

        print(str(timestep_bonds) + " total bonds in this timestep")
        # Divide timestep total distance by total number of bonds to get average
        timestep_average /= timestep_bonds
        print("Average bond length for this timestep = " + str(timestep_average))
        print()

    # Next timestep
    timestep += 100
