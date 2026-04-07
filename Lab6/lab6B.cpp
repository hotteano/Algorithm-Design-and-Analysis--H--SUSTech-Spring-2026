#include <iostream>
#include <vector>
#include <queue>

using namespace std;

struct Node {
    int id;
    long long w;
    long long sz;
    bool operator<(const Node& other) const {
        return w * other.sz < other.w * sz;
    }
};

int find_set(int v, vector<int>& parent) {
    if (v == parent[v])
        return v;
    return parent[v] = find_set(parent[v], parent);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, F;
    if (!(cin >> n >> F)) return 0;

    vector<long long> C(n + 1);
    long long ans = 0;
    for (int i = 1; i <= n; ++i) {
        cin >> C[i];
        ans += C[i];
    }

    vector<int> tree_parent(n + 1, 0);
    for (int i = 0; i < n - 1; ++i) {
        int u, v;
        cin >> u >> v;
        tree_parent[v] = u;
    }

    vector<int> dsu(n + 1);
    vector<long long> W = C;
    vector<long long> Size(n + 1, 1);
    priority_queue<Node> pq;

    for (int i = 1; i <= n; ++i) {
        dsu[i] = i;
        if (i != F) {
            pq.push({i, W[i], Size[i]});
        }
    }

    while (!pq.empty()) {
        Node curr = pq.top();
        pq.pop();

        int v = curr.id;
        if (v != dsu[v] || curr.w != W[v] || curr.sz != Size[v]) {
            continue;
        }

        int u = find_set(tree_parent[v], dsu);

        ans += Size[u] * W[v];
        W[u] += W[v];
        Size[u] += Size[v];
        dsu[v] = u;

        if (u != F) {
            pq.push({u, W[u], Size[u]});
        }
    }

    cout << ans << "\n";

    return 0;
}
