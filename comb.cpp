#include "utility.h"
// combination related, eg, backtracking.

template <typename Iterator, typename T = typename std::iterator_traits<Iterator>::value_type>
bool next_combination(Iterator first, Iterator mid, Iterator last) {
    // assert(is_sorted(first, mid) && is_sorted(mid, last));
    if (first == mid || mid == last) return false;
    if (*first < *--last) {
        auto left = std::lower_bound(first, mid, *last);
        --left;
        auto right = std::upper_bound(mid, last, *left);
        std::iter_swap(left, right);
        if (++left != mid) { // make trailing of *left as small as possible
            ++right;
            ++last;
            mid = std::rotate(left, mid, right);
            std::sort(mid, last);
            std::rotate(left, mid, right);
        }
        return true;
    } else {
        std::rotate(first, mid, ++last);
        return false;
    }
}

namespace test {
    void next_combination() {
        std::vector<std::vector<int>> vec;
        // vec.push_back({1,2,2,3,5,  3});
        auto v = range(1,8);
        v.push_back(3);
        // vec.push_back(v);
        vec.push_back({1,2,2,4,4,6,8,9,  4});
        // std::string v = "ABCDEF";
        // int k = 2; // sep = 0;
        for (auto &v: vec) {
            assert(!v.empty());
            auto k = v.back();
            v.pop_back();
            assert(k >= 0 && v.size() >= size_t(k));
            auto first = v.begin(), mid = first + k, last = first + v.size();
            assert(is_sorted(first, mid) && is_sorted(mid, last));
            do {
                print(first, mid, "",-1,"|");
                print(mid, last);
            } while (::next_combination(v.begin(), v.begin() + k, v.end()));
            // } while (::nextCombination(v.begin(), v.begin() + k, v.end()));
        }
    }
}

// 77. Combinations https://leetcode.com/problems/combinations/?tab=Description
// Given two integers n and k, return all possible combinations of k numbers out of 1 ... n.
// For example, If n = 4 and k = 2, a solution is: [[2,4], [3,4], [2,3], [1,2], [1,3], [1,4],]
    vector<vector<int>> combine(int n, int k) {
        vector<vector<int>> res;
        if (n >= k && k > 0) {
            auto v = range(1,n+1);
            do {
                res.push_back(vector<int>(v.begin(), v.begin() + k));
            } while (next_combination(v.begin(), v.begin() + k, v.end()));
        }
        return res;
    }
vector<vector<int>> combine_others(int n, int k) { // https://discuss.leetcode.com/topic/26689/short-iterative-c-answer-8ms
    vector<vector<int>> result;
    int i = 0;
    vector<int> p(k, 0);
    while (i >= 0) {
        cout << " i = " << i << "\t"; print(p,"tmp");
        // wait();
        p[i]++;
        if (p[i] > n) --i;
        else if (i == k - 1) result.push_back(p);
        else {
            ++i;
            p[i] = p[i - 1];
        }
    }
    return result;
}

namespace test {
    void combine() {
        // int n = 4, k = 2;
        int n = 5, k = 3;
        // int n = 1, k = 1;
        auto res = ::combine(n,k);
        // auto res = ::combine_others(n,k);
        print(res,"res");
    }
}

int main() {
    test::init();
    // test::next_combination();
    test::combine();
}
