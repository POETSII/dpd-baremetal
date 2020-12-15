import json
import numpy as np

volLength = 25
min_timestep = 0
max_timestep = 5005000
emitperiod = 1000
in_dir = "../25_bond_frames/"
out_filepath = "../test.pdb"
bead_types = ["WAT", "OIL", "AIL"]
includeWater = False

volLength_str = "{:.3f}".format(volLength)

f = open(out_filepath, "w+")
print("Writing PDB preamble", end="\r")
f.write("TITLE     MDANALYSIS FRAMES FROM 0\n")
f.write("CRYST1   " + str(volLength_str) + "   " + str(volLength_str) + "   " + str(volLength_str) + "   0.00   0.00   0.00 P 1           1\n")

model_num = 1
coords = {}
types = {}

print("Converting now             ")
timestep = min_timestep
while timestep <= max_timestep:
    if (timestep == 244000):
        timestep += emitperiod
        continue
    print("Timestep " + str(timestep) + ": Loading state from JSON file", end="\r")
    in_filepath = in_dir + "state_" + str(timestep) + ".json"
    with open(in_filepath) as json_file:
        data = json.load(json_file) # Load JSON file
        for b in data["beads"]: # For each bead in the file
            bead_id = b["id"]
            bead_type = int(b["type"])
            if includeWater or (bead_type != 0):
                x_pos = b["x"]
                y_pos = b["y"]
                z_pos = b["z"]
                coords[bead_id] = {"x": x_pos, "y": y_pos, "z": z_pos}
                types[bead_id] = bead_type
    # Print bead data in PDB format
    print("Timestep " + str(timestep) + ": Printing to PDB              ", end="\r")
    f.write("MODEL        " + str(model_num) + "\n")
    counter = 1
    for b in coords:
        type = bead_types[types[b]]
        f.write("ATOM  ")
        # Atom ID
        if (counter < 10000):
            f.write(" ")
        if (counter < 1000):
            f.write(" ")
        if (counter < 100):
            f.write(" ")
        if (counter < 10):
            f.write(" ")
        f.write(str(counter))
        # Atom type
        f.write("  " + str(type[0]))
        # Atom type again (not sure whats happening here honestly)
        f.write("   " + str(type))
        # Put a 1 in there apparently
        f.write("     1")
        # Now onto coordinates
        # X
        f.write("      ")
        if (coords[b]["x"] < 10):
            f.write(" ")
        f.write("{:.3f}".format(coords[b]["x"]))
        # Y
        f.write("  ")
        if (coords[b]["y"] < 10):
            f.write(" ")
        f.write("{:.3f}".format(coords[b]["y"]))
        # Z
        f.write("  ")
        if (coords[b]["z"] < 10):
            f.write(" ")
        f.write("{:.3f}".format(coords[b]["z"]))
        # Now some other values
        f.write("  1.00  0.00           ")
        # And the type again
        f.write(str(type[0]))
        # leovnpv = str(coords[b]["x"]) + "  " + str(coords[b]["y"]) + "   " + str(coords[b]["z"]) + "  1.00  0.00           " + str(type[0]) + "\n")
        f.write("\n")
        counter += 1

    timestep += emitperiod
    model_num += 1
    f.write("ENDMDL\n")

f.write("END\n")
f.close()
print("\nComplete")
