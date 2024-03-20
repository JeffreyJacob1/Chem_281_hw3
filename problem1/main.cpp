#include <iostream>
#include "chemistry_utils.h"

int main(int argc, char *argv[]) {
    double mass = 10.0;
    double volume = 5.0;
    std::string element = "Hydrogen";
    int atomicNumber = 1;

    std::cout << "Density: " << calculateDensity(mass, volume) << " g/mL" << std::endl;
    std::cout << "Molar Mass of " << element << ": " << calculateMolarMass(element) << " g/mol" << std::endl;
    std::cout << "Element Name: " << getElementName(atomicNumber) << std::endl;
    std::cout << "Atomic Number of " << element << ": " << getAtomicNumber(element) << std::endl;

    return 0;
}