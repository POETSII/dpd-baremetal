import shutil


class MacroFlag:
    flag_string = ""
    makefile_string = ""
    clashing_flags = []

    def __init__(self, flag_string, makefile_string, *args, **kwargs):
        self.flag_string = flag_string
        self.makefile_string = makefile_string
        self.clashing_flags = []

    def add_clashing_flags(self, flags):
        self.clashing_flags += flags

    def __str__(self):
        return self.makefile_string

    def __repr__(self):
        return str(self)


class SimulatorFlag(MacroFlag):
    sim_recipe = ""
    objs = ""
    metis = ""

    def __init__(self, sim_recipe, objs, metis, *args, **kwargs):
        self.sim_recipe = sim_recipe
        self.objs = objs
        self.metis = metis
        super().__init__(*args, **kwargs)


class ExampleFlag(MacroFlag):
    filename = ""

    def __init__(self, filename, *args, **kwargs):
        self.filename = filename
        super().__init__(*args, **kwargs)


# Contains the pre-arranged recipes for the source files
makefile_helper_path = "./Makefile.txt"
# The location of the new Makefile
new_makefile_path = "./Makefile"

# Copy Makefile helper and we'll add recipes to this
shutil.copyfile(makefile_helper_path, new_makefile_path)

# *****************************Create the flags********************************

# Simulator choices
sync = SimulatorFlag("base-sync", "POLITE_OBJS", "-lmetis", "", "sync")
gals = SimulatorFlag("base-gals", "POLITE_OBJS", "-lmetis", "-DGALS", "gals")
improved_gals = SimulatorFlag("base-gals", "POLITE_OBJS", "-lmetis",
                              "-DGALS -DIMPROVED_GALS", "improvedgals")
serial = SimulatorFlag("serial", "SERIAL_OBJS", "", "-DSERIAL", "serial")

simulators = [sync, gals, improved_gals, serial]

# Operations
visual = MacroFlag("-DVISUALISE", "visual")
timed = MacroFlag("-DTIMER", "timed")
stats = MacroFlag("-DSTATS", "stats")
testing = MacroFlag("-DTESTING", "test")

operations = [visual, timed, stats]
# Don't include testing that gets its own section

# Test flags
default = MacroFlag("", "")
large_test = MacroFlag("-DLARGE_TEST", "large")
bonds = MacroFlag("-DBONDS -DVESICLE_SELF_ASSEMBLY", "bonds")

test_flags = [default, large_test, bonds]

# DRAM flag
dram = MacroFlag("-DDRAM", "dram")

# Examples
oilwater = ExampleFlag("oilWater", "", "oilwater")
vesicle = ExampleFlag("VesicleSelfAssembly",
                      "-DVESICLE_SELF_ASSEMBLY -DBONDS",
                      "vesicle", )
corners = ExampleFlag("corner-tests", "", "corners")
gravity = ExampleFlag("BoxOilWaterGravity",
                      "-DGRAVITY", "gravity")

examples = [oilwater, vesicle, corners, gravity]

# Local calculation options
onebyone = MacroFlag("-DONE_BY_ONE", "onebyone")
sendtoself = MacroFlag("-DSEND_TO_SELF", "sendtoself")

local_calcs = [default, onebyone, sendtoself]

# Features
improved_verlet = MacroFlag("-DBETTER_VERLET", "betterverlet")
small_dt_early = MacroFlag("-DSMALL_DT_EARLY", "dtchange")
double_sqrt = MacroFlag("-DDOUBLE_SQRT", "doublesqrt")
msg_mgmt = MacroFlag("-DMESSAGE_MANAGEMENT", "msgmgmt")
float_only = MacroFlag("-DFLOAT_ONLY", "floatonly")
reduced_local_calcs = MacroFlag("-DREDUCED_LOCAL_CALCS", "reducedlocalcalcs")
single_force_loop = MacroFlag("-DSINGLE_FORCE_LOOP", "singleforceloop")
singleloopreduced_flag = "-DSINGLE_FORCE_LOOP -DREDUCED_LOCAL_CALCS"
single_reduced_force_loop = MacroFlag(singleloopreduced_flag,
                                      "singlereducedforceloop")

features = [improved_verlet, small_dt_early, double_sqrt,
            msg_mgmt, float_only, reduced_local_calcs,
            single_force_loop, single_reduced_force_loop]

# All the macros leads to way too many options.
# Lets' split it into:
# 1. Best result
# 2. Fastest combination
# 3. Smallest combination (in terms of instruction space).
# Best result *should* be used on everything
# Fastest and Smallest will be the two options
# Each of these will be applied to every simulator, operation macro and
# example, so will include some that don't apply to every simulator.
# Testing will still have everything in combination for full regression testing
# PLEASE UPDATE THIS IF YOU HAVE A NEW MACRO AND REGENERATE THE MAKEFILE

