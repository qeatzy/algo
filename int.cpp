#include "utility.h"

// 504. Base 7 https://leetcode.com/problems/base-7/#/description
// Given an integer, return its base 7 string representation.
// Example 1: Input: 100 Output: "202"
// Example 2: Input: -7 Output: "-10"
// Note: The input will be in range of [-1e7, 1e7].
    string convertToBase7(int num) {
        if (num == 0) return "0";
       bool isneg = false;
       string res;
       if (num < 0) { isneg = true, num = -num; res.push_back('-'); }
       for (; num; num /= 7) res.push_back('0' + num % 7);
       std::reverse(res.begin() + isneg, res.end());
       return res;
    }

namespace test {
    void convertToBase7() {
        std::vector<int> nums{100,-7, 0};
        for (auto x: nums) {
            cout << x << " in base 7 is " << ::convertToBase7(x) << endl;
        }
    }
}

int main() {
    test::init();
    test::convertToBase7();
}
