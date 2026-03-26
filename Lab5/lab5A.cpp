#include <bits/stdc++.h>
using namespace std;

struct Edge {
    int u, v, w;
};

int n, m, T;
vector<Edge> edges;
int fa[10008];

int find(int x) {
    if(fa[x] != x) fa[x] = find(fa[x]);
    return fa[x];
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    cin >> n >> m >> T;
    for(int i = 1; i <= n; i++) fa[i] = i;
    
    for(int i = 0; i < m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        edges.push_back({u, v, w});
    }
    
    sort(edges.begin(), edges.end(), [](Edge a, Edge b) {
        return a.w > b.w;
    });
    
    int provinces = n;
    for(auto &e : edges) {
        int fu = find(e.u), fv = find(e.v);
        if(fu != fv && e.w < T) {
            fa[fu] = fv;
            provinces--;
        }
    }
    
    cout << provinces << endl;
    return 0;
}