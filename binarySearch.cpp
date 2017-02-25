#include "utility.h"
// binary search related. eg, search in rotated sorted array.
// note binary search for first truth value. -- if you need to find last, it's just reversely first.
// logarithm complexity of binary search like algorithm comes from at each step we reduce
// the range to half, what if we reduce both part by half, will it still be logarithm? A such case with analysis here, https://discuss.leetcode.com/topic/16486/9-11-lines-o-log-n

// http://en.cppreference.com/w/cpp/algorithm/lower_bound
// template<typename Iterator, typename T>
// Iterator* lower_bound(Iterator* low, Iterator* high, T val) {
//     if (low < high) {
//     // cout << "low = " << *low << ", high = " << high[-1] << ", val = " << val << endl;
//     // wait();
//         auto mid = low + (high - low) / 2;
//         // find first such that *pos < val is false.
//         if (*mid < val) {
//             return lower_bound(mid + 1, high, val);
//         } else {
//             return lower_bound(low, mid, val);
//         }
//     } else {
//         return low;
//     }
// }
// template<typename Iterator, typename T>
// Iterator* lower_bound(Iterator* low, Iterator* high, T val) {
//     while (low < high) {
//         auto mid = low + (high - low) / 2;
//         if (*mid < val) {
//             low = mid + 1;
//         } else {
//             high = mid;
//         }
//     }
//     return low;
// }
template <typename RandomIt, typename T, typename Compare = std::less<typename std::iterator_traits<RandomIt>::value_type>> // http://stackoverflow.com/a/2447556/3625404
RandomIt* lower_bound(RandomIt* low, RandomIt* high, T val, Compare comp = Compare()) {
    while (low < high) {
        auto mid = low + (high - low) / 2;
        // if (*mid < val) {
        if (comp(*mid, val)) {
            low = mid + 1;
        } else {
            high = mid;
        }
    }
    return low;
}
// http://en.cppreference.com/w/cpp/algorithm/binary_search
// consider find **first** true value, after apply appropriate comparator.
// -- that is, first half are all false value, return start point of second half.
// if mid is false, then [low, mid + 1) are all false, search for [mid + 1, high).
// if mid is true, then [mid, high) are all true, search for [low, mid).
// Does the range always been reduced? -- confirm yourself that the answer is 'yes'.
// -- since low <= mid, mid < high, always (due to floor behavior of integer division).
// -- (low <= mid) < high, low < (mid + 1 <= high)
// -- one final note, last is first in reverse order.
int binarySearch(const std::vector<int> seq, int val) {
    auto it = lower_bound(seq.begin(), seq.end(), val);
    auto res = it - seq.begin();
    // may fail if it - seq.begin() > INT_MAX.
    return (it != seq.end() && *it == val)? res : -1 - res;
    // return (*it == val)? it - seq.begin() : -1;
    // return std::lower_bound(&seq[0], &seq[0] + seq.size(), val) - &seq[0];
}

int binarySearch2(const std::vector<int> seq, int val) {
    auto it = std::lower_bound(seq.begin(), seq.end(), val);
    auto res = it - seq.begin();
    // may fail if it - seq.begin() > INT_MAX.
    return (it != seq.end() && *it == val)? res : -1 - res;
}


namespace test {
    void binarySearch() {
        vector<int> seq {2, 4, 6, 8, 10, 12, 14};
        print(seq);
        for (auto x: range(0,seq.back() + 2)) {
            cout << "value being queried: " << x << ", index " << ::binarySearch(seq, x) << endl;
            // cout << "value being queried: " << x << ", index " << ::binarySearch2(seq, x) << endl;
        }
        cout << endl;
    }
}

