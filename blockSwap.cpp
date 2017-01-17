#include "utility.h"
#include <list>

// http://en.cppreference.com/w/cpp/algorithm/rotate
// currently 'possible implementation' fails on corner case for std::rotate on cppreference.
// corner case first != mid && mid == last.
// at worst n-1 swap operation needed, not n, since if n-1 items are in place, the remaining is in
// place too.
// at best n/2 swap operation needed, eg, mid - first == last - midlle. (omitting
// trivial case).
// if first == mid || mid == last, no operation is needed. eg, first + 1 == last.
// search for __rotate in file /usr/lib/gcc/i686-pc-cygwin/5.4.0/include/c++/bits/stl_algo.h
// a sensible return value is first + (last - mid). since c++11.

template <typename ForwardIt>
ForwardIt swapBlock_(ForwardIt first, ForwardIt mid, ForwardIt last, std::forward_iterator_tag) {
    if (mid == last) return first; // right half non-empty
    if (first == mid) return last;
    auto next = mid;
    auto ret = first;
    while (next != last) { ++ret, ++next; }
    next = mid;
    while (first != mid) { // left half non-empty
        std::iter_swap(first, next);
        ++first, ++next;
        if (next == last) { // right half exhausted, reset
            next = mid;
        } else if (first == mid) { // left half exhausted, reset
            mid = next;
        }
    }
    return ret;
}

template <typename RandomIt>
RandomIt swapBlock_(RandomIt first, RandomIt mid, RandomIt last, std::random_access_iterator_tag) {
    if (mid == last) return first; // right half non-empty
    auto sz_left = mid - first;
    auto next = mid;
    // precondition and loop invariant, right half are non-empty, mid != last.
    // exit-condition of loop, exit after finish swap range if mid - fisrt == last - mid.
    // progress, first always move forward.
    while (first != mid) { // left half non-empty
        std::iter_swap(first, next);
        ++first, ++next;
        if (next == last) { // right half exhausted, reset
            next = mid;
        } else if (first == mid) { // left half exhausted, reset
            mid = next;
        }
    }
    return last - sz_left;
}

template <typename Iterator>
Iterator swapBlock(Iterator first, Iterator mid, Iterator last) {
    return swapBlock_(first, mid, last, typename std::iterator_traits<Iterator>::iterator_category());
}

namespace test {
    void swapBlock() {
        // res.push_back({0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
        auto v = range(7);
        print(v, " pre rotate");
        auto it = ::swapBlock(v.begin(), v.begin() + 7, v.end());
        // ::swapBlock(v.begin(), v.end(), v.end());
        print(v, "post rotate");
        cout << "next start point is " << *it << endl;

        std::list<int> l = { 7, 5, 16, 8 };
        print(l, " pre rotate");
        auto l_it = ::swapBlock(l.begin(), ++decltype(l.begin())(l.begin()), l.end());
        print(v, "post rotate");
        cout << "next start point is " << *l_it << endl;
    }
}

int main() {
    test::swapBlock();
}
