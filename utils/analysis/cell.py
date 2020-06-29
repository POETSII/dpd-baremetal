import csv
import math

class Cell:

    def __init__(self, x, y, z):
        self.x = x
        self.y = y
        self.z = z
        self.beads = []

    def getNeighbourLoc(self, n_x, n_y, n_z, cells, vol_width):
        r_x = self.x + n_x
        r_y = self.y + n_y
        r_z = self.z + n_z

        if r_x < 0:
            r_x = r_x + vol_width
        elif r_x >= 0:
            if r_x >= vol_width:
                r_x = r_x - vol_width
        else:
            r_x = self.x + r_x

        if r_y < 0:
            r_y = vol_width + r_y
        elif r_y >= 0:
            if r_y >= vol_width:
                r_y = r_y - vol_width
        else:
            r_y = self.y + r_y

        if r_z < 0:
            r_z = vol_width + r_z
        elif r_z >= 0:
            if r_z >= vol_width:
                r_z = r_z - vol_width
        else:
            r_z = self.z + r_z


        # if n_x == -1:
        #     if self.x == 0:
        #         r_x = vol_width - 1
        #     else:
        #         r_x = self.x - 1
        # if n_x == 1:
        #     if self.x == vol_width - 1:
        #         r_x = 0
        #     else:
        #         r_x = self.x + 1

        # if n_y == -1:
        #     if self.y == 0:
        #         r_y = vol_width - 1
        #     else:
        #         r_y = self.y - 1
        # if n_y == 1:
        #     if self.y == vol_width - 1:
        #         r_y = 0
        #     else:
        #         r_y = self.y + 1

        # if n_z == -1:
        #     if self.z == 0:
        #         r_z = vol_width - 1
        #     else:
        #         r_z = self.z - 1
        # if n_z == 1:
        #     if self.z == vol_width - 1:
        #         r_z = 0
        #     else:
        #         r_z = self.z + 1

        return cells[r_x][r_y][r_z]

class Bead:

    def __init__(self, id, type):
        self.id = id
        self.type = type
        self.pos = Vector(0.0, 0.0, 0.0)
        self.velo = Vector(0.0, 0.0, 0.0)

class Vector:

    def __init__(self, x, y, z):
        self.x = x
        self.y = y
        self.z = z

    def addVec(self, vec):
        return Vector(self.x + vec.x, self.y + vec.y, self.z + vec.z)

    def subtractVec(self, pos):
        return Vector(self.x - pos.x, self.y - pos.y, self.z - pos.z)

    def multiplyVec(self, pos):
        return Vector(self.x * pos.x, self.y * pos.y, self.z * pos.z)

    def mag(self):
        return math.sqrt(self.x * self.x + self.y * self.y + self.z * self.z)

    def getEuclideanDistance(self, pos):
        sub = self.subtractVec(pos)
        return sub.mag()

    def getSquareEuclideanDistance(self, pos):
        sub = self.subtractVec(pos)
        return (sub.x * sub.x + sub.y * sub.y + sub.z * sub.z)

def getUniverseAtTimestepForWidth(timestep, vol_width):
    cells = [[[0 for _ in range(vol_width)] for _ in range(vol_width)] for _ in range(vol_width)]

    for x in range(0, vol_width):
        for y in range(0, vol_width):
            for z in range(0, vol_width):
                c = Cell(x, y, z)
                cells[x][y][z] = c

    filepath = "/media/jrbeaumont/full_bead_info/" + str(vol_width) + "_" + str(vol_width) + "_" + str(vol_width) + "_time_" + str(timestep) + ".csv"

    with open(filepath) as csv_file:
        csv_reader = csv.reader(csv_file, delimiter=',')
        for row in csv_reader:
            # Get bead info from file
            bead_id = int(row[0].strip())
            bead_type = int(row[1].strip())
            cell_x = int(row[2].strip())
            cell_y = int(row[3].strip())
            cell_z = int(row[4].strip())
            pos_x = float(row[5].strip())
            pos_y = float(row[6].strip())
            pos_z = float(row[7].strip())
            pos = Vector(pos_x, pos_y, pos_z)
            vel_x = float(row[8].strip())
            vel_y = float(row[9].strip())
            vel_z = float(row[10].strip())
            vel = Vector(vel_x, vel_y, vel_z)

            # Generate bead
            bead = Bead(bead_id, bead_type)
            bead.pos = pos
            bead.velo = vel

            # Add to cell
            cells[cell_x][cell_y][cell_z].beads.append(bead)

    return cells

def getTotalBeads(cells):
    totalBeads = 0
    for x in cells:
        for y in x:
            for z in y:
                totalBeads = totalBeads + len(z.beads)
    return totalBeads

