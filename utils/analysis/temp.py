from cell import *
import math

vol_width = 3 # Constant for each run

max_timestep = 10000
min_timestep = 1

# Open file to store temperature data in
outputFile = str(vol_width) + "_temp.csv"
f = open(outputFile, "w")

f.write("Timestep, Temp\n")

total_beads = 0
divisor = 0
timestep = min_timestep
# For each timestep calculate the average bead velocity magnitude
while timestep <= max_timestep:
    # Print timestep so we can see the progress
    print("Timestep: " + str(timestep))
    # Put timestep in the csv file
    f.write(str(timestep) + ",")
    # Get universe for this timestep from files
    cells = getUniverseAtTimestepForWidth(timestep, vol_width)
    # Count the total beads for averaging (done once)
    if (total_beads == 0):
        total_beads = getTotalBeads(cells)
        divisor = 3 * total_beads
    # Accumulator for magnitude of velocities
    temp = 0
    # Iterate through each cell
    for x in range(0, vol_width):
        for y in range(0, vol_width):
            for z in range(0, vol_width):
                # Current cell
                c = cells[x][y][z]
                # For each local bead
                for i in c.beads:
                    temp = temp + (((i.velo.x * i.velo.x) + (i.velo.y * i.velo.y) + (i.velo.z * i.velo.z)) / divisor)
    f.write(str(temp) + "\n")
    # End of timestep, move to next line
    if (timestep < 9999):
        timestep = min(timestep + 50, 9999)
    else:
        break

# Close file
f.close()
# Why not?
print("Finished")
