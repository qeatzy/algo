#include "utility.h"
// "ls, premium problem, search on google, there must be problem description and solution, and make your own test case.
// leetcode Word Pattern II
// leetcode Android Unlock Patterns
// leetcode Word Squares
// leetcode 
// leetcode 
// leetcode 
// Reverse Words in a String II     [LeetCode] Reverse Words in a String II 翻转字符串中的单词之二 http://www.cnblogs.com/grandyang/p/5186294.html
// Leetcode 算法题解  403 being largest number of problem. https://www.hrwhisper.me/leetcode-algorithm-solution/
// leetcode 60 Find the Celebrity
// leetcode 61 Paint Fence

// 36. Valid Sudoku https://leetcode.com/problems/valid-sudoku/?tab=Description
// The Sudoku board could be partially filled, where empty cells are filled with the character '.'.
// A valid Sudoku board (partially filled) is not necessarily solvable. Only the filled cells need to be validated.
// There are just 3 rules to Sudoku. Each row must have the numbers 1-9 occuring just once.
// Each column must have the numbers 1-9 occuring just once.
// And the numbers 1-9 must occur just once in each of the 9 sub-boxes of the grid.
    bool isValidSudoku_set(vector<vector<char>>& board) {
        std::set<char> row, col, cube;
        for (int i = 0; i < 9; ++i) {
            auto r_start = 3 * (i / 3), c_start = 3 * (i % 3);
            cout << "r_start = " << r_start << ", c_start = " << c_start << endl;
            for (int j = 0; j < 9; ++j) {
                if (board[i][j] != '.') {
                    if (row.find(board[i][j]) != row.end()) return false;
                    row.insert(board[i][j]);
                }
                if (board[j][i] != '.') {
                    if (col.find(board[j][i]) != col.end()) return false;
                    col.insert(board[j][i]);
                }
                auto r = r_start + j / 3, c = c_start + j % 3;
                if (board[r][c] != '.') {
                    if (cube.find(board[r][c]) != cube.end()) return false;
                    cube.insert(board[r][c]);
                }
            }
            row.clear();
            col.clear();
            cube.clear();
        }
        return true;
    }
    bool isValidSudoku_mark(vector<vector<char>>& board) {
        char vis[9][9];
        memset(vis, 0, sizeof(vis));
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                if (board[i][j] != '.') {
                    int k = board[i][j] - '1';
                    auto row = &vis[i][k];
                    auto col = &vis[j][k];
                    auto cube = &vis[3 * (i / 3) + (j / 3)][k];
                    if ((*row & 1) || (*col & 2) || (*cube & 4)) return false;
                    *row |= 1;
                    *col |= 2;
                    *cube |= 4;
                }
            }
        }
        return true;
    }
    bool isValidSudoku(vector<vector<char>>& board) {
        // return isValidSudoku_set(board);
        return isValidSudoku_mark(board);
    }

// 37. Sudoku Solver https://leetcode.com/problems/sudoku-solver/?tab=Description
// Write a program to solve a Sudoku puzzle by filling the empty cells. Empty cells are indicated by the character '.'.
// You may assume that there will be only one unique solution.
// TODO
    void solveSudoku(vector<vector<char>>& board) {
        
    }

namespace test {
    void printSudoku(vector<vector<char>>& board) {
        for (auto v: board) {
            print(v);
        }
    }
    void isValidSudoku() {
        auto test_case = []() {
            // std::vector<std::string> res;
            std::vector<std::vector<char>> res;
            // std::vector<const char*> res;
            // for (string s: {"....5..1.",".4.3.....",".....3..1","8......2.","..2.7....",".15......",".....2...",".2.9.....","..4......"}) {
            for (string s: {".........","......3..","...18....","...7.....","....1.97.",".........","...36.1..",".........",".......2."}) {
                auto row = vector<char>(s.begin(), s.end());
                res.push_back(row);
            }
            // res.push_back();
            return res;
        };
        auto board = test_case();
        print(board,"board");
        auto res = ::isValidSudoku(board);
        cout << res;
    }
}

