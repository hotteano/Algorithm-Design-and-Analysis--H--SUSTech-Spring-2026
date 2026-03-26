#include <bits/stdc++.h>
using namespace std;

const int INF = 0x3f3f3f3f;

struct Edge {
    int u, v, w;
};

int Chu_Liu(vector<Edge> edges, int n, int rt, int pre[]) {
    int res = 0;
    while (true) {
        vector<int> in(n, INF), id(n, -1), vis(n, -1);
        vector<int> tmp_pre(n, -1);
        
        for (auto &e : edges) {
            if (e.u != e.v && e.w < in[e.v]) {
                in[e.v] = e.w;
                tmp_pre[e.v] = e.u; 
            }
        }
        in[rt] = 0;
        
        for (int i = 0; i < n; i++) 
            if (in[i] == INF) return -1;
        
        int cnt = 0;
        for (int i = 0; i < n; i++) {
            res += in[i];
            int v = i;
            while (vis[v] != i && id[v] == -1 && v != rt) {
                vis[v] = i;
                v = tmp_pre[v];
            }
            if (v != rt && id[v] == -1) {
                for (int u = tmp_pre[v]; u != v; u = tmp_pre[u])
                    id[u] = cnt;
                id[v] = cnt++;
            }
        }
        if (cnt == 0) {
            for (int i = 0; i < n; i++) pre[i] = tmp_pre[i];
            break;
        }
        
        for (int i = 0; i < n; i++)
            if (id[i] == -1) id[i] = cnt++;
        
        vector<Edge> new_edges;
        for (auto &e : edges) {
            int u = id[e.u], v = id[e.v];
            if (u != v) {
                new_edges.push_back({u, v, e.w - in[e.v]});
            }
        }
        
        n = cnt;
        rt = id[rt];
        edges = new_edges;
    }
    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, m;
    cin >> n >> m;
    vector<Edge> edges;
    for (int i = 0; i < m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        edges.push_back({u, v, w});
    }
    
    int best_rt = -1, min_cost = INF;
    vector<int> best_pre(n);
    
    for (int rt = 0; rt < n; rt++) {  
        vector<int> pre(n);
        int cost = Chu_Liu(edges, n, rt, pre.data());
        if (cost != -1 && cost < min_cost) {
            min_cost = cost;
            best_rt = rt;
            best_pre = pre;
        }
    }
    
    if (best_rt == -1) {
        cout << "impossible" << endl;
    } else {
        cout << min_cost << " " << best_rt << endl;
        for (int i = 0; i < n; i++) {
            if (i != best_rt) {
                cout << i << " " << best_pre[i] << endl;
            }
            else{
                cout << i << endl;
            }
        }
    }
    return 0;
}