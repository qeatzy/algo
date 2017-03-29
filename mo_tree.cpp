// http://blog.csdn.net/ACMmaxx/article/details/20058911

//#pragma comment(linker, "/STACK:102400000,102400000")
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
#define MP(x,y) make_pair((x),(y))
#define PB(x) push_back(x)
// typedef __int64 LL;
typedef long long LL;
//typedef unsigned __int64 ULL;
/* ****************** */
const int INF=1000111222;
const double INFF=1e200;
const double eps=1e-8;
const int mod=1000000007;
const int NN=100010;
const int MM=10010;
/* ****************** */

int a[NN],colors[NN];
struct G
{
    int v,next;
}E[NN*2];

int p[NN],T,tsp;
int ll[NN],rr[NN];
struct Q
{
    int l,r,id,fg,k;
}line[NN];
int ans[NN],num[NN],ff[NN];

void add(int u,int v)
{
    E[T].v=v;
    E[T].next=p[u];
    p[u]=T++;
}

void dfs(int u,int fa) // vertex, parent
{
    ll[u]=++tsp;
    a[tsp]=colors[u];
    int i,v;
    for(i=p[u];i+1;i=E[i].next)
    {
        v=E[i].v;
        if(fa==v)
            continue;
        dfs(v,u);
    }
    rr[u]=tsp;
}

bool cmp(Q x,Q y)
{
    if(x.fg==y.fg)
        return x.r<y.r;
    return x.fg<y.fg;
}

void solve(int n,int m)
{
    sort(line+1,line+1+m,cmp);
    int l,r,i;
    l=1;
    r=0;
    for(i=1;i<=m;i++)
    {
        while(l>line[i].l)
        {
            l--;
            num[a[l]]++;
            ff[ num[a[l]] ]++;
        }
        while(l<line[i].l)
        {
            ff[ num[a[l]] ]--;
            num[a[l]]--;
            l++;
        }
        while(r>line[i].r)
        {
            ff[ num[a[r]] ]--;
            num[a[r]]--;
            r--;
        }
        while(r<line[i].r)
        {
            r++;
            num[a[r]]++;
            ff[ num[a[r]] ]++;
        }
        ans[ line[i].id ]=ff[ line[i].k ];
    }
    for(i=1;i<=m;i++)
    {
        printf("%d\n",ans[i]);
    }
}

int main()
{
    memset(p,-1,sizeof(p));
    T=tsp=0;

    int n,m,i,u,v,S;
    scanf("%d%d",&n,&m);
    for(i=1;i<=n;i++)
        scanf("%d",&colors[i]);
    for(i=1;i<n;i++)
    {
        scanf("%d%d",&u,&v);
        add(u,v);
        add(v,u);
    }

    dfs(1,-1);
    S=sqrt(n+0.0);

    for(i=1;i<=m;i++)
    {
        scanf("%d%d",&v,&line[i].k);
        line[i].id=i;
        line[i].l=ll[v];
        line[i].r=rr[v];
        line[i].fg=ll[v]/S;
    }

    solve(n,m);

    return 0;
}
