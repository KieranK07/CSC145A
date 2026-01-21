#include <iostream>
#include <vector>
using namespace std;

//inputs a sorted vector and a target value
bool divideandConquer(const vector<int>& arr, int target, int left, int right) {
    if (left > right) {
        return false; // base is target no can findy
    }
    int mid = left + (right - left) / 2;
    if (arr[mid] == target) {
        return true; // target foundy
    } else if (arr[mid] > target) {
        return divideandConquer(arr, target, left, mid - 1); // search left 
    } else {
        return divideandConquer(arr, target, mid + 1, right); // search right
    }
}

int main() {
    vector<int> sortedArray = {1, 3, 5, 7, 9, 11, 13, 15, 17, 19};
    int target = 7;

    if (divideandConquer(sortedArray, target, 0, sortedArray.size() - 1)) { // bc it needs to just search the array minus the one
        cout << "Found " << target << " in the array" << endl;
    } else {
        cout << "Couldn't find  " << target << "  in the array" << endl;
    }

    return 0;
}