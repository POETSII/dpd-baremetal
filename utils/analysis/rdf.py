from cell import *
import math

vol_width = 10 # Constant for each run
number_density = 3
total_cells = vol_width * vol_width * vol_width

def clearArray(array):
    for i in range(len(array)):
        array[i] = 0

max_timestep = 10001
min_timestep = 1
rmax = vol_width / 2
min_r = -math.ceil(vol_width / 2)
max_r = math.ceil(vol_width / 2)
dr = rmax / 50

# Used to help adjust the relative positions for the periodic boundary
def period_bound_adj(dim):
    if dim > min_r:
        return -(vol_width - dim)
    elif dim < max_r:
        return vol_width + dim
    else:
        return dim

# File paths
waterWater = str(vol_width) + "_rdf_water_water_dr_" + str(dr) +".csv"
oil1Oil1 = str(vol_width) + "_rdf_oil1_oil1_dr_" + str(dr) +".csv"
oil2Oil2 = str(vol_width) + "_rdf_oil2_oil2_dr_" + str(dr) +".csv"
waterOil1 = str(vol_width) + "_rdf_water_oil1_dr_" + str(dr) +".csv"
waterOil2 = str(vol_width) + "_rdf_water_oil2_dr_" + str(dr) +".csv"
oil1Oil2 = str(vol_width) + "_rdf_oil1_oil2_dr_" + str(dr) +".csv"

# Files
waterWaterFile = open(waterWater, "w")
oil1Oil1File = open(oil1Oil1, "w")
oil2Oil2File = open(oil2Oil2, "w")
waterOil1File = open(waterOil1, "w")
waterOil2File = open(waterOil2, "w")
oil1Oil2File = open(oil1Oil2, "w")

# A1 in file should be empty
waterWaterFile.write(",")
oil1Oil1File.write(",")
oil2Oil2File.write(",")
waterOil1File.write(",")
waterOil2File.write(",")
oil1Oil2File.write(",")

# Prep top line of csv
r = 0
# Prep array of beads in shell counts
water_water = []
oil1_oil1 = []
oil2_oil2 = []
water_oil1 = []
water_oil2 = []
oil1_oil2 = []

while r <= rmax:
    # r is inner radius, r_dr is outer radius
    r_dr = r + dr
    # Write r value as the top line of files
    waterWaterFile.write("%.3f," % r)
    oil1Oil1File.write("%.3f," % r)
    oil2Oil2File.write("%.3f," % r)
    waterOil1File.write("%.3f," % r)
    waterOil2File.write("%.3f," % r)
    oil1Oil2File.write("%.3f," % r)
    # Set size of arrays
    water_water.append(0)
    oil1_oil1.append(0)
    oil2_oil2.append(0)
    water_oil1.append(0)
    water_oil2.append(0)
    oil1_oil2.append(0)
    # increment r
    r = r + dr

waterWaterFile.write("\n")
oil1Oil1File.write("\n")
oil2Oil2File.write("\n")
waterOil1File.write("\n")
waterOil2File.write("\n")
oil1Oil2File.write("\n")

