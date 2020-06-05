from cell import *
import math

vol_width = 3 # Constant for each run
number_density = 3

max_timestep = 10000
min_timestep = 1

# Open file to store temperature data in
outputFile = str(vol_width) + "_temp.csv"
f = open(outputFile, "w")

f.write("Timestep, Total velocity magnitude, Average velocity magnitude, Temp (Average velocity / number density),\n")

total_beads = 0

# For each timestep calculate the average bead velocity magnitude
for timestep in range(min_timestep, max_timestep):
    # Print timestep so we can see the progress
    print("Timestep: " + str(timestep))
    # Put timestep in the csv file
    f.write(str(timestep) + ",")
    # Get universe for this timestep from files
    cells = getUniverseAtTimestepForWidth(timestep, vol_width)
    # Count the total beads for averaging (done once)
    if (total_beads == 0):
        total_beads = getTotalBeads(cells)
    # Accumulator for magnitude of velocities
    total_magnitude_velocity = 0
    # Iterate through each cell
    for x in range(0, vol_width):
        for y in range(0, vol_width):
            for z in range(0, vol_width):
                # Current cell
                c = cells[x][y][z]
                # For each local bead
                for i in c.beads:
                    total_magnitude_velocity = total_magnitude_velocity + i.velo.mag()
    # Accumulated total magnitude.
    f.write(str(total_magnitude_velocity) + ",")
    # Now find average per bead
    average_velocity = total_magnitude_velocity / total_beads
    # Print this to file
    f.write(str(average_velocity) + ",")
    # Divide by number density to get temp
    temp = average_velocity / number_density
    f.write(str(temp) + "\n")
    # End of timestep, move to next line

# Close file
f.close()
# Why not?
print("Finished")
