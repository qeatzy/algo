#include "utility.h"
// diff.cpp, difference of array, see also " bucket.cpp " rank.cpp
// Minimum difference in an array   http://stackoverflow.com/questions/12505426/minimum-difference-in-an-array

// 539. Minimum Time Difference 
// Given a list of 24-hour clock time points in "Hour:Minutes" format, find the minimum minutes difference between any two time points in the list.
// Example 1: Input: ["23:59","00:00"] Output: 1
// Note: The number of time points in the given list is at least 2 and won't exceed 20000. The input time is legal and ranges from 00:00 to 23:59.
    int findMinDifference(vector<string>& timePoints) {
        if (timePoints.size() > 1440) return 0; 
        std::vector<int> times(1440, 0);
        auto getTime = [](string t) { return 60 * ((t[0] - '0') * 10 + (t[1] - '0')) + ((t[3] - '0') * 10 + (t[4] - '0')); };
        for (int i = 0, sz = std::min(timePoints.size(), times.size()); i < sz; ++i) {
            if (times[getTime(timePoints[i])]++) return 0;
        }
        if (timePoints.size() > 1440 / 2) return 1; 
        int res = 1440, prev = -1440, first = -1440;
        // for (int i = 0, sz = times.size(); i < sz; ++i) {
        for (int i = 0, sz = times.size(); i < sz && res > 1; ++i) {
            if (times[i]) {
                if (first < 0) first = i;
                res = std::min(res, i - prev);
                prev = i;
            }
        }
        res = std::min(res, 1440 - (prev - first));  // handle `last - first` case
        return res;
    }


namespace test {
    void findMinDifference() {
        std::vector<std::vector<std::string>> vec;
        vec.push_back({"23:59","00:00"});
        for (auto &v: vec) {
            auto res = ::findMinDifference(v);
            cout << "::findMinDifference(v) = " << res << endl;
        }
    }
}

int main() {
    test::init();
    test::findMinDifference();
}
