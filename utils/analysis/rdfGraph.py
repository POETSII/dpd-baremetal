from mpl_toolkits.mplot3d import axes3d
import matplotlib.pyplot as plt
import numpy as np
import csv

# Read all data from file and generate axes and series
filepath = "3_rdf_dr_0.05.csv"

X = []
Y = []
Z = []
with open(filepath) as csv_file:
    firstLine = False
    csv_reader = csv.reader(csv_file, delimiter=',')
    m = 0
    for row in csv_reader:
        # First line is one axis
        if not firstLine:
            for i in range(1, len(row)):
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
        # if m > 100:
        #     break

fig, ax1 = plt.subplots(1, 1, subplot_kw={'projection': '3d'})

# Get the test data
# X, Y, Z = axes3d.get_test_data(0.05)
X = np.array(X)
X.shape = (len(X),1)
Y = np.array(Y)
Z = np.array(Z)

# Give the first plot only wireframes of the type y = c
ax1.plot_surface(X, Y, Z, cmap=plt.cm.RdYlGn)
# ax1.plot_wireframe(X, Y, Z)
plt.xlabel("Timestep")
plt.ylabel("Radius")

ax1.set_xlabel('Timestep')
ax1.set_ylabel('Radius')
ax1.set_zlabel('RDF')

# plt.tight_layout()
plt.show()