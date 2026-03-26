#include <bits/stdc++.h>
using namespace std;

struct Edge {
    int u;
    int v;
    long long w;
};

static const long long INF = (1LL << 60);

long long directed_mst(int n, int root, const vector<Edge> &edges_input, vector<int> &parent) {
    if (n <= 0 || root < 0 || root >= n) {
        parent.clear();
        return -1;
    }

    function<long long(int, int, const vector<Edge>&, vector<int>&)> solve =
        [&](int n_now, int rt_now, const vector<Edge> &cur_edges, vector<int> &par) -> long long {
        if (n_now <= 0 || rt_now < 0 || rt_now >= n_now) {
            return -1;
        }

        vector<long long> in(n_now, INF);
        vector<int> pre_edge(n_now, -1);

        for (int i = 0; i < static_cast<int>(cur_edges.size()); i++) {
            const auto &e = cur_edges[i];
            if (e.u < 0 || e.u >= n_now || e.v < 0 || e.v >= n_now) {
                continue;
            }
            if (e.u != e.v && e.w < in[e.v]) {
                in[e.v] = e.w;
                pre_edge[e.v] = i;
            }
        }

        in[rt_now] = 0;
        for (int i = 0; i < n_now; i++) {
            if (in[i] == INF) {
                return -1;
            }
        }

        vector<int> id(n_now, -1), vis(n_now, -1);
        int cnt = 0;
        long long add_cost = 0;
        for (int i = 0; i < n_now; i++) {
            add_cost += in[i];
        }

        for (int i = 0; i < n_now; i++) {
            int v = i;
            while (vis[v] != i && id[v] == -1 && v != rt_now) {
                vis[v] = i;
                v = cur_edges[pre_edge[v]].u;
            }
            if (v != rt_now && id[v] == -1) {
                for (int u = cur_edges[pre_edge[v]].u; u != v; u = cur_edges[pre_edge[u]].u) {
                    id[u] = cnt;
                }
                id[v] = cnt++;
            }
        }

        if (cnt == 0) {
            par.assign(n_now, -1);
            for (int i = 0; i < n_now; i++) {
                if (i == rt_now) {
                    continue;
                }
                par[i] = cur_edges[pre_edge[i]].u;
            }
            return add_cost;
        }

        for (int i = 0; i < n_now; i++) {
            if (id[i] == -1) {
                id[i] = cnt++;
            }
        }

        vector<vector<int>> comp(cnt);
        for (int i = 0; i < n_now; i++) {
            comp[id[i]].push_back(i);
        }

        vector<Edge> new_edges;
        new_edges.reserve(cur_edges.size());
        for (const auto &e : cur_edges) {
            if (e.u < 0 || e.u >= n_now || e.v < 0 || e.v >= n_now) {
                continue;
            }
            int u = id[e.u], v = id[e.v];
            if (u != v) {
                new_edges.push_back({u, v, e.w - in[e.v]});
            }
        }

        vector<int> sub_par;
        long long sub_cost = solve(cnt, id[rt_now], new_edges, sub_par);
        if (sub_cost == -1) {
            return -1;
        }

        par.assign(n_now, -1);

        auto choose_enter_edge = [&](int parent_cid, int cid) {
            int enter_u = -1, enter_v = -1;
            long long best = INF;
            for (const auto &e : cur_edges) {
                if (e.u < 0 || e.u >= n_now || e.v < 0 || e.v >= n_now) {
                    continue;
                }
                if (id[e.u] == parent_cid && id[e.v] == cid) {
                    long long adjusted = e.w - in[e.v];
                    if (adjusted < best) {
                        best = adjusted;
                        enter_u = e.u;
                        enter_v = e.v;
                    }
                }
            }
            return pair<int, int>{enter_u, enter_v};
        };

        for (int cid = 0; cid < cnt; cid++) {
            int parent_cid = sub_par[cid];
            if (static_cast<int>(comp[cid].size()) == 1) {
                int node = comp[cid][0];
                if (node == rt_now) {
                    par[node] = -1;
                } else if (parent_cid == -1) {
                    par[node] = cur_edges[pre_edge[node]].u;
                } else {
                    auto [enter_u, enter_v] = choose_enter_edge(parent_cid, cid);
                    if (enter_v == node) {
                        par[node] = enter_u;
                    } else {
                        par[node] = cur_edges[pre_edge[node]].u;
                    }
                }
                continue;
            }

            int enter_u = -1, enter_v = -1;
            if (parent_cid != -1) {
                tie(enter_u, enter_v) = choose_enter_edge(parent_cid, cid);
            }

            for (int node : comp[cid]) {
                if (node == enter_v) {
                    par[node] = enter_u;
                } else {
                    par[node] = cur_edges[pre_edge[node]].u;
                }
            }
        }

        par[rt_now] = -1;
        return add_cost + sub_cost;
    };

    return solve(n, root, edges_input, parent);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    while (cin >> n >> m) {
        if (n <= 0 || m < 0) {
            cout << "impossible\n\n";
            for (int i = 0; i < max(0, m); ++i) {
                int u, v, w;
                cin >> u >> v >> w;
            }
            continue;
        }

        vector<Edge> edges;
        edges.reserve(m + n);

        long long sum = 0;
        bool has_invalid_endpoint = false;
        for (int i = 0; i < m; ++i) {
            int u, v, w;
            cin >> u >> v >> w;
            if (u < 0 || u >= n || v < 0 || v >= n) {
                has_invalid_endpoint = true;
                continue;
            }
            edges.push_back({u, v, w});
            sum += w;
        }

        if (has_invalid_endpoint) {
            cout << "impossible\n\n";
            continue;
        }

        long long super_weight = sum + 1;
        for (int i = 0; i < n; ++i) {
            edges.push_back({n, i, super_weight});
        }

        vector<int> parent;
        long long mst_cost = directed_mst(n + 1, n, edges, parent);

        if (mst_cost == -1) {
            cout << "impossible\n\n";
            continue;
        }

        vector<int> roots;
        for (int i = 0; i < n; ++i) {
            if (parent[i] == n) {
                roots.push_back(i);
            }
        }

        if (roots.size() != 1 || mst_cost >= 2 * super_weight) {
            cout << "impossible\n\n";
            continue;
        }

        int manager = roots[0];
        cout << (mst_cost - super_weight) << ' ' << manager << "\n";

        for (int i = 0; i < n; ++i) {
            if (i == manager) {
                cout << i << "\n";
            } else {
                cout << i << ' ' << parent[i] << "\n";
            }
        }
        cout << "\n";
    }

    return 0;
}
