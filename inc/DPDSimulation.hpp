#include <stdio.h>
#include <map>

typedef struct Date {
    uint8_t day;
    uint8_t month;
    uint8_t year;

    bool validate() {
        // Year must be 2 digits
        if (year > 99) {
            printf("DATE ERROR: Year is greater than 99. Year must be two digits.\n");
            return false;
        }

        // Year should ideally be two digits, but it's a pain to ensure that
        if (month < 1 || month > 12) {
            printf("DATE ERROR: Month must be two digits, and any number from 1 to 12 (inclusive).\n");
            return false;
        }

        // Day is complicated based on the month and year etc, but we'll give it a simple try
        if (day < 1) {
            printf("DATE ERROR: Day must be any number from 1 to 31 (inclusive) depending on the month\n");
            return false;
        }

        switch (month) {
            case 1:
            case 3:
            case 5:
            case 7:
            case 8:
            case 10:
            case 12: {
                if (day > 31) {
                    printf("DATE ERROR: The day is too large for the given month, it must be less than or equal to 31\n");
                    return false;
                }
                break;
            }

            case 4:
            case 6:
            case 9:
            case 11: {
                if (day > 30) {
                    printf("DATE ERROR: The day is too large for the given month, it must be less than or equal to 30\n");
                    return false;
                }
                break;
            }

            // Just for fun, why not?
            // https://support.microsoft.com/en-gb/help/214019/method-to-determine-whether-a-year-is-a-leap-year
            case 2: {
                bool leap = false;
                if ((year % 4) == 0) {
                    if ((year % 100) == 0) {
                        if ((year % 400) == 0) {
                            leap = true;
                        }
                    } else {
                        leap = true;
                    }
                }
                if (leap) {
                    if (day > 29) {
                        printf("DATE ERROR: The day is too large for the February of the given year\n");
                        printf("The given year is a leap year, allowing 29 days in February, but the day given is too large\n");
                        return false;
                    }
                } else {
                    if (day > 28) {
                        printf("DATE ERROR: The day is too large for the February of the given year\n");
                        printf("The given year is NOT a leap year, allowing ONLY 28 days in February, but the day given is too large\n");
                        return false;
                    }
                }
                break;
            }

        }
        return true;
    }
} Date;

typedef enum { RANDOM, RESTART, LAMELLA, COMPOSITELAMELLA } initial_state;

typedef char bead_type_id;

typedef uint8_t bead_type_enum;

typedef struct Bead_type {
    float radius;
    bead_type_enum type;
    float fraction;
} Bead_type;

typedef struct Bond_type {
    bead_type_id bead1;
    bead_type_id bead2;
    float hookean_constant;
    float spring_length;
} Bond_type;

typedef struct Stiff_bond_type {
    bead_type_id bead1;
    bead_type_id bead2;
    bead_type_id bead3;
    float bending_constant;
    float preferred_angle;
} Stiff_bond_type;

typedef enum { CHAIN, BEAD, BRANCH, LOOP } polymer_type;

typedef struct Polymer_structure {
    polymer_type type;
    std::vector<Polymer_structure> elements;
    int loopNum;
    bead_type_id bead_type;
} Polymer_structure;

typedef struct Polymer {
    std::string name;
    float fraction;
    std::string structure_string;
    Polymer_structure structure;
    // TODO: structure parsed in some way
} Polymer;

class DPDSimulation {
    private:
        std::string _title;
        Date _date;
        std::string _comment;
        initial_state _state;
        std::map<bead_type_id, Bead_type> _bead_types;
        std::vector<std::vector<float>> _conservative_parameters;
        std::vector<std::vector<float>> _dissipative_parameters;
        std::vector<Bond_type> _bond_types;
        std::vector<Stiff_bond_type> _stiff_bond_types;
        std::vector<Polymer> _polymers;

    public:
        // Using vectors so just making it easier to combine polymers
        std::vector<Polymer_structure> combinePolymers(std::vector<Polymer_structure> a, std::vector<Polymer_structure> b);

        // Useful function to parse the structure string of a polymer
        std::tuple<Polymer_structure, int> parsePolymerStructure(std::string s, int start);

        // Setters and getters
        void setTitle(std::string title);
        std::string getTitle();

        bool setDate(Date date);
        Date getDate();

        void setComment(std::string comment);
        std::string getComment();

        void setInitialState(initial_state state);
        initial_state getInitialState();

        void addBeadType(bead_type_id id, Bead_type beadType);
        std::map<bead_type_id, Bead_type> getBeadTypes();

        void addConservativeParameters(std::vector<float> conservativeParameter);
        std::vector<std::vector<float>> getConservativeParameters();

        void addDissipativeParameters(std::vector<float> dissipativeParameter);
        std::vector<std::vector<float>> getDissipativeParameters();

        void addBondType(Bond_type bondType);
        std::vector<Bond_type> getBondTypes();

        void addStiffBondType(Stiff_bond_type stiffBondType);
        std::vector<Stiff_bond_type> getStiffBondTypes();

        void addPolymer(Polymer p);
        std::vector<Polymer> getPolymers();
};

#include "../src/DPDSimulation.cpp"