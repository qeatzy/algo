#include "utility.h"
// "tp(two pointer), 1. crossing, eg, Hoare partition. 2. sliding window. 3. slow-fast, eg, floyd cycle detection.
// be careful for sliding window, ask yourself 'left < right' or 'left <= right' is needed.
// 1. Finding intersection of two sorted arrays.  http://articles.leetcode.com/here-is-phone-screening-question-from/


// 532. K-diff Pairs in an Array https://leetcode.com/problems/k-diff-pairs-in-an-array/?tab=Description
// Given an array of integers and an integer k, you need to find the number of unique k-diff pairs in the array.
// Here a k-diff pair is defined as an integer pair (i, j), where i and j are both numbers in the array and their absolute difference is k.
// Input: [3, 1, 4, 1, 5], k = 2 Output: 2 Explanation: There are two 2-diff pairs in the array, (1, 3) and (3, 5).
// Although we have two 1s in the input, we should only return the number of unique pairs.
    int findPairs_sort_binary_search(vector<int>& nums, int k) {
        if (k < 0 || nums.size() < 2) return 0;
        int cnt = 0;
        auto first = nums.begin(), last = nums.end();
        std::sort(first, last);
        if (k == 0) {
            for (; first != last;) {
                auto prev = first, curr = first;
                for (; ++curr != last && *curr == *prev; prev = curr) {}
                if (prev != first) ++cnt;
                first = curr;
            }
            return cnt;
        }
        last = std::unique(first, last);
        for (; first < last; ++first) {
            auto p = std::lower_bound(first, last, *first + k);
            if (p == last) break;
            if (*p == *first +k) { ++cnt; }
        }
        return cnt;
    }
    int findPairs_two_pointer_after_sort(vector<int>& nums, int k) { // sliding window
        if (k < 0 || nums.size() < 2) return 0;
        int cnt = 0;
        auto first = nums.begin(), last = nums.end();
        std::sort(first, last);
        for (auto mid = first + 1; mid < last;) { // loop invariant, first < mid
            for (; mid != last && *mid - *first < k; ++mid) {} // bug, *first + k may overflow.
            if (mid == last) break;
            if (*mid == *first + k) {
                ++cnt;
                // print(nums.begin(), mid, "util mid");
                // print(nums.begin(), first, "util first");
            }
            for (++first; first != last && first[0] == first[-1]; ++first) {}
            if (mid <= first) mid = first + 1;
        }
        return cnt;
    }
    int findPairs_hash(vector<int>& nums, int k) {
        if (k < 0 || nums.size() < 2) return 0;
        int cnt = 0;
        auto counter = uCounter(nums);
        // print(counter);
        for (auto t: counter) {
            auto x = t.first, y = x + k;
            if (k == 0) {
                if (t.second > 1) ++cnt;
            } else {
                if (counter.find(y) != counter.end()) ++cnt;
            }
        }
        return cnt;
    }
    int findPairs(vector<int>& nums, int k) {
        // return findPairs_hash(nums,k);
        return findPairs_two_pointer_after_sort(nums,k);
        return findPairs_sort_binary_search(nums,k);
    }

namespace test {
    void findPairs() {
        std::vector<std::vector<int>> vec;
        vec.push_back({1,3,1,5,4, 0});
        vec.push_back({3,1,4,1,5, 2});
        vec.push_back({3,3,1,4,1,5, 2});
        vec.push_back({0,0,0,  0});
        vec.push_back({6,7,3,6,4,6,3,5,6,9,   4});
        for (auto &v: vec) {
            int k = v.back(); v.pop_back();
            auto res = ::findPairs(v, k);
            print(v,"v");
            cout << "k = " << k << ", res = " << res << endl;
        }
    }
}

int main() {
    test::init();
    test::findPairs();
    // test::isValidSudoku();
}
