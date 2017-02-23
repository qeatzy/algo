#include "utility.h"
#include <unordered_map>
#include <initializer_list>
// fc (flow control).

// 20. Valid Parentheses https://leetcode.com/problems/valid-parentheses/?tab=Description
// Given a string containing just the characters '(', ')', '{', '}', '[' and ']', determine if the input string is valid.
bool isValid(string s) {
    // const std::map<char, char> m {{')','('},{']','['},{'}','{'}};
    std::map<char, char> m {{')','('},{']','['},{'}','{'}};
    string stack;
    for (auto c: s) {
        if (c == '(' || c == '[' || c == '{')
            stack.push_back(c);
        else if ((c == ')' || c == ']' || c == '}') && !stack.empty() && stack.back() == m[c]) {
            stack.pop_back();
        } else return false;
    }
    return stack.empty();
}
namespace test {
    void isValid() {
        std::vector<std::string> vec{"([)]"};
        for (auto s: vec) {
            auto res = ::isValid(s);
            cout << "s == \" " << s << " \", ::isValid(s) == " << res << endl;
        }
    }
}

// 241. Different Ways to Add Parentheses https://leetcode.com/problems/different-ways-to-add-parentheses/?tab=Description
// TODO 
// 301. Remove Invalid Parentheses https://leetcode.com/problems/remove-invalid-parentheses/?tab=Description
// Remove the minimum number of invalid parentheses in order to make the input string valid. Return all possible results.
// Note: The input string may contain letters other than the parentheses (and).
// Examples: "()())()" -> ["()()()", "(())()"] "(a)())()" -> ["(a)()()", "(a())()"] ")(" -> [""]
// TODO https://discuss.leetcode.com/topic/34875/easy-short-concise-and-fast-java-dfs-3-ms-solution/
    template <typename Iterator, typename T = typename std::iterator_traits<Iterator>::value_type>
    void remove(Iterator first, Iterator last, string buf, vector<string> &res) {
        int cnt = 0;
        auto p = first;
        for (; p != last && cnt >= 0; ++p) {
            if (*p == '(') {
                // if (cnt < 0) { cnt = 0; }
                ++cnt;
            } else if (*p == ')') {
                --cnt;
                // if (cnt < -1) continue;
            }
        }
        // if (cnt < 0)
    }
    vector<string> removeInvalidParentheses(string s) {
        auto first = s.begin(), last = s.end();
        for (; first != last && *first != '('; ++first) {}
        for (; last != first && last[-1] != ')'; --last) {}
        string t;
        int cnt = 0;
        for (auto p = first; p != last; ++p) {
            if (*p == '(') {
                if (cnt < 0) { cnt = 0; }
                ++cnt;
            } else if (*p == ')') {
                --cnt;
                if (cnt < -1) continue;
            } else {
            }
            t.push_back(*p);
        }
        print(t);
    }

// 32. Longest Valid Parentheses https://leetcode.com/problems/longest-valid-parentheses/?tab=Description
// Given a string containing just the characters '(' and ')', find the length of the longest valid (well-formed) parentheses substring.
// -- basic, extra closing paren invalidate/separate valid parentheses.
// 1. two pass O(1) space. 2. stack of index. 3. DP solution.
// https://discuss.leetcode.com/topic/81517/none-of-java-solution-can-pass-oj-now-neither-stack-nor-dp
    int longestValidParentheses_two_stack(string s) {
        int res = 0;
        vector<int> cnt{0};
        vector<size_t> stack{0};
        size_t sz = 0; // sz >= stack.back()
        for (auto ch: s) {
            // assert(cnt.size() == stack.size());
            if (ch == '(') {
                ++sz;
            } else if (sz != 0) {
                // print(stack,"stack");
                // print(cnt,"cnt");
                --sz;
                if (sz > stack.back()) {
                    res = std::max(res, cnt.back());
                    stack.push_back(sz);
                    cnt.push_back(1);
                } else {
                    ++cnt.back();
                    if (sz == stack.back() || sz > stack[stack.size() - 2]) {
                        stack.back() = sz;
                    } else {
                        cnt[cnt.size() - 2] += cnt.back();
                        stack.pop_back();
                        cnt.pop_back();
                    }
                }
            } else {
                res = std::max(res, cnt.back());
                // stack.back() = 0;
                cnt.back() = 0;
            }
        }
        res = std::max(res, cnt.back());
        return 2 * res;
    }
    int longestValidParentheses_dp(string s) {
        int res = 0;
        vector<int> dp(s.size(),0);
        for (int i = 1, sz = s.size(); i < sz; ++i) {
            if (s[i] == ')') {
                if (s[i-1] == '(') {
                    dp[i] = 2 + (i-2>=0 ? dp[i-2] : 0);
                } else {
                    int j = i - 1 - dp[i-1];
                    if (j >= 0 && s[j] == '(')   // "()(())"
                        dp[i] = 2 + dp[i-1] + (j >= 1 ? dp[j-1] : 0);
                        // dp[i] = 2 + dp[i-1] + dp[j];
                }
                res = std::max(res, dp[i]);
            }
        }
        return res;
    }
    int longestValidParentheses_two_pass(string s) { // a similar solution. https://discuss.leetcode.com/topic/7376/why-people-give-conclusion-that-this-cannot-be-done-with-o-1-space-my-ac-solution-o-n-run-time-o-1-space
        int left = -1;
        int cnt = 0;
        int res = 0;
        for (int i = 0, sz = s.size(); i < sz; ++i) {
            cnt += (s[i] == '(') * 2 - 1;
            if (cnt < 0) {
                left = i;
                cnt = 0;
            } else if (cnt == 0) {
                res = std::max(res, i - left);
            }
        }
        // if (cnt != 0)
        //     res = std::max(res, int(s.size()) - left - cnt);
        cout << "cnt = " << cnt << endl;
        if (cnt != 0 && res < s.size() - left - 1) {
            cnt = 0;
            int i = s.size() - 1, sz = left;
            int right = s.size();
            for (; i > sz; --i) {
                cnt += (s[i] == ')') * 2 - 1;
                if (cnt < 0) {
                    right = i;
                    cnt = 0;
                } else if (cnt == 0) {
                    res = std::max(res, right - i);
                }
            }
        }
        return res;
    }
    int longestValidParentheses_stack_of_index(string s) {
        int res = 0;
        vector<int> stack{-1};
        for (int i = 0, sz = s.size(); i < sz; ++i) {
            if (s[i] == '(') {
                stack.push_back(i);
            } else if (stack.size() > 1){
            // cout << "i = " << i << ", stack.back() = " << stack.back() << endl;
                stack.pop_back();
                res = std::max(res, i - stack.back());
            } else { // reset starting point, since extra ')' encountered.
                stack.back() = i;
            }
        }
        return res;
    }
    int longestValidParentheses(string s) {
        // return longestValidParentheses_two_stack(s);
        // return longestValidParentheses_stack_of_index(s);
        return longestValidParentheses_dp(s);
        return longestValidParentheses_two_pass(s);
    }