// 7. Reverse Integer https://leetcode.com/problems/reverse-integer/?tab=Description
// Reverse digits of an integer. Example1: x = 123, return 321 Example2: x = -123, return -321
// TODO how to handle (integer) overflow_error systematically, without relying undefined behavior?
    int reverse(int x) {
        if (x == INT_MIN) return 0;
        int sign = 1;
        cout << (x < 0) << endl;
        cout << (INT_MIN < 0) << endl;
        cout << INT_MIN << endl;
        cout << (x - INT_MIN) << endl;
        cout << (x == INT_MIN) << endl;
        putchar('\n');
        if (x < 0) {
            cout << "x = " << x << ", -x = " << -x << endl;
            x = -x; sign = -1;
            cout << "after change sign, x = " << x << endl;
        }
        putchar('\n');
        cout << (x < 0) << endl;
        cout << (INT_MIN < 0) << endl;
        cout << INT_MIN << endl;
        cout << (x - INT_MIN) << endl;
        cout << (x == INT_MIN) << endl;
        putchar('\n');
        if (x < 0) return 0;
        int res = 0;
        for (; x && res < INT_MAX / 10; x /= 10) {
            res = res * 10 + x % 10;
        }
        cout << "x = " << x << ", res = " << res << endl;
        if (x != 0) {
            if (res > INT_MAX / 10 || x > 8 || (x == 8 && sign > 0) || res < 0) return 0;
            res = res * 10 + x % 10;
        }
        return res * sign;
    }

namespace test {
    void reverse_integer() {
        // std::vector<int> vec{123,321,1534236469,-2147483648};
        std::vector<int> vec{-2147483648};
        for (auto x: vec) {
            auto res = ::reverse(x);
            cout << "reverse of " << x << " is " << res << endl;
        }
    }
}

// 205. Isomorphic Strings https://leetcode.com/problems/isomorphic-strings/?tab=Description
// For example, Given "egg", "add", return true. Given "foo", "bar", return false. Given "paper", "title", return true.
// **aha**, use previous index as common value, thus two comparsion could be reduced to only one, in java one line core code.  https://discuss.leetcode.com/topic/32385/java-solution-with-1-line-core-code
// **aha**, since value of char is of limited range, we could use an array as hash table, with index (value of char minus some offset) as hash code.
// 10 lines C solution, without hash table.  https://discuss.leetcode.com/topic/16137/10-lines-c-solution-without-hash-table
// in python, return len(set(zip(s, t))) == len(set(s)) == len(set(t))   https://discuss.leetcode.com/topic/16693/1-line-in-python
    bool isIsomorphic(string s, string t) {
        if (s.size() != t.size()) return false;
        vector<char> s_arr(256, -1), t_arr(256, -1); // **bug**, fail if char is unsigned? or not? or increase the value space, eg, change char to int? or change push part to i+1 instead of i?
        for (size_t i = 0; i < s.size(); ++i) {
            if (s_arr[s[i]] == t_arr[t[i]]) { s_arr[s[i]] = t_arr[t[i]] = i; }
            else return false;
        }
        return true;
    }
    template < typename Container_s, typename Container_t>
    bool isIsomorphic_hash(Container_s s, Container_t t) {
        if (s.size() != t.size()) return false;
        std::map<typename Container_s::value_type, size_t> s_hash;;
        std::map<typename Container_t::value_type, size_t> t_hash;;
        for (size_t i = 0; i < s.size(); ++i) {
            auto p = s_hash.find(s[i]);
            auto q = t_hash.find(t[i]);
            if (p == s_hash.end() && q == t_hash.end()) { s_hash[s[i]] = t_hash[t[i]] = i + 1; }
            else if (p != s_hash.end() && q != t_hash.end() && s_hash[s[i]] == t_hash[t[i]]) {}
            else return false;
        }
        return true;
    }
