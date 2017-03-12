#include "utility.h"
// bit manipulation. 1. or: sum not false. 2. xor: mod of sum. 3.1 and: div(or carry) of sum. 3.2 and: prod not false. 4. divmod(sum,2) = and, xor.
// get rightmost set bit, x & -x. -- works for unsigned integer, and signed integer in two-s complement.
// unset rightmost set bit, x &= x - 1, or x -= x & -x.   http://stackoverflow.com/a/42747608/3625404   http://stackoverflow.com/a/4704041/3625404
// clear with and zero, set with or one.
// 100000 - 1 == 11111      || 11111 + 1 == 100000
// -- use case, find highest non-zero bit in a number, log(log(n)), eg, Java's Integer.highestOneBit().
// parallel bit manipulation, which is log(log(n)) is better we can achieve.
// bit manipulation.  https://leetcode.com/tag/bit-manipulation/
// 6.59 Other Built-in Functions Provided by GCC https://gcc.gnu.org/onlinedocs/gcc/Other-Builtins.html
// TODO A collection of bit manipulation related problem solution on leetcode. https://discuss.leetcode.com/topic/50315/a-summary-how-to-use-bit-manipulation-to-solve-problems-easily-and-efficiently/

// 371. Sum of Two Integers https://leetcode.com/problems/sum-of-two-integers/#/description
// Calculate the sum of two integers a and b, but you are not allowed to use the operator + and -.
// Example: Given a = 1 and b = 2, return 3.
// TODO 
    int getSum(int a, int b) {
        return 0;
    }

// 461. Hamming Distance https://leetcode.com/problems/hamming-distance/#/description
// The Hamming distance between two integers is the number of positions at which the corresponding bits are different.
// Given two integers x and y, calculate the Hamming distance.
// Note: 0 ≤ x, y < 231.
// Example: Input: x = 1, y = 4 Output: 2
// Explanation:
// 1   (0 0 0 1)
// 4   (0 1 0 0)
//        ↑   ↑
// The above arrows point to positions where the corresponding bits are different.
// -- count bits, a loopkup table method may be better, eg, break a 32-bit integer to 4 8-bit number, then sum the count. The lookup table could be implicit, eg, with magic number.
// -- the partition could be disjoint or overlapping, eg, 01010101 and 10101010, vs, 11110000 and 00001111.
// fast implementation of count bit in JDK, and others. https://tech.liuchao.me/2016/11/count-bits-of-integer/
    int hammingDistance(int x, int y) {
        x ^= y, y = 0;
        while (x) x &= x - 1, ++y;
        return y;
    }
namespace test {
    void hammingDistance() {
        std::vector<int> nums{1,4};
        while (nums.size() >= 2) {
            auto y = nums.back(); nums.pop_back();
            auto x = nums.back(); nums.pop_back();
            cout << "hamming distance of " << x  << " and " << y << " is " << ::hammingDistance(x, y) << endl;
        }
    }
}

// 477. Total Hamming Distance https://leetcode.com/problems/total-hamming-distance/#/description
// The Hamming distance between two integers is the number of positions at which the corresponding bits are different.
// Now your job is to find the total Hamming distance between all pairs of the given numbers.
// Example: Input: 4, 14, 2 Output: 6
// Explanation: In binary representation, the 4 is 0100, 14 is 1110, and 2 is 0010 (just showing the four bits relevant in this case).
// So the answer will be: HammingDistance(4, 14) + HammingDistance(4, 2) + HammingDistance(14, 2) = 2 + 2 + 2 = 6.
// Note: Elements of the given array are in the range of 0 to 10^9 Length of the array will not exceed 10^4.
// TODO achieve O(n) solution.
// discuss. https://discuss.leetcode.com/category/609/total-hamming-distance
    int totalHammingDistance(vector<int>& nums) {
        int cnt = 0;
        for (int i = 0, sz = nums.size(); i < sz; ++i) {
            for (int j = i + 1; j < sz; ++j) {
                cnt += hammingDistance(nums[i], nums[j]);
            }
        }
        return cnt;
    }

// 476. Number Complement https://leetcode.com/problems/number-complement/#/description
// Given a positive integer, output its complement number. The complement strategy is to flip the bits of its binary representation.
// Note: The given integer is guaranteed to fit within the range of a 32-bit signed integer.
// You could assume no leading zero bit in the integer's binary representation.
// Example 1: Input: 5 Output: 2 Explanation: The binary representation of 5 is 101 (no leading zero bits), and its complement is 010. So you need to output 2.
// Example 2: Input: 1 Output: 0 Explanation: The binary representation of 1 is 1 (no leading zero bits), and its complement is 0. So you need to output
// -- set all bit after leftmost set bit to one, then xor to original number, or logical and with flipped original number.
    int findComplement(int num) {
        unsigned x = num;
        x |= x >> 1, x |= x >> 2, x |= x >> 4, x |= x >> 8, x |= x >> 16; 
        return x & ~num;    // or `x ^ num`, since x and num have common leading 0s, x trailing all 1. xor 1 is flip.
    }
