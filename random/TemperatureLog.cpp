#include "TemperatureLog.h"
#include <iostream>

// add readings funct
void TemperatureLog::addReading(double temp) {
    readings.push_back(temp);
}

double TemperatureLog::getAverage() const {
    // calc average temperatur
    if (readings.empty()) return 0.0;
    double sum = 0.0;
    // loop through to calc
    for (double temp : readings) {
        sum += temp;
    }
    return sum / readings.size();
}

// get max temperature
double TemperatureLog::getMax() const {
    if (readings.empty()) return 0.0;
    // assume first is max
    double maxTemp = readings[0];
    for (double temp : readings) {
        if (temp > maxTemp) {
            maxTemp = temp;
        }
    }
    return maxTemp;

}

// get min temperature
double TemperatureLog::getMin() const {
    if (readings.empty()) return 0.0;
    // again assume first is the min
    double minTemp = readings[0];
    for (double temp : readings) {
        if (temp < minTemp) {
            minTemp = temp;
        }
    }
    return minTemp;
}

// get loc name
std::string TemperatureLog::getLocation() const {
    return location;
}

// print summary of temp log
void TemperatureLog::printSummary() const {
    std::cout << "temp log for " << location << ":\n";
    std::cout << "Number of readings: " << readings.size() << "\n";
    if (!readings.empty()) {
        std::cout << "average temp: " << getAverage() << "\n";
        std::cout << "max temp: " << getMax() << "\n";
        std::cout << "min temp: " << getMin() << "\n";
    } else {
        std::cout << "nothing available\n";
    }
}



