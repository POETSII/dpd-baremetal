from cell import *
import math

def clearArray(array):
    for i in range(len(array)):
        array[i] = 0

# Used to help adjust the relative positions for the periodic boundary
def period_bound_adj(dim):
    if dim > 1:
        return -1;
    elif dim < -1:
        return 1;
    else:
        return dim;


vol_width = 3 # Constant for each run
number_density = 3

max_timestep = 10000
min_timestep = 1

rmax = 1
dr = 0.05

outputFile = str(vol_width) + "_rdf_dr_" + str(dr) +".csv"
f = open(outputFile, "w")

# Prep top line of csv
f.write(",0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.55,0.6,0.65,0.7,0.75,0.8,0.85,0.9,0.95\n")

# Prep array of beads in shell counts
beads_within_shells = []
r = 0
while r < rmax:
    beads_within_shells.append(0)
    r = r + dr

total_beads = 0

# For each timestep calculate the RDF for 10 shells between radius 0 and 1
for timestep in range(min_timestep, max_timestep):
    # Reset array
    clearArray(beads_within_shells)
    # Print timestep so we can see the progress
    print("Timestep: " + str(timestep))
    # Put timestep in the csv file
    f.write(str(timestep) + ",")
    # Get universe for this timestep from files
    cells = getUniverseAtTimestepForWidth(timestep, vol_width)
    # Count the total beads for averaging (done once)
    if (total_beads == 0):
        total_beads = getTotalBeads(cells)
    # Iterate through each cell
    for x in range(0, vol_width):
        for y in range(0, vol_width):
            for z in range(0, vol_width):
                # Current cell
                c = cells[x][y][z]
                # Iterate through all neighbours of this cell
                for n_x in [-1, 0, 1]:
                    for n_y in [-1, 0, 1]:
                        for n_z in [-1, 0, 1]:
                            # Neighbour of current cell
                            n = c.getNeighbourLoc(n_x, n_y, n_z, cells, vol_width)
                            # For each local bead
                            for i in c.beads:
                                # For each bead in neighbour
                                for j in n.beads:
                                    # Neighbour can be the same as the cell so don't calculate distance between same bead
                                    if (i.id != j.id):
                                        # Adjust the position of the neighbour bead relative to the current cell bead
                                        x_rel = period_bound_adj(n.x - c.x)
                                        y_rel = period_bound_adj(n.y - c.y)
                                        z_rel = period_bound_adj(n.z - c.z)
                                        adj_j = Vector(j.pos.x + x_rel, j.pos.y + y_rel, j.pos.z + z_rel)
                                        # Get the Euclidean distance to between beads
                                        dist = i.pos.getEuclideanDistance(adj_j)
                                        # For each shell distance
                                        index = 0
                                        r = 0
                                        while r < rmax:
                                            # r is inner radius, r_dr is outer radius
                                            r_dr = r + dr
                                            # If distance between beads is within this shell
                                            if dist > r and dist < r_dr:
                                                beads_within_shells[index] = beads_within_shells[index] + 1
                                            # incrament r and i
                                            r = r + dr
                                            index = index + 1
    # All beads have had all shells checked
    # Now lets calculate the values
    r = 0
    index = 0
    while r < rmax:
        # r is inner radius, r_dr is outer radius
        r_dr = r + dr
        # Average of total beads
        avg = float(beads_within_shells[index]) / float(total_beads)
        # Volume of shell: Volume of outer sphere - volume of inner sphere
        volume = (4 / 3) * math.pi * (r_dr**3 - r**3)
        # Divide average by volume - Accounts for volumes being larger the further out you look
        g1 = avg / volume
        # Divide by number density
        g2 = g1 / number_density
        # Print this to the file
        f.write(str(g2) + ",")
        # Next shell
        r = r + dr
        index = index + 1

    # End of timestep, move to next line
    f.write("\n")

# Close file
f.close()
# Why not?
print("Finished")
