#include "utility.h"

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
template <typename Iterator>
void swapBlock(Iterator first, Iterator mid, Iterator last) {
    if (mid == last) return; // right half non-empty
    auto next = mid;
    // precondition and loop invariant, right half are non-empty, mid != last.
    // exit-condition of loop, exit after finish swap range if mid - fisrt == last - mid.
    // progress, first always move forward.
    while (first != mid) { // left half non-empty
        // std::iter_swap(first++, next++);
        std::iter_swap(first, next);
        ++first, ++next;
        if (next == last) { // right half exhausted, reset
            next = mid;
        } else if (first == mid) { // left half exhausted, reset
            mid = next;
        }
        // cout << "left = " << *first << ", next = " << *next << endl;
        // wait();
        // print(p, last);
    }
}

auto test_case() {
    std::vector<vector<int>> res;
    // res.push_back(range(7));
    res.push_back({0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
    // res.push_back(range(2));
    return res;
}

int main() {
    // std::vector<int> v = range(7);
    for (auto& v: test_case()) {
        print(v);
        // swapBlock(v.begin(), v.begin() + 2, v.end());
        swapBlock(v.begin(), v.begin() + 7, v.end());
        // swapBlock(v.begin(), v.end(), v.end());
        print(v);
    }
}