// 2.15 Give an efficient algorithm to determine if there exists an integer i such that Ai = i in an array of integers A1 < A2 < A3 < · · · < AN. What is the running time of your algorithm?
int first_index_equal_to_value_in_an_increasing_sequence(const std::vector<int>& seq) {
    // std::remove_reference_t<decltype(seq)> vec;
    auto vec = decltype(seq) {};
    for (int i = 0; (size_t) i < seq.size(); ++i) {
        vec.push_back(seq[i] - i);
    }
    // print(vec);
    auto res = binarySearch(vec, 0);
    return res;
}
int first_index_equal_to_value_in_an_increasing_sequence2(const std::vector<int>& seq) {
    int first = 0, last = seq.size();
    while (first != last) {
        auto mid = first + (last - first) / 2;
        if (seq[mid] - mid >= 0) {
            last = mid;
        } else {
            first = mid + 1;
        }
    }
    return ((size_t)last != seq.size() && seq[last] == last)? last : -1 - last;
}

namespace test {
    void first_index_equal_to_value_in_an_increasing_sequence() {
        auto test_case = []() {
            std::vector<std::vector<int>> res;
            res.push_back(range(7));
            res.push_back({-3, -1, 1, 3, 4, 6, 7});
            res.push_back({4,5,6});
            res.push_back({-4,-5,-6});
            res.push_back({});
            return res;
        };
        for (auto v: test_case()) {
            cout << ::first_index_equal_to_value_in_an_increasing_sequence(v) << endl;
            cout << ::first_index_equal_to_value_in_an_increasing_sequence2(v) << endl;
        }
    }
}

// 33. Search in Rotated Sorted Array https://leetcode.com/problems/search-in-rotated-sorted-array/?tab=Description
// Suppose an array sorted in ascending order is rotated at some pivot unknown to you beforehand. (i.e., 0 1 2 4 5 6 7 might become 4 5 6 7 0 1 2).
// You are given a target value to search. If found in the array return its index, otherwise return -1.
// You may assume no duplicate exists in the array.
// -- this problem is one the them that two pass is better than one pass, both in clarity and efficiency.
// C++ find turning point of rotated sorted array
// 4 5 6 7 0 1 2       6 7 8 0 1 2 3 4 5
// 3 1  |  2 3 1 | 3 1 2
template <typename RandomIt>
RandomIt findPivot(RandomIt first, RandomIt last) { // return position of smallest in a possibly rotated array.
    if (first == last || *first < *--last || first == last) return first;
    // guarantee *first > *(last - 1)
    // 1,2,1,1,1,1, 1,1,1,1,1
    if (*first == *last) {
        auto mid = first + (last - first) / 2;
        if (*first == *mid) {
            auto left = first + (mid - first) / 2;
            if (*first == *left) {
                mid = mid + (last - mid) / 2;  // reuse mid for last.
                if (*first == *mid) {
                    mid = first;
                    while (++mid < --last && *mid == *last) {
                        if (*mid != 1) {
                        }
                    }
                    if (last < mid) return first; // all equal
                    if (*first == *mid) mid = last;
                    // 1 1 2 ... 1 1 1
                    // 1 1 1 ... 0 1 1  -- this case need equal test in below test. -- // if (*first <= *mid) first = mid;
                    // 1 1 2 ... 0 1 1
                }
            } else mid = left;
        }
        if (*first <= *mid) first = mid;
        else last = mid;
    }
    while (*first > *last) {
        // 3 3 3 3 0 1 2  || 3 3 0 2 2 2 2  || 2 1
        auto mid = first + (last - first) / 2;
        if (*first <= *mid) first = ++mid;
        else last = mid;
    }
    return first;
}
template <typename RandomIt, typename T = typename std::iterator_traits<RandomIt>::value_type>
RandomIt search_rotated_sorted(RandomIt first, RandomIt last, T target) {
    if (first == last || *first == target) return first;
    auto mid = findPivot(first, last);
    // 3 4 5 0 1 2  target = 4 or 1
    if (first != mid) { // rotated
        if (*first < target) last = mid;
        else first = mid;
    }
    return std::lower_bound(first, last, target);
}
    int search(vector<int>& nums, int target) {
        auto first = nums.begin(), last = nums.end(), p = search_rotated_sorted(first, last, target);
        return (p == last || *p != target) ? -1 : p - first;
    }
