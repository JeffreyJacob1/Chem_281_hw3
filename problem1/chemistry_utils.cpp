#include "chemistry_utils.h"
#include <iostream>

double calculateDensity(double mass, double volume) {
    return mass / volume;
}

double calculateMolarMass(const std::string& element) {
    // Simulate calculating molar mass based on element
    if (element == "Hydrogen")
        return 1.008;
    else if (element == "Carbon")
        return 12.011;
    else if (element == "Oxygen")
        return 15.999;
    else
        return 0.0;
}

std::string getElementName(int atomicNumber) {
    // Simulate getting element name based on atomic number
    switch (atomicNumber) {
        case 1:
            return "Hydrogen";
        case 6:
            return "Carbon";
        case 8:
            return "Oxygen";
        default:
            return "Unknown";
    }
}

int getAtomicNumber(const std::string& element) {
    // Simulate getting atomic number based on element name
    if (element == "Hydrogen")
        return 1;
    else if (element == "Carbon")
        return 6;
    else if (element == "Oxygen")
        return 8;
    else
        return 0;
}