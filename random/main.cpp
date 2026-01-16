#include "TemperatureLog.h"

//main function
int main() {
    // create temp object
    TemperatureLog log("San Frantokyo");
    // add some readings
    log.addReading(68.5);
    log.addReading(70.2);
    log.addReading(65.8);
    log.addReading(72.4);
    log.addReading(69.9);
    // print summary
    log.printSummary();
    return 0;
}