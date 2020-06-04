from cell import *
import math
import numpy

vol_width = 3 # Constant for each run
number_density = 3

max_timestep = 10000
min_timestep = 1

rmax = 1
dr = 0.1

outputFile = str(vol_width) + "_rdf_dr_" + str(dr) +".csv"
f = open(outputFile, "w")

# Prep top line of csv
f.write(",0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9\n")

# For each timestep calculate the RDF for 10 shells between radius 0 and 1
for timestep in range(min_timestep, max_timestep):
    print("Timestep: " + str(timestep))
    f.write(str(timestep) + ",")
    # Get universe for this timestep from files
    cells = getUniverseAtTimestepForWidth(timestep, vol_width)
    total_beads = getTotalBeads(cells)
    # Iterate over all values of r in rs
    r = 0.0
    while r < (rmax - dr):
        beads_within_shells = 0
        max_r = r + dr;
        for x in range(0, vol_width):
            for y in range(0, vol_width):
                for z in range(0, vol_width):
                    c = cells[x][y][z]
                    for n_x in [-1, 0, 1]:
                        for n_y in [-1, 0, 1]:
                            for n_z in [-1, 0, 1]:
                                n = c.getNeighbourLoc(n_x, n_y, n_z, cells, vol_width)
                                for i in c.beads:
                                    for j in n.beads:
                                        if (i.id != j.id):
                                            adj_j = Vector(j.pos.x + n_x, j.pos.y + n_y, j.pos.z + n_z)
                                            dist = i.pos.getEuclideanDistance(adj_j)
                                            if dist > r and dist < max_r:
                                                beads_within_shells = beads_within_shells + 1
        avg = float(beads_within_shells) / float(total_beads)
        volume = (4 / 3) * numpy.pi * ((r + dr) ** 3 - r ** 3)
        g1 = avg / volume
        g2 = g1 / number_density
        f.write(str(g2) + ",")
        # Next shell
        r = r + dr
    # End of timestep
    f.write("\n")

print("Finished")
