#include "utility.h"
// tag: String. https://leetcode.com/tag/string/
// edit distance. Levenshtein distance, Longest common subsequence (LCS) distance(insertions and deletions only), Hamming distance.  https://en.wikipedia.org/wiki/Edit_distance
// https://en.wikipedia.org/wiki/Longest_common_subsequence_problem
// fast string search.

// 139. Word Break https://leetcode.com/problems/word-break/?tab=Description
// Given a non-empty string s and a dictionary wordDict containing a list of non-empty words, determine if s can be segmented into a space-separated sequence of one or more dictionary words. You may assume the dictionary does not contain duplicate words. 
// TODO prefix-tree? if more than one match, try which one first, longer one? similarly, suffix-tree could be used or both.
    bool wordBreak(string s, vector<string>& wordDict) {
        return false;
    }

// 8. String to Integer (atoi) https://leetcode.com/problems/string-to-integer-atoi/?tab=Description
    int myAtoi(string str) {
        // implement base 10 first.  return INT_MAX/INT_MIN for overflow_error.
        int res = 0;
        auto p = str.begin();
        for (; p != str.end() && isspace(*p); ++p) {}
        int sign = 1;
        if (*p == '-') { ++p; sign = -1; }
        else if (*p == '+') { ++p; }
        auto last = std::min(p + 9, str.end());
        for (; p != last && isdigit(*p); ++p) { res = res * 10 + (*p - '0'); }
        if (p == str.end() || !isdigit(*p)) return res * sign;
        if (res > INT_MAX / 10) return (sign > 0)? INT_MAX : INT_MIN;
        res = res * 10 + (*p - '0');
        if ((++p != str.end() && isdigit(*p)) || res < 0) return (sign > 0)? INT_MAX : INT_MIN;
        return res * sign;
    }
namespace test {
    void atoi() {
        auto test_case = []() {
            std::vector<const char*> res;
            res.push_back("abc");
            res.push_back("126");
            res.push_back("  126");
            res.push_back("  126abc");
            res.push_back(" -126");
            res.push_back(" - 126");
            res.push_back("12666666666666666");
            res.push_back("2147483648");
            res.push_back("-2147483648");
            res.push_back("-2147483649");
            res.push_back("1");
            res.push_back("      -11919730356x");
            return res;
        };
        int num;
        for (auto s: test_case()) {
            // num = std::atoi(s);
            // num = ::atoi(s);
            num = ::myAtoi(s);
            cout << num << "\tfrom [" << s << "]" << endl;
        }
    }
}

// 65. Valid Number https://leetcode.com/problems/valid-number/?tab=Description
// Validate if a given string is numeric.
// Some examples: "0" => true " 0.1 " => true "abc" => false "1 a" => false "2e10" => true
// Note: It is intended for the problem statement to be ambiguous.
// You should gather all requirements up front before implementing one.
// -- see how python interpreter handle invalid numeric input.
    bool isNumber(string s) {
        auto first = s.begin(), last = s.end();
        for (; first != last && isspace(*first); ++first) {}
        for (; last != first && isspace(last[-1]); --last) {}
        if (first == last) return false;
        // int sign = 1;
        if(*first == '+' || *first == '-') {
            // if (*first == '-') sign = -1;
            ++first;
            // if (!isdigit(*first) && *first != '.') return false;
        }
        bool pre_dot = false;
        if (*first == '.') {
            ++first;
            pre_dot = true;
            // if (!isdigit(*first)) return false;
        }
        if (first == last || !isdigit(*first)) return false; // first letter must be digit, with optional leading '+-.'.
        for (; ++first != last && isdigit(*first);) {}
        if (first != last && *first == '.') { // handle trailing dot if no leading dot.
            if (pre_dot) return false;
            for (; ++first != last && isdigit(*first);) {}
        }
        if (first != last && *first == 'e') { // handle exponent
            ++first;
            if (*first == '+' || *first == '-') ++first;
            if (first == last || !isdigit(*first)) return false; // first letter must be digit after 'e'.
            for (; ++first != last && isdigit(*first);) {}
        }
        return first == last;
    }
namespace test {
    void isNumber() {
        std::vector<std::string> vec;
        vec.push_back("2e10");
        vec.push_back("0");
        vec.push_back("0.1");
        vec.push_back("abc");
        vec.push_back("1 a");
        vec.push_back(".34");
        vec.push_back("34.");
        vec.push_back(".34.");
        vec.push_back("34.e2");
        vec.push_back(".34e-12");
        vec.push_back("1e.");
        vec.push_back("e");
        vec.push_back("0e");
        vec.push_back("");
        for (auto &s: vec) {
            auto res = ::isNumber(s);
            cout << "s = " << s << ", ::isNumber(s) == " << res << endl;
        }
    }
}

