from mpl_toolkits.mplot3d import axes3d
import matplotlib.pyplot as plt
import numpy as np
import csv

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
waterWater = "3_rdf_water_water_dr_0.015.csv"
oil1Oil1 = "3_rdf_oil1_oil1_dr_0.015.csv"
oil2Oil2 = "3_rdf_oil2_oil2_dr_0.015.csv"
waterOil1 = "3_rdf_water_oil1_dr_0.015.csv"
waterOil2 = "3_rdf_water_oil2_dr_0.015.csv"
oil1Oil2 = "3_rdf_oil1_oil2_dr_0.015.csv"

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

waterWaterFig, waterWaterAx1 = plt.subplots(1, 1, subplot_kw={'projection': '3d'})
oil1Oil1Fig, oil1Oil1Ax1 = plt.subplots(1, 1, subplot_kw={'projection': '3d'})
oil2Oil2Fig, oil2Oil2Ax1 = plt.subplots(1, 1, subplot_kw={'projection': '3d'})
waterOil1Fig, waterOil1Ax1 = plt.subplots(1, 1, subplot_kw={'projection': '3d'})
waterOil2Fig, waterOil2Ax1 = plt.subplots(1, 1, subplot_kw={'projection': '3d'})
oil1Oil2Fig, oil1Oil2Ax1 = plt.subplots(1, 1, subplot_kw={'projection': '3d'})

# Convert arrays to numpy
(waterWaterX, waterWaterY, waterWaterZ) = convertArraysToNumpy(waterWaterX, waterWaterY, waterWaterZ)
(oil1Oil1X, oil1Oil1Y, oil1Oil1Z) = convertArraysToNumpy(oil1Oil1X, oil1Oil1Y, oil1Oil1Z)
(oil2Oil2X, oil2Oil2Y, oil2Oil2Z) = convertArraysToNumpy(oil2Oil2X, oil2Oil2Y, oil2Oil2Z)
(waterOil1X, waterOil1Y, waterOil1Z) = convertArraysToNumpy(waterOil1X, waterOil1Y, waterOil1Z)
(waterOil2X, waterOil2Y, waterOil2Z) = convertArraysToNumpy(waterOil2X, waterOil2Y, waterOil2Z)
(oil1Oil2X, oil1Oil2Y, oil1Oil2Z) = convertArraysToNumpy(oil1Oil2X, oil1Oil2Y, oil1Oil2Z)

# Plot them and give them colour
waterWaterAx1.plot_surface(waterWaterX, waterWaterY, waterWaterZ, cmap=plt.cm.RdYlGn)
oil1Oil1Ax1.plot_surface(oil1Oil1X, oil1Oil1Y, oil1Oil1Z, cmap=plt.cm.RdYlGn)
oil2Oil2Ax1.plot_surface(oil2Oil2X, oil2Oil2Y, oil2Oil2Z, cmap=plt.cm.RdYlGn)
waterOil1Ax1.plot_surface(waterOil1X, waterOil1Y, waterOil1Z, cmap=plt.cm.RdYlGn)
waterOil2Ax1.plot_surface(waterOil2X, waterOil2Y, waterOil2Z, cmap=plt.cm.RdYlGn)
oil1Oil2Ax1.plot_surface(oil1Oil2X, oil1Oil2Y, oil1Oil2Z, cmap=plt.cm.RdYlGn)

# Set figure and axis titles
waterWaterAx1.set_title('RDF of water')
waterWaterAx1.set_xlabel('Timestep')
waterWaterAx1.set_ylabel('Radius')
waterWaterAx1.set_zlabel('RDF')
oil1Oil1Ax1.set_title('RDF of oil type 1 (major)')
oil1Oil1Ax1.set_xlabel('Timestep')
oil1Oil1Ax1.set_ylabel('Radius')
oil1Oil1Ax1.set_zlabel('RDF')
oil2Oil2Ax1.set_title('RDF of oil type 2 (minor)')
oil2Oil2Ax1.set_xlabel('Timestep')
oil2Oil2Ax1.set_ylabel('Radius')
oil2Oil2Ax1.set_zlabel('RDF')
waterOil1Ax1.set_title('RDF of water and oil type 1 (major)')
waterOil1Ax1.set_xlabel('Timestep')
waterOil1Ax1.set_ylabel('Radius')
waterOil1Ax1.set_zlabel('RDF')
waterOil2Ax1.set_title('RDF of water and oil type 2 (minor)')
waterOil2Ax1.set_xlabel('Timestep')
waterOil2Ax1.set_ylabel('Radius')
waterOil2Ax1.set_zlabel('RDF')
oil1Oil2Ax1.set_title('RDF of all oil - Types 1 and 2')
oil1Oil2Ax1.set_xlabel('Timestep')
oil1Oil2Ax1.set_ylabel('Radius')
oil1Oil2Ax1.set_zlabel('RDF')

# plt.tight_layout()
plt.show()