namespace test {
    void isIsomorphic() {
        std::vector<std::string> vec {"egg", "add", "foo", "bar", "paper", "title"};
        assert(vec.size() % 2 == 0);
        while (!vec.empty()) {
            auto s = vec.back();
            vec.pop_back();
            auto t = vec.back();
            vec.pop_back();
            auto res = ::isIsomorphic(s,t);
            cout << "::isIsomorphic(s,t) == " << res << ", s = " << s << ", t = " << t << endl;
        }
    }
}

// 290. Word Pattern https://leetcode.com/problems/word-pattern/?tab=Description
// Given a pattern and a string str, find if str follows the same pattern. Here follow means a full match, such that there is a bijection between a letter in pattern and a non-empty word in str.
// or intead of using split(), we could use stream to read the string on the fly.
    bool wordPattern(string pattern, string str) {
        auto words = split(str, ' ');
        // print(words);
        return isIsomorphic_hash(pattern, words);
    }

// leetcode Word Pattern II
// TODO 
namespace test {
    void wordPattern() {
        std::vector<std::string> vec;
        vec.push_back("abba");
        vec.push_back("dog cat cat dog");
        vec.push_back("abba");
        vec.push_back("dog cat cat fish");
        vec.push_back("aaaa");
        vec.push_back("dog cat cat dog");
        vec.push_back("abba");
        vec.push_back("dog dog dog dog");
        assert(vec.size() % 2 == 0);
        while (!vec.empty()) {
            auto str = vec.back();
            vec.pop_back();
            auto pattern = vec.back();
            vec.pop_back();
            auto res = ::wordPattern(pattern, str);
            cout << "pattern = [" << pattern << "], str = [" << str << "], ::wordPattern(pattern, str) == " << res << endl;
        }
    }
}

// 58. Length of Last Word https://leetcode.com/problems/length-of-last-word/?tab=Description
// Given a string s consists of upper/lower-case alphabets and empty space characters ' ', return the length of last word in the string. If the last word does not exist, return 0.
    int lengthOfLastWord(string s) {
        int cnt = 0;
        auto it = s.rbegin();
        for (; it != s.rend() && *it == ' '; ++it) { }
        for (; it != s.rend() && *it != ' '; ++it) { ++cnt; }
        return cnt;
    }

