#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

const long long INF = 1e18; 

struct Edge {
    int to;
    long long cap, flow;
    int rev; 
};

struct Dinic {
    int n;
    vector<vector<Edge>> adj;
    vector<int> level, ptr;

    Dinic(int n) : n(n), adj(n), level(n), ptr(n) {}

    void add_edge(int from, int to, long long cap) {
        adj[from].push_back({to, cap, 0, (int)adj[to].size()});
        adj[to].push_back({from, 0, 0, (int)adj[from].size() - 1});
    }

    bool bfs(int s, int t) {
        fill(level.begin(), level.end(), -1);
        level[s] = 0;
        queue<int> q;
        q.push(s);
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            for (auto& edge : adj[v]) {
                if (edge.cap - edge.flow > 0 && level[edge.to] == -1) {
                    level[edge.to] = level[v] + 1;
                    q.push(edge.to);
                }
            }
        }
        return level[t] != -1;
    }

    // 标准的多路增广 DFS，一次性榨干当前分层图的所有残量
    long long dfs(int v, int t, long long pushed) {
        if (pushed == 0) return 0;
        if (v == t) return pushed;
        long long total_pushed = 0;
        for (int& cid = ptr[v]; cid < adj[v].size(); ++cid) {
            auto& edge = adj[v][cid];
            int tr = edge.to;
            if (level[v] + 1 != level[tr] || edge.cap - edge.flow == 0) continue;
            long long push = dfs(tr, t, min(pushed, edge.cap - edge.flow));
            if (push == 0) continue;
            edge.flow += push;
            adj[tr][edge.rev].flow -= push;
            pushed -= push;
            total_pushed += push;
            if (pushed == 0) break; // 当前节点的输入流已经用完，收工
        }
        return total_pushed;
    }

    long long max_flow(int s, int t) {
        long long flow = 0;
        while (bfs(s, t)) {
            fill(ptr.begin(), ptr.end(), 0);
            while (long long pushed = dfs(s, t, INF)) {
                flow += pushed;
            }
        }
        return flow;
    }
};

int main() {
    // 关流加速，应对大输入量
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    if (!(cin >> n >> m)) return 0;

    int S = 0, T = 1;
    auto get_id = [&](int i, int j) {
        return 2 + i * m + j;
    };
    int node_cnt = 2 + n * m; 

    // 预估最大总节点数，安全开大
    int total_nodes = 2 + n * m + 2 * (n - 1) * m + 2 * n * (m - 1) + 10;
    Dinic dinic(total_nodes);

    long long total_sum = 0;
    long long val;

    // 1. Linux 基础分矩阵
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            cin >> val;
            total_sum += val;
            dinic.add_edge(S, get_id(i, j), val);
        }
    }

    // 2. Windows 基础分矩阵
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            cin >> val;
            total_sum += val;
            dinic.add_edge(get_id(i, j), T, val);
        }
    }

    // 3. 垂直相邻 Linux 奖励
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < m; ++j) {
            cin >> val;
            total_sum += val;
            int v_node = node_cnt++;
            dinic.add_edge(S, v_node, val);
            dinic.add_edge(v_node, get_id(i, j), INF);
            dinic.add_edge(v_node, get_id(i + 1, j), INF);
        }
    }

    // 4. 垂直相邻 Windows 奖励
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < m; ++j) {
            cin >> val;
            total_sum += val;
            int v_node = node_cnt++;
            dinic.add_edge(get_id(i, j), v_node, INF);
            dinic.add_edge(get_id(i + 1, j), v_node, INF);
            dinic.add_edge(v_node, T, val);
        }
    }

    // 5. 水平相邻 Linux 奖励
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m - 1; ++j) {
            cin >> val;
            total_sum += val;
            int v_node = node_cnt++;
            dinic.add_edge(S, v_node, val);
            dinic.add_edge(v_node, get_id(i, j), INF);
            dinic.add_edge(v_node, get_id(i, j + 1), INF);
        }
    }

    // 6. 水平相邻 Windows 奖励
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m - 1; ++j) {
            cin >> val;
            total_sum += val;
            int v_node = node_cnt++;
            dinic.add_edge(get_id(i, j), v_node, INF);
            dinic.add_edge(get_id(i, j + 1), v_node, INF);
            dinic.add_edge(v_node, T, val);
        }
    }

    long long min_cut = dinic.max_flow(S, T);
    cout << total_sum - min_cut << "\n";

    return 0;
}