best_result = [improved_verlet, small_dt_early]
fastest = [onebyone, reduced_local_calcs]
smallest = [sendtoself, single_force_loop]

# ********************State what can't be used together***********************
# Simulator choices
sync.add_clashing_flags([gals, serial, improved_gals])
gals.add_clashing_flags([sync, serial, sendtoself])
improved_gals.add_clashing_flags([sync, serial, sendtoself])
serial.add_clashing_flags([sync, gals, stats, onebyone,
                           sendtoself, improved_gals, msg_mgmt])

# Operations
visual.add_clashing_flags([timed, stats, testing, large_test])
timed.add_clashing_flags([visual, stats, testing, large_test])
stats.add_clashing_flags([serial, visual, timed, testing, large_test])
testing.add_clashing_flags([visual, timed, stats, float_only])

# Testing flags
large_test.add_clashing_flags([visual, timed, stats, bonds])

# DRAM (can work with anything, but isn't used for testing)
dram.add_clashing_flags([testing, large_test])

# Examples
oilwater.add_clashing_flags([testing, large_test, vesicle, corners, gravity])
vesicle.add_clashing_flags([testing, large_test, oilwater, corners, gravity])
corners.add_clashing_flags([testing, large_test, oilwater, vesicle, gravity])
gravity.add_clashing_flags([testing, large_test, oilwater, vesicle, corners])

# Local calculation options
# default.add_clashing_flags([])
onebyone.add_clashing_flags([serial, ])
sendtoself.add_clashing_flags(
    [serial, reduced_local_calcs, gals, improved_gals])

# Features
bonds.add_clashing_flags([large_test])
improved_verlet.add_clashing_flags([])
small_dt_early.add_clashing_flags([])
double_sqrt.add_clashing_flags([])
msg_mgmt.add_clashing_flags([serial])
float_only.add_clashing_flags([testing])
reduced_local_calcs.add_clashing_flags([sendtoself])
single_force_loop.add_clashing_flags([])

# Open the file
makefile = open(new_makefile_path, "a+")

# Now let's start by putting the recipes for the examples
makefile.write("\n# ************** EXAMPLES **************\n")

for example in examples:
    makefile.write("""
{makefile_string}: DFLAGS+={flag_string}
{makefile_string}: $(DPD_EXAMPLES)/{filename}
\tg++ -O2 -std=c++11 $(DFLAGS) $(EXTERNAL_FLAGS) \\
\t-I $(HL) -I $(DPD_INC) -c -o $(DPD_BIN)/{filename}.o \
$(DPD_EXAMPLES)/{filename}.cpp
\tg++ -O2 -std=c++11 -o $(DPD_BIN)/run $(OBJS) $(DPD_BIN)/{filename}.o \\
\t\t-static-libgcc -static-libstdc++ \
\t\t-Wl,-rpath, $(METIS) -lpthread -lboost_program_options \\
\t\t-lboost_filesystem -lboost_system -fopenmp
""".format(makefile_string=example.makefile_string,
           flag_string=example.flag_string,
           filename=example.filename))

# Store all testing recipes so we can run all tests
testing_recipes = []


def clash(flags_a, flags_b):
    # Get the intersection of the flags and clashes
    # If this is empty there is no clash
    clashes = list(set(flags_a) & set(flags_b))
    return (clashes != [])


