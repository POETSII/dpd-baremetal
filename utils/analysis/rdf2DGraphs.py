from mpl_toolkits.mplot3d import axes3d
import matplotlib.pyplot as plt
import numpy as np
import csv

vol_width = 25
rmax = vol_width / 2
dr = rmax / 100

def openFilesFillArrays(filepath, X, Y, Z):
    with open(filepath) as csv_file:
        firstLine = False
        csv_reader = csv.reader(csv_file, delimiter=',')
        m = 0
        for row in csv_reader:
            # First line is one axis
            if not firstLine:
                for i in range(1, len(row) - 1):
                    Y.append(float(row[i]))
                firstLine = True
            else:
                # First column of row is timestep
                X.append(int(row[0]))
                # Array of data for next columns
                a = []
                for i in range(1, len(row) - 1):
                    a.append(float(row[i]))
                # Append array to Z (series)
                Z.append(a)
                m = m + 1

def convertArraysToNumpy(X, Y, Z):
    X = np.array(X)
    X.shape = (len(X),1)
    Y = np.array(Y)
    Z = np.array(Z)
    return (X, Y, Z)

# Read all data from files and generate axes and series
waterWater = str(vol_width) + "_rdf_water_water_dr_" + str(dr) + ".csv"
oil1Oil1 = str(vol_width) + "_rdf_oil1_oil1_dr_" + str(dr) + ".csv"
oil2Oil2 = str(vol_width) + "_rdf_oil2_oil2_dr_" + str(dr) + ".csv"
waterOil1 = str(vol_width) + "_rdf_water_oil1_dr_" + str(dr) + ".csv"
waterOil2 = str(vol_width) + "_rdf_water_oil2_dr_" + str(dr) + ".csv"
oil1Oil2 = str(vol_width) + "_rdf_oil1_oil2_dr_" + str(dr) + ".csv"

# X is timestep
# Y is radius
# Z is 2D matrix of RDF values

waterWaterX = []
waterWaterY = []
waterWaterZ = []
oil1Oil1X = []
oil1Oil1Y = []
oil1Oil1Z = []
oil2Oil2X = []
oil2Oil2Y = []
oil2Oil2Z = []
waterOil1X = []
waterOil1Y = []
waterOil1Z = []
waterOil2X = []
waterOil2Y = []
waterOil2Z = []
oil1Oil2X = []
oil1Oil2Y = []
oil1Oil2Z = []

openFilesFillArrays(waterWater, waterWaterX, waterWaterY, waterWaterZ)
openFilesFillArrays(oil1Oil1, oil1Oil1X, oil1Oil1Y, oil1Oil1Z)
openFilesFillArrays(oil2Oil2, oil2Oil2X, oil2Oil2Y, oil2Oil2Z)
openFilesFillArrays(waterOil1, waterOil1X, waterOil1Y, waterOil1Z)
openFilesFillArrays(waterOil2, waterOil2X, waterOil2Y, waterOil2Z)
openFilesFillArrays(oil1Oil2, oil1Oil2X, oil1Oil2Y, oil1Oil2Z)

plt.style.use('grayscale')

# Water water
print("Water graphs")
for timeIndex in range(0, len(waterWaterX)):
    print(waterWaterX[timeIndex])
    fig = plt.figure(figsize=(8, 6))
    ax = plt.axes()
    ax.set_xlabel('Radius')
    ax.set_ylabel('RDF')
    plt.xticks(np.arange(0, rmax + 1, 0.5), rotation=90)
    plt.yticks(np.arange(0, max(waterWaterZ[timeIndex]) + 1, 0.05))
    ax.plot(waterWaterY, waterWaterZ[timeIndex])
    plt.grid(b=True, which='major', color='#cccccccc', linestyle='-')
    ax.set_title("RDF of water at Timestep " + str(waterWaterX[timeIndex] - 1))
    fileName = "2D-plots/water_" + str(waterWaterX[timeIndex] - 1) + ".pdf"
    plt.savefig(fileName)
    plt.close()
    # plt.show()

# Oil1 oil1
print("Oil 1 graphs")
for timeIndex in range(0, len(oil1Oil1X)):
    print(oil1Oil1X[timeIndex])
    fig = plt.figure(figsize=(8, 6))
    ax = plt.axes()
    ax.set_xlabel('Radius')
    ax.set_ylabel('RDF')
    plt.xticks(np.arange(0, rmax + 1, 0.5), rotation=90)
    plt.yticks(np.arange(0, max(oil1Oil1Z[timeIndex]) + 1, 0.05))
    ax.plot(oil1Oil1Y, oil1Oil1Z[timeIndex])
    plt.grid(b=True, which='major', color='#cccccccc', linestyle='-')
    ax.set_title("RDF of Oil type 1 at Timestep " + str(oil1Oil1X[timeIndex] - 1))
    fileName = "2D-plots/oil1_" + str(oil1Oil1X[timeIndex] - 1) + ".pdf"
    plt.savefig(fileName)
    plt.close()
    # plt.show()

