// This is a parser for DMPCI input files for DPD simulations.
// DMPCI files are comprised of ordered key value pairs of parameters of a
// simulation, and as such, this parser reads the file, and then reads through
// the list and prepares a class to set up and build the universe and run it.

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string.hpp>
#include <vector>
#include "../inc/DPDSimulation.hpp"

// Stores each line of the input file for easier reading
std::vector<std::string> lines;
// Iterator for moving through the lines
std::vector<std::string>::iterator i;
// Class to hold parameters for the simulation
DPDSimulation sim;

// Get the title
// Strings within a DMPCI file are started and terminated with " " " (A quotation mark with a space either side)
// This needs to be detected and trimmed (in my opinion).
bool title() {
    std::string title = *i;
    if (boost::starts_with(title, "Title")) {
        title =  title.substr(5, title.size() - 5);
        boost::trim(title);
        if (boost::starts_with(title, "\" ") && boost::ends_with(title, " \"")) {
            title = title.substr(2, title.size() - 3);
            boost::trim(title);
            sim.setTitle(title);
            return true;
        } else {
            printf("ERROR: Strings must be started and terminated with \" \" \" (Whitespace, inverted comma, whitespace).\n");
            printf("The title string has been ignored\n");
            return true;
        }
    } else {
        printf("No title is provided. Continuing to parse without\n");
        return false;
    }
}

bool date() {
    std::string date = *i;
    if (boost::starts_with(date, "Date")) {
        date = date.substr(4, date.size() - 4);
        boost::trim(date);
        std::stringstream ss(date);
        std::string s;
        std::vector<std::string> dates;
        uint8_t c = 0;
        while (std::getline(ss, s, '/')) {
            dates.push_back(s);
        }
        if (dates.size() < 3 || dates.size() > 3) {
            printf("ERROR: The date format has not been recognised. It must be in the form \"DD/MM/YY\".\n");
            printf("The date will be ignored.\n");
            return true;
        }
        Date d;
        d.day = std::stoi(dates.at(0));
        d.month = std::stoi(dates.at(1));
        d.year = std::stoi(dates.at(2));
        bool valid = sim.setDate(d);
        if (!valid) {
            printf("ERROR: The date is being ignored\n");
        }
        return true;
    } else {
        printf("No date is provided. Continuing to parse without\n");
        return false;
    }
}

bool comment() {
    std::string comment = *i;
    if (boost::starts_with(comment, "Comment")) {
        comment = comment.substr(7, comment.size - 7);
        boots::trim(comment);
        if (boost::starts_with(comment, "\" ") && boost::ends_with(comment, " \"")) {

        } else if (boost::starts_with(comment, "\" ")) {

        }
    } else {
        printf("No comment is provided. Continuing to parse without\n");
        return false;
    }
}

// Empty lines are used to separate sections of parameters and can be ignored
std::vector<std::string>::iterator getNextLine() {
    i = std::next(i);
    while (*i == "") {
        i = std::next(i);
    }
    return i;
}

int main(int argc, char *argv[]) {

    if (argc < 2) {
        printf("Not enough arguments. Please provide an input filepath\n");
        return 1;
    } else if (argc > 2) {
        printf("Too many arguments. Please provide an input filepath ONLY\n");
        return 1;
    }

    std::string filepath;
    filepath = argv[1];

    // std::cout << "Filepath: " << filepath.c_str() << "\n";

    // Input file stream to read the input file one line at a time
    std::ifstream inputFile(filepath);
    // This string holds the current line
    std::string line;

    // Store each line of a file in a vector to iterator over that.
    // Going back and forth in a file is much more difficult
    while(std::getline(inputFile, line)) {
        boost::trim(line);
        lines.push_back(line);
    }

    i = lines.begin();

    // First line must contain "dpd" only.
    if (*i != "dpd") {
        printf("First line of input file is not \"dpd\". This is required and will not parse correctly otherwise\n");
        return 1;
    }

    i = getNextLine();
    bool needToGetNextLine = true;

    // Try to get the title
    needToGetNextLine = title();

    if (needToGetNextLine) {
        i = getNextLine();
    }

    // Try to get the date
    needToGetNextLine = date();

    if (needToGetNextLine) {
        i = getNextLine();
    }

    // Try to get the comment
    needToGetNextLine = comment();

    if (needToGetNextLine) {
        i = getNextLine();
    }

    std::cout << *i << "\n";
}