// 541. Reverse String II https://leetcode.com/problems/reverse-string-ii/#/description
// Given a string and an integer k, you need to reverse the first k characters for every 2k characters counting from the start of the string.
// If there are less than k characters left, reverse all of them.
// If there are less than 2k but greater than or equal to k characters, then reverse the first k characters and left the other as original.
// Example: Input: s = "abcdefg", k = 2 Output: "bacdfeg"
// Restrictions: The string consists of lower English letters only. Length of the given string and k will in the range [1, 10000]
    string reverseStr(string s, int k) {
        for (auto first = s.begin(), last = s.end(); ; first += 2 * k) {
            if (last - first < 2 * k) {
                if (last - first > k) last = first + k;
                std::reverse(first, last);
                return s;
            }
            std::reverse(first, first + k);
        }
    }

// 71. Simplify Path https://leetcode.com/problems/simplify-path/?tab=Description
// Given an absolute path for a file (Unix-style), simplify it.
// For example, path = "/home/", => "/home" path = "/a/./b/../../c/", => "/c"
// click to show corner cases.
// -- or use getline and stream. getline accept third argument as delimter. but be careful, getline is not split and do not skip empty string. eg, https://discuss.leetcode.com/topic/8678/c-10-lines-solution
    string simplifyPath_getline(string path) {
        std::vector<std::string> sim;
        std::stringstream ss(path);
        string s, res;
        while (getline(ss,s,'/')) {
            if (s == "" || s == ".") {
            } else if (s == "..") {
                if (!sim.empty())
                    sim.pop_back();
            } else {
                sim.push_back(s);
            }
        }
        // for (auto part: sim) res += "/" + part;
        for (auto part: sim) res += "/" + part;
        return res.empty() ? "/" : res;
    }
    string simplifyPath_spilt_and_join(string path) {
        std::vector<std::string> sim;
        auto vec = split(path,'/');
        for (auto s: vec) {
            if (s == ".") {
            } else if (s == "..") {
                if (!sim.empty())
                    sim.pop_back();
            } else {
                sim.push_back(s);
            }
        }
        // print(vec,"vec",'\n');
        // print(sim,"sim");
        return "/"+join(sim,"/");
    }
    string simplifyPath(string path) {
        return simplifyPath_getline(path);
        return simplifyPath_spilt_and_join(path);
    }

namespace test {
    void simplifyPath() {
        std::vector<std::string> vec;
        vec.push_back("/a/./b/../../c/");
        vec.push_back("/home/");
        vec.push_back("///");
        for (auto &path: vec) {
            auto res = ::simplifyPath(path);
            cout << "res = " << res << endl;
        }
    }
}

// 273. Integer to English Words https://leetcode.com/problems/integer-to-english-words/?tab=Description
// Convert a non-negative integer to its english words representation.
// Given input is guaranteed to be less than 231 - 1.
// For example, 123 -> "One Hundred Twenty Three" 12345 -> "Twelve Thousand Three Hundred Forty Five" 1234567 -> "One Million Two Hundred Thirty Four Thousand Five Hundred Sixty Seven"
// click to show hint.
// TODO 
    string numberToWords(int num) {
        return "";
    }

// 127. Word Ladder https://leetcode.com/problems/word-ladder/?tab=Description
// 126. Word Ladder II https://leetcode.com/problems/word-ladder-ii/?tab=Description

// 72. Edit Distance https://leetcode.com/problems/edit-distance/?tab=Description
// Given two words word1 and word2, find the minimum number of steps required to convert word1 to word2. (each operation is counted as 1 step.)
// You have the following 3 operations permitted on a word: a) Insert a character b) Delete a character c) Replace a character
// -- see also 127 word ladder.
// TODO 
// remove common prefix/suffix first. then convert shorter to longer. -- then we won't do deletion unless have large benefit, eg, large LCS length.
    int minDistance(string word1, string word2) {
        int res = 0;
        return res;
    }
namespace test {
    void minDistance() {
        std::vector<std::string> vec;
        vec.push_back("kitten");
        vec.push_back("sitting");
        vec.push_back("xabcde");
        vec.push_back("eabcdx");
        vec.push_back("horse");
        vec.push_back("ros"); // ros -> hos -> hors -> horse
        for (; vec.size() > size_t(1);) {
            auto lhs = vec.back();
            vec.pop_back();
            auto rhs = vec.back();
            vec.pop_back();
            auto res = ::minDistance(lhs, rhs);
            print({lhs,rhs},"lhs,rhs");
            cout << "::minDistance(lhs, rhs) = " << res << endl;
        }
    }
}

// 28. Implement strStr() https://leetcode.com/problems/implement-strstr/?tab=Description
// Implement strStr(). Returns the index of the first occurrence of needle in haystack, or -1 if needle is not part of haystack. 