namespace test {
    void longestValidParentheses() {
        std::vector<std::string> vec;
        vec.push_back("(()");
        vec.push_back(")()())()()(");
        vec.push_back("(()(((()");
        vec.push_back("(()())");
        vec.push_back("(()()");
        vec.push_back("((()))())");
        vec.push_back(")()))()(");
        vec.push_back("))))())()()(()");
        vec.push_back("()(())");
        for (auto s: vec) {
            auto res = ::longestValidParentheses(s);
            cout << "s == \" " << s << " \", ::longestValidParentheses(s) == " << res << endl;
        }
    }
}

// 278. First Bad Version https://leetcode.com/problems/first-bad-version/?tab=Description
// binary search, inclusive, well defined. as rhs-exclusive, stop if empty, here low > high, always low == high - 1 when exit loop, unless never enter in loop.
// when there is only one element left, here low == high, either low is incremented or high is decremented.
// Forward declaration of isBadVersion API.
bool isBadVersion(int version);
class Solution {
public:
    int firstBadVersion(int n) {
        int low = 1;
        while (low <= n) {
            auto mid = low + (n - low) / 2;
            if (isBadVersion(mid)) {
                n = mid - 1;
            } else {
                low = mid + 1;
            }
        }
        return low;
    }
};

// 165. Compare Version Numbers https://leetcode.com/problems/compare-version-numbers/?tab=Description
// You may assume that the version strings are non-empty and contain only digits and the. character.
// 0.1 < 1.1 < 1.2 < 13.37
// TODO 
    int compareVersion(string version1, string version2) {
        return 0;
    }

// 168. Excel Sheet Column Title https://leetcode.com/problems/excel-sheet-column-title/?tab=Description
// Given a positive integer, return its corresponding column title as appear in an Excel sheet.
// 1 -> A
// 26 -> Z
// 27 -> AA
    string convertToTitle(int n) {
        string res;
        for (; n > 0; n = (n - 1) / 26) {
            res.push_back('A' + (n - 1) % 26);
        }
        std::reverse(res.begin(), res.end());
        return res;
    }
// 171. Excel Sheet Column Number https://leetcode.com/problems/excel-sheet-column-number/?tab=Description
// Given a column title as appear in an Excel sheet, return its corresponding column number.
// A -> 1
// Z -> 26
// AA -> 27
    int titleToNumber(string s) {
        int res = 0;
        for (auto ch: s) {
            res = res * 26 + (ch - ('A' - 1));
        }
        return res;
    }

// 3. Longest Substring Without Repeating Characters https://leetcode.com/problems/longest-substring-without-repeating-characters/?tab=Description
// Given a string, find the length of the longest substring without repeating characters.
// Given "pwwkew", the answer is "wke", with the length of 3. Note that the answer must be a substring, "pwke" is a subsequence and not a substring.
// -- if not repeat incremente cnt, else cnt will be decreased wich depending on previous index of current value.
// the point is thinking, if the size will increase/decrease, then when? and how? this often leads to think the first time/index a break point happens.
// TODO below need subscribe now, search with search engine.
// leetcode Longest Substring with At Most Two Distinct Characters
// leetcode Longest Substring with At Most K Distinct Characters
    int lengthOfLongestSubstring(string s) {
        // std::unordered_map<char, int> hash;
        // std::vector<int> hash(sizeof(s[0]), -1);
        std::vector<int> hash(256, -1);
        int res = 0, cnt = 0;
        for (int i = 0; size_t(i) < s.size(); ++i) {
            // if (hash.find(s[i]) == hash.end() || i - hash[s[i]] > cnt) {
            if (i - hash[s[i]] > cnt) { // bug, or should I cast s[i] to unsigned char first, if hash is an array?
                ++cnt;
                res = std::max(res, cnt);
            } else {
                cnt = i - hash[s[i]];
            }
            hash[s[i]] = i;
        }
        return res;
    }

namespace test {
    void lengthOfLongestSubstring() {
        std::vector<std::string> vec;
        // vec.push_back("pwwkew");
        vec.push_back("bullvfdykezshekcfacmvifuwqcycosfkdqfkvgwbfoklznqbpewzjqwbvwdlqkwvcdgxjcb");
        for (auto &s: vec) {
            auto res = ::lengthOfLongestSubstring(s);
            cout << "s = [" << s << "], ::lengthOfLongestSubstring(s) == " << res << endl;
        }
    }
}

