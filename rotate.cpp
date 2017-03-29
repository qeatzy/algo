#include "utility.h"
#include <list>
// rotate, or swap contiguous sub array.
// 1. reverse, reverse each part, then reverse whole array. -- similar case, reverse words in a string.
// 2. gcd-like.
// 3. juggling rotate, shell sort like. -- worse cache performance.
// at worst n-1 swap operation needed, not n, since if n-1 items are in place, the remaining is in place too.
// at best n/2 swap operation needed, eg, mid - first == last - midlle. (omitting trivial case).
// search for __rotate in file /usr/lib/gcc/i686-pc-cygwin/5.4.0/include/c++/bits/stl_algo.h
// a sensible return value is first + (last - mid). since c++11.
// http://en.cppreference.com/w/cpp/algorithm/rotate
// http://stackoverflow.com/questions/4457277/algorithm-to-rotate-an-array-in-linear-time
// http://stackoverflow.com/questions/876293/fastest-algorithm-for-circle-shift-n-sized-array-for-m-position/42714626#42714626
// http://stackoverflow.com/questions/31174840/how-to-rotate-an-array/42714433#42714433

// same logic with STL implementation, but simpler, since no return value needed.
template <typename Iterator>
Iterator rotate_by_gcd_like_swap(Iterator first, Iterator mid, Iterator last) {
    auto result = first + (last - mid);
    if (first == mid) return last;  // ensure left half non-empty.
    Iterator old = mid;
    for (; mid != last;) {
        std::iter_swap(first, mid);
        ++first, ++mid;
        if (first == old) old = mid;        // left half exhausted, reset
        else if (mid == last) mid = old;    // right half exhausted, reset
    }
    return result;
}

// same logic with STL implementation
template <typename Iterator>
Iterator rotate_by_gcd_like_swap_then_return_new_mid(Iterator first, Iterator mid, Iterator last) {
    if (first == mid) return last;
    if (mid == last) return first;
    Iterator old = mid;
    for(;;) {
        std::iter_swap(first, mid);
        ++first, ++mid;
        if (first == old) old = mid;
        if (mid == last) break;
    }
    Iterator result = first; // when first time `mid == last`, the position of `first` is the new `mid`.
    for (mid = old; mid != last;) {
    // for (mid = old; first != old;) {
        std::iter_swap(first, mid);
        ++first, ++mid;
        if (first == old) old = mid;    // bug, why error if swap order of this 'if-else' clause?? -- else test be same as loop condition. why?
        else if (mid == last) mid = old;
        // if (mid == last) mid = old;
        // else if (first == old) old = mid;    // bug, why error if swap order of this 'if-else' clause?? -- else test be same as loop condition. why?
    }
    // cout << "result - f_old = " << result - f_old << ", first - f_old = " << first - f_old << ", old - f_old = " << old - f_old << ", last - f_old = " << last - f_old << endl;
    return result;
}

template <typename Iterator, typename T = typename std::iterator_traits<Iterator>::value_type>
Iterator rotate_by_juggling(Iterator first, Iterator mid, Iterator last) {
}

template <typename Iterator, typename T = typename std::iterator_traits<Iterator>::value_type>
Iterator rotate_by_reverse(Iterator first, Iterator mid, Iterator last) {
    // if (first == mid || mid == last) return;
    if (first != mid && mid != last) {
        std::reverse(first, mid);
        std::reverse(mid, last);
        std::reverse(first, last);
    }
    return first + (last - mid);
}

namespace test {
    void rotate() {
        std::vector<std::vector<int>> vec;
        vec.push_back({3,4,5,6,7,8,9,0,1,2, 3});
        vec.push_back({3,4,5,6,7,8,9,0,1,2, 7});
        for (auto &v: vec) {
            assert(!v.empty() && v.back() < int(v.size()) && v.back() >= 0);
            auto k = v.back(); v.pop_back();
            auto first = v.begin(), mid =  v.begin() + k, last = v.end();
            print(first, mid," pre rotate",-1); cout << "| ";
            print(mid, last, "",-1); cout << "\t sz = " << last - first << ", k = " << k << endl;
            mid = ::rotate_by_gcd_like_swap(v.begin(), v.begin() + k, v.end());
            // mid = ::rotate_by_gcd_like_swap_then_return_new_mid(first, mid, last);
            // mid = std::rotate(v.begin(), v.begin() + k, v.end());
            print(first, mid,"post rotate",-1); cout << "| ";
            print(mid, last, "",-1); cout << "\t sz = " << last - first << ", k = " << k << endl;
        }
        // std::list<int> l = { 7, 5, 16, 8 };
        // print(l, " pre rotate list");
        // auto l_it = ::rotate(l.begin(), ++decltype(l.begin())(l.begin()), l.end());
        // print(l, "post rotate list");
        // cout << "next start point is " << *l_it << endl;
    }
}

// 189. Rotate Array https://leetcode.com/problems/rotate-array/?tab=Description
// Rotate an array of n elements to the right by k steps.
// For example, with n = 7 and k = 3, the array [1,2,3,4,5,6,7] is rotated to [5,6,7,1,2,3,4].
// Note: Try to come up as many solutions as you can, there are at least 3 different ways to solve this problem.

int main() {
    // test::binarySearch();
    // test::first_index_equal_to_value_in_an_increasing_sequence();
    test::rotate();
}