// 345. Reverse Vowels of a String https://leetcode.com/problems/reverse-vowels-of-a-string/?tab=Description
// Write a function that takes a string as input and reverse only the vowels of a string.
// Example 1: Given s = "hello", return "holle".
// Example 2: Given s = "leetcode", return "leotcede".
// Note: The vowels does not include the letter "y".
    string reverseVowels(string s) {
        auto isVowel = [](char ch) { return ch == 'e' || ch == 'i' || ch == 'a' || ch == 'o' || ch == 'u' ||  ch == 'E' || ch == 'I' || ch == 'A' || ch == 'O' || ch == 'U'; };
        if (s.empty()) return s;
        auto left = s.begin(), right = s.end() - 1;
        for (;;) {
            for (; !isVowel(*left); ++left) {}
            for (; !isVowel(*right); --right) {}
            if (left >= right) break;
            std::iter_swap(left, right);
            ++left, --right;
        }
        return s;
    }

namespace test {
    void reverseVowels() {
        std::vector<std::string> vec{"hello", "leetcode", "aA"};
        for (auto &s: vec) {
            auto res = ::reverseVowels(s);
            cout << "s = " << s << ", ::reverseVowels(s) = " << res << endl;
        }
    }
}

// 151. Reverse Words in a String https://leetcode.com/problems/reverse-words-in-a-string/?tab=Description
// Given an input string, reverse the string word by word.
// For example, Given s = "the sky is blue", return "blue is sky the". 
// For C programmers: Try to solve it in-place in O(1) space. 
// A clean C++ solution, 7 lines client code, given an implementation of trimAll().  https://discuss.leetcode.com/topic/82262/
// the idea is very simple, as the code clearly exemplify.
// 1. trimAll. trim leading and trailing space, trim inner contiguous spaces to single one.
// 2. reverse whole string.
// 3. reverse each word.
    void reverseWords(string &s) {
        trimAll(s,' ');
        std::reverse(s.begin(), s.end());
        for (auto first = s.begin(), last = s.end(); ;) {
            auto mid = std::find(first, last, ' ');
            // auto mid = std::find_if(first, last, ::isspace); // :: is a hack, since isspace is overloaded and the compiler can't disambiguate, std::isspace not work.  see http://stackoverflow.com/a/8364707/3625404
            std::reverse(first, mid);
            if (mid == last) break;
            first = ++mid;
        }
    }

namespace test {
    void reverseWords() {
        std::vector<std::string> vec;
        vec.push_back("the sky is blue");
        vec.push_back(" ");
        vec.push_back("1 ");
        vec.push_back(" 1");
        vec.push_back("   a   b ");
        vec.push_back("   a   ");
        for (auto &s: vec) {
            auto copy = s;
            ::reverseWords(s);
            cout << " s = [" << copy << "], reverseWords(s) = [" << s << "]" << endl;
        }
    }
}

// 30. Substring with Concatenation of All Words https://leetcode.com/problems/substring-with-concatenation-of-all-words/?tab=Description
// You are given a string, s, and a list of words, words, that are all of the same length.
// Find all starting indices of substring(s) in s that is a concatenation of each word in words exactly once and without any intervening characters.
// For example, given: s: "barfoothefoobarman" words: ["foo", "bar"] You should return the indices: [0,9]. (order does not matter). 
// TODO 
    vector<int> findSubstring(string s, vector<string>& words) {
        vector<int> res;
        return res;
    }

// 76. Minimum Window Substring https://leetcode.com/problems/minimum-window-substring/?tab=Description
// Given a string S and a string T, find the minimum window in S which will contain all the characters in T in complexity O(n).
// For example, S = "ADOBECODEBANC" T = "ABC" Minimum window is "BANC".
// Note: If there is no such window in S that covers all characters in T, return the empty string "".
// If there are multiple such windows, you are guaranteed that there will always be only one unique minimum window in S. 
// TODO search
    string minWindow(string s, string t) {
        return s;
    }

// 44. Wildcard Matching https://leetcode.com/problems/wildcard-matching/?tab=Description
// TODO 
// Implement wildcard pattern matching with support for '?' and '*'.
// '?' Matches any single character.
// '*' Matches any sequence of characters (including the empty sequence).
// The matching should cover the entire input string (not partial).
// The function prototype should be:
// bool isMatch(const char *s, const char *p)
// Some examples:
// isMatch("aa","a") → false
// isMatch("aa","aa") → true
// isMatch("aaa","aa") → false
// isMatch("aa", "*") → true
// isMatch("aa", "a*") → true
// isMatch("ab", "?*") → true
// isMatch("aab", "c*a*b") → false
    bool isMatch(string s, string p) {
        return false;
    }

// "ls
// leetcode Ternary Expression Parser    http://www.cnblogs.com/grandyang/p/6022498.html    https://discuss.leetcode.com/category/564/ternary-expression-parser
// 394. Decode String https://leetcode.com/problems/decode-string/?tab=Description

int main() {
    test::init();
    // test::atoi();
    // test::simplifyPath();
    // test::reverseVowels();
    // test::minDistance();
    test::reverseWords();
    // test::maxProfit();
}
