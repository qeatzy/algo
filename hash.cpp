#include "utility.h"
// hash and/or set/map related. use hash instead of set/map, one for easy type, one for non-conflict with standard library.

// 349. Intersection of Two Arrays https://leetcode.com/problems/intersection-of-two-arrays/#/description
// Given two arrays, write a function to compute their intersection.
// Example: Given nums1 = [1, 2, 2, 1], nums2 = [2, 2], return [2].
// Note: Each element in the result must be unique. The result can be in any order.
// TODO solution with sort. "tp(two pointer)  https://discuss.leetcode.com/topic/45616/my-c-solution-with-sort
    vector<int> intersection_optimal(vector<int>& nums1, vector<int>& nums2) { // https://discuss.leetcode.com/topic/45846/small-c-solution
        vector<int> res;
        auto m = makeSet(nums1);
        for (auto x: nums2) {
            if (m.erase(x)) res.push_back(x);
        }
        return res;
    }
    vector<int> intersection(vector<int>& nums1, vector<int>& nums2) {
        vector<int> res;
        auto m1 = makeSet(nums1), m2 = makeSet(nums2);
        for (auto x: m2) {
            if (m1.find(x) != m1.end()) res.push_back(x);
        }
        return res;
    }

namespace test {
    void intersection() {
        std::vector<std::vector<int>> vec;
        vec.push_back({1, 2, 2, 1});
        vec.push_back({2, 2});
        vec.push_back({1});
        vec.push_back({1,1});
        while (vec.size() >= 2) {
            auto nums2 = vec.back(); vec.pop_back();
            auto nums1 = vec.back(); vec.pop_back();
            auto res = ::intersection(nums1, nums2);
            print(nums1,"nums1");
            print(nums2,"nums2");
            print(res,"intersection");
        }
    }
}

int main() {
    test::init();
    test::intersection();
}
