#include "utility.h"

// 389. Find the Difference https://leetcode.com/problems/find-the-difference/#/description
// Given two strings s and t which consist of only lowercase letters.
// String t is generated by random shuffling string s and then add one more letter at a random position.
// Find the letter that was added in t.
// Example: Input: s = "abcd" t = "abcde" Output: e
// Explanation: 'e' is the letter that was added.
    char findTheDifference_sort(string s, string t) {
        std::sort(s.begin(), s.end());
        std::sort(t.begin(), t.end());
        int i = 0, sz = s.size();
        for (; i < sz && s[i] == t[i]; ++i) {}
        return t[i];
    }
    char findTheDifference_xor(string s, string t) {
        char res = 0;
        int sz = s.size();
        for (int i = 0; i < sz; ++i) res ^= s[i] ^ t[i];
        return res ^ t[sz];
    }
    char findTheDifference_sum(string s, string t) {
        int sz = s.size();
        char res = 0;
        for (int i = 0; i < sz; ++i) res += t[i] - s[i];
        return res + t[sz];
    }
    char findTheDifference_hash(string s, string t) {
        auto m1 = uCounter(s), m2 = uCounter(t);
        for (auto x: m2) if (x.second != m1[x.first]) return x.first;
        return -1;  // just for suppress warning
    }
    char findTheDifference(string s, string t) {
        return findTheDifference_hash(s,t);
        return findTheDifference_sum(s,t);
        return findTheDifference_xor(s,t);
        return findTheDifference_sort(s,t);
    }

namespace test {
    void findTheDifference() {
        std::vector<std::string> vec;
        vec.push_back("abcd");
        vec.push_back("abcde");
        while (vec.size() >= 2) {
            auto t = vec.back(); vec.pop_back();
            auto s = vec.back(); vec.pop_back();
            assert(t.size() == s.size() + 1);
            auto res = ::findTheDifference(s,t);
            cout << "difference of [" << s << "] and [" << t << "] is " << res << endl;
        }
    }
}

// 487 Max Consecutive Ones II  http://www.cnblogs.com/codeskiller/p/6512811.html  http://bookshadow.com/weblog/2017/01/15/leetcode-max-consecutive-ones-ii/
// Given a binary array, find the maximum number of consecutive 1s in this array if you can flip at most one 0.
// Example 1: Input: [1,0,1,1,0] Output: 4
// Explanation: Flip the first zero will get the the maximum number of consecutive 1s.
// After flipping, the maximum number of consecutive 1s is 4.
// Note: The input array will only contain 0 and 1. The length of input array is a positive integer and will not exceed 10,000
// Follow up: What if the input numbers come in one by one as an infinite stream? In other words, you can't store all numbers coming from the stream as it's too large to hold in memory. Could you solve it efficiently?
// -- pad stream with empty 1 block, and record single_zero or not when 0 encountered.
// https://discuss.leetcode.com/category/633/max-consecutive-ones-ii
    int findMaxConsecutiveOnes(vector<int>& nums) {
        int res = 0, prev = 0, cnt = 0;
        bool single_zero = false;
        for (int i = 0, sz = nums.size(); i < sz;) {
            if (nums[i] == 0) {
                std::swap(cnt, prev);
                cnt = single_zero? cnt + prev + 1 : prev;
                res = std::max(res, cnt);
                cnt = 0;
                if (++i < sz && nums[i] == 0)  {
                    single_zero = false;
                    for (; ++i < sz && nums[i] == 0;) {}
                } else single_zero = true;
            } else ++cnt, ++i;
        }
        if (single_zero) cnt += prev + 1;
        return std::max(res, cnt);
    }

// 485. Max Consecutive Ones https://leetcode.com/problems/max-consecutive-ones/#/description
// Given a binary array, find the maximum number of consecutive 1s in this array.
// Example 1: Input: [1,1,0,1,1,1] Output: 3
// Explanation: The first two digits or the last three digits are consecutive 1s. The maximum number of consecutive 1s is 3.
// Note: The input array will only contain 0 and 1. The length of input array is a positive integer and will not exceed 10,000
    int findMaxConsecutiveOnes_level_one(vector<int>& nums) {
        int res = 0, cnt = 0;
        for (int i = 0, sz = nums.size(); i < sz;) {
            if (nums[i] == 0) {
                res = std::max(res, cnt);
                cnt = 0;
                for (; ++i < sz && nums[i] == 0;) {}
            } else ++cnt, ++i;
        }
        return std::max(res, cnt);
    }