total_beads = 0
timestep = min_timestep
# For each timestep calculate the RDF for 10 shells between radius 0 and 1
while timestep <= max_timestep:
    # Reset array
    clearArray(water_water)
    clearArray(oil1_oil1)
    clearArray(oil2_oil2)
    clearArray(water_oil1)
    clearArray(water_oil2)
    clearArray(oil1_oil2)
    # Print timestep so we can see the progress
    # Put timestep in the files
    waterWaterFile.write(str(timestep) + ",")
    oil1Oil1File.write(str(timestep) + ",")
    oil2Oil2File.write(str(timestep) + ",")
    waterOil1File.write(str(timestep) + ",")
    waterOil2File.write(str(timestep) + ",")
    oil1Oil2File.write(str(timestep) + ",")
    # Get universe for this timestep from files
    cells = getUniverseAtTimestepForWidth(timestep, vol_width)
    # Number of each bead considered
    reference_beads = [0, 0, 0]
    # Count the cells so we can see how far through we are
    done_cells = 0
    # Each cell vs each cell need only be done once
    alreadyDone = []
    # Iterate through each cell
    for x in range(0, vol_width):
        for y in range(0, vol_width):
            for z in range(0, vol_width):
                # Current cell
                c = cells[x][y][z]
                done_cells = done_cells + 1
                print("Timestep " + str(timestep) + ": Cell " + str(done_cells) + "/" + str(total_cells))
                done = 0
                # Iterate through all neighbours of this cell
                for n_x in range(0, max_r):
                    for n_y in range(0, max_r):
                        for n_z in range(0, max_r):
                            # Neighbour of current cell
                            n = c.getNeighbourLoc(n_x, n_y, n_z, cells, vol_width)
                            ad = (n.x, n.y, n.z)
                            # Check if the current cell has already been tested agains the neighbouring cell
                            if ad not in alreadyDone:
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
                                            dist2 = i.pos.getSquareEuclideanDistance(adj_j)
                                            # For each shell distance
                                            index = 0
                                            r = 0
                                            while r <= rmax:
                                                r2 = r*r
                                                # r is inner radius, r_dr is outer radius
                                                r_dr = r + dr
                                                # If distance between beads is within this shell
                                                r_dr2 = r_dr*r_dr
                                                if dist2 > r2 and dist2 < r_dr2:
                                                    if i.type == 0 and j.type == 0:
                                                        water_water[index] = water_water[index] + 2
                                                    elif i.type == 1 and j.type == 1:
                                                        oil1_oil1[index] = oil1_oil1[index] + 2
                                                    elif i.type == 2 and j.type == 2:
                                                        oil2_oil2[index] = oil2_oil2[index] + 2
                                                    elif (i.type == 0 and j.type == 1) or (i.type == 1 and j.type == 0):
                                                        water_oil1[index] = water_oil1[index] + 2
                                                    elif (i.type == 0 and j.type == 2) or (i.type == 2 and j.type == 0):
                                                        water_oil2[index] = water_oil2[index] + 2
                                                    elif (i.type == 1 and j.type == 2) or (i.type == 2 and j.type == 1):
                                                        oil1_oil2[index] = oil1_oil2[index] + 2
                                                # incrament r and i
                                                r = r + dr
                                                index = index + 1
                            else:
                                done = done + 1

                # Every bead is used as a reference, so count these
                for i in c.beads:
                    reference_beads[i.type] = reference_beads[i.type] + 1
                # Add this cell to the neighbours "done" list
                alreadyDone.append((c.x, c.y, c.z))
                print(str(c.x) + ", " + str(c.y) + ", " + str(c.z))
                print("Already done for this cell = " + str(done))
                print("Ref beads = " + str(reference_beads[0] + reference_beads[1] + reference_beads[2]))
                done = 0
    # All beads have had all shells checked
    # Now lets calculate the values
    r = 0
    index = 0
    while r <= rmax:
        # r is inner radius, r_dr is outer radius
        r_dr = r + dr
        # Volume of shell: Volume of outer sphere - volume of inner sphere
        volume = (4 / 3) * math.pi * (r_dr**3 - r**3)
        # Average over reference beads
        water_water_avg = float(water_water[index]) / float(reference_beads[0])
        oil1_oil1_avg = float(oil1_oil1[index]) / float(reference_beads[1])
        oil2_oil2_avg = float(oil2_oil2[index]) / float(reference_beads[2])
        water_oil1_avg = float(water_oil1[index]) / float(reference_beads[0] + reference_beads[1])
        water_oil2_avg = float(water_oil2[index]) / float(reference_beads[0] + reference_beads[2])
        oil1_oil2_avg = float(oil1_oil2[index]) / float(reference_beads[1] + reference_beads[2])
        # Divide average by volume - Accounts for volumes being larger the further out you look
        water_water_g1 = water_water_avg / volume
        oil1_oil1_g1 = oil1_oil1_avg / volume
        oil2_oil2_g1 = oil2_oil2_avg / volume
        water_oil1_g1 = water_oil1_avg / volume
        water_oil2_g1 = water_oil2_avg / volume
        oil1_oil2_g1 = oil1_oil2_avg / volume
        # Divide by number density
        water_water_g2 = water_water_g1 / number_density
        oil1_oil1_g2 = oil1_oil1_g1 / number_density
        oil2_oil2_g2 = oil2_oil2_g1 / number_density
        water_oil1_g2 = water_oil1_g1 / number_density
        water_oil2_g2 = water_oil2_g1 / number_density
        oil1_oil2_g2 = oil1_oil2_g1 / number_density
        # Print these to the files
        waterWaterFile.write(str(water_water_g2) + ",")
        oil1Oil1File.write(str(oil1_oil1_g2) + ",")
        oil2Oil2File.write(str(oil2_oil2_g2) + ",")
        waterOil1File.write(str(water_oil1_g2) + ",")
        waterOil2File.write(str(water_oil2_g2) + ",")
        oil1Oil2File.write(str(oil1_oil2_g2) + ",")
        # Next shell
        r = r + dr
        index = index + 1

    # End of timestep, calculate the next timestep
    timestep = timestep + 50

    # End of timestep, move to next line
    waterWaterFile.write("\n")
    oil1Oil1File.write("\n")
    oil2Oil2File.write("\n")
    waterOil1File.write("\n")
    waterOil2File.write("\n")
    oil1Oil2File.write("\n")

# Close file
waterWaterFile.close()
oil1Oil1File.close()
oil2Oil2File.close()
waterOil1File.close()
waterOil2File.close()
oil1Oil2File.close()

# Why not?
print("Finished")
