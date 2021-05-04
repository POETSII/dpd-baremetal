import json
import numpy as np

volLength = 3
min_timestep = 1
max_timestep = 10000
emitperiod = 1
xml_dir = "/home/jrbeaumont/xml-dpd-states/"
pol_dir = "/home/jrbeaumont/polite-dpd-states/"

print("Checking similarity of POLite and XML DPD")

xml_pos = {}
xml_vel = {}
xml_types = {}

timestep = min_timestep
while timestep <= max_timestep:
    print("Timestep " + str(timestep) + ": Loading XML state")
    xml_path = xml_dir + "state_" + str(timestep) + ".json"
    pol_path = pol_dir + "state_" + str(timestep) + ".json"

    with open(xml_path) as json_file:
        data = json.load(json_file) # Load JSON file
        for b in data["beads"]: # For each bead in the file
            xml_bead_id = b["id"]
            xml_pos[xml_bead_id] = {"x": float(b["x"]), "y": float(b["y"]), "z": float(b["z"])}
            xml_vel[xml_bead_id] = {"x": float(b["vx"]), "y": float(b["vy"]), "z": float(b["vz"])}
            xml_types[xml_bead_id] = int(b["type"])

    print("Timestep " + str(timestep) + ": Loading POLite state")
    with open(pol_path) as json_file:
        data = json.load(json_file) # Load JSON file
        for b in data["beads"]: # For each bead in the file
            fail = False
            pol_bead_id = b["id"]
            pol_bead_type = int(b["type"])
            pol_pos = {"x": float(b["x"]), "y": float(b["y"]), "z": float(b["z"])}
            pol_vel = {"x": float(b["vx"]), "y": float(b["vy"]), "z": float(b["vz"])}

            # Compare this bead to XML version
            if pol_bead_type != xml_types[pol_bead_id]:
                fail = True
            elif pol_pos["x"] != xml_pos[pol_bead_id]["x"]:
                fail = True
            elif pol_pos["y"] != xml_pos[pol_bead_id]["y"]:
                fail = True
            elif pol_pos["z"] != xml_pos[pol_bead_id]["z"]:
                fail = True
            elif pol_vel["x"] != xml_vel[pol_bead_id]["x"]:
                fail = True
            elif pol_vel["y"] != xml_vel[pol_bead_id]["y"]:
                fail = True
            elif pol_vel["z"] != xml_vel[pol_bead_id]["z"]:
                fail = True

            if fail:
                print("Divergence detected in bead " + str(pol_bead_id))
                print("POLite type = " + str(pol_bead_type) + ", XML type = " + str(xml_types[pol_bead_id]))
                print("POLite position = (" + str(pol_pos["x"]) + ", " + str(pol_pos["y"]) + ", " + str(pol_pos["z"]) + ")")
                print("XML position    = (" + str(xml_pos[pol_bead_id]["x"]) + ", " + str(xml_pos[pol_bead_id]["y"]) + ", " + str(xml_pos[pol_bead_id]["z"]) + ")")
                print("POLite velocity = (" + str(pol_vel["x"]) + ", " + str(pol_vel["y"]) + ", " + str(pol_vel["z"]) + ")")
                print("XML velocity    = (" + str(xml_vel[pol_bead_id]["x"]) + ", " + str(xml_vel[pol_bead_id]["y"]) + ", " + str(xml_vel[pol_bead_id]["z"]) + ")")
                input()


    timestep += emitperiod

print("\nComplete")
