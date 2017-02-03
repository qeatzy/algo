#include <set>
#include "utility.h"
// https://leetcode.com/problems/letter-combinations-of-a-phone-number/
// https://discuss.leetcode.com/topic/8465/my-java-solution-with-fifo-queue/2

    const std::vector<std::string> phoneTable {"abc", "def", "ghi", "jkl", "mno", "pqrs", "tuv", "wxyz"};
    const std::string alphabet = "23456789";

    std::vector<std::string> phoneLetterComb_loop(const string &digits) {
        // if (DEBUG) assert(issubset(digits, alphabet));
        std::vector<std::string> res;
        if (digits.empty()) return res;
        res.push_back(""); // res{""} instead of res{}, since identity of multiplication is 1, not 0.
        std::vector<std::string> prev;
        for (auto digit: digits) {
            prev.clear();
            for (auto ch: phoneTable[digit - '2'])
                for (auto const &prefix: res) {
                    prev.push_back(prefix + ch);
                }
            prev.swap(res);
        }
        return res;
    }

    std::vector<std::string> phoneLetterComb_recurse_iter(const char *first, const char *last) {
        // if (DEBUG) assert(issubset(string(first, last), alphabet));
        std::vector<std::string> res;
        if (first == last) return res;
        auto const &str = phoneTable[*--last - '2'];
        auto subRes = phoneLetterComb_recurse_iter(first, last);
        if (subRes.empty()) { subRes.emplace_back(""); } // fix to identity(1, not 0) for trivial recurse. delegate-and-fix instead of pre-check.
        // for (auto ch: phoneTable[digit - '2'])
        for (auto ch: str) {
            for (auto const &prefix: subRes) {
                res.push_back(prefix + ch);
            }
        }
        return res;
    }
    std::vector<std::string> phoneLetterComb_recurse(const string &digits) {
        return phoneLetterComb_recurse_iter(&digits[0], &digits[0] + digits.size());
    }

    // https://discuss.leetcode.com/topic/13160/8-line-backtracking-function-c-solution/
    void phoneLetterComb_backtrack_iter(const char *first, const char *last, string &prefix, std::vector<std::string> &res) {
        // if (DEBUG) assert(issubset(string(first, last), alphabet));
        if (first == last) {
            res.push_back(prefix);
        } else {
            for (auto ch: phoneTable[*first++ - '2']) {
                prefix.push_back(ch);
                phoneLetterComb_backtrack_iter(first, last, prefix, res);
                prefix.pop_back();
            }
        }
    }
    std::vector<std::string> phoneLetterComb_backtrack(const string &digits) {
        std::vector<std::string> res;
        if (digits.empty()) return res;
        string prefix;
        phoneLetterComb_backtrack_iter(&digits[0], &digits[0] + digits.size(), prefix, res);
        return res;
    }


    std::vector<std::string> phoneLetterComb(string digits) {
        // return phoneLetterComb_loop(digits);
        // return phoneLetterComb_recurse(digits);
        return phoneLetterComb_backtrack(digits);
    }

namespace test {
    void phoneLetterComb() {
        auto test_case = []() {
            std::vector<std::string> res;
            res.push_back("23");
            return res;
        };
        for (auto v: test_case()) {
            auto res = ::phoneLetterComb(v);
            print(res);
        }
    }
}

int main() {
    test::phoneLetterComb();
    // test::makeSet();
    // test::deduction();
}