# Now for the recipes for individual simulators
for simulator in simulators:
    makefile.write("\n# ************** Simulator: ")
    makefile.write(f"{simulator.makefile_string}**************\n")
    for operation in operations:
        if clash(operation.clashing_flags, [simulator]):
            continue
        sim_op_recipe = simulator.makefile_string + "-"
        sim_op_recipe += operation.makefile_string
        makefile.write("\n# **********Operation: ")
        makefile.write(f"{operation.makefile_string}**********\n\n")
        makefile.write(sim_op_recipe + ": ")
        makefile.write(f"OBJS+=$({simulator.objs})\n")
        makefile.write(sim_op_recipe + ": ")
        makefile.write(f"METIS={simulator.metis}\n")
        makefile.write(f"{sim_op_recipe}: ")
        makefile.write(f"DFLAGS+={operation.flag_string}\n")
        makefile.write(sim_op_recipe + ": ")
        makefile.write(f"$({simulator.objs}) ")
        makefile.write(f"{simulator.sim_recipe}\n")
        for example in examples:
            if clash(example.clashing_flags, [simulator, operation]):
                continue

            makefile.write("\n# ******Example: ")
            makefile.write(f"{example.makefile_string}******\n\n")
            sim_op_example_recipe = f"{sim_op_recipe}-"
            sim_op_example_recipe += f"{example.makefile_string}"
            makefile.write(f"{sim_op_example_recipe}: ")
            makefile.write(f"DFLAGS+={example.flag_string}\n")
            makefile.write(f"{sim_op_example_recipe}: ")
            makefile.write(f"{sim_op_recipe} {example.makefile_string}\n\n")

            # Build the fastest and smallest recipes
            fastest_flags = ""
            smallest_flags = ""

            if clash(best_result, [simulator, operation, example]):
                continue
            if clash(fastest, [simulator, operation, example]):
                continue
            if clash(smallest, [simulator, operation, example]):
                continue

            # Best are all used in fastest and smallest
            for best_flag in best_result:
                fastest_flags += best_flag.flag_string + " "
                smallest_flags += best_flag.flag_string + " "

            # Fastest
            for fast in fastest:
                fastest_flags += fast.flag_string + " "

            # Smallest
            for small in smallest:
                smallest_flags += small.flag_string + " "

            fastest_flags.strip()
            smallest_flags.strip()

            # Write fastest
            makefile.write(f"{sim_op_example_recipe}-fastest: ")
            makefile.write(f"DFLAGS+={fastest_flags}\n")
            makefile.write(f"{sim_op_example_recipe}-fastest: ")
            makefile.write(f"{sim_op_example_recipe}\n\n")

            # Write the smallest
            makefile.write(f"{sim_op_example_recipe}-smallest: ")
            makefile.write(f"DFLAGS+={smallest_flags}\n")
            makefile.write(f"{sim_op_example_recipe}-smallest: ")
            makefile.write(f"{sim_op_example_recipe}\n\n")


# Write testing
makefile.write("#****************** REGRESSION TESTING ******************")
for simulator in simulators:
    makefile.write("\n# ************** TEST Simulator: ")
    makefile.write(f"{simulator.makefile_string}**************\n\n")
    test_sim_recipe = f"test-{simulator.makefile_string}"
    makefile.write(f"{test_sim_recipe}: ")
    makefile.write(f"OBJS=$({simulator.objs})\n")
    makefile.write(f"{test_sim_recipe}: ")
    makefile.write(f"METIS={simulator.metis}\n")
    makefile.write(f"{test_sim_recipe}: ")
    makefile.write(f"DFLAGS+={testing.flag_string} ")
    makefile.write(f"{simulator.flag_string}\n")
    makefile.write(f"{test_sim_recipe}: ")
    makefile.write(f"$({simulator.objs}) ")
    makefile.write(f"{simulator.sim_recipe} ")
    makefile.write(f"test\n\n")
    testing_recipes.append(test_sim_recipe)
    # Test the testing options (large and bonds)
    for test_flag in test_flags:
        if clash(test_flag.clashing_flags, [testing, simulator]):
            continue
        recipe_with_test_flag = test_sim_recipe
        if test_flag.makefile_string != "":
            recipe_with_test_flag += "-" + test_flag.makefile_string
            makefile.write(f"{recipe_with_test_flag}: ")
            makefile.write(f"DFLAGS+={test_flag.flag_string}\n")
            makefile.write(f"{recipe_with_test_flag}: ")
            makefile.write(f"{test_sim_recipe}\n\n")
            testing_recipes.append(recipe_with_test_flag)

        # Test each of the local calculation options
        for local_calc in local_calcs:
            if clash(local_calc.clashing_flags,
                     [testing, simulator, test_flag]):
                continue
            recipe_with_loc = f"{recipe_with_test_flag}"
            if local_calc.makefile_string != "":
                makefile.write("# **TEST Local calculation method: ")
                makefile.write(f"{local_calc.makefile_string}**\n")
                recipe_with_loc += f"-{local_calc.makefile_string}"
                makefile.write(f"{recipe_with_loc}: ")
                makefile.write(f"DFLAGS+={local_calc.flag_string}\n")
                makefile.write(f"{recipe_with_loc}: ")
                makefile.write(f"{recipe_with_test_flag}\n\n")
                testing_recipes.append(recipe_with_loc)

            for feature in features:
                if clash(feature.clashing_flags,
                         [testing, simulator, test_flag, local_calc]):
                    continue
                new_recipe = recipe_with_loc + "-"
                new_recipe += feature.makefile_string
                makefile.write(f"{new_recipe}: ")
                makefile.write(f"DFLAGS+={feature.flag_string}\n")
                makefile.write(f"{new_recipe}: ")
                makefile.write(f"{recipe_with_loc}\n\n")
                testing_recipes.append(new_recipe)

# Close the file
makefile.close()

# Write all test options to a file so we can run them
test_file = open("test-options.txt", "w+")

for test in testing_recipes:
    test_file.write(test + "\n")

test_file.close()
