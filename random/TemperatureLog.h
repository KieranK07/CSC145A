#pragma once
#include <string>
#include <vector>

class TemperatureLog {
private:
    std::string location;
    std::vector<double> readings;

public:
    //constructor inputs a location name
    TemperatureLog(const std::string& loc) : location(loc) {}

    void addReading(double temp);

    double getAverage() const;

    double getMax() const;

    double getMin() const;

    std::string getLocation() const;

    void printSummary() const;
};
