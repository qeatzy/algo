// http://mayokoex.hatenablog.com/entry/2016/02/12/204453

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

typedef std::pair<int,int> pii;

const int MAXN = 100010;
int colors[MAXN], ans[MAXN], size[MAXN];
vector<int> N[MAXN], G[MAXN];
map<int, int> M[MAXN];
vector<pii> queries[MAXN];    // queries

void merge(int v, int u) { // vertex, child??
    if (size[v] < size[u]) {
        cout << "v = " << v << ", u = " << u << ", ";
        cout << " error??\n";
        // -- the child's property is parent's too? and the child no longer need that??
        swap(size[v], size[u]);
        swap(M[v], M[u]);
        swap(N[v], N[u]);
    }
    size[v] += size[u];
    for (auto p : M[u]) {   // p : {color, count}
        int x = M[v][p.first];
        for (int i = x+1; i <= x+p.second; i++) {
            if (N[v].size() < i) N[v].push_back(1);
            else N[v][i-1]++;
        }
        M[v][p.first] += p.second;
    }
}

void dfs(int v, int p) { // vertex, parent
    cout << " in: " << v << '\n';
    M[v][colors[v]]++;
    N[v].push_back(1);
    size[v] = 1;
    for (int ch : G[v]) {
        if (ch==p) continue;
        dfs(ch, v);
        merge(v, ch);
    }
    for (pii p : queries[v]) {
        if (p.first <= N[v].size()) ans[p.second] = N[v][p.first-1];
    }
    cout << "out: " << v << '\n';
}

int main() {
    cin.tie(0);
    ios::sync_with_stdio(false);
    int n, m;
    cin >> n >> m;
    for (int i = 0; i < n; i++) {
        cin >> colors[i];
    }
    for (int i = 0; i < n-1; i++) {
        int u, v;
        cin >> u >> v;
        u--; v--;
        G[u].push_back(v);
        G[v].push_back(u);
    }
    for (int i = 0; i < m; i++) {
        int v, k;
        cin >> v >> k;
        queries[v-1].emplace_back(k, i);
    }
    dfs(0, -1);
    for (int i = 0; i < m; i++) cout << ans[i] << endl;
    return 0;
}
