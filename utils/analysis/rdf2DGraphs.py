import matplotlib.pyplot as plt
import numpy as np
import csv
import os

vol_width = 100
rmax = vol_width / 10
dr = rmax / 100

rdf_data_dir = "../../rdf_data/"
graph_out_dir = str(vol_width) + "_vesicle_rdf_graphs/";

if not os.path.exists(graph_out_dir):
    os.makedirs(graph_out_dir)

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
headhead = rdf_data_dir + str(vol_width) + "_rdf_head_head.csv"
headtail = rdf_data_dir + str(vol_width) + "_rdf_head_tail.csv"
headWater = rdf_data_dir + str(vol_width) + "_rdf_head_water.csv"
tailtail = rdf_data_dir + str(vol_width) + "_rdf_tail_tail.csv"
tailWater = rdf_data_dir + str(vol_width) + "_rdf_tail_water.csv"
waterWater = rdf_data_dir + str(vol_width) + "_rdf_water_water.csv"

# Graph output filepaths (ordered into directories for neatness)
headhead_dir = str(graph_out_dir) + "head_graphs/"
headtail_dir = str(graph_out_dir) + "tail_graphs/"
headWater_dir = str(graph_out_dir) + "head_water_graphs/"
tailtail_dir = str(graph_out_dir) + "tail_graphs/"
tailWater_dir = str(graph_out_dir) + "tail_water_graphs/"
waterWater_dir = str(graph_out_dir) + "water_graphs/"

# X is timestep
# Y is radius
# Z is 2D matrix of RDF values

headheadX = []
headheadY = []
headheadZ = []
headtailX = []
headtailY = []
headtailZ = []
headWaterX = []
headWaterY = []
headWaterZ = []
tailtailX = []
tailtailY = []
tailtailZ = []
tailWaterX = []
tailWaterY = []
tailWaterZ = []
waterWaterX = []
waterWaterY = []
waterWaterZ = []

openFilesFillArrays(headhead, headheadX, headheadY, headheadZ)
openFilesFillArrays(headtail, headtailX, headtailY, headtailZ)
openFilesFillArrays(headWater, headWaterX, headWaterY, headWaterZ)
openFilesFillArrays(tailtail, tailtailX, tailtailY, tailtailZ)
openFilesFillArrays(tailWater, tailWaterX, tailWaterY, tailWaterZ)
openFilesFillArrays(waterWater, waterWaterX, waterWaterY, waterWaterZ)

# plt.style.use('grayscale')

# Head head
print("Head graphs")
if not os.path.exists(headhead_dir):
    os.makedirs(headhead_dir)
for timeIndex in range(0, len(headheadX)):
    print(str(headheadX[timeIndex]) + "    ", end = "\r")
    fig = plt.figure(figsize=(8, 6))
    ax = plt.axes()
    ax.set_xlabel('Radius')
    ax.set_ylabel('RDF')
    plt.xticks(np.arange(0, rmax + 1, 2), rotation=90)
    plt.yticks(np.arange(0, max(headheadZ[timeIndex]) + 1, 0.1))
    ax.plot(headheadY, headheadZ[timeIndex])
    plt.grid(b=True, which='major', color='#cccccccc', linestyle='-')
    ax.set_title("RDF of Head beads at Timestep " + str(headheadX[timeIndex]))
    fileName = str(headhead_dir) + "head_" + str(headheadX[timeIndex]) + ".pdf"
    plt.savefig(fileName)
    plt.close()
    # plt.show()
print()

# head tail
print("Head and Tail graphs")
if not os.path.exists(headtail_dir):
    os.makedirs(headtail_dir)
for timeIndex in range(0, len(tailtailX)):
    print(str(tailtailX[timeIndex]) + "    ", end = "\r")
    fig = plt.figure(figsize=(8, 6))
    ax = plt.axes()
    ax.set_xlabel('Radius')
    ax.set_ylabel('RDF')
    plt.xticks(np.arange(0, rmax + 1, 2), rotation=90)
    plt.yticks(np.arange(0, max(tailtailZ[timeIndex]) + 1, 0.1))
    ax.plot(tailtailY, tailtailZ[timeIndex])
    plt.grid(b=True, which='major', color='#cccccccc', linestyle='-')
    ax.set_title("RDF of Head and Tail at Timestep " + str(tailtailX[timeIndex]))
    fileName = str(headtail_dir) + "head_tail_" + str(tailtailX[timeIndex]) + ".pdf"
    plt.savefig(fileName)
    plt.close()
    # plt.show()
print()

# Head water
print("Head and Water graphs")
if not os.path.exists(headWater_dir):
    os.makedirs(headWater_dir)
for timeIndex in range(0, len(headWaterX)):
    print(str(headWaterX[timeIndex]) + "    ", end = "\r")
    fig = plt.figure(figsize=(8, 6))
    ax = plt.axes()
    ax.set_xlabel('Radius')
    ax.set_ylabel('RDF')
    plt.xticks(np.arange(0, rmax + 1, 2), rotation=90)
    plt.yticks(np.arange(0, max(headWaterZ[timeIndex]) + 1, 0.1))
    ax.plot(headWaterY, headWaterZ[timeIndex])
    plt.grid(b=True, which='major', color='#cccccccc', linestyle='-')
    ax.set_title("RDF of Head and Water beads at Timestep " + str(headWaterX[timeIndex]))
    fileName = str(headWater_dir) +  "head_water_" + str(headWaterX[timeIndex]) + ".pdf"
    plt.savefig(fileName)
    plt.close()
    # plt.show()
