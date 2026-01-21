#include <iostream>
#include <vector>
using namespace std;

//inputs a sorted vector and a target value
bool binarySearch(const vector<int>& arr, int target, int left, int right) {
    if (left > right) {
        return false; // base is target no can findy
    }
    int mid = left + (right - left) / 2;
    if (arr[mid] == target) {
        return true; // target foundy
    } else if (arr[mid] > target) {
        return binarySearch(arr, target, left, mid - 1); // search left 
    } else {
        return binarySearch(arr, target, mid + 1, right); // search right
    }
}