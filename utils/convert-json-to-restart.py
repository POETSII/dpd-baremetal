import json

volLength = 100
timestep = 719000
in_filepath = "../" + str(volLength) + "_bond_frames/state_" + str(timestep) + ".json"
out_filepath = "../restart_" + str(timestep) + ".csv"
f = open(out_filepath, "w+")

print("Writing CSV file preamble")
f.write("VOLUME DIMENSIONS\n")
f.write(str(volLength) + ", " + str(volLength) + ", " + str(volLength) + "\n")
f.write("TIMESTEP\n")
f.write(str(timestep) + "\n")
f.write("BONDED BEAD IDS\n")
f.write("BEAD POSITIONS\n")

print("Loading JSON state file")
count = 1
with open(in_filepath) as json_file:
    data = json.load(json_file) # Load JSON file
    for b in data["beads"]: # For each bead in the file
        print("Converting bead " + str(count) + " of " + str(len(data["beads"])), end = "\r")
        bead_id = b["id"]
        bead_type = b["type"]
        x_pos = b["x"]
        y_pos = b["y"]
        z_pos = b["z"]
        x_vel = b["vx"]
        y_vel = b["vy"]
        z_vel = b["vz"]
        f.write(str(bead_id) + ", " + str(bead_type) + ", " + str(x_pos) + ", " + str(y_pos) + ", " + str(z_pos) + ", " + str(x_vel) + ", " + str(y_vel) + ", " + str(z_vel) + "\n")
        count += 1

f.close()
print("\nComplete")
