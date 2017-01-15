#include "utility.h"

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

int main() {
    // test::binarySearch();
    test::first_index_equal_to_value_in_an_increasing_sequence();
}