// 287. Find the Duplicate Number https://leetcode.com/problems/find-the-duplicate-number/?tab=Description
// Given an array nums containing n + 1 integers where each integer is between 1 and n (inclusive), prove that at least one duplicate number must exist. Assume that there is only one duplicate number, find the duplicate one. 
// There is only one duplicate number in the array, but it could be repeated more than once.
// You must use only constant, O(1) extra space.
// Your runtime complexity should be less than O(n2). You must not modify the array (assume the array is read only).
// TODO similar to sort, 1. selection sort solution. 2. insertion sort solution. 3. merge sort solution (O(n) combine step). or 4. shell sort like?
// or with overlapping but slightly-overlapping partition? eg, 1 2 2 3 4 -> 1 2 2 + 2 3 4.
// -- not work, try 1 2 3 2 4.   6 7 8 9  1 2 3 2 4.     6 7 2 9 1 8 3 2 4.
// -- actually works, if both partition return false, then there is only two number than are equal, we could sum then construct, in linear time.
// -- but wait, that works for the final merge, what if the immediate merge?
// -- or we just return the sum for immediate case?
// or bottom up merge sort like?
// or sum then re-construct?
// or use better trimming for search path, instead of left to right.
// -- why not quick sort like, if pivot = x, count the value that equal to, less than, greater than x. -- binary search.
// TODO O(n) time O(1) space solution. 1. Floyd cycle detection 2. swap. use isomorphism info, similar to Floyd cycle // detection but use swap to restore to identity isomorphism.
// TODO O(n) time O(n) space solution, use array as hash, with the value record cnt. -- similar to swap solution, but which use hash inplace. eg, https://discuss.leetcode.com/topic/45526/three-different-solutions-from-worst-to-best-in-c-well-commented
    int findDuplicate_floyd_cycle_detection(vector<int>& nums) {
        int slow = 0, high = 0;
        do {
            slow = nums[slow];
            high = nums[nums[high]];
        } while (slow != high);
        slow = 0;
        while (slow != high) {
            slow = nums[slow];
            high = nums[high];
        }
        return slow;
    }
    int findDuplicate_binary_search(vector<int>& nums) {
        int sz = nums.size();
        int low = 1, high = sz + 1;
        for (; low < high;) {
            auto pivot = low + (high - low) / 2;
            int c1 = 0, c2 = 0;
            for (auto x: nums) {
                if (x < low || x >= high) continue;
                c1 += (x < pivot);
                c2 += (x > pivot);
            }
            // cout << "sz = " << high - low << ", low = " << low << ", high = " << high << endl;
            // cout << "pivot = " << pivot << ", c1 = " << c1 << ", c2 = " << c2 << endl;
            if (c1 + c2 + 1 < sz) return pivot;
            if (c1 <= c2) {
                low = pivot + 1;
                sz = c2;
            }
            else {
                high = pivot;
                sz = c1;
            }
        }
        return high;
    }
    template <typename RandomIt>
    int findDuplicate_selection_sort(RandomIt first, RandomIt last) {
        for (; first != last; ++first) {
            auto p = first;
            for (; ++p != last && *p != *first; ) {}
            if (p != last) return *first;
        }
        return -2;
    }
    template <typename RandomIt>
    int findDuplicate_merge_sort(RandomIt first, RandomIt last) { // not work, no feasible merge strategy that is linear.
        auto sz = last - first;
        if (sz < 2) return -2;
        if (sz == 2) {}
        auto mid = first + (1 + sz) / 2;
        auto res = findDuplicate_merge_sort(first, mid);
        if (res < 0) {
            mid = first + (last - first) / 2;
            res = findDuplicate_merge_sort(mid, last);
        }
        return res;
    }
    int findDuplicate_isomorphism(vector<int>& nums) {
        int sz = nums.size();
        for (int i = 0; i < sz; ++i) {
            for (; nums[i] != i;) {
                if (nums[i] == nums[nums[i]]) return nums[i];
                std::swap(nums[i], nums[nums[i]]);
            }
        }
        return nums[0];
    }
    int findDuplicate(vector<int>& nums) {
        // return findDuplicate_selection_sort(nums.begin(), nums.end());
        // return findDuplicate_binary_search(nums);
        // return findDuplicate_floyd_cycle_detection(nums);
        return findDuplicate_isomorphism(nums);
    }

namespace test {
    void findDuplicate() {
        std::vector<std::vector<int>> vec;
        vec.push_back({2,2,2,2,2});
        vec.push_back({1,1,2});
        for (auto nums: vec) {
            print(nums,"nums");
            auto res = ::findDuplicate(nums);
            cout << "duplicate is " << res << endl;
        }
    }
}

