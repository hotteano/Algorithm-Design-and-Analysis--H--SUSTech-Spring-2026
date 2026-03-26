#include<iostream>
#include<string.h>
using namespace std;
const double g=10.0,eps=1e-9;
const int N=1000+10,maxn=40000+10,inf=0x3f3f3f3f;
int in[N],vis[N],Hash[N];
int pre[N];
int ans,ansnum;
struct edge
{
    int u,v,w;
}e[maxn];
int dirmst(int root,int nv,int ne)
{
    ans=0;
    while(1)
    {
        memset(in,inf,sizeof in);
        for(int i=0;i<ne;i++)
        {
            int u=e[i].u,v=e[i].v;
            if(e[i].w<in[v]&&v!=u)
            {
                in[v]=e[i].w;//找最小入边
                pre[v]=u;//记录前驱
                if(u==root)
                    ansnum=i;
            }
        }
        in[root]=0;
        for(int i=0;i<nv;i++)
            if(in[i]==inf)
               return 0;//如果不能构成最小树形图
        int cntnum=0;
        memset(vis,-1,sizeof vis);
        memset(Hash,-1,sizeof Hash);
        for(int i=0;i<nv;i++)
        {
            ans+=in[i];
            int v=i;
            while(vis[v]!=i&&v!=root&&Hash[v]==-1)
				vis[v]=i,v=pre[v];
            if(v!=root&&Hash[v]==-1)//有环
            {
                for(int u=pre[v];u!=v;u=pre[u])
                    Hash[u]=cntnum;
                Hash[v]=cntnum++;
            }
        }
        if(cntnum==0)
			return 1;//没有环
        for(int i=0;i<nv;i++)
            if(Hash[i]==-1)
                Hash[i]=cntnum++;//缩点
        for(int i=0;i<ne;i++)
        {
            int v=e[i].v;
            e[i].u=Hash[e[i].u];//重新编号
            e[i].v=Hash[e[i].v];
            if(e[i].u!=e[i].v)e[i].w-=in[v];//因为前面已经将in[v]加到ans中了
        }
        nv=cntnum;
        root=Hash[root];
    }
    return 1;
}
int main()
{
    int n,m;
    while(cin>>n>>m)
    {
         int sum=0;
         for(int i=0;i<m;i++)
         {
            int a,b,c;
            cin>>a>>b>>c;
            e[i].u=a;e[i].v=b;e[i].w=c;
            sum+=c;
         }
         int s=m;
         for(int i=0;i<n;i++)
         {
             e[s].u=n;
             e[s].v=i;
             e[s].w=sum+1;//为了只有一个节点与root相连
             s++;
         }
         ansnum=0;
         if(!dirmst(n,n+1,n+m)||ans>=2*sum+2)
	     cout<<"impossible"<<endl;
         else
	     cout<<ans-sum-1<<" "<<ansnum-m<<endl;
         cout<<endl;
    }
    return 0;
}