// 5. Longest Palindromic Substring https://leetcode.com/problems/longest-palindromic-substring/?tab=Description
// Given a string s, find the longest palindromic substring in s. You may assume that the maximum length of s is 1000.
// Example: Input: "babad" Output: "bab" Note: "aba" is also a valid answer.
// Example: Input: "cbbd" Output: "bb"
// TODO prove below algorithm is O(n)?  see this, I think what they do -- modify the input -- is unnecessary. http://articles.leetcode.com/longest-palindromic-substring-part-ii  https://www.felix021.com/blog/read.php?2040
// -- a range denote a palindrome, but if [left, right] is palindrome, we don't need to check [left + 1, right - 1]. A palindrome is odd or even depend on how many equal characters are same in the center, to avoid branch on odd/even, we begin at left of center, then expand the center, then expand the palindrome.
    string longestPalindrome(string s) {
        auto first = s.begin(), start = first;
        int len = 0;
        for (; first != s.end();) {
            auto p = first, next = first;
            int cnt = 1;
            for (; ++next != s.end() && *next == *p;) { ++cnt; }
            first = next;
            for (; p != s.begin() && next != s.end() && *--p == *next; ++next) { cnt += 2; }
            // cout << "cnt = " << cnt << endl;
            if (cnt > len) {
                len = cnt;
                start = next - cnt;
            }
        }
        // cout << "len = " << len << endl;
        return string(start, start + len);
    }
    string longestPalindrome_refactor(string s) { // TODO, fix, how to calculate cnt afterwards, is it possible?
        auto first = s.begin(), start = first;
        int len = 0;
        for (; first != s.end();) {
            auto p = first, next = first;
            for (; ++next != s.end() && *next == *p;) {}
            first = next;
            for (; p != s.begin() && next != s.end() && *--p == *next; ++next) {}
            // abac  |  abaa  |  eabac  |  aabac
            int cnt = (next - p) + 1; // fix
            // cout << "cnt = " << cnt << endl;
            if (cnt > len) {
                len = cnt;
                start = next - cnt;
            }
        }
        // cout << "len = " << len << endl;
        return string(start, start + len);
    }
namespace test {
    void longestPalindrome() {
        std::vector<std::string> vec;
        vec.push_back("babad");
        vec.push_back("cbbd");
        for (auto &s: vec) {
            auto res = ::longestPalindrome(s);
            cout << "res.size() = " << res.size() << endl;
            cout << res << endl;
            for (auto i: range(res.size())) {
                cout << int(res[i]) << ' ';
            } cout << endl;
            cout << "s = [" << s << "], ::longestPalindrome(s) == [" << res << "]" << endl;
        }
    }
}
// 516. Longest Palindromic Subsequence https://leetcode.com/problems/longest-palindromic-subsequence/?tab=Description
// Given a string s, find the longest palindromic subsequence's length in s. You may assume that the maximum length of s is 1000.
// Example 1: Input: "bbbab" Output: 4 One possible longest palindromic subsequence is "bbbb".
// Example 2: Input: "cbbd" Output: 2 One possible longest palindromic subsequence is "bb".
// TODO abc ... abbc
    int longestPalindromeSubseq(string s) {
        
    }
// 131. Palindrome Partitioning https://leetcode.com/problems/palindrome-partitioning/?tab=Description
// Given a string s, partition s such that every substring of the partition is a palindrome. 
// Return all possible palindrome partitioning of s. 
// For example, given s = "aab", Return [["aa","b"], ["a","a","b"]]
// TODO 
    vector<vector<string>> partition(string s) {
        
    }
// 132. Palindrome Partitioning II https://leetcode.com/problems/palindrome-partitioning-ii/?tab=Description
// Given a string s, partition s such that every substring of the partition is a palindrome. 
// Return the minimum cuts needed for a palindrome partitioning of s. 
// For example, given s = "aab", Return 1 since the palindrome partitioning ["aa","b"] could be produced using 1 cut.
// TODO 
    int minCut(string s) {
        
    }
// 214. Shortest Palindrome https://leetcode.com/problems/shortest-palindrome/?tab=Description
// Given a string S, you are allowed to convert it to a palindrome by adding characters in front of it.
// Find and return the shortest palindrome you can find by performing this transformation. 
// For example: Given "aacecaaa", return "aaacecaaa". Given "abcd", return "dcbabcd".
// TODO 
    string shortestPalindrome(string s) {
        
    }
// 336. Palindrome Pairs https://leetcode.com/problems/palindrome-pairs/?tab=Description
// Given a list of unique words, find all pairs of distinct indices (i, j) in the given list, so that the concatenation of the two words, i.e. words[i] + words[j] is a palindrome.
// Example 1: Given words = ["bat", "tab", "cat"] Return [[0, 1], [1, 0]] The palindromes are ["battab", "tabbat"]
// Example 2: Given words = ["abcd", "dcba", "lls", "s", "sssll"] Return [[0, 1], [1, 0], [3, 2], [2, 4]] The palindromes are ["dcbaabcd", "abcddcba", "slls", "llssssll"]
    vector<vector<int>> palindromePairs(vector<string>& words) {
        
    }
