#include <bits/stdc++.h>
using namespace std;

using ll = long long;

const ll INF = numeric_limits<ll>::max() / 4;

struct Edge {
    int to, rev;
    ll cap;
};

struct Dinic {
    int n;
    vector<vector<Edge>> g;
    vector<int> level, it;

    Dinic(int n) : n(n), g(n), level(n), it(n) {}

    void addEdge(int from, int to, ll cap) {
        Edge a{to, (int)g[to].size(), cap};
        Edge b{from, (int)g[from].size(), 0};
        g[from].push_back(a);
        g[to].push_back(b);
    }

    bool bfs(int s, int t) {
        fill(level.begin(), level.end(), -1);
        queue<int> q;
        level[s] = 0;
        q.push(s);

        while (!q.empty()) {
            int v = q.front();
            q.pop();

            for (const Edge &e : g[v]) {
                if (e.cap > 0 && level[e.to] == -1) {
                    level[e.to] = level[v] + 1;
                    q.push(e.to);
                }
            }
        }

        return level[t] != -1;
    }

    ll dfs(int v, int t, ll pushed) {
        if (v == t) {
            return pushed;
        }

        for (int &i = it[v]; i < (int)g[v].size(); i++) {
            Edge &e = g[v][i];
            if (e.cap <= 0 || level[e.to] != level[v] + 1) {
                continue;
            }

            ll got = dfs(e.to, t, min(pushed, e.cap));
            if (got == 0) {
                continue;
            }

            e.cap -= got;
            g[e.to][e.rev].cap += got;
            return got;
        }

        return 0;
    }

    ll maxflow(int s, int t) {
        ll flow = 0;

        while (bfs(s, t)) {
            fill(it.begin(), it.end(), 0);
            while (true) {
                ll pushed = dfs(s, t, INF);
                if (pushed == 0) {
                    break;
                }
                flow += pushed;
            }
        }

        return flow;
    }
};

int main() {
    cin.tie(nullptr)->sync_with_stdio(false);

    int n;
    if (!(cin >> n)) {
        return 0;
    }

    vector<int> a(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }

    vector<ll> x(n);
    for (int i = 0; i < n; i++) {
        cin >> x[i];
    }

    vector<int> dp(n, 1);
    int longest = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < i; j++) {
            if (a[j] <= a[i]) {
                dp[i] = max(dp[i], dp[j] + 1);
            }
        }
        longest = max(longest, dp[i]);
    }

    auto inNode = [](int i) {
        return 2 * i;
    };

    auto outNode = [](int i) {
        return 2 * i + 1;
    };

    int source = 2 * n;
    int sink = 2 * n + 1;
    Dinic dinic(2 * n + 2);

    for (int i = 0; i < n; i++) {
        dinic.addEdge(inNode(i), outNode(i), x[i]);

        if (dp[i] == 1) {
            dinic.addEdge(source, inNode(i), INF);
        }

        if (dp[i] == longest) {
            dinic.addEdge(outNode(i), sink, INF);
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (a[i] <= a[j] && dp[j] == dp[i] + 1) {
                dinic.addEdge(outNode(i), inNode(j), INF);
            }
        }
    }

    cout << longest << '\n';

    cout << dinic.maxflow(source, sink) << '\n';

    return 0;
}
