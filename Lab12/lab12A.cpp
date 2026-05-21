#include <bits/stdc++.h>
using namespace std;

using ll = long long;

struct Dinic {
    struct Edge {
        int to, rev;
        ll cap;
    };

    int n;
    vector<vector<Edge>> g;
    vector<int> level, it;

    Dinic(int n) : n(n), g(n), level(n), it(n) {}

    int addEdge(int from, int to, ll cap) {
        int idx = (int)g[from].size();
        Edge a{to, (int)g[to].size(), cap};
        Edge b{from, (int)g[from].size(), 0};
        g[from].push_back(a);
        g[to].push_back(b);
        return idx;
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
        const ll INF = numeric_limits<ll>::max() / 4;

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

    void disableEdge(int from, int idx) {
        Edge &e = g[from][idx];
        e.cap = 0;
        g[e.to][e.rev].cap = 0;
    }
};

int main() {
    cin.tie(nullptr)->sync_with_stdio(false);

    int n, m;
    cin >> n >> m;

    vector<ll> d(m);
    for (int i = 0; i < m; i++) {
        cin >> d[i];
    }

    int source = n + m;
    int sink = n + m + 1;
    int superSource = n + m + 2;
    int superSink = n + m + 3;
    int nodeCount = n + m + 4;

    Dinic dinic(nodeCount);
    vector<ll> need(nodeCount, 0);
    vector<pair<int, int>> auxiliaryEdges;

    auto dayNode = [&](int day) {
        return day;
    };

    auto workerNode = [&](int worker) {
        return n + worker;
    };

    auto addLowerEdge = [&](int from, int to, ll lower, ll upper) {
        dinic.addEdge(from, to, upper - lower);
        need[from] -= lower;
        need[to] += lower;
    };

    ll sumT = 0;
    for (int k = 0; k < n; k++) {
        int sk;
        ll tk;
        cin >> sk >> tk;
        sumT += tk;

        addLowerEdge(source, dayNode(k), 0, tk);

        for (int j = 0; j < sk; j++) {
            int worker;
            ll l, r;
            cin >> worker >> l >> r;

            addLowerEdge(dayNode(k), workerNode(worker), l, r);
        }
    }

    const ll inf = sumT;
    for (int i = 0; i < m; i++) {
        if (d[i] > inf) {
            cout << -1 << '\n';
            return 0;
        }
        addLowerEdge(workerNode(i), sink, d[i], inf);
    }

    int returnEdgeIdx = dinic.addEdge(sink, source, inf);

    ll demand = 0;
    for (int v = 0; v < nodeCount; v++) {
        if (need[v] > 0) {
            int idx = dinic.addEdge(superSource, v, need[v]);
            auxiliaryEdges.push_back({superSource, idx});
            demand += need[v];
        } else if (need[v] < 0) {
            int idx = dinic.addEdge(v, superSink, -need[v]);
            auxiliaryEdges.push_back({v, idx});
        }
    }

    ll feasibleFlow = dinic.maxflow(superSource, superSink);
    if (feasibleFlow != demand) {
        cout << -1 << '\n';
        return 0;
    }

    ll baseFlow = dinic.g[source][dinic.g[sink][returnEdgeIdx].rev].cap;
    dinic.disableEdge(sink, returnEdgeIdx);
    for (auto [from, idx] : auxiliaryEdges) {
        dinic.disableEdge(from, idx);
    }

    ll answer = baseFlow + dinic.maxflow(source, sink);
    cout << answer << '\n';

    return 0;
}