// 268. Missing Number https://leetcode.com/problems/missing-number/?tab=Description
// Given an array containing n distinct numbers taken from 0, 1, 2, ..., n, find the one that is missing from the array. 
// TODO swap solution, is that similar to find duplicate problem? or not? https://discuss.leetcode.com/topic/22601/swapping-numbers-to-the-same-index-cell
// and from d40a  https://discuss.leetcode.com/topic/23427/3-different-ideas-xor-sum-binary-search-java-code/6#
// 136. Single Number https://leetcode.com/problems/single-number/?tab=Description
// Given an array of integers, every element appears twice except for one. Find that single one.
    int missingNumber_xor(vector<int>& nums) { // index [0,n-1], value [0,n] exclude one of them.
        int sz = nums.size(), res = sz;
        for (int i = 0; i < sz; ++i) {
            res = res ^ i ^ nums[i];
        }
        return res;
    }
    int missingNumber_sum_reconstruct(vector<int>& nums) {
        long long sum = std::accumulate(nums.begin(), nums.end(), 0L), sz = nums.size();
        return sz * (sz + 1) / 2 - sum;
    }
    int missingNumber_binary_search(vector<int>& nums) { // TODO fix this binary search. -- seem [] better than [)?
        int sz = nums.size();
        int low = 0, high = nums.size() + 1;
        while (low < high) {
            auto mid = low + (high - low) / 2;
            int c1 = 0, c2 = 0;
            for (auto x: nums) {
                if (x < low || x >= high) continue;
                c1 += (x < mid);
                c2 += (x > mid);
            }
            // cout << "sz = " << sz << ", c1 = " << c1 << ", c2 = " << c2 << endl;
            // cout << "low = " << low << ", high = " << high << ", mid = " << mid << endl;
            if (c1 + c2 == sz) return mid;
            if (c1 <= c2) {
                high = mid;
                sz = c1;
            }
            else {
                low = mid + 1;
                sz = c2;
            }
        }
        return low;
    }
    int missingNumber_isomorphism(vector<int>& nums) {
        int sz = nums.size();
        for (int i = 0; i < sz; ++i) {
            for (; nums[i] < sz && nums[i] !=i;) { // no duplicate, thus no need to to check further whether v is in place.
                std::swap(nums[i], nums[nums[i]]);
            }
        }
        int i = 0;
        for (; i < sz && nums[i] == i; ++i) {}
        return i;
    }
    int missingNumber(vector<int>& nums) {
        return missingNumber_isomorphism(nums);
        return missingNumber_binary_search(nums);
        return missingNumber_xor(nums);
    }

namespace test {
    void missingNumber() {
        std::vector<std::vector<int>> vec;
        vec.push_back({0});
        vec.push_back({1});
        vec.push_back({0,2});
        for (auto v: vec) {
            auto res = ::missingNumber(v);
            print(v,"v",-1);
            cout << "\t missingNumber(v) == " << res << endl;
        }
    }
}

