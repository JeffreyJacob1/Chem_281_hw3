#ifndef CHEMISTRY_UTILS_H
#define CHEMISTRY_UTILS_H

#include <string>

double calculateDensity(double mass, double volume);
double calculateMolarMass(const std::string& element);
std::string getElementName(int atomicNumber);
int getAtomicNumber(const std::string& element);

#endif