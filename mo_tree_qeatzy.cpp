// D. Tree and Queries http://codeforces.com/problemset/problem/375/D

#include<cstdio>
#include<cstring>
#include<vector>
#include<queue>
#include<cmath>
#include<cctype>
#include<string>
#include<algorithm>
#include<iostream>
#include<ctime>
#include<map>
#include<set>
using namespace std;

// const int N = 2e5+7;
const int N = 1e5+7;
const int M = 1e5+7;
const int max_value = 1e5+7;

std::vector<int> neighbours[N];
int colors[max_value];  // freq[max_value];
std::map<int,int> counts[N];
std::vector<std::pair<int,int>> queries[N];
int ans[M];

void merge(int v, int p) {
    if (counts[v].size() > counts[p].size()) {
        counts[v].swap(counts[p]);
    }
    for (auto kv: counts[v]) {
        counts[p][kv.first] += kv.second;
    }
}

void dfs(int v, int p) {
    counts[v][colors[v]]++;
    for (auto ch: neighbours[v]) {
        if (ch == p) continue;
        dfs(ch, v);
        merge(ch, v);
    }
    // std::memset(freq, (counts[v].back().first + 1) * sizeof(decltype(freq[0])), 0);
    if (queries[v].empty()) return;
    std::vector<int> freq;
    for (auto p: counts[v]) freq.push_back(p.second);
    std::sort(freq.begin(), freq.end());
    for (auto p: queries[v]) {
        ans[p.second] = freq.end() - std::lower_bound(freq.begin(), freq.end(), p.first);
        // cout << "cnt = " << p.first << ", id = " << p.second << endl;
        // for (auto count: counts[v]) {
        //     if (count.second >= p.first) { ++ans[p.second]; }
        // }
    }
}

int main() {
    cin.tie(0);
    ios::sync_with_stdio(false);
    int n, m;
    cin >> n >> m;
    // cout << "n = " << n << ", m = " << m << endl;
    for (int i = 0; i < n; ++i) {
        cin >> colors[i];
    }
    for (int i = 0; i < n-1; ++i) {
        int v, ch;
        cin >> v >> ch;
        --v, --ch;
        neighbours[v].push_back(ch);
        neighbours[ch].push_back(v);
    }
    for (int i = 0; i < m; ++i) {
        int v, count;
        cin >> v >> count;
        // cout << "v = " << v << ", count = " << count << endl;
        --v;
        queries[v].emplace_back(count, i);
    }
    // for (int i = 0; i < n; ++i) { cout << "q[" << i << "].size() = " << queries[i].size() << endl; }
    dfs(0,-1);
    for (int i = 0; i < m; ++i) {
        cout << ans[i] << '\n';
    }
}
