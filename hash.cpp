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

// 350. Intersection of Two Arrays II https://leetcode.com/problems/intersection-of-two-arrays-ii/#/description
// Given two arrays, write a function to compute their intersection.
// Example: Given nums1 = [1, 2, 2, 1], nums2 = [2, 2], return [2, 2].
// Note: Each element in the result should appear as many times as it shows in both arrays.
// The result can be in any order.
// Follow up:
//     What if the given array is already sorted? How would you optimize your algorithm?
//     What if nums1's size is small compared to nums2's size? Which algorithm is better?
//     What if elements of nums2 are stored on disk, and the memory is limited such that you cannot load all elements into the memory at once?
// TODO  The intersection of two sorted arrays  http://stackoverflow.com/a/4601106/3625404
// http://articles.leetcode.com/here-is-phone-screening-question-from/
    vector<int> intersect(vector<int>& nums1, vector<int>& nums2) {
        vector<int> res;
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
