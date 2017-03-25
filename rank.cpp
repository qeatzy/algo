#include "utility.h"
// sort/rank related. eg, count inversion.
// tag:  Binary Indexed Tree  https://leetcode.com/tag/binary-indexed-tree/

// 506. Relative Ranks https://leetcode.com/problems/relative-ranks/#/description
// Given scores of N athletes, find their relative ranks and the people with the top three highest scores, who will be awarded medals: "Gold Medal", "Silver Medal" and "Bronze Medal".
// Example 1: Input: [5, 4, 3, 2, 1] Output: ["Gold Medal", "Silver Medal", "Bronze Medal", "4", "5"]
// Explanation: The first three athletes got the top three highest scores, so they got "Gold Medal", "Silver Medal" and "Bronze Medal".
// For the left two athletes, you just need to output their relative ranks according to their scores.
// Note: N is a positive integer and won't exceed 10,000. All the scores of athletes are guaranteed to be unique.
// -- one array is enough??
    vector<string> findRelativeRanks(vector<int>& nums) {
        auto index = range(nums.size()), rank = index;
        std::sort(index.begin(), index.end(), [&nums](int i, int j) { return nums[i] > nums[j]; });
        std::sort(rank.begin(), rank.end(), [&index](int i, int j) { return index[i] < index[j]; });
        // print(index,"index");
        // print(rank,"rank");
        vector<string> res;
        for (auto i: rank) {
            if (i > 2) res.push_back(std::to_string(i+1));
            else if (i == 0) res.push_back("Gold Medal");
            else if (i == 1) res.push_back("Silver Medal");
            else if (i == 2) res.push_back("Bronze Medal");
        }
        return res;
    }

namespace test {
    void findRelativeRanks() {
        std::vector<std::vector<int>> vec;
        // vec.push_back({5, 4, 3, 2, 1});
        vec.push_back({10,3,8,9,4});
        for (auto &v: vec) {
            auto res = ::findRelativeRanks(v);
            print(v,"nums");
            print(res,"relative rank");
        }
    }
}

// Reverse Pairs https://www.lintcode.com/en/problem/reverse-pairs/
// For an array A, if i < j, and A [i] > A [j], called (A [i], A [j]) is a reverse pair. return total of reverse pairs in A.
// Given A = [2, 4, 1, 3, 5], (2, 1), (4, 1), (4, 3) are reverse pairs. return 3
// TODO 
// -- does you solution handle duplicate case?
    long long reversePairs_lintcode(vector<int>& A) {
        long long res = 0;
        auto rank = Rank(A);
        // print(rank, "rank");
        vector<int> cnt(A.size(), 0);
        for (auto it = A.rbegin(); it != A.rend(); ++it) {
            // auto p = 1 + rank[*it];
        }
        return res;
    }

namespace test {
    void reversePairs_lintcode() {
        std::vector<std::vector<int>> vec;
        vec.push_back({2, 4, 1, 3, 5});
        for (auto &v: vec) {
            auto res = ::reversePairs_lintcode(v);
            cout << res << " reversePairs in total" << endl;
        }
    }
}

// 493. Reverse Pairs https://leetcode.com/problems/reverse-pairs/?tab=Description
// Given an array nums, we call (i, j) an important reverse pair if i < j and nums[i] > 2*nums[j].
// You need to return the number of important reverse pairs in the given array.
// Example1: Input: [1,3,2,3,1] Output: 2 Example2: Input: [2,4,3,5,1] Output: 3
// Note: The length of the given array will not exceed 50,000.
// All the numbers in the input array are in the range of 32-bit integer.
// TODO 3 solution. http://blog.csdn.net/gqk289/article/details/57083606
    int reversePairs(vector<int>& nums) {
        return 0;
    }

namespace test {
    void reversePairs() {
        std::vector<std::vector<int>> vec;
        vec.push_back({2, 4, 1, 3, 5});
        for (auto &v: vec) {
            auto res = ::reversePairs(v);
            cout << res << " reversePairs in total" << endl;
        }
    }
}

// 315. Count of Smaller Numbers After Self https://leetcode.com/problems/count-of-smaller-numbers-after-self/?tab=Description
//  You are given an integer array nums and you have to return a new counts array.
//  The counts array has the property where counts[i] is the number of smaller elements to the right of nums[i].
// Example:
// Given nums = [5, 2, 6, 1]
// To the right of 5 there are 2 smaller elements (2 and 1).
// To the right of 2 there is only 1 smaller element (1).
// To the right of 6 there is 1 smaller element (1).
// To the right of 1 there is 0 smaller element.
// Return the array [2, 1, 1, 0]. 
    vector<int> countSmaller(vector<int>& nums) {
        return {0,0};
    }

// 57. Insert Interval https://leetcode.com/problems/insert-interval/?tab=Description
// Given a set of non-overlapping intervals, insert a new interval into the intervals (merge if necessary).
// You may assume that the intervals were initially sorted according to their start times.
// Example 1: Given intervals [1,3],[6,9], insert and merge [2,5] in as [1,5],[6,9].
// Example 2: Given [1,2],[3,5],[6,7],[8,10],[12,16], insert and merge [4,9] in as [1,2],[3,10],[12,16].
// This is because the new interval [4,9] overlaps with [3,5],[6,7],[8,10].
/**
 * Definition for an interval.
 * struct Interval {
 *     int start;
 *     int end;
 *     Interval() : start(0), end(0) {}
 *     Interval(int s, int e) : start(s), end(e) {}
 * };
 */
    // vector<Interval> insert(vector<Interval>& intervals, Interval newInterval) {
    //     
    // }

int main() {
    test::init();
    // test::find132pattern();
    // test::largestRectangleArea();
    // test::reversePairs_lintcode();
    // test::reversePairs();
    test::findRelativeRanks();
}
