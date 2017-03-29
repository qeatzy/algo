
// DQUERY - D-query  http://www.spoj.com/problems/DQUERY/

#include <iostream>
#include <cmath>
#include <vector>
#include <tuple>
#include <algorithm>
using namespace std;

    const int max_value = 1e6 + 7;
    const int N = 30000 + 7, Q = 200000 + 7;
    int n, q, sz_mo;
    int nums[N];
    struct Node {
        int l, r, id;
    } queries[Q];
    int counts[max_value];
    int ans[Q];
    int cur_ans;

int mo_left, mo_right;

bool cmp_mo(const Node &a, const Node &b) {
    auto block1 = a.l / sz_mo, block2 = b.l / sz_mo;
    if (block1 != block2) return block1 < block2;
    return a.r < b.r;
}

void del (int i) {
    --counts[nums[i]];
    if (counts[nums[i]] == 0) --cur_ans;
}
void add (int i) {
    if (counts[nums[i]] == 0) ++cur_ans;
    ++counts[nums[i]];
}

int main() {
    std::ios::sync_with_stdio(false); cin.tie(nullptr);
    cin >> n;
    sz_mo = std::sqrt(n);
    for (int i = 0; i < n; ++i) {
        cin >> nums[i];
    }
    cin >> q;
    for (int i = 0; i < q; ++i) {
        cin >> queries[i].l >> queries[i].r;
        --queries[i].l, --queries[i].r;
        queries[i].id = i;
    }
    mo_left = 0, mo_right = -1;
    cur_ans = 0;
    std::sort(&queries[0], &queries[q], cmp_mo);
    // for (auto const &x: queries) {
    for (int i = 0; i < q; ++i) {
        auto const &x = queries[i];
        while (mo_left < x.l) del(mo_left++);
        while (mo_left > x.l) add(--mo_left);
        while (mo_right < x.r) add(++mo_right);
        while (mo_right > x.r) del(mo_right--);
        ans[x.id] = cur_ans;
    }
    for (int i = 0; i < q; ++i) {
        cout << ans[i] << '\n';
    }
}
