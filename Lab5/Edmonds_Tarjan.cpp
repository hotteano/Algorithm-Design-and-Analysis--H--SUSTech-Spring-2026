#include <bits/stdc++.h>
using namespace std;

const long long INF = (1LL << 60);

/* ============================================
 * 左偏树（可并堆）
 * ============================================ */
struct Node {
    int id;                 // 边的原始编号
    long long w;            // 边权（实际值 = w + add）
    int dis;                // 距离（右子树高度）
    Node *l, *r;
    
    Node(int _id, long long _w) : 
        id(_id), w(_w), dis(0), l(nullptr), r(nullptr) {}
};

Node* merge(Node *a, Node *b) {
    if (!a) return b;
    if (!b) return a;
    if (a->w > b->w) swap(a, b);
    a->r = merge(a->r, b);
    if (!a->l || a->l->dis < a->r->dis) swap(a->l, a->r);
    a->dis = a->r ? a->r->dis + 1 : 0;
    return a;
}

Node* pop(Node *h) {
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
 * 
 * 核心思想：
 * 1. 每个点用左偏树维护入边（小根堆）
 * 2. 找最小入边时沿前驱遍历，发现环则缩点
 * 3. 并查集维护连通块，支持O(1)查询
 * 4. 延迟标记支持快速堆合并
 * 
 * 参数：
 *   root  - 根节点
 *   n     - 节点数
 *   edges - 边集
 *   in    - 输出：每个点的入边编号
 * 
 * 返回：最小树形图权值，无解返回 -1
 * ============================================ */
long long dmst(int root, int n, const vector<Edge> &edges, vector<int> &in) {
    int m = edges.size();
    
    // ----- 初始化：为每个点建堆 -----
    vector<Node*> h(n, nullptr);     // 每个点的堆
    vector<long long> add(n, 0);      // 延迟标记
    
    for (int i = 0; i < m; i++) {
        const auto &e = edges[i];
        if (e.u != e.v) {            // 忽略自环
            h[e.v] = merge(h[e.v], new Node(i, e.w));
        }
    }
    
    DSU dsu(n);
    vector<int> pre(n, -1);          // 前驱点
    vector<int> ine(n, -1);          // 入边编号
    vector<int> vis(n, -1);          // 访问标记
    long long ans = 0;
    
    // ----- 处理每个点找最小入边 -----
    for (int i = 0; i < n; i++) {
        if (i == root) continue;
        
        int u = i;
        vector<int> path;            // 记录遍历路径
        
        // 沿前驱走，直到根、成环或遇到已处理链
        while (vis[u] != i && u != root && dsu.find(u) == u) {
            vis[u] = i;
            path.push_back(u);
            
            // 获取堆顶（最小入边），跳过无效边
            while (h[u]) {
                int eid = h[u]->id;
                int from = edges[eid].u;
                if (dsu.find(from) == dsu.find(u)) {
                    h[u] = pop(h[u]);  // 同连通块，删除
                    continue;
                }
                // 更新延迟标记，使w变为实际值
                add[u] = edges[eid].w - h[u]->w;
                pre[u] = from;
                ine[u] = eid;
                break;
            }
            
            if (!h[u]) return -1;     // 无入边，无解
            ans += h[u]->w + add[u];
            u = pre[u];
        }
        
        // 连接到已有树，无需缩点
        if (u == root || dsu.find(u) != u) continue;
        
        // ----- 发现环，缩点 -----
        int cur = dsu.find(pre[u]);
        
        // 合并环上所有点
        for (int v : path) {
            if (dsu.find(v) == cur) break;
            dsu.f[v] = cur;
            
            // 合并堆：下传延迟标记后merge
            if (h[cur] && h[v]) {
                h[cur]->w += add[cur];
                h[v]->w += add[v];
                add[cur] = add[v] = 0;
                h[cur] = merge(h[cur], h[v]);
            } else if (h[v]) {
                h[v]->w += add[v];
                add[cur] = 0;
                h[cur] = h[v];
            }
        }
        
        // 删除指向环内的边（惰性）
        while (h[cur] && dsu.find(edges[h[cur]->id].v) == cur) {
            h[cur] = pop(h[cur]);
        }
        
        i--;  // 重新处理当前点
    }
    
    // ----- 还原入边 -----
    in = ine;
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