// 41. First Missing Positive https://leetcode.com/problems/first-missing-positive/?tab=Description
// Given an unsorted integer array, find the first missing positive integer. 
// For example, Given [1,2,0] return 3, and [3,4,-1,1] return 2.
// TODO O(n) solution.
// first uniqfy, then quick select? but we unique-fy in place in O(n) time?
// -- use hash map?
// O(n) time O(n) extra space solution. 1. pigeon hole k <= n. 2. use bool array to encode visited-ness.  http://stackoverflow.com/a/1586940/3625404
// or similar to quick select?
// or similar to make heap?
// or swap solution similar to missing number?
    int firstMissingPositive_copy_and_sort(vector<int>& nums) {
        std::vector<int> positive;
        std::copy_if(nums.begin(), nums.end(), std::back_inserter(positive), [](int x) { return x > 0; });
        std::sort(positive.begin(), positive.end());
        if (positive.empty() || positive[0] > 1) return 1;
        auto prev = positive.begin(), it = prev;
        for (++it; it != positive.end() && *it - *prev <= 1; prev = it, ++it) {}
        return *prev + 1;
    }
    int firstMissingPositive_copy_and_sort_but_exclude_one_first(vector<int>& nums) {
        int min_positive = INT_MAX;
        for (auto x: nums) {
            if (x > 0 && x < min_positive) { min_positive = x; }
        }
        if (min_positive > 1) return 1;
        std::vector<int> positive;
        std::copy_if(nums.begin(), nums.end(), std::back_inserter(positive), [](int x) { return x > 1; });
        std::sort(positive.begin(), positive.end());
        if (positive.empty() || positive[0] > 2) return 2;
        auto prev = positive.begin(), it = prev;
        for (++it; it != positive.end() && *it - *prev <= 1; prev = it, ++it) {}  // TODO could be refactored to std::adjacent_find??
        return *prev + 1;
    }
    int firstMissingPositive_partition_then_sort(vector<int>& nums) {
        if (nums.empty()) return 1;
        auto first = nums.begin(), last = nums.end();
        for (; first < last;) { // TODO merge to partition library.
            for (; first < last && *first > 0; ++first) {}
            for (; first < last && *--last <= 0;) {}
            if (first < last) {
                std::iter_swap(first, last);
                ++first;
            }
        }
        // print(nums); cout << "*first = " << *first <<  ", index = " << first - nums.begin() << endl;
        auto min_positive = *std::min_element(nums.begin(), last);
        if (min_positive > 1) return 1;
        std::sort(nums.begin(), last);
        // print(nums, "after sort");
        first = nums.begin();
        int min = 0;
        for (; first != last && *first - min < 2; min = *first, ++first) {}
        return min + 1;
    }
    int firstMissingPositive_binary_search(vector<int>& nums) {
        int low = INT_MAX, high = 1;
        int cnt = 0;
        for (auto x: nums) {
            if (x > 0) {
                ++cnt;
                low = std::min(low, x);
                high = std::max(high, x);
            }
        }
        if (low > 1) return 1;
        auto mid = low + (high - low) / 2;
        auto first = nums.begin(), last = nums.end();
        for (; first < last;) { // not work yet, work if unique, we could unique-fy with n extra space though. pigeon hole + trivial hash. http://stackoverflow.com/a/1586940/3625404
            for (; first < last && *first > mid; ++first) {}
            for (; first < last && *--last <= mid;) {}
            if (first < last) {
                std::iter_swap(first, last);
                ++first;
            }
        }
        return 0;
    }
    int firstMissingPositive_isomorphism(vector<int>& nums) {
        int sz = nums.size();
        for (int i = 0; i < sz; ++i){
            for (;;) { // TODO combine to one loop since they share similar invariants. eg, https://discuss.leetcode.com/topic/10351/o-1-space-java-solution
                auto v = nums[i] - 1;
                if (v < 0 || v >= sz || v == i || nums[v] - 1 == v) break;
                std::swap(nums[i], nums[v] );
            }
        }
        int i = 0;
        for (; i < sz && nums[i]- 1 == i; ++i){
        }
        return 1 + i;
    }
    int firstMissingPositive_isomorphism_but_mark_visited(vector<int>& nums) {
        // TODO mark A[i] to -A[i] to indicate value i+1 exist and could still retain A[i] in case needed is really smart. https://discuss.leetcode.com/topic/2633/share-my-o-n-time-o-1-space-solution
        return 0;
    }
    int firstMissingPositive(vector<int>& nums) {
        return firstMissingPositive_isomorphism(nums);
        return firstMissingPositive_partition_then_sort(nums);
        return firstMissingPositive_copy_and_sort(nums);
        return firstMissingPositive_copy_and_sort_but_exclude_one_first(nums);
    }

namespace test {
    void firstMissingPositive() {
        std::vector<std::vector<int>> vec;
        vec.push_back({0});
        vec.push_back({2,1});
        vec.push_back({1,2,0});
        vec.push_back({3,4,-1,1});
        vec.push_back({1,2,2,1,3,1,0,4,0});
        for (auto v: vec) {
            auto res = ::firstMissingPositive(v);
            print(v,"v",-1);
            cout << "\t firstMissingPositive(v) == " << res << endl;
        }
    }
}

// 442. Find All Duplicates in an Array https://leetcode.com/problems/find-all-duplicates-in-an-array/?tab=Description
// Given an array of integers, 1 ≤ a[i] ≤ n (n = size of array), some elements appear twice and others appear once. Find all the elements that appear twice in this array.
    vector<int> findDuplicates_inplace_hash_self_encode(vector<int>& nums) {
        vector<int> res;
        int sz = nums.size();
        for (int i = 0; i < sz; ++i) {
            int v = -1 + abs(nums[i]);
            if (nums[v] > 0) { nums[v] *= -1; }
            else { res.push_back(v + 1); }
        }
        return res;
    }
    vector<int> findDuplicates(vector<int>& nums) {
        return findDuplicates_inplace_hash_self_encode(nums);
    }