// 409. Longest Palindrome https://leetcode.com/problems/longest-palindrome/?tab=Description
// Given a string which consists of lowercase or uppercase letters, find the length of the longest palindromes that can be built with those letters.
// This is case sensitive, for example "Aa" is not considered a palindrome here.
// Assume the length of given string will not exceed 1,010. 
// Example: Input: "abccccdd" Output: 7 Explanation: One longest palindrome that can be built is "dccaccd", whose length is 7.
    // int longestPalindrome(string s) {
    //     
    // }
// TODO below need subscribe now, search with search engine.
// leetcode Palindrome Permutation
// leetcode Palindrome Permutation II

// 479. Largest Palindrome Product https://leetcode.com/problems/largest-palindrome-product/?tab=Description
// Find the largest palindrome made from the product of two n-digit numbers. Since the result could be very large, you should return the largest palindrome mod 1337.
// Example: Input: 2 Output: 987 Explanation: 99 x 91 = 9009, 9009 % 1337 = 987
// Note: The range of n is [1,8].
// TODO 
    int largestPalindrome(int n) {
        
    }

// 4. Median of Two Sorted Arrays 
// There are two sorted arrays nums1 and nums2 of size m and n respectively.
// Find the median of the two sorted arrays. The overall run time complexity should be O(log (m+n)).
// Example 1: nums1 = [1, 3] nums2 = [2] The median is 2.0
// Example 2: nums1 = [1, 2] nums2 = [3, 4] The median is (2 + 3)/2 = 2.5
// TODO  [1 2 3 4 5 6] [2 5 7 9] 6 + 4  4th and 5th, first is 0th.
// --    mid 3  5  [3 .. 6] [2 .. 5] 2th and 3th
    double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
        auto b1 = nums1.begin(), e1 = nums1.end(), b2 = nums2.begin(), e2 = nums2.end();
        if (nums1.size() < nums2.size()) {
            std::swap(b1, b2);
            std::swap(e1, e2);
        }
        int nth = (nums1.size() + nums2.size()) / 2; // bug, overflow
        while (b2 != e2 && nth > 1) {
            auto c1 = (e1 - b1) / 2, c2 = (e2 - b2) / 2;
            auto m1 = b1 + c1;
            auto m2 = b2 + c2;
            nth -= c1 + c2;
            if (*m1 >= *m2) {
                b1 = m1;
            } else {
            }
        }
    }

// 42. Trapping Rain Water https://leetcode.com/problems/trapping-rain-water/?tab=Description
// Given n non-negative integers representing an elevation map where the width of each bar is 1, compute how much water it is able to trap after raining.
// For example, Given [0,1,0,2,1,0,1,3,2,1,2,1], return 6.
// -- one of the example unguarded make code much cleaner.
    int trap(vector<int>& height) {
        if (height.size() < 2) return 0;
        int res = 0;
        auto left = height.begin(), right = height.end() - 1;
        for (; left != right;) {
            if (*left <= *right) {
                int lh = *left;
                for (; *++left < lh;) { res += lh - *left; }
            } else {
                int lh = *right;
                for (; *--right < lh;) { res += lh - *right; }
            }
        }
        return res;
    }
    int trap_one_loop(vector<int>& height) {
        // TODO 
        if (height.size() < 2) return 0;
        int res = 0;
        auto left = height.begin(), right = height.end() - 1;
        int lh = 0;
        for (; left != right;) {
            if (*left <= *right) {
                res += lh; // fix
                ++left;
            } else {
            }
        }
        return res;
    }
namespace test {
    void trap() {
        std::vector<std::vector<int>> vec;
        // vec.push_back({0,1,0,2,1,0,1,3,2,1,2,1});
        // vec.push_back({4,2,3});
        vec.push_back({4,9,4,5,3,2});
        for (auto &v: vec) {
            auto res = ::trap(v);
            print(v,"v");
            cout << "\t res = " << res << endl;
        }
    }
}

// 43. Multiply Strings https://leetcode.com/problems/multiply-strings/?tab=Description
// Given two non-negative integers num1 and num2 represented as strings, return the product of num1 and num2.
// Note: The length of both num1 and num2 is < 110.
// Both num1 and num2 contains only digits 0-9.
// Both num1 and num2 does not contain any leading zero.
// You must not use any built-in BigInteger library or convert the inputs to integer directly.
    string multiply(string num1, string num2) {
        if (num1 == "0" || num2 == "0") return "0";
        string res(num1.size() + num2.size(), 0);
        if (num1.size() > num2.size()) num1.swap(num2);
        for (auto &ch: num1) ch -= '0';
        for (auto &ch: num2) ch -= '0';
        auto start = res.begin();
        for (auto it1 = num1.rbegin(); it1 != num1.rend(); ++it1) {
            auto p = start++;
            int carry = 0;
            for (auto it2 = num2.rbegin(); it2 != num2.rend(); ++it2) {
                carry += *it1 * *it2 + *p;
                *p++ = carry % 10;
                carry /= 10;
            }
            cout << int(*p) << "  " << carry << endl;
            *p += carry;
        }
        if (res.back() == 0) res.pop_back();
        for (auto &ch: res) ch += '0';
        std::reverse(res.begin(), res.end());
        return res;
    }
namespace test {
    void multiply() {
        std::vector<std::string> vec;
        vec.push_back("345");
        vec.push_back("6789");
        vec.push_back("111");
        vec.push_back("111");
        assert(vec.size() % 2 == 0);
        for (; !vec.empty();) {
            auto num2 = vec.back();
            vec.pop_back();
            auto num1 = vec.back();
            vec.pop_back();
            auto res = ::multiply(num1,num2);
            cout << num1 << " * " << num2 << " == " << res << endl;
        }
    }
}

