import json
import numpy as np

volLength = 11
min_timestep = 0
max_timestep = 2000
emitperiod = 1
in_dir = "../serial-dpd-states/"
out_filepath = "../serial-dpd-test.pdb"
bead_types = ["WWW", "OOO", "AAA"]
includeWater = True
waterTypeNum = 0
append = False

volLength_str = "{:.3f}".format(volLength)

if not append:
    f = open(out_filepath, "w+")
    print("Writing PDB preamble", end="\r")
    f.write("TITLE     MDANALYSIS FRAMES FROM 0\n")
    f.write("CRYST1   " + str(volLength_str) + "   " + str(volLength_str) + "   " + str(volLength_str) + "   0.00   0.00   0.00 P 1           1\n")
else:
    f = open(out_filepath, "a+")

model_num = 0
coords = {}
types = {}

def increment_id(id):
    for i in range (0, len(id)):
        c = id[i]
        if c == '9':
            id[i] = 'A'
        elif c == 'Z':
            id[i] = 'a'
        elif c == 'z':
            id[i] = '0'
            if len(id) == (i + 1):
                id.append('0')
            else:
                continue
        else:
            id[i] = chr(ord(c) + 1)

        return

print("Converting now             ")
timestep = min_timestep
while timestep <= max_timestep:
    print("Timestep " + str(timestep) + ": Loading state from JSON file", end="\r")
    in_filepath = in_dir + "state_" + str(timestep) + ".json"

    with open(in_filepath) as json_file:
        data = json.load(json_file) # Load JSON file
        for b in data["beads"]: # For each bead in the file
            bead_id = b["id"]
            bead_type = int(b["type"])
            if includeWater or (bead_type != waterTypeNum):
                x_pos = b["x"]
                y_pos = b["y"]
                z_pos = b["z"]
                coords[bead_id] = {"x": x_pos, "y": y_pos, "z": z_pos}
                types[bead_id] = bead_type
    # Print bead data in PDB format
    print("Timestep " + str(timestep) + ": Printing to PDB              ", end="\r")
    f.write("MODEL        " + str(model_num) + "\n")
    bead_id = ['0']
    for b, v in sorted(coords.items()):
        type = bead_types[types[b]]
        f.write("ATOM  ")
        # Atom ID
        if (len(bead_id) < 5):
            f.write(" ")
        if (len(bead_id) < 4):
            f.write(" ")
        if (len(bead_id) < 3):
            f.write(" ")
        if (len(bead_id) < 2):
            f.write(" ")
        for c in bead_id:
            f.write(str(c))
        # Atom type
        f.write("  " + str(type[0]) + str(type[1]))
        # Atom type again (not sure whats happening here honestly)
        f.write("  " + str(type))
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
        increment_id(bead_id)

    timestep += emitperiod
    model_num += 1
    f.write("ENDMDL\n")

f.write("END\n")
f.close()
print("\nComplete")
