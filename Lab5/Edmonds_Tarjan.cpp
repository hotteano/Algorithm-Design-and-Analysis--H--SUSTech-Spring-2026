#include <bits/stdc++.h>
using namespace std;

const long long INF = (1LL << 60);

/* ============================================
 * 左偏树（可并堆）
 * ============================================ */
struct Node {
    int id;                 // 边的原始编号
    long long w;            // 边权
    long long lazy;         // 延迟标记
    int dis;                // 距离（右子树高度）
    Node *l, *r;
    
    Node(int _id, long long _w) : 
        id(_id), w(_w), lazy(0), dis(0), l(nullptr), r(nullptr) {}
};

void apply(Node *a, long long val) {
    if (!a) return;
    a->w += val;
    a->lazy += val;
}

void pushdown(Node *a) {
    if (!a || a->lazy == 0) return;
    apply(a->l, a->lazy);
    apply(a->r, a->lazy);
    a->lazy = 0;
}

Node* merge(Node *a, Node *b) {
    if (!a) return b;
    if (!b) return a;
    pushdown(a);
    pushdown(b);
    if (a->w > b->w) swap(a, b);
    a->r = merge(a->r, b);
    if (!a->l || a->l->dis < a->r->dis) swap(a->l, a->r);
    a->dis = a->r ? a->r->dis + 1 : 0;
    return a;
}

Node* pop(Node *h) {
    pushdown(h);
    return merge(h->l, h->r);
}

/* ============================================
 * 并查集
 * ============================================ */
struct DSU {
    vector<int> f;
    DSU(int n = 0) { init(n); }
    
    void init(int n) {
        f.resize(n);
        iota(f.begin(), f.end(), 0);
    }
    
    int find(int x) {
        return f[x] == x ? x : f[x] = find(f[x]);
    }
};

/* ============================================
 * 边结构
 * ============================================ */
struct Edge {
    int u, v;           // 起点，终点
    long long w;        // 权值
};

/* ============================================
 * Edmonds 算法（Tarjan 优化版）
 * 时间复杂度：O(E log V)
 * 仅求最小树形图边权和
 * 
 * 核心思想：
 * 1. 每个点用左偏树维护入边（小根堆）
 * 2. 找最小入边时发现环则缩点
 * 3. 并查集维护连通块，支持O(1)查询
 * 4. 延迟标记支持快速堆合并
 * ============================================ */
long long dmst(int root, int n, const vector<Edge> &edges, vector<int> &in) {
    int m = edges.size();
    vector<Node*> h(n, nullptr);
    
    for (int i = 0; i < m; i++) {
        const auto &e = edges[i];
        if (e.u != e.v && e.v != root) { // 忽略自环和指向根的边
            h[e.v] = merge(h[e.v], new Node(i, e.w));
        }
    }
    
    DSU dsu(n);         // 维护缩点后的真实节点
    DSU w_dsu(n);       // 维护弱连通分量，用于高效发现环
    vector<int> pre(n, -1);
    long long ans = 0;
    
    vector<int> q;
    for (int i = 0; i < n; i++) {
        if (i != root) q.push_back(i);
    }
    
    while (!q.empty()) {
        int u = q.back();
        if (dsu.find(u) != u) {
            q.pop_back();
            continue;
        }
        
        while (h[u] && dsu.find(edges[h[u]->id].u) == u) {
            h[u] = pop(h[u]);
        }
        
        if (!h[u]) return -1; // 无入边，图不连通
        
        int min_e = h[u]->id;
        long long w = h[u]->w;
        ans += w;
        apply(h[u], -w); // 减去选中的入边权值，下传 lazy 标记
        
        int v = dsu.find(edges[min_e].u);
        pre[u] = v;
        
        if (w_dsu.find(u) == w_dsu.find(v)) {
            // 成环，将环上所有点往 u 上合并
            int curr = v;
            while (curr != u) {
                dsu.f[curr] = u;
                h[u] = merge(h[u], h[curr]);
                curr = dsu.find(pre[curr]);
            }
            // 成环后 u 代指整个环，不弹出 q.back()，代表下一轮继续给缩点后的 u 找入边
        } else {
            // 未成环，合并弱连通分量
            w_dsu.f[w_dsu.find(u)] = w_dsu.find(v);
            q.pop_back();
        }
    }
    
    return ans;
}

/* ============================================
 * 主函数
 * ============================================ */
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, m, root;
    cin >> n >> m >> root;
    
    vector<Edge> edges(m);
    for (int i = 0; i < m; i++) {
        cin >> edges[i].u >> edges[i].v >> edges[i].w;
    }
    
    vector<int> in;
    long long ans = dmst(root, n, edges, in);
    
    if (ans == -1) {
        cout << "impossible\n";
    } else {
        cout << ans << "\n";
    }
    
    return 0;
}
