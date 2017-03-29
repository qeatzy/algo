// 算法马拉松 数列积
// 51Nod 算法马拉松23 ziqian2000  http://www.voidcn.com/blog/ziqian2000/article/p-6546533.html
// 算法马拉松23  数列积  https://www.51nod.com/contest/problem.html#!problemId=1592
// UESTCACM 每周算法讲堂 莫队算法  http://www.bilibili.com/video/av4291097/
#include "utility.h"
#include<cmath> 
#include<ctime>
#include<cstdio>
#include<algorithm>
#define N 50005
#define lowbit(_i) (_i&-_i)
#define R register
using namespace std;
namespace runzhe2000
{
    typedef unsigned long long ull;
    int read()
    {
        int r = 0; char c = getchar();
        for(; c < '0' || c > '9'; c = getchar());
        for(; c >='0' && c <='9'; r = r*10+c-'0', c = getchar());
        return r;
    }
    int n, S, qcnt, mx, a[N], block[N], ccnt;
    ull out[N];
    struct que
    {
        int l, r, id;
        bool operator < (const que &that) const 
        {
            return block[l] != block[that.l] 
                ? block[l] < block[that.l] 
                : (block[l] & 1) ? r < that.r : that.r < r;
        }
    }q[N];
    struct BIT
    {
        ull t[N<<1];
        ull ask(R int x){R ull r = 0; for(; x; x -= lowbit(x)) r += t[x]; return r;}
        void mod(R int x, R ull v){for(; x <= mx; x += lowbit(x)) t[x] += v;}
    } tsum, tsum_i, tsum_a, tsum_1;
    ull add(int j)
    {
        int pos = a[j];
        ull ret = 0, sum = tsum.ask(pos), sum_i = tsum_i.ask(pos), sum_a = tsum_a.ask(pos), sum_1 = tsum_1.ask(pos);
        ret += -sum+pos*sum_i+(j-n)*(sum_1*pos-sum_a);
        sum = tsum.ask(mx)-sum; sum_i = tsum_i.ask(mx)-sum_i; sum_a = tsum_a.ask(mx)-sum_a; sum_1 = tsum_1.ask(mx)-sum_1;
        ret -= -sum+pos*sum_i+(j-n)*(sum_1*pos-sum_a);
        tsum.mod(pos, (ull)(n-j)*pos);
        tsum_i.mod(pos, n-j);
        tsum_a.mod(pos, pos);
        tsum_1.mod(pos, 1);
        return ret;
    }
    ull del(int j)
    {
        int pos = a[j];
        tsum.mod(pos, (ull)(j-n)*pos);
        tsum_i.mod(pos, -n+j);
        tsum_a.mod(pos, -pos);
        tsum_1.mod(pos, -1);
        ull ret = 0, sum = tsum.ask(pos), sum_i = tsum_i.ask(pos), sum_a = tsum_a.ask(pos), sum_1 = tsum_1.ask(pos);
        ret -= -sum+pos*sum_i+(j-n)*(sum_1*pos-sum_a);
        sum = tsum.ask(mx)-sum; sum_i = tsum_i.ask(mx)-sum_i; sum_a = tsum_a.ask(mx)-sum_a; sum_1 = tsum_1.ask(mx)-sum_1;
        ret += -sum+pos*sum_i+(j-n)*(sum_1*pos-sum_a);
        return ret;
    }
    void main()
    {
        n = read(); S = sqrt((double)n);
        for(int i = 1; i <= n; i++) a[i] = read(), block[i] = i % S == 1 ? block[i-1]+1 : block[i-1], mx < a[i] ? mx = a[i] : 0; mx++;
        qcnt = read();  for(int i = 1; i <= qcnt; i++) q[i].l = read(), q[i].r = read(), q[i].id = i;
        sort(q+1, q+1+qcnt); R int lef = 1, rig = 0, l, r; ull ans = 0; 
        for(int i = 1; i <= qcnt; i++)
        {
            l = q[i].l, r = q[i].r;
            for(; l < lef; lef--) ans -= add(lef-1);
            for(; rig < r; rig++) ans += add(rig+1);
            for(; lef < l; lef++) ans -= del(lef);
            for(; r < rig; rig--) ans += del(rig);
            out[q[i].id] = ans;
        }
        for(int i = 1; i <= qcnt; i++) printf("%llu\n",out[i]);
    }
}
int main()
{
    runzhe2000::main();
}
