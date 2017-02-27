#include "utility.h"

// 231. Power of Two https://leetcode.com/problems/power-of-two/?tab=Description
    bool isPowerOfTwo(int n) {
        // return (n&(n-1)) == 0 && n > 0;
        return n > 0 && (4294967296%n==0); // https://discuss.leetcode.com/topic/35624/how-about-this-trick/3
    }

// 326. Power of Three https://leetcode.com/problems/power-of-three/?tab=Description
// Follow up: Could you do it without using any loop / recursion?
// ** A summary of `all` solutions.  https://discuss.leetcode.com/topic/33536/a-summary-of-all-solutions-new-method-included-at-15-30pm-jan-8th
    bool isPowerOfThree(int n) {
        return  n > 0 && (1162261467%n==0); // https://discuss.leetcode.com/topic/36150/1-line-java-solution-without-loop-recursion
        if (n > 1) 
            while (n % 3 == 0) { n /= 3; }
        return n == 1;
    }

// 342. Power of Four https://leetcode.com/problems/power-of-four/?tab=Description
    bool isPowerOfFour(int n) {
        // return ((n&(n-1)) == 0 && !(0xaaaaaaaa & n) && n > 0);
        // return (n&(n-1)) == 0 && (n-1) % 3 == 0; // https://discuss.leetcode.com/topic/44430/simple-c-o-1-solution-without-0x55555555
        return (n&(n-1)) == 0 && (0x55555555 & n); // https://discuss.leetcode.com/topic/43801/simplest-c-solution-maybe
    }

namespace test {
    void isPowerOfTwo() {
        for (auto x: {2}) {
            auto res = ::isPowerOfTwo(x);
            cout << "x = " << x << ", ::isPowerOfTwo(x) == " << res << endl;
        }
    }
}

int main() {
    test::init();
    // test::isValidSudoku();
    // test::reverse_integer();
    // test::isIsomorphic();
    // test::wordPattern();
    // test::findDuplicate();
    // test::missingNumber();
    // test::firstMissingPositive();
    // test::findDisappearedNumbers();
    test::isPowerOfTwo();
}
