#include <iostream>
#include <vector>
#include <queue>

using namespace std;

struct Edge {
    int to;
    double v;
    double p;
};

int n, m;
vector<vector<Edge>> adj;

bool check(double r) {
    vector<double> dist(n + 1, 0.0);
    vector<int> cnt(n + 1, 0);
    vector<bool> in_queue(n + 1, true);
    queue<int> q;

    for (int i = 1; i <= n; i++) {
        q.push(i);
    }

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        in_queue[u] = false;

        for (const auto& edge : adj[u]) {
            int v = edge.to;
            double weight = edge.v - r * edge.p;

            if (dist[v] < dist[u] + weight) {
                dist[v] = dist[u] + weight;
                cnt[v] = cnt[u] + 1;

                if (cnt[v] >= n) { 
                    return true;
                }

                if (!in_queue[v]) {
                    q.push(v);
                    in_queue[v] = true;
                }
            }
        }
    }
    return false;
}

bool has_cycle() {
    vector<int> in_degree(n + 1, 0);
    for (int i = 1; i <= n; i++) {
        for (const auto& edge : adj[i]) {
            in_degree[edge.to]++;
        }
    }
    queue<int> q;
    for (int i = 1; i <= n; i++) {
        if (in_degree[i] == 0) q.push(i);
    }
    int count = 0;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        count++;
        for (const auto& edge : adj[u]) {
            if (--in_degree[edge.to] == 0) {
                q.push(edge.to);
            }
        }
    }
    return count != n;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    if (!(cin >> n >> m)) return 0;
    
    adj.resize(n + 1);

    vector<pair<int, int>> edges_info(m);
    vector<double> v_vals(m);
    for (int i = 0; i < m; i++) {
        cin >> edges_info[i].first >> edges_info[i].second >> v_vals[i];
    }

    vector<double> p_vals(m);
    for (int i = 0; i < m; i++) {
        cin >> p_vals[i];
    }

    for (int i = 0; i < m; i++) {
        Edge e;
        e.to = edges_info[i].second;
        e.v = v_vals[i];
        e.p = p_vals[i];
        adj[edges_info[i].first].push_back(e);
    }

    if (!has_cycle()) {
        cout << -1 << "\n";
        return 0;
    }

    double L = 0.0, R = 205.0; 
    while (R - L > 1e-8) {
        double mid = L + (R - L) / 2.0;
        if (check(mid)) {
            L = mid; 
        } else {
            R = mid; 
        }
    }

    cout.precision(1);
    cout << fixed << L << "\n";

    return 0;
}