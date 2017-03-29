#include "utility.h"
#include <cstdio>
using namespace std;
const int maxn = 1e6+7;

struct node {
    int l, r, id;
} Q[maxn];

int pos[maxn];
long long ans[maxn];
long long flag[maxn];
int a[maxn];
bool cmp (node a, node b) {
    if (pos[a.l] == pos [b.l]) // same block
        return a.r < b.r;
    return pos[a.l] < pos[b.l];
}

int n, m, k;
int L=1, R=0;
long long Ans = 0;
void add (int x) {
    cout << "x = " << x << ", a[x] = " << a[x] << ", a[x]^k = " << (a[x]^k) << std::endl;
    // cout << "x = " << x << std::endl;
    print(flag, flag+1+x+1, " pre add");
    Ans += flag[a[x]^k];
    flag[a[x]]++;
    print(flag, flag+1+x+1, "post add");
}
void del (int x) {
    cout << "x = " << x << ", a[x] = " << a[x] << ", a[x]^k = " << (a[x]^k) << std::endl;
    print(flag, flag+1+x+1, " pre del");
    flag[a[x]]--;
    Ans -= flag[a[x]^k];
    print(flag, flag+1+x+1, "post del");
}
int main() {
    scanf("%d%d%d", &n, &m, &k);
    int sz = sqrt(n);
    for (int i = 1; i <= n; i++) {
        scanf("%d",&a[i]);
        a[i] ^= a[i-1];
        pos[i] = i / sz;
    }
    print(a+1,a+1+n,"array");
    for (int i = 1; i <= m; ++i) {
        scanf("%d%d",&Q[i].l,&Q[i].r);
        Q[i].id = i;
    }
    std::sort(Q+1,Q+1+m,cmp);
    flag[0] = 1;
    for (int i = 1; i <= m; ++i) {
        cout << "left = " << Q[i].l << ", right = " << Q[i].r << '\n';
        while (L<Q[i].l) {
            del(L-1);
            L++;
        }
        while (L>Q[i].l) {
            L--;
            add(L-1);
        }
        while (R<Q[i].r) {
            R++;
            add(R);
        }
        while (R>Q[i].r) {
            del(R);
            R--;
        }
        cout << "Ans = " << Ans << std::endl;
        ans[Q[i].id] = Ans;
    }
    for (int i = 1; i <= m; ++i) {
        printf("%lld\n", ans[i]);
    }
}