# Oil2 oil2
print("Oil 2 graphs")
for timeIndex in range(0, len(oil2Oil2X)):
    print(oil2Oil2X[timeIndex])
    fig = plt.figure(figsize=(8, 6))
    ax = plt.axes()
    ax.set_xlabel('Radius')
    ax.set_ylabel('RDF')
    plt.xticks(np.arange(0, rmax + 1, 0.5), rotation=90)
    plt.yticks(np.arange(0, max(oil2Oil2Z[timeIndex]) + 1, 0.05))
    ax.plot(oil2Oil2Y, oil2Oil2Z[timeIndex])
    plt.grid(b=True, which='major', color='#cccccccc', linestyle='-')
    ax.set_title("RDF of Oil type 1 at Timestep " + str(oil2Oil2X[timeIndex] - 1))
    fileName = "2D-plots/oil2_" + str(oil2Oil2X[timeIndex] - 1) + ".pdf"
    plt.savefig(fileName)
    plt.close()
    # plt.show()

# Water oil1
print("Water and Oil type 1 graphs")
for timeIndex in range(0, len(waterOil1X)):
    print(waterOil1X[timeIndex])
    fig = plt.figure(figsize=(8, 6))
    ax = plt.axes()
    ax.set_xlabel('Radius')
    ax.set_ylabel('RDF')
    plt.xticks(np.arange(0, rmax + 1, 0.5), rotation=90)
    plt.yticks(np.arange(0, max(waterOil1Z[timeIndex]) + 1, 0.05))
    ax.plot(waterOil1Y, waterOil1Z[timeIndex])
    plt.grid(b=True, which='major', color='#cccccccc', linestyle='-')
    ax.set_title("RDF of Water and Oil type 1 at Timestep " + str(waterOil1X[timeIndex] - 1))
    fileName = "2D-plots/water_oil1_" + str(waterOil1X[timeIndex] - 1) + ".pdf"
    plt.savefig(fileName)
    plt.close()
    # plt.show()

# Water oil2
print("Water and Oil type 2 graphs")
for timeIndex in range(0, len(waterOil2X)):
    print(waterOil2X[timeIndex])
    fig = plt.figure(figsize=(8, 6))
    ax = plt.axes()
    ax.set_xlabel('Radius')
    ax.set_ylabel('RDF')
    plt.xticks(np.arange(0, rmax + 1, 0.5), rotation=90)
    plt.yticks(np.arange(0, max(waterOil2Z[timeIndex]) + 1, 0.05))
    ax.plot(waterOil2Y, waterOil2Z[timeIndex])
    plt.grid(b=True, which='major', color='#cccccccc', linestyle='-')
    ax.set_title("RDF of Water and Oil type 1 at Timestep " + str(waterOil2X[timeIndex] - 1))
    fileName = "2D-plots/water_oil2_" + str(waterOil2X[timeIndex] - 1) + ".pdf"
    plt.savefig(fileName)
    plt.close()
    # plt.show()

# oil1 oil2
print("Oil types 1 and 2 graphs")
for timeIndex in range(0, len(oil1Oil2X)):
    print(oil1Oil2X[timeIndex])
    fig = plt.figure(figsize=(8, 6))
    ax = plt.axes()
    ax.set_xlabel('Radius')
    ax.set_ylabel('RDF')
    plt.xticks(np.arange(0, rmax + 1, 0.5), rotation=90)
    plt.yticks(np.arange(0, max(oil1Oil2Z[timeIndex]) + 1, 0.05))
    ax.plot(oil1Oil2Y, oil1Oil2Z[timeIndex])
    plt.grid(b=True, which='major', color='#cccccccc', linestyle='-')
    ax.set_title("RDF of All Oil (Oil types 1 and 2) at Timestep " + str(oil1Oil2X[timeIndex] - 1))
    fileName = "2D-plots/oil1_oil2_" + str(oil1Oil2X[timeIndex] - 1) + ".pdf"
    plt.savefig(fileName)
    plt.close()
    # plt.show()

# Final states of oil1, oil2, water + oil1 and water + oil2
print("Final state comparison graph")
fig = plt.figure(figsize=(8, 6))
ax = plt.axes()
ax.set_xlabel('Radius')
ax.set_ylabel('RDF')
plt.xticks(np.arange(0, rmax + 1, 0.5), rotation=90)
plt.gray()
maxTimeIndex = len(oil1Oil1X) - 1
ms = []
ms.append(max(oil1Oil1Z[maxTimeIndex]))
ms.append(max(oil2Oil2Z[maxTimeIndex]))
ms.append(max(waterOil1Z[maxTimeIndex]))
ms.append(max(waterOil2Z[maxTimeIndex]))
plt.yticks(np.arange(0, max(ms) + 1, 0.05))
ax.plot(oil1Oil1Y, oil1Oil1Z[maxTimeIndex], label = "Oil type 1")
ax.plot(oil1Oil2Y, oil1Oil2Z[maxTimeIndex], label = "Oil type 2")
ax.plot(waterOil1Y, waterOil1Z[maxTimeIndex], label = "Water and Oil type 1")
ax.plot(waterOil2Y, waterOil2Z[maxTimeIndex], label = "Water and Oil type 2")
plt.grid(b=True, which='major', color='#cccccccc', linestyle='-')
ax.set_title("Comparison of RDF at end of simulation (Timestep 9950)")
fileName = "2D-plots/end_of_timestep_comparison.pdf"
plt.legend()
plt.savefig(fileName)
plt.close()
# plt.show()
