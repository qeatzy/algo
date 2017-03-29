
// Codeforces Round #340 (Div. 2) E. XOR and Favorite Number  http://www.aichengxu.com/other/9897523.htm

#include <cstdio>
#include <algorithm>
#include <cmath>
using namespace std;
typedef long long ll;
const int maxn = 100100;
int n, m, k;
int num[maxn], block[maxn];
ll ans[maxn];
int cnt[2000100];
struct query
{
    int l, r, id;
}node[maxn];

int cmp(query A, query B)
{
    if (block[A.l] == block[B.l]) return A.r < B.r;
    else return A.l < B.l;
}

int main()
{
    scanf("%d%d%d", &n, &m, &k);
    int block_size = sqrt(n);
    num[0] = 0;
    for (int i = 1; i <= n; i++)
    {
        scanf("%d", &num[i]);
        num[i] ^= num[i - 1];
        block[i] = i / block_size;
    }
    for (int i = 0; i < m; i++)
    {
        scanf("%d%d", &node[i].l, &node[i].r);
        node[i].l--;
        node[i].id = i;
    }
    sort(node, node + m, cmp);
    int l = 1, r = 0;
    ll tmp = 0;
    for (int i = 0; i < m; i++)
    {
        while (r < node[i].r)
        {
            r++;
            tmp += cnt[num[r] ^ k];
            cnt[num[r]]++;
        }
        while (r > node[i].r)
        {
            cnt[num[r]]--;
            tmp -= cnt[num[r] ^ k];
            r--;
        }
        while (l < node[i].l)
        {
            cnt[num[l]]--;
            tmp -= cnt[num[l] ^ k];
            l++;
        }
        while(l > node[i].l)
        {
            l--;
            tmp += cnt[num[l] ^ k];
            cnt[num[l]]++;
        }
        ans[node[i].id] = tmp;
    }
    for (int i = 0; i < m; i++)
        printf("%I64d\n", ans[i]);
    return 0;
}