// 44. Wildcard Matching https://leetcode.com/problems/wildcard-matching/?tab=Description
// Implement wildcard pattern matching with support for '?' and '*'.
// The matching should cover the entire input string (not partial).
// isMatch("aa","a") → false
// isMatch("aa", "*") → true
// isMatch("aa", "a*") → true
// isMatch("ab", "?*") → true
// isMatch("aab", "c*a*b") → false
// TODO 
    bool isMatch(string s, string p) {
        return false;
    }

// 45. Jump Game II https://leetcode.com/problems/jump-game-ii/?tab=Description
// Given an array of non-negative integers, you are initially positioned at the first index of the array.
// Each element in the array represents your maximum jump length at that position.
// Your goal is to reach the last index in the minimum number of jumps.
// For example: Given array A = [2,3,1,1,4] The minimum number of jumps to reach the last index is 2.
// (Jump 1 step from index 0 to 1, then 3 steps to the last index.)
// Note: You can assume that you can always reach the last index.
// simple O(n) solution. https://discuss.leetcode.com/topic/11408/single-loop-simple-java-solution
    int jump_O_n_BFS_disguised(vector<int>& nums) {
        int farthest = 0, prev_level_farthest = 0, level = 0;
        for (int i = 0; i < int(nums.size()) - 1; ++i) {
            farthest = std::max(farthest, i + nums[i]);
            if (i == prev_level_farthest) {
                if (farthest == prev_level_farthest) return -1;
                ++level;
                prev_level_farthest = farthest;
            }
        }
        return level;
    }
    int jump(vector<int>& nums) { // uneffcient, TLE for last, 92th test case, which is quite long and the value is quite large.
        return jump_O_n_BFS_disguised(nums);
        if (nums.size() < 2) return 0;
        auto steps = vector<int>(nums.size(), nums.size());
        steps[0] = 0;
        int min = 0;
        for (int i = 0; i < nums.size() - 1; ++i) {
            if (steps[i] <= min && i + nums[i] >= nums.size() - 1) {
                return steps[i] + 1;
            }
            for (int j = i + 1, j_end = std::min(j + nums[i], int(nums.size())); j < j_end; ++j) {
                if (steps[j] > steps[i] + 1) steps[j] = steps[i] + 1;
            }
            min = std::min(min, steps[i]);
            ++min;
        }
        // print(steps);
        return steps.back();
    }
// 55. Jump Game https://leetcode.com/problems/jump-game/?tab=Description
// Given an array of non-negative integers, you are initially positioned at the first index of the array.
// Each element in the array represents your maximum jump length at that position.
// Determine if you are able to reach the last index.
// For example: A = [2,3,1,1,4], return true. A = [3,2,1,0,4], return false. 
// Linear and simple solution in C++  https://discuss.leetcode.com/topic/4911/linear-and-simple-solution-in-c
    bool canJump(vector<int>& nums) {
        return jump_O_n_BFS_disguised(nums) != -1;
    }
namespace test {
    void jump() {
        std::vector<std::vector<int>> vec;
        // vec.push_back({2,3,1,1,4});
        // vec.push_back({3,4,3,1,0,7,0,3,0,2,0,3});
        // vec.push_back({1,2,1,1,1});
        vec.push_back({4,0,6,2,3,7,2,4,0});
        for (auto &v: vec) {
            auto res = ::jump(v);
            print(v,"v",-1);
            cout << "\t ::jump(v) == " << res << endl;
        }
    }
}

// 49. Group Anagrams https://leetcode.com/problems/anagrams/?tab=Description
// Given an array of strings, group anagrams together.
// For example, given: ["eat", "tea", "tan", "ate", "nat", "bat"],
//     Return: [["ate", "eat","tea"], ["nat","tan"], ["bat"]]
// Note: All inputs will be in lower-case.
    vector<vector<string>> groupAnagrams(vector<string>& strs) {
        vector<vector<string>> res; 
        std::map<string, size_t> hash;
        for (auto &s: strs) {
            auto copy = s;
            std::sort(s.begin(), s.end());
            if (hash.find(s) == hash.end()) {
                hash[s] = res.size();
                res.push_back({std::move(copy)});
            } else {
                res[hash[s]].push_back(std::move(copy));
            }
        }
        return res;
    }
namespace test {
    void groupAnagrams() {
        std::vector<std::vector<std::string>> vec;
        vec.push_back({"eat", "tea", "tan", "ate", "nat", "bat"});
        for (auto &v: vec) {
            auto res = ::groupAnagrams(v);
            print(v,"v");
            print(res,"res");
        }
    }
}

// 48. Rotate Image https://leetcode.com/problems/rotate-image/?tab=Description
// You are given an n x n 2D matrix representing an image.
// Rotate the image by 90 degrees (clockwise).
// Follow up: Could you do this in-place?
// reverse columns then transpose, or transpose then reverse rows, here latter is better.  // -- actually the former is better.
// or rotate ring by ring.
    void rotate_tranpose_then_reverse(vector<vector<int>>& matrix) {
        std::reverse(matrix.begin(), matrix.end());
        for (size_t i = 0; i < matrix.size(); ++i) {
            for (size_t j = i + 1; j < matrix.size(); ++j) {
                std::swap(matrix[i][j], matrix[j][i]);
            }
        }
    }
    void rotate(vector<vector<int>>& matrix) {
        for (int lo = 0, hi = int(matrix.size()) - 1; lo < hi; ++lo, --hi) {
            for (int i = 0; i < hi - lo; ++i) {
                auto tmp = matrix[lo][lo + i];
                matrix[lo][lo + i] = matrix[hi - i][lo];
                matrix[hi - i][lo] = matrix[hi][hi - i];
                matrix[hi][hi - i] = matrix[lo + i][hi];
                matrix[lo + i][hi] = tmp;
            }
        }
    }

