#include <iostream>
#include <vector>
#include <queue>

using namespace std;

const long long INF = 1e18;

struct Edge {
    int to;
    int cap;
    long long cost;
    int rev;
};

vector<vector<Edge>> adj;
vector<long long> dist_val;
vector<int> in_queue;
vector<int> pre_v;
vector<int> pre_e;

void add_edge(int u, int v, int cap, long long cost) {
    adj[u].push_back({v, cap, cost, (int)adj[v].size()});
    adj[v].push_back({u, 0, -cost, (int)adj[u].size() - 1});
}

bool spfa(int S, int T, int total_nodes) {
    dist_val.assign(total_nodes + 1, INF);
    in_queue.assign(total_nodes + 1, 0);
    pre_v.assign(total_nodes + 1, -1);
    pre_e.assign(total_nodes + 1, -1);

    queue<int> q;
    dist_val[S] = 0;
    q.push(S);
    in_queue[S] = 1;

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        in_queue[u] = 0;

        for (int i = 0; i < adj[u].size(); i++) {
            Edge &e = adj[u][i];
            if (e.cap > 0 && dist_val[e.to] > dist_val[u] + e.cost) {
                dist_val[e.to] = dist_val[u] + e.cost;
                pre_v[e.to] = u;
                pre_e[e.to] = i;
                if (!in_queue[e.to]) {
                    q.push(e.to);
                    in_queue[e.to] = 1;
                }
            }
        }
    }
    return dist_val[T] != INF;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m;
    if (!(cin >> n >> m)) return 0;

    int total_nodes = 2 * n;
    adj.assign(total_nodes + 1, vector<Edge>());

    for (int i = 2; i <= n - 1; i++) {
        add_edge(i, i + n, 1, 0);
    }

    for (int i = 0; i < m; i++) {
        int u, v;
        long long w;
        cin >> u >> v >> w;

        int from = u;
        if (u != 1 && u != n) {
            from = u + n; 
        }

        int to = v; 

        add_edge(from, to, 1, w);
    }

    int S = 1, T = n;
    long long max_flow = 0;
    long long min_cost = 0;

    while (spfa(S, T, total_nodes)) {
        long long flow = INF;
        int curr = T;

        while (curr != S) {
            int p = pre_v[curr];
            int edge_idx = pre_e[curr];
            flow = min(flow, (long long)adj[p][edge_idx].cap);
            curr = p;
        }

        curr = T;
        while (curr != S) {
            int p = pre_v[curr];
            int edge_idx = pre_e[curr];
            int rev_idx = adj[p][edge_idx].rev;

            adj[p][edge_idx].cap -= flow;
            adj[curr][rev_idx].cap += flow;
            curr = p;
        }

        max_flow += flow;
        min_cost += flow * dist_val[T];
    }

    cout << max_flow << " " << min_cost << "\n";

    return 0;
}