namespace test {
    void findComplement() {
        std::vector<int> nums{5,1};
        for (auto x: nums) {
            cout << "complement of " << x << " is " << ::findComplement(x) << endl;
        }
    }
}

// 136. Single Number https://leetcode.com/problems/single-number/?tab=Description
// Given an array of integers, every element appears twice except for one. Find that single one.
// Your algorithm should have a linear runtime complexity. Could you implement it without using extra memory? 
// TODO solve similar problem.
    int singleNumber_xor(vector<int>& nums) {
        int res = 0;
        for (auto x: nums) { res ^= x; }
        return res;
    }
    int singleNumber_sum(vector<int>& nums) { // see singleNumber_cnt
        return 1;
    }
    // int singleNumber(vector<int>& nums) {
    //     return singleNumber_xor(nums);
    // }

// 137. Single Number II https://leetcode.com/problems/single-number-ii/?tab=Description
// Given an array of integers, every element appears three times except for one, which appears exactly once. Find that single one. 
// Your algorithm should have a linear runtime complexity. Could you implement it without using extra memory? 
// full adder solution. n*log(k) time, log(k) space.
// count solution. n*log(max value) time, O(1) or log(max value) space. O(1) space, https://discuss.leetcode.com/topic/43166/ 
    int singleNumber_bit(vector<int>& nums) { // a full adder, 0 -> 1 -> 2 -> 3 -> 0.
        int lo = 0, hi = 0;
        for (auto x: nums) {
            int carry = lo & x; // div of sum perspective.
            lo ^= x;
            hi ^= carry;
            carry = ~(hi & lo);    // if lo == true, hi == true, reset both to 0. // product perspective.
            hi &= carry;
            lo &= carry;
        }
        return lo;
    }
    int singleNumber_cnt(vector<int>& nums) {
        int res = 0;
        for (int bit = 1;; bit <<= 1) {
            int cnt = 0;
            for (auto x: nums) {
                if (x & bit)
                cnt = (cnt + 1) % 3;
            }
            res += bit * cnt;
            if (bit == INT_MIN) break; // here assume 2's complement encoding.
        }
        return res;
    }
    int singleNumber(vector<int>& nums) {
        // return singleNumber_bit(nums);
        return singleNumber_cnt(nums);
    }

// 201. Bitwise AND of Numbers Range https://leetcode.com/problems/bitwise-and-of-numbers-range/?tab=Description
// Given a range [m, n] where 0 <= m <= n <= 2147483647, return the bitwise AND of all numbers in this range, inclusive.
// For example, given the range [5, 7], you should return 4.
// -- the best complexity is log(n)??
// -- no, the best we can do is of complexity log(log(n)). similar to Java's Integer.highestOneBit().
// solution. clear all bits after first bit where m and n differ, including that bit too.
// a interesting solution. The key point: reduce n by removing the rightest '1' bit until n<=m;  https://discuss.leetcode.com/topic/20176/2-line-solution-with-detailed-explanation/4#
// -- m := common bits + 0 + remaining bits,   n :=  common bits + 1 + remaining bits.  given m < n;
// branch-less log(log(n)) solution. https://discuss.leetcode.com/topic/17491/fast-three-line-c-solution-and-explanation-with-no-loops-or-recursion-and-one-extra-variable
// simple clear explanation. last bit of (odd number & even number) is 0.  https://discuss.leetcode.com/topic/12133/bit-operation-solution-java
    int rangeBitwiseAnd_shift(int m, int n) {
        int shift = 0;
        while (m != n) { // if m != n then last bit is zero, since last bit of (odd number & even number) is 0.
            m >>= 1, n >>= 1;
            ++shift;
        }
        return m << shift;
    }
    int rangeBitwiseAnd_log_log_n(int m, int n) {
        unsigned x = m ^ n;
        x |= x >> 1, x |= x >> 2, x |= x >> 4, x |= x >> 8, x |= x >> 16; // assume 32-bit int
        return m & ~x;
    }
    int rangeBitwiseAnd_clear_last_bit_until_n_less_than_m(int m, int n) {
        while (m < n) n &= n - 1;
        return n;
    }
    int rangeBitwiseAnd(int m, int n) {
        // return rangeBitwiseAnd_shift(m, n);
        // return rangeBitwiseAnd_log_log_n(m, n);
        return rangeBitwiseAnd_clear_last_bit_until_n_less_than_m(m, n);
    }

namespace test {
    void rangeBitwiseAnd() {
        std::vector<std::pair<int,int>> vec;
        vec.push_back({5,7});
        vec.push_back({0,0});
        vec.push_back({0,1});
        vec.push_back({1,2});
        vec.push_back({2,2});
        vec.push_back({2,3});
        for (auto x: vec) {
            auto res = ::rangeBitwiseAnd(x.first, x.second);
            cout << "bitwise and between " << x.first << " and " << x.second << " is " << res << endl;
        }
    }
}

int main() {
    test::init();
    test::rangeBitwiseAnd();
}