namespace test {
    void findMaxConsecutiveOnes() {
        std::vector<std::vector<int>> vec;
        vec.push_back({1,1,0,1,1,1});
        vec.push_back({1,0,1,1,0});
        for (auto &v: vec) {
            print(v,"nums");
            cout << "max length of consecutive one is " << ::findMaxConsecutiveOnes(v) << endl;
        }
    }
}

// 412. Fizz Buzz https://leetcode.com/problems/fizz-buzz/#/description
// Write a program that outputs the string representation of numbers from 1 to n.
// But for multiples of three it should output 'Fizz' instead of the number and for the multiples of five output 'Buzz'.
// For numbers which are multiples of both three and five output 'FizzBuzz'.
    vector<string> fizzBuzz(int n) {
        vector<string> res;
        for (int i = 1; i <= n; ++i) {
            switch (((i % 3 == 0) << 1) + (i % 5 == 0)) {
                case 3: res.push_back("FizzBuzz"); break;
                case 2: res.push_back("Fizz"); break;
                case 1: res.push_back("Buzz"); break;
                default: res.push_back(std::to_string(i));
            }
        }
        return res;
    }

// 500. Keyboard Row https://leetcode.com/problems/keyboard-row/?tab=Description
// Given a List of words, return the words that can be typed using letters of alphabet on only one row's of American keyboard like the image below.
// Example 1: Input: ["Hello", "Alaska", "Dad", "Peace"] Output: ["Alaska", "Dad"]
// Note: You may use one character in the keyboard more than once.
// You may assume the input string will only contain letters of alphabet.
    // vector<string> findWords(vector<string>& words) {
    // // const std::string[3] = {"qwertyuiop","asdfghjkl","zxcvbnm"};
    //     
    // }

// 266 Palindrome Permutation
// Given a string, determine if a permutation of the string could form a palindrome.
// For example "code" -> False, "aab" -> True, "carerac" -> True.
// -- one boost is if alphabet size is small -- eg ASCII -- use int array of char set size as Counter. -- similar to count sort.
// "bit, many trick solutions by StefanPochmann. https://discuss.leetcode.com/topic/21999/1-4-lines-python-ruby-c-c-java
    bool canPermutePalindrome(string s) {
        auto cnt = uCounter(s);
        int odd = 0;
        for (auto x: cnt) {
            if (x.second % 2) ++odd;
            if (odd > 1) return false;
        }
        return true;
    }
namespace test {
    void canPermutePalindrome() {
        std::vector<std::string> vec;
        vec.push_back("code");
        vec.push_back("aab");
        vec.push_back("carerac");
        for (auto &s: vec) {
            auto res = ::canPermutePalindrome(s);
            cout << "s = " << s << "::canPermutePalindrome(s) = " << res << endl;
        }
    }
}

// 267 Palindrome Permutation II  http://www.cnblogs.com/grandyang/p/5315227.html
// Given a string s, return all the palindromic permutations (without duplicates) of it. Return an empty list if no palindromic permutation could be form.
// For example:
// Given s = "aabb", return ["abba", "baab"].
// Given s = "abc", return [].
// Hint:
// If a palindromic permutation exists, we just need to generate the first half of the string.
// To generate all distinct permutations of a (half of) string, use a similar approach from: Permutations II or Next Permutation.
    vector<string> generatePalindromes(string s) {
        vector<string> res;
        return res;
    }

// 346. Moving Average from Data Stream  https://tenderleo.gitbooks.io/leetcode-solutions-/GoogleEasy/346.html
// concise python solution.  https://discuss.leetcode.com/topic/44116/4-line-python-solution-using-deque/4
// C++ solution using queue.  https://discuss.leetcode.com/topic/44101/c-easy-solution-using-queue
// Given a stream of integers and a window size, calculate the moving average of all integers in the sliding window.
// For example,
// MovingAverage m = new MovingAverage(3);
// m.next(1) = 1
// m.next(10) = (1 + 10) / 2
// m.next(3) = (1 + 10 + 3) / 3
// m.next(5) = (10 + 3 + 5) / 3
vector<double> MovingAverage(vector<int> nums, int k) {
    vector<double> res;
    return res;
}

namespace test {
    void MovingAverage() {
        std::vector<std::vector<int>> vec;
        vec.push_back({1,10,3,5});
        for (auto &v: vec) {
            assert(!v.empty() && v.back() < int(v.size()));
            auto k = v.back(); v.pop_back();
            auto res = ::MovingAverage(v, k);
        }
    }
}

int main() {
    test::init();
    // test::canPermutePalindrome();
    // test::findMaxConsecutiveOnes();
    test::findTheDifference();
}