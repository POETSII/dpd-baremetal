import json
import math
import sys
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

print("Loading force values")
filepath = "../force-2147488052.csv"
filepath1 = "../force-calc-2147488052.csv"
# filepath = "../force-2147483648.csv"
# filepath1 = "../force-calc-2147483648.csv"
timestep = 1

with open(filepath) as f:
    with open(filepath1) as g:
        line = f.readline()
        line1 = g.readline()
        # For each line
        while line:
            print("Timestep " + str(timestep), end = "\r")
            forces = line.split(',')
            forces1 = line1.split(',')
            index = 0
            force_num = 0
            force_nums = []
            x_values = []
            y_values = []
            z_values = []
            x_acc_values = []
            y_acc_values = []
            z_acc_values = []
            x_acc = 0
            y_acc = 0
            z_acc = 0
            while forces[index] != " \n":
                x = float(forces[index].strip())
                x_acc += float(forces1[index].strip())
                index += 1
                y = float(forces[index].strip())
                y_acc += float(forces1[index].strip())
                index += 1
                z = float(forces[index].strip())
                z_acc += float(forces1[index].strip())
                index += 1
                force_nums.append(force_num)
                x_values.append(x)
                y_values.append(y)
                z_values.append(z)
                x_acc_values.append(x_acc)
                y_acc_values.append(y_acc)
                z_acc_values.append(z_acc)
                force_num += 1
            data_frame = pd.DataFrame()
            data_frame["Force num"] = force_nums
            data_frame["x"] = x_values
            data_frame["y"] = y_values
            data_frame["z"] = z_values
            data_frame["x_acc"] = x_acc_values
            data_frame["y_acc"] = y_acc_values
            data_frame["z_acc"] = z_acc_values
            if timestep >= 1900:
                ax = plt.gca()
                ax = data_frame.plot(kind = 'line', x = 'Force num', y = "x", ax = ax, linewidth=1, figsize=(8, 5))
                ax = data_frame.plot(kind = 'line', x = 'Force num', y = "y", ax = ax, linewidth=1, figsize=(8, 5))
                ax = data_frame.plot(kind = 'line', x = 'Force num', y = "z", ax = ax, linewidth=1, figsize=(8, 5))
                ax = data_frame.plot(kind = 'line', x = 'Force num', y = "x_acc", ax = ax, linewidth=1, figsize=(8,5))
                ax = data_frame.plot(kind = 'line', x = 'Force num', y = "y_acc", ax = ax, linewidth=1, figsize=(8,5))
                ax = data_frame.plot(kind = 'line', x = 'Force num', y = "z_acc", ax = ax, linewidth=1, figsize=(8,5))
                ax.set_title("Timestep " + str(timestep))
                plt.show()
            timestep += 1
            line = f.readline()
            line1 = g.readline()
# Close the input file
f.close()


