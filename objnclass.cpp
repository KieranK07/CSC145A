#include <iostream>
using namespace std;

// Simple Parking Meter

class ParkingMeter {
    private:
        int minutesRemaining;
    public:
        //constructor
        ParkingMeter() : minutesRemaining(0) {}

        void addTime(int minutes) {
            if (minutes > 0) {
                minutesRemaining += minutes;
            }
        }

        void tick() {
            if (minutesRemaining > 0) {
                minutesRemaining--;
            }
        }   
        
        bool isExpired() {
            if (minutesRemaining == 0) {
                return true;
            } else {
                return false;  
            }
        }

        int print() {
            return minutesRemaining;
        }
};




int main() {
    //create parking meter at 5 min
    ParkingMeter meter;
    meter.addTime(5);

    //call tick 3 times
    meter.tick();
    meter.tick();
    meter.tick();

    //add 2 min of time
    meter.addTime(2);

    //call tick, print, and isExpired until the parking meter has expired
    while (!meter.isExpired()) {
        meter.tick();
        cout << "Minutes remaining: " << meter.print() << endl;
    }
}