// 59. Spiral Matrix II https://leetcode.com/problems/spiral-matrix-ii/?tab=Description
// Given an integer n, generate a square matrix filled with elements from 1 to n2 in spiral order.
// For example, Given n = 3, You should return the following matrix: [[1, 2, 3], [8, 9, 4], [7, 6, 5]]
    vector<vector<int>> generateMatrix(int n) {
        vector<vector<int>> res;
        for (int i = 0; i < n; ++i) res.emplace_back(n);
        int m = 0;
        n = n - 1;
        int val = 0;
        for (; m < n && m < n; ++m, --n) {
            for (int i = m; i < n; ++i) res[m][i] = ++val;
            for (int i = m; i < n; ++i) res[i][n] = ++val;
            for (int i = n; i > m; --i) res[n][i] = ++val;
            for (int i = n; i > m; --i) res[i][m] = ++val;
        }
        if (m == n) res[m][m] = ++val;
        return res;
    }
// 54. Spiral Matrix https://leetcode.com/problems/spiral-matrix/?tab=Description
// Given a matrix of m x n elements (m rows, n columns), return all elements of the matrix in spiral order.
// For example, Given the following matrix: [[1, 2, 3], [4, 5, 6], [7, 8, 9]] You should return [1,2,3,6,9,8,7,4,5].
// scan ring by ring.
    vector<int> spiralOrder(vector<vector<int>>& matrix) {
        vector<int> res;
        if (matrix.empty()) return res;
        int r1 = 0, r2 = matrix.size() - 1;
        int c1 = 0, c2 = matrix[0].size() - 1;
        for (; r1 < r2 && c1 < c2; ++r1, --r2, ++c1, --c2) {
            for (int i = c1; i < c2; ++i) res.push_back(matrix[r1][i]);
            for (int i = r1; i < r2; ++i) res.push_back(matrix[i][c2]);
            for (int i = c2; i > c1; --i) res.push_back(matrix[r2][i]);
            for (int i = r2; i > r1; --i) res.push_back(matrix[i][c1]);
        }
        if (r1 == r2) for (; c1 <= c2; ++c1) res.push_back(matrix[r1][c1]);
        else if (c1 == c2) for (; r1 <= r2; ++r1) res.push_back(matrix[r1][c1]);
        return res;
    }
namespace test {
    void generateMatrix() {
        for (auto x: {3}) {
            auto res = ::generateMatrix(x);
            print(res,"res");
        }
    }
}

// 62. Unique Paths https://leetcode.com/problems/unique-paths/?tab=Description
// A robot is located at the top-left corner of a m x n grid (marked 'Start' in the diagram below).
// The robot can only move either down or right at any point in time.
// The robot is trying to reach the bottom-right corner of the grid (marked 'Finish' in the diagram below).
// How many possible unique paths are there?
// Note: m and n will be at most 100.
// calculate combination number. https://discuss.leetcode.com/topic/5864/simple-c-version-using-math  http://stackoverflow.com/questions/1838368/calculating-the-amount-of-combinations   http://stackoverflow.com/questions/9330915/number-of-combinations-n-choose-r-in-c
    int uniquePaths(int m, int n) {
        --m, --n;
        if (m < n) { std::swap(m, n); }
        n += m;
        long long res = 1;
        for (int i = 1; i <= n - m; ++i) {
            res = res * (m+i) / i;
        }
        return res; // long long to int conversion is implementation defined, contrary to unsigned type, which truncate high bits.
    }

// 63. Unique Paths II https://leetcode.com/problems/unique-paths-ii/?tab=Description
// Follow up for "Unique Paths": Now consider if some obstacles are added to the grids.
// How many unique paths would there be? An obstacle and empty space is marked as 1 and 0 respectively in the grid.
// For example, There is one obstacle in the middle of a 3x3 grid as illustrated below.
// [
//   [0,0,0],
//   [0,1,0],
//   [0,0,0]
// ]
// The total number of unique paths is 2.
// Note: m and n will be at most 100.
// combination problem, could variant of nextCombination be used?
// given (i,j) being obstacle, then count(val='down',from=0,to=i+j+2) != i+1.
// maintain on row. https://discuss.leetcode.com/topic/5862/i-saw-many-answers-are-using-2-d-vector-which-is-unnecessary-check-my-1-d-vector-solution
// only initialize dp's top-left item. https://discuss.leetcode.com/topic/4325/my-c-dp-solution-very-simple/2
    int uniquePathsWithObstacles_m_n_space(vector<vector<int>>& obstacleGrid) {
        int m = obstacleGrid.size(), n = obstacleGrid[0].size();
        vector<vector<int>> dp(m, vector<int>(n, 0));
        // auto dp = obstacleGrid;
        // for (auto &v: dp) for (auto &x: v) x ^= 1;
        for (int i = 0; i < m && obstacleGrid[i][0] == 0; ++i) dp[i][0] = 1;
        for (int i = 0; i < n && obstacleGrid[0][i] == 0; ++i) dp[0][i] = 1;
        for (int i = 1; i < m; ++i) {
            for (int j = 1; j < n; ++j) {
                if (obstacleGrid[i][j] == 0)
                    dp[i][j] += dp[i-1][j] + dp[i][j-1];
            }
        }
        return dp.back().back();
    }
    int uniquePathsWithObstacles(vector<vector<int>>& obstacleGrid) {
        int n = obstacleGrid[0].size();
        vector<int> dp(n,0);
        dp[0] = 1;
        for (auto &row: obstacleGrid) {
            dp[0] &= (row[0] == 0);
            for (int i = 1; i < n; ++i) {
                if (row[i] == 0)
                    dp[i] += dp[i-1];
                else dp[i] = 0;
            }
        }
        return dp.back();
    }

