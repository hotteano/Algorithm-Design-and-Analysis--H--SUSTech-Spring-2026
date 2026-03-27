#include <bits/stdc++.h>
using namespace std;

const int INF = 1e9;
const int MAXN = 1005;

struct edge{
    int u, v, w;
};

int Chu_Liu_Alg(int root, int n, vector<edge> edges){

    int ret = 0;

    while(true){
        vector<int> in(n, INF); 
        vector<int> pre(n, -1);

        // 第一步：找到每个结点的最小入边
        for(auto &e : edges){
            if(e.u != e.v && e.w < in[e.v]){
                in[e.v] = e.w;
                pre[e.v] = e.u;
            }
        }
        //找到所有结点非自环的最小入边

        in[root] = 0;

        for(int i = 0; i < n; i++){
            if(in[i] == INF) return -1; 
        }
        // 如果不可达，返回-1

        // 第二步：寻找环
        int cnt = 0;
        vector<int> id(n, -1), vis(n, -1);
        // 分别表示超级结点的编号和访问标记
        
        for(int i = 0; i < n; i++) ret += in[i]; //累加当前层的最小入边权值


        // 累加之后，通过dfs寻找环，并将环内的结点标记为同一个超级结点
        for(int i = 0; i < n; i++){
            int v = i;
            while(vis[v] != i && id[v] == -1 && v != root){
                vis[v] = i;
                v = pre[v];
            } // 迭代搜索前驱结点，直到找到一个环或者回到根结点
            if(v != root && id[v] == -1){
                for(int u = pre[v]; u != v; u = pre[u]) id[u] = cnt;
                id[v] = cnt++;
            } // 如果找到一个环，就将环内的结点标记为同一个超级结点
        }

        if(cnt == 0) break; // 如果没有环，算法结束

        // 第三步：缩点
        for(int i = 0; i < n; i++){
            if(id[i] == -1) id[i] = cnt++;
        } // 将没有被标记的结点也分配一个超级结点编号

        // 重建边权
        vector<edge> new_edges;
        for(auto &e : edges){
            int u = id[e.u], v = id[e.v];
            if(u != v) new_edges.push_back({u, v, e.w - in[e.v]});
            // 算法关键：所有入边权值都要减去对应结点的最小入边权值
        }

        n = cnt; // 更新结点数量为超级结点数量
        root = id[root]; // 更新根结点编号为对应的超级结点编号
        edges = move(new_edges); // 更新边集合为缩点后的边集合
    }

    return ret;
}

int main(){
    int n, m;
    while(cin >> n >> m){
        if(n <= 0 || m < 0){
            cout << "impossible\n\n";
            for(int i = 0; i < max(0, m); i++){
                int u, v, w;
                cin >> u >> v >> w;
            }
            continue;
        }

        vector<edge> edges(m);
        for(int i = 0; i < m; i++){
            cin >> edges[i].u >> edges[i].v >> edges[i].w;
        }

        int result = Chu_Liu_Alg(0, n, edges);
        if(result == -1){
            cout << "impossible\n\n";
        } else {
            cout << result << "\n\n";
        }
    }

    return 0;
}