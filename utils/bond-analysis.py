import json
import math

volLength = 25

def smallestDiff(p1, p2):
    # print("P1 = " + str(p1))
    # print("P2 = " + str(p2))
    d1 = p1 - p2
    # print("d1 = " + str(d1))
    if (p1 > p2):
        d2 = (p1 - volLength) - p2
    else:
        d2 = p1 - (p2 - volLength)

    # print("d2 = " + str(d2))
    d1_2 = d1**2
    d2_2 = d2**2
    if (d1_2 < d2_2):
        # print("Returning d1 = " + str(d1))
        # input()
        return d1
    else:
        # print("Returning d2 = " + str(d2))
        # input()
        return d2

def is_end_of_chain(id, n_id, beads):
    result = False
    if (n_id + 1) not in beads:
        print("End of chain")
        result = True
    if (id - 1) not in beads:
        print("Start of chain")
        result = True
    if not result:
        print("NOT end of chain")
    return result

def is_oil_and_water(id, n_id, beads):
    if beads[id]["type"] != beads[n_id]["type"]:
        print("Is oil and water")
        return True
    if beads[id]["type"] == 0:
        print("Both are water")
    if beads[id]["type"] == 1:
        print("Both are oil1")
    return False

bonded_beads = {} # Dictionary of beads that are bonded
all_bonded_beads = 0 # Total number of bonded beads across all timesteps
all_bonds = 0 # Total number of bonds across all timesteps
full_average = 0.0 # Average of all bonds across all timesteps
timestep = 0
min = 100
max = 0


# For each timestep
while timestep <= 10000:
    euc_range = [0] * volLength

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
        all_bonded_beads += total_bonded # Increase total number of beads for all timesteps

        timestep_average = 0.0 # Average for this timestep
        timestep_bonds = 0 # Total number of bonds for this timestep
        num = 0
        loc_min = 100
        loc_max = 0
        for k in bonded_beads.keys(): # For each stored bead
            # print("ID: " + str(bonded_beads[k]["id"]))
            # print("x : " + str(bonded_beads[k]["x"]))
            # print("y : " + str(bonded_beads[k]["y"]))
            # print("z : " + str(bonded_beads[k]["z"]))
            n = k + 1 # One of the two potential bonded beads
            if n not in bonded_beads: # If this bead doesn't exist just move on
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

            diff_x = smallestDiff(k_x, n_x)
            diff_y = smallestDiff(k_y, n_y)
            diff_z = smallestDiff(k_z, n_z)

            # Calculate Euclidean distance
            euc = math.sqrt(diff_x ** 2 + diff_y ** 2 + diff_z ** 2)
            if euc < loc_min:
                loc_min = euc
            if euc > loc_max:
                loc_max = euc
            for i in range(1, volLength):
                if euc < i:
                    euc_range[i] += 1
                    break

            if euc > 1:
                # print("Bond is long: " + str(euc))
                is_end_of_chain(k, n, bonded_beads)
                is_oil_and_water(k, n, bonded_beads)
                input()

            # if (euc > 12.5):
            #     print("euc = " + str(euc))
            #     print("k = (" + str(k_x) + ", " + str(k_y) + ", " + str(k_z) + ")")
            #     print("n = (" + str(n_x) + ", " + str(n_y) + ", " + str(n_z) + ")")
            #     print("diff = (" + str(diff_x) + ", " + str(diff_y) + ", " + str(diff_z) + ")")
            #     input()
            # print("r_ij               = (" + str(k_x - n_x) + ", " + str(k_y - n_y) + ", " + str(k_z - n_z) + ")")
            # print("r_ij_dist          = " + str(euc))
            # print("r_ij/r_ij_dist     = (" + str((k_x - n_x)/euc) + ", " + str((k_y - n_y)/euc) + ", " + str((k_z - n_z)/euc) + ")")
            # print("r_ij_dist - r0     = " + str(0.5 - euc))
            # print("r_ij_dist - r0     = " + str(euc - 0.5))
            # print("| r_ij_dist - r0 | = " + str(math.sqrt((euc - 0.5) * (euc - 0.5))))
            f_x = ((k_x - n_x) / euc) * 128 * math.sqrt((euc - 0.5) * (euc - 0.5))
            f_y = ((k_y - n_y) / euc) * 128 * math.sqrt((euc - 0.5) * (euc - 0.5))
            f_z = ((k_z - n_z) / euc) * 128 * math.sqrt((euc - 0.5) * (euc - 0.5))
            # if euc >= 0.5:
            #     print("Force              = (" + str(-f_x) + ", " + str(-f_y) + ", " + str(-f_z) + ")")
            # else:
            #     print("Force              = (" + str(f_x) + ", " + str(f_y) + ", " + str(f_z) + ")")
            r_ij_x = k_x - n_x
            r_ij_y = k_y - n_y
            r_ij_z = k_z - n_z

            # Add to average
            # print("Bond " + str(num) + " = " + str(euc))
            num += 1
            timestep_average += euc
            full_average += euc

        print(str(timestep_bonds) + " total bonds in this timestep")
        print(str(total_bonded) + " total bonded beads in this timestep")
        for i in range(1, volLength):
            print("Euc range " + str(i) + " = " +str(euc_range[i]))
        # input()
        # Divide timestep total distance by total number of bonds to get average
        timestep_average /= timestep_bonds
        if timestep_average < min:
            min = timestep_average
        if timestep_average > max:
            max = timestep_average

        print("min = " + str(loc_min))
        print("max = " + str(loc_max))
        print("Average bond length for this timestep = " + str(timestep_average))
        print()

    # Next timestep
    # if timestep == 0:
    timestep += 1
    # else:
        # timestep += 100

print("Min = " + str(min))
print("Max = " + str(max))
    # input()
