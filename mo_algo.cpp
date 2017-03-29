// #include "utility.h"
// #include <cstdio>

// 86 D. Powerful array http://codeforces.com/contest/86/problem/D

// std::ostream& operator<<(std::ostream &out, std::tuple<int,int,int> t) {
//     return out << "[" << std::get<0>(t) << "," << std::get<1>(t) << "," << std::get<2>(t) << "]";
//     // return out << "[" << std::get<0>(t) << "," << std::get<1>(t) << "]";
// }

#include <iostream>
#include <cmath>
#include <vector>
#include <tuple>
#include <algorithm>
using namespace std;
int main() {
    int maxn = 1e6 + 7;
    int n, t, sz_mo; // len of array, len of queries
    cin >> n >> t;
    sz_mo = std::sqrt(n);
    std::vector<int> nums(n);
    std::vector<int> counts(maxn);
    for (int i = 0; i < n; ++i) {
        cin >> nums[i];
    }
    std::vector<long long> ans(t);
    long long cur_ans = 0;
    std::vector<std::tuple<int, int, int>> queries (t);
    for (int i = 0; i < t; ++i) {
        cin >> std::get<0>(queries[i]) >> std::get<1>(queries[i]);
        --std::get<0>(queries[i]), --std::get<1>(queries[i]);
        std::get<2>(queries[i]) = i;
    }
    std::sort(queries.begin(), queries.end(), [sz_mo](const std::tuple<int,int,int> & lhs, const std::tuple<int,int,int> &rhs) {
            int block1 = std::get<0>(lhs) / sz_mo, block2 = std::get<0>(rhs) / sz_mo;
            return (block1 == block2) ? std::get<1>(lhs) < std::get<1>(rhs) : block1 < block2;
            });
    int mo_left = 0, mo_right = -1;
    auto add = [&cur_ans,&counts,&nums](int i) {
        ++counts[nums[i]];
        cur_ans += nums[i] * (counts[nums[i]] * 2 - 1);
    };
    auto del = [&cur_ans,&counts,&nums](int i) {
        cur_ans -= nums[i] * (counts[nums[i]] * 2 - 1);
        --counts[nums[i]];
    };
    for (auto const &t: queries) {
        auto l = std::get<0>(t);
        auto r = std::get<1>(t);
        auto id = std::get<2>(t);
        while (mo_left < l) del(mo_left++);
        while (mo_left > l) add(--mo_left);
        while (mo_right > r) del(mo_right--);
        while (mo_right < r) add(++mo_right);
        ans[id] = cur_ans;
    }
    for (auto x: ans) {
        cout << x << '\n';
    }
    // print(nums,"nums");
    // print(queries,"queries");
    // print(ans,"ans");
}
