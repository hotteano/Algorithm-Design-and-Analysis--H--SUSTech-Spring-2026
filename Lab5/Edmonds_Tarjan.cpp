#include <bits/stdc++.h>
using namespace std;

const long long INF = (1LL << 60);
const int MAXN = 1005;

struct edge {
    int u, v;
    long long w;
};

int Edmonds_Tarjan(){

}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    vector<edge> edges;
    edges.reserve(m);
    for (int i = 0; i < m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        edges.push_back({u, v, w});
    }
}