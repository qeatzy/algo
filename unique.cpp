#include "utility.h"
// unique.cpp, or set property, depend on both value space (char set) and length of sequence. see also " isomorphism.cpp

// check if a given array is unique, eg, a string.
// non-unique if len > len(char set), -- pigeon-hole principle.
// for small value space, use trivial linear hash table based on value. eg, m[256] for ASCII, m[26] for lower-case.
// non-trivial hash table solution.
// sort then compare adjacent element solution.

// 387. First Unique Character in a String  https://leetcode.com/problems/first-unique-character-in-a-string/#/description
// Given a string, find the first non-repeating character in it and return it's index. If it doesn't exist, return -1.
// Examples:
// s = "leetcode"
// return 0.
// s = "loveleetcode",
// return 2.
// Note: You may assume the string contain only lowercase letters. 
    int firstUniqChar(string s) {
        int hash[256] = {0};
        int pos[256];
        for (int i = 0, sz = s.size(); i < sz; ++i) {
            ++hash[s[i]];
            pos[s[i]] = i;
        }
        int res = s.size();
        for (int i = 'a'; i <= 'z'; ++i) {
            if (hash[i] == 1 && res > pos[i]) res = pos[i];
        }
        return (res == s.size()) ? -1 : res;
    }

namespace test {
    void firstUniqChar() {
        std::vector<std::string> vec{"leetcode", "loveleetcode"};
        for (auto &s: vec) {
            auto res = ::firstUniqChar(s);
            cout << "::firstUniqChar(s) = " << res << ", where s = " << s << endl;
        }
    }
}

int main() {
    test::init();
    test::firstUniqChar();
}