print()

# Tail tail
print("Tail graphs")
if not os.path.exists(tailtail_dir):
    os.makedirs(tailtail_dir)
for timeIndex in range(0, len(tailtailX)):
    print(str(tailtailX[timeIndex]) + "    ", end = "\r")
    fig = plt.figure(figsize=(8, 6))
    ax = plt.axes()
    ax.set_xlabel('Radius')
    ax.set_ylabel('RDF')
    plt.xticks(np.arange(0, rmax + 1, 2), rotation=90)
    plt.yticks(np.arange(0, max(tailtailZ[timeIndex]) + 1, 0.1))
    ax.plot(tailtailY, tailtailZ[timeIndex])
    plt.grid(b=True, which='major', color='#cccccccc', linestyle='-')
    ax.set_title("RDF of Tail beads at Timestep " + str(tailtailX[timeIndex]))
    fileName = str(tailtail_dir) + "tail_" + str(tailtailX[timeIndex]) + ".pdf"
    plt.savefig(fileName)
    plt.close()
    # plt.show()
print()

# Tail water
print("Tail and Water graphs")
if not os.path.exists(tailWater_dir):
    os.makedirs(tailWater_dir)
for timeIndex in range(0, len(tailWaterX)):
    print(str(tailWaterX[timeIndex]) + "    ", end = "\r")
    fig = plt.figure(figsize=(8, 6))
    ax = plt.axes()
    ax.set_xlabel('Radius')
    ax.set_ylabel('RDF')
    plt.xticks(np.arange(0, rmax + 1, 2), rotation=90)
    plt.yticks(np.arange(0, max(tailWaterZ[timeIndex]) + 1, 0.1))
    ax.plot(tailWaterY, tailWaterZ[timeIndex])
    plt.grid(b=True, which='major', color='#cccccccc', linestyle='-')
    ax.set_title("RDF of Tail and Water at Timestep " + str(tailWaterX[timeIndex]))
    fileName = str(tailWater_dir) + "tail_water_" + str(tailWaterX[timeIndex]) + ".pdf"
    plt.savefig(fileName)
    plt.close()
    # plt.show()
print()

# Water water
print("Water graphs")
if not os.path.exists(waterWater_dir):
    os.makedirs(waterWater_dir)
for timeIndex in range(0, len(waterWaterX)):
    print(str(waterWaterX[timeIndex]) + "    ", end = "\r")
    fig = plt.figure(figsize=(8, 6))
    ax = plt.axes()
    ax.set_xlabel('Radius')
    ax.set_ylabel('RDF')
    plt.xticks(np.arange(0, rmax + 1, 2), rotation=90)
    plt.yticks(np.arange(0, max(waterWaterZ[timeIndex]) + 1, 0.1))
    ax.plot(waterWaterY, waterWaterZ[timeIndex])
    plt.grid(b=True, which='major', color='#cccccccc', linestyle='-')
    ax.set_title("RDF of water at Timestep " + str(waterWaterX[timeIndex]))
    fileName = str(waterWater_dir) + "water_" + str(waterWaterX[timeIndex]) + ".pdf"
    plt.savefig(fileName)
    plt.close()
    # plt.show()
print()

# Final states of head, head + tail, head + water, tail, and tail + water
print("Timestep Comparison graphs")
for timeIndex in range(0, len(waterWaterX)):
    print(str(waterWaterX[timeIndex]) + "    ", end = "\r")
    fig = plt.figure(figsize=(8, 6))
    ax = plt.axes()
    ax.set_xlabel('Radius')
    ax.set_ylabel('RDF')
    plt.xticks(np.arange(0, rmax + 1, 2), rotation=90)
    plt.gray()
    ms = []
    ms.append(max(headheadZ[timeIndex]))
    ms.append(max(headtailZ[timeIndex]))
    ms.append(max(headWaterZ[timeIndex]))
    ms.append(max(tailtailZ[timeIndex]))
    ms.append(max(tailWaterZ[timeIndex]))
    ms.append(max(waterWaterZ[timeIndex]))
    plt.yticks(np.arange(0, max(ms) + 1, 0.1))
    ax.plot(headheadY, headheadZ[timeIndex], label = "Head")
    ax.plot(headtailY, headtailZ[timeIndex], label = "Head and Tail")
    ax.plot(headWaterY, headWaterZ[timeIndex], label = "Head and Water")
    ax.plot(tailtailY, tailtailZ[timeIndex], label = "Tail")
    ax.plot(tailWaterY, tailWaterZ[timeIndex], label = "Tail and Water")
    ax.plot(waterWaterY, waterWaterZ[timeIndex], label = "Water")
    plt.grid(b=True, which='major', color='#cccccccc', linestyle='-')
    ax.set_title("Comparison of RDF at Timestep " + str(headheadX[timeIndex]))
    fileName = str(graph_out_dir) + "timestep_" + str(waterWaterX[timeIndex]) + "_comparison.pdf"
    plt.legend()
    plt.savefig(fileName)
    plt.close()
    plt.show()
