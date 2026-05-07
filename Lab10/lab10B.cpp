#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const long long INF = 1e18;

int n, m;
vector<int> w, p;
vector<long long> edge_d;
vector<vector<int>> adj;
vector<long long> dist_to_root;
vector<int> depth_idx;
vector<int> sizes;
vector<vector<vector<long long>>> dp;
int max_m;

void get_depth(int u, long long current_dist, int current_depth) {
    dist_to_root[u] = current_dist;
    depth_idx[u] = current_depth;
    for (int c : adj[u]) {
        get_depth(c, current_dist + edge_d[c], current_depth + 1);
    }
}

void dfs(int u, vector<int>& anc) {
    int d_u = depth_idx[u];
    dp[u].assign(d_u + 1, vector<long long>(max_m + 1, INF));
    
    int sz = (u == 0) ? 0 : 1;
    
    if (u == 0) {
        dp[0][0][0] = 0; 
    } else {
        dp[u][d_u][1] = 0; // u is designated center
        for (int d = 0; d < d_u; ++d) {
            int v = anc[d];
            dp[u][d][0] = (long long)w[u] * (dist_to_root[u] - dist_to_root[v]);
        }
    }
    
    anc.push_back(u);
    
    for (int c : adj[u]) {
        dfs(c, anc);
        
        if (u == 0) {
            vector<long long> next_dp(max_m + 1, INF);
            int max_j = min(max_m, sz);
            int max_k = min(max_m, sizes[c]);
            for (int j = 0; j <= max_j; ++j) {
                if (dp[0][0][j] == INF) continue;
                for (int k = 0; k <= max_k; ++k) {
                    if (j + k > max_m) break;
                    if (dp[c][0][k] == INF) continue;
                    next_dp[j + k] = min(next_dp[j + k], dp[0][0][j] + dp[c][0][k]);
                }
            }
            for (int j = 0; j <= max_m; ++j) {
                dp[0][0][j] = next_dp[j];
            }
        } else {
            for (int d = 0; d <= d_u; ++d) {
                vector<long long> next_dp(max_m + 1, INF);
                int max_j = min(max_m, sz);
                int max_k = min(max_m, sizes[c]);
                for (int j = 0; j <= max_j; ++j) {
                    if (dp[u][d][j] == INF) continue;
                    for (int k = 0; k <= max_k; ++k) {
                        if (j + k > max_m) break;
                        if (dp[c][d][k] == INF) continue;
                        next_dp[j + k] = min(next_dp[j + k], dp[u][d][j] + dp[c][d][k]);
                    }
                }
                for (int j = 0; j <= max_m; ++j) {
                    dp[u][d][j] = next_dp[j];
                }
            }
        }
        sz += sizes[c];
        
        // 释放已合并子节点的内存以防深度和M过大时爆内存
        dp[c].clear();
        dp[c].shrink_to_fit();
    }
    sizes[u] = sz;
    anc.pop_back();
    
    if (u != 0) {
        for (int d = 0; d < d_u; ++d) {
            for (int j = 0; j <= max_m; ++j) {
                dp[u][d][j] = min(dp[u][d][j], dp[u][d_u][j]);
            }
        }
    }
}

void solve() {
    if (!(cin >> n >> m)) return;
    w.assign(n + 1, 0);
    p.assign(n + 1, 0);
    edge_d.assign(n + 1, 0);
    adj.assign(n + 1, vector<int>());
    dist_to_root.assign(n + 1, 0);
    depth_idx.assign(n + 1, 0);
    sizes.assign(n + 1, 0);
    dp.resize(n + 1);

    for (int i = 1; i <= n; ++i) {
        cin >> w[i] >> p[i] >> edge_d[i];
        adj[p[i]].push_back(i);
    }

    get_depth(0, 0, 0);

    max_m = min(n, m);
    
    vector<int> anc;
    dfs(0, anc);

    cout << dp[0][0][max_m] << "\n";
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}