// 81. Search in Rotated Sorted Array II https://leetcode.com/problems/search-in-rotated-sorted-array-ii/?tab=Description
// Follow up for "Search in Rotated Sorted Array": What if duplicates are allowed? Would this affect the run-time complexity? How and why?
// Suppose an array sorted in ascending order is rotated at some pivot unknown to you beforehand. (i.e., 0 1 2 4 5 6 7 might become 4 5 6 7 0 1 2). Write a function to determine if a given target is in the array. The array may contain duplicates.
// reduce to no duplicates case if first trim leading and trailing duplicates, or trim one side is enough?
// TODO write one pass solution, for both I and II.
// binarySearch is always log(n) time complexity, duplicates or not, however for rotated sorted array, find pivot may degrade to O(n) if duplicates was allowed.
// a concise solution, posted by 1337c0d3r. https://discuss.leetcode.com/topic/310/when-there-are-duplicates-the-worst-case-is-o-n-could-we-do-better/3#
    // bool search(vector<int>& nums, int target) {
    //     
    // }
namespace test {
    void search_rotated() {
        std::vector<std::vector<int>> vec;
        vec.push_back({4,5,6,7,0,1,2,  6});
        vec.push_back({1, 0});
        vec.push_back({1, 1});
        vec.push_back({1,3, 2});
        vec.push_back({1,3, 3});
        vec.push_back({3,1, 0});
        vec.push_back({3,3,4,5,6,1,2,2,3,3,  2});
        vec.push_back({1,1,1,1,1,0,0,0,1,1,1,1,1,1,  0});
        vec.push_back({1,1,3,1,1,1,1,1,1,1,1,1,  3});
        // vec.push_back({1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 2});
        for (auto &v: vec) {
            auto target = v.back();
            v.pop_back();
            auto res = ::search(v, target);
            print(v,"v",-1);
            cout << "\t target = " << target << ", index = " << res << endl;
            if (v.size() > 100) {
                cout << "v.size() = " << v.size() << ", ";
                auto p = std::find(v.begin(), v.end(), 2) - v.begin();
                cout << "p = " << p;
            }
        }
    }
}

// 34. Search for a Range https://leetcode.com/problems/search-for-a-range/?tab=Description
// Given an array of integers sorted in ascending order, find the starting and ending position of a given target value.
// Your algorithm's runtime complexity must be in the order of O(log n).
// If the target is not found in the array, return [-1, -1].
// For example, Given [5, 7, 7, 8, 8, 10] and target value 8, return [3, 4].
// TODO write and analysis the complexity of divide and conquer solution, similar to this  https://discuss.leetcode.com/topic/16486/9-11-lines-o-log-n
    vector<int> searchRange(vector<int>& nums, int target) {
        auto bounds = std::equal_range(nums.begin(), nums.end(), target);
        if (bounds.first == bounds.second)
            return {-1,-1};
        return {bounds.first - nums.begin(), bounds.second - nums.begin() - 1};
        auto p = std::lower_bound(nums.begin(), nums.end(), target);
        if (p != nums.end() && *p == target) {
            int right = nums.rend() - std::lower_bound(nums.rbegin(), nums.rend(), target, std::greater<int>()) - 1;
            return {p - nums.begin(), right};
        } else return {-1,-1};
    }

namespace test {
    void searchRange() {
        std::vector<std::vector<int>> vec;
        vec.push_back({5, 7, 7, 8, 8, 10, 8});
        vec.push_back({1,0});
        for (auto &v: vec) {
            assert(!v.empty());
            auto target = v.back(); v.pop_back();
            auto res = ::searchRange(v, target);
            print(res,"interval of exist");
        }
    }
}

int main() {
    // test::binarySearch();
    // test::first_index_equal_to_value_in_an_increasing_sequence();
    // test::search_rotated();
    test::searchRange();
}
