#include <stdio.h>

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

class DPDSimulation {
    private:
        std::string _title;
        Date _date;

    public:
        void setTitle(std::string title);
        std::string getTitle();

        bool setDate(Date date);
        Date getDate();
};

#include "../src/DPDSimulation.cpp"