// 64. Minimum Path Sum https://leetcode.com/problems/minimum-path-sum/?tab=Description
// Given a m x n grid filled with non-negative numbers, find a path from top left to bottom right which minimizes the sum of all numbers along its path.
// Note: You can only move either down or right at any point in time.
    int minPathSum(vector<vector<int>>& grid) {
        vector<int> dp(grid[0].size(), INT_MAX);
        dp[0] = 0;
        for (auto &v: grid) {
            // print(dp,"dp");
            dp[0] += v[0];
            for (int i = 1; i < dp.size(); ++i) dp[i] = std::min(dp[i], dp[i-1]) + v[i];
        }
            // print(dp,"dp");
        return dp.back();
    }
namespace test {
    void minPathSum() {
        // vector<vector<int>> vec{{1,2},{1,1}};
        vector<vector<int>> vec{{1,3,1},{1,5,1},{4,2,1}};
        auto res = ::minPathSum(vec);
        print(vec,"grid");
        cout << "::minPathSum(grid) == " << res << endl;
    }
}

// 174. Dungeon Game https://leetcode.com/problems/dungeon-game/?tab=Description
// TODO below failed on last few test cases.
// why start from top-left not work? or too complex to work, compared with starting from destination?
// dp[i][j]表示进入这个格子后保证knight不会死所需要的最小HP。如果一个格子的值为负，那么进入这个格子之前knight需要有的最小HP是-dungeon[i][j] + 1.如果格子的值非负，那么最小HP需求就是1.  http://blog.csdn.net/likecool21/article/details/42516979
// 不过这道题从左上角开始递推不好推，因为要考虑两个因素，一是剩余的血越多越好 (血量多可以使得以后的路程中血量更加高一点) ，行进过程中血量最低值越高越好 (这个直接影响到结果) ，这两个因素都要考虑，且相互影响。其实我们可以从右下角开始推，dp表示如果从该格子出发，最少要多少初始血量。 http://blog.csdn.net/u013291394/article/details/51674910
    int calculateMinimumHP_start_at_start_point_not_work_yet(vector<vector<int>>& dungeon) {
        vector<int> m(dungeon[0]);  // max
        std::partial_sum(m.begin(), m.end(), m.begin());
        vector<int> m_val(m);
        for (int i = 1; i < m_val.size(); ++i) { m_val[i] = std::min(m_val[i], m_val[i-1]); }
        vector<int> mm(m);
        vector<int> mm_val(m_val);
            print(m,"m");
            print(m_val,"m_val");
            print(mm,"mm");
            print(mm_val,"mm_val");
            cout << endl;
        for (int j = 1; j < dungeon.size(); ++j) {
            auto &v = dungeon[j];
            m[0] = mm[0] = (m[0] + v[0]);
            m_val[0] = mm_val[0] = std::min(m_val[0], m[0]);
            for (int i = 1; i < v.size(); ++i) {
                if (m[i-1] > m[i]) {
                    m[i] = m[i-1];
                    m_val[i] = m_val[i-1];
                }
                m[i] += v[i];
                m_val[i] = std::min(m_val[i], m[i]);
                auto left = std::min(mm_val[i-1], mm[i-1] + v[i]);
                mm_val[i] = std::min(mm_val[i], mm[i] + v[i]);
                if (left > mm_val[i] || (left == mm_val[i] && mm[i-1] > mm[i])) {
                // if (mm_val[i-1] > mm_val[i] || (mm_val[i-1] == mm_val[i] && mm[i-1] > mm[i])) {
                    mm_val[i] = left;
                    mm[i] = mm[i-1];
                }
                mm[i] += v[i];
                if (m_val[i] > mm_val[i] || (m_val[i] == mm_val[i] && m[i] > mm[i])) {
                    mm_val[i] = m_val[i];
                    mm[i] = m[i];
                }
                if (mm[i] == m[i] && mm_val[i] > m_val[i]) { m_val[i] = mm_val[i]; }
            }
            print(m,"m");
            print(m_val,"m_val");
            print(mm,"mm");
            print(mm_val,"mm_val");
            cout << endl;
        }
        return (mm_val.back() <= 0)? (1 - mm_val.back()) : 1;
    }

    int calculateMinimumHP(vector<vector<int>>& dungeon) {
        vector<int> dp(dungeon[0].size() + 1, INT_MAX);
        dp[dp.size() - 2] = 1;
        for (int m = dungeon.size(); --m >= 0;) {
            auto &v = dungeon[m];
            for (int n = dungeon[0].size(); --n >= 0;) {
                dp[n] = std::max(1, std::min(dp[n], dp[n+1]) - v[n]);
            }
        }
        return dp[0];
    }

namespace test {
    void calculateMinimumHP() {
        vector<vector<vector<int>>> ts;
        ts.push_back({{0}});    // 1
        ts.push_back({{100}});    // 1
        ts.push_back({{-2,-3,3},{-5,-10,1},{10,30,-5}}); // 7, -2 -3 3 1 -5
        ts.push_back({{1,-3,2},{0,-1,2},{0,0,-2}});     // 1, 1 0 -1 2 -2
        ts.push_back({{1,2,1},{-2,-3,-3},{3,2,-2}});    // 1, 1 2 3 2 -2
        for (auto &vec: ts) {
            auto res = ::calculateMinimumHP(vec);
            print(vec,"dungeon");
            cout << "::calculateMinimumHP(dungeon) == " << res << endl;
        }
    }
}

// 68. Text Justification https://leetcode.com/problems/text-justification/?tab=Description
// Given an array of words and a length L, format the text such that each line has exactly L characters and is fully (left and right) justified. You should pack your words in a greedy approach; that is, pack as many words as you can in each line. Pad extra spaces ' ' when necessary so that each line has exactly L characters. Extra spaces between words should be distributed as evenly as possible. If the number of spaces on a line do not divide evenly between words, the empty slots on the left will be assigned more spaces than the slots on the right. For the last line of text, it should be left justified and no extra space is inserted between words. 
    vector<string> fullJustify(vector<string>& words, int maxWidth) {
        vector<string> res;
        for (int i = 0, sz = words.size(); i < sz;) {
            int cnt = words[i].size();
            int j = i;
            for (++i; i < sz && cnt + words[i].size() + 1 <= maxWidth; ++i) { cnt += words[i].size() + 1; }
            string s (maxWidth, ' ');
            int n_space = 1, one_more = 0;
            if (i != sz && i - j > 1) n_space = 1 + (maxWidth - cnt) / (i - j - 1), one_more = (maxWidth - cnt) % (i - j - 1);
            // print({i,j,cnt,maxWidth,n_space,one_more},"i,j,cnt,maxWidth,n_space,one_more");
            auto p = s.begin();
            for (; j < i; ++j) {
                for (int k = 0; k < words[j].size(); ++k) { *p++ = words[j][k]; }
                p += n_space + (one_more-- > 0);
            }
            res.push_back(std::move(s));
        }
        // cout << "res.size() == " << res.size() << endl;
        return res;
    }

namespace test {
    void fullJustify() {
        // std::vector<std::string> v{"This", "is", "an", "example", "of", "text", "justification."};
        // int L = 16;
        std::vector<std::string> v{"What","must","be","shall","be."};
        int L = 12;

        auto res = ::fullJustify(v, L);
        print(res,"",'\n');
    }
}

// 496. Next Greater Element I https://leetcode.com/problems/next-greater-element-i/?tab=Description
// nums1 is subset of nums2.
// TODO makeHashmap makeumap makehmap
// -- key point, decreasing stack.
    vector<int> nextGreaterElement_brute_force(vector<int>& findNums, vector<int>& nums) {
        vector<int> res(findNums.size(), -1);
        for (int i = 0; i < findNums.size(); ++i) {
            auto p = std::find(nums.begin(), nums.end(), findNums[i]);
            if (p != nums.end()) {
                auto q = std::find_if(p, nums.end(), [p](int x) { return x > *p; });
                if (q != nums.end()) res[i] = *q;
            }
        }
        return res;
    }
    vector<int> nextGreaterElement_hashmap(vector<int>& findNums, vector<int>& nums) {
        vector<int> stk;
        std::unordered_map<int, int> hash;
        vector<int> res(findNums.size(), -1);
        int cnt = 0;
        for (auto x: findNums) hash[x] = cnt++;
        for (int i = 0; i < nums.size(); ++i) {
            for (; !stk.empty() && findNums[stk.back()] < nums[i];) {
                res[stk.back()] = nums[i];
                stk.pop_back();
            }
            auto p = hash.find(nums[i]) ;
            if (p != hash.end()) {
                stk.push_back(p->second);
            }
        }
        return res;
    }
    vector<int> nextGreaterElement(vector<int>& findNums, vector<int>& nums) {
        return nextGreaterElement_hashmap(findNums, nums);
        return nextGreaterElement_brute_force(findNums, nums);
    }
namespace test {
    void nextGreaterElement() {
        std::vector<std::vector<int>> vec;
        // vec.push_back({4,1,2});
        // vec.push_back({1,3,4,2});
        vec.push_back({1,3,2,5,4});
        vec.push_back({3,2,1});
        for (; vec.size() > size_t(1);) {
            auto lhs = vec.back();
            vec.pop_back();
            auto rhs = vec.back();
            vec.pop_back();
            auto res = ::nextGreaterElement(lhs, rhs);
            print(res,"res");
        }
    }
}


// 503. Next Greater Element II https://leetcode.com/problems/next-greater-element-ii/?tab=Description
    vector<int> nextGreaterElements(vector<int>& nums) {
        std::vector<int> res(nums.size(), -1);
        if (nums.size() > 1) {
            const int start = std::max_element(nums.begin(), nums.end()) - nums.begin();
            std::vector<int> stk;
            for (int sz = nums.size(), i = (start + 1) % sz; ;) {
                for (; !stk.empty() && nums[stk.back()] < nums[i];) {
                    res[stk.back()] = nums[i];
                    stk.pop_back();
                }
                if (nums[i] != nums[start]) { stk.push_back(i); }
                if(i == start) break;
                i = (i + 1) % sz;
            }
        }
        return res;
    }
namespace test {
    void nextGreaterElements() {
        std::vector<std::vector<int>> vec;
        vec.push_back({1,3,2,5,4});
        for (auto &v: vec) {
            auto res = ::nextGreaterElements(v);
            print(res,"res");
        }
    }
}

int main() {
    test::init();
    // test::isValid();
    test::longestValidParentheses();
    // test::lengthOfLongestSubstring();
    // test::longestPalindrome();
    // test::search();
    // test::trap();
    // test::multiply();
    // test::jump();
    // test::groupAnagrams();
    // test::generateMatrix();
    // test::minPathSum();
    // test::calculateMinimumHP();
    // test::fullJustify();
    // test::fullJustify();
}