// 448. Find All Numbers Disappeared in an Array https://leetcode.com/problems/find-all-numbers-disappeared-in-an-array/?tab=Description
// Given an array of integers where 1 ≤ a[i] ≤ n (n = size of array), some elements appear twice and others appear once.
// Find all the elements of [1, n] inclusive that do not appear in this array.
// Could you do it without extra space and in O(n) runtime? You may assume the returned list does not count as extra space.
// Example: Input: [4,3,2,7,8,2,3,1] Output: [5,6]
// perfect linear inplace hash + self encoding mark visited.
    vector<int> findDisappearedNumbers_inplace_hash_self_encode(vector<int>& nums) {
        vector<int> res;
        int sz = nums.size();
        for (int i = 0; i < sz; ++i) {
            int v = -1 + abs(nums[i]);
            if (nums[v] > 0) { nums[v] *= -1; }
        }
        for (int i = 0; i < sz; ++i) {
            if (nums[i] > 0) res.push_back(i+1);
        }
        return res;
    }
    vector<int> findDisappearedNumbers_isomorphism(vector<int>& nums) {
        vector<int> res;
        int sz = nums.size();
        for (int i = 0; i < sz;) {
            if (nums[i] - 1 == i || nums[i] == nums[nums[i] - 1]) ++i;
            else { std::swap(nums[i], nums[nums[i] - 1]); }
        }
        for (int i = 0; i < sz; ++i) {
            if (nums[i] - 1 != i) res.push_back(i+1);
        }
        return res;
    }
    vector<int> findDisappearedNumbers(vector<int>& nums) {
        return findDisappearedNumbers_isomorphism(nums);
        return findDisappearedNumbers_inplace_hash_self_encode(nums);
    }

namespace test {
    void findDisappearedNumbers() {
        std::vector<std::vector<int>> vec;
        vec.push_back({4,3,2,7,8,2,3,1});
        for (auto v: vec) {
            auto res = ::findDisappearedNumbers(v);
            print(v,"v",-1);
            print(res,"\t findDisappearedNumbers(v) == ");
        }
    }
}

// 6. ZigZag Conversion https://leetcode.com/problems/zigzag-conversion/?tab=Description
// zig then zag conversion. "PAYPALISHIRING" -->  "PAHNAPLSIIGYIR". 3 rows.
// P   A   H   N
// A P L S I I G
// Y   I   R
    string convert(string s, int numRows) { // cycle 2*n - 2
        if (numRows < 2) return s;
        string res;
        int sz = s.size(), cycle = 2*(numRows - 1);
        // cout << "cycle = " << cycle << endl;
        for (int i = 0; i < numRows; ++i) {
            for (int start = i; start < sz; start += cycle) {
                res += s[start];
                // cout << res.back() << ' ';
                if (i != 0 && i != numRows - 1) {
                    if (start - i + cycle - i < sz)
                        res += s[start - i + cycle - i];
                // cout << res.back() << ' ';
                }
            }
            // cout << endl;
        }
        return res;
    }
namespace test {
    void convert() {
        string s = "PAYPALISHIRING";
        int n = 3;
        auto res = ::convert(s, n);
        cout << "s = [" << s << "], n = " << n << ", ::convert(s,n) = " << res << endl;
    }
}

// // 142. Linked List Cycle II  https://leetcode.com/problems/linked-list-cycle-ii/    // Given a linked list, return the node where the cycle begins. If there is no cycle, return null. 
// // TODO merge algo description to List.cpp
// // if has cycle, then then n-th link to one of [1,n], let the length of cycle be C,
// // remaining R = N - C. then when slow meets fast, slow move k step in cycle, fast move R
// // + 2*k step in cycle, thus (R + k) % C == 0, or k = C - (R % C). if let head move R
// // step, head will arrive link point, and fast too, --, (k + R) % C == (C - (R % C) + R) % C == 0.
//     ListNode *detectCycle(ListNode *head) {
//         ListNode *slow = head, *fast = head;
//         while (fast && fast->next) {
//             slow = slow->next;
//             fast = fast->next->next;
//             if (fast == slow) break;
//         }
//         if (fast && fast->next) {
//             while (head != fast) {
//                 head = head->next;
//                 fast = fast->next;
//             }
//             return head;
//         } else return nullptr;
//     }

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
    test::convert();
}

