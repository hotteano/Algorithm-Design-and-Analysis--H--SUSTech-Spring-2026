#include <bits/stdc++.h>
using namespace std;

using ull = unsigned long long;

struct MaxCliqueAnnealing {
    int n, blocks;
    vector<vector<ull>> adj;
    vector<int> degree;
    mt19937 rng;
    ull lastMask;

    MaxCliqueAnnealing(int n) : n(n), blocks((n + 63) / 64), adj(n, vector<ull>(blocks, 0)),
                               degree(n, 0), rng((uint32_t)chrono::steady_clock::now().time_since_epoch().count()) {
        int rest = n & 63;
        lastMask = rest == 0 ? ~0ULL : ((1ULL << rest) - 1);
    }

    void addEdge(int u, int v) {
        if (u == v) {
            return;
        }
        if (((adj[u][v >> 6] >> (v & 63)) & 1ULL) == 0) {
            adj[u][v >> 6] |= 1ULL << (v & 63);
            adj[v][u >> 6] |= 1ULL << (u & 63);
            degree[u]++;
            degree[v]++;
        }
    }

    bool hasEdge(int u, int v) const {
        return (adj[u][v >> 6] >> (v & 63)) & 1ULL;
    }

    void setAll(vector<ull> &bits) {
        fill(bits.begin(), bits.end(), ~0ULL);
        bits.back() &= lastMask;
    }

    void resetBit(vector<ull> &bits, int v) {
        bits[v >> 6] &= ~(1ULL << (v & 63));
    }

    int popcount(const vector<ull> &bits) const {
        int cnt = 0;
        for (ull x : bits) {
            cnt += __builtin_popcountll(x);
        }
        return cnt;
    }

    vector<int> listBits(const vector<ull> &bits) const {
        vector<int> result;
        for (int b = 0; b < blocks; b++) {
            ull x = bits[b];
            while (x) {
                int low = __builtin_ctzll(x);
                int v = b * 64 + low;
                if (v < n) {
                    result.push_back(v);
                }
                x &= x - 1;
            }
        }
        return result;
    }

    vector<ull> buildCandidates(const vector<int> &clique) {
        vector<ull> cand(blocks);
        setAll(cand);

        for (int v : clique) {
            for (int b = 0; b < blocks; b++) {
                cand[b] &= adj[v][b];
            }
        }

        for (int v : clique) {
            resetBit(cand, v);
        }

        return cand;
    }

    int chooseCandidate(const vector<ull> &cand) {
        vector<int> vertices = listBits(cand);
        if (vertices.empty()) {
            return -1;
        }

        int samples = min<int>(12, vertices.size());
        uniform_int_distribution<int> dist(0, (int)vertices.size() - 1);
        int best = vertices[dist(rng)];

        for (int i = 1; i < samples; i++) {
            int v = vertices[dist(rng)];
            if (degree[v] > degree[best]) {
                best = v;
            }
        }

        return best;
    }

    void expand(vector<int> &clique) {
        vector<ull> cand = buildCandidates(clique);

        while (popcount(cand) > 0) {
            int v = chooseCandidate(cand);
            if (v == -1) {
                break;
            }

            clique.push_back(v);
            for (int b = 0; b < blocks; b++) {
                cand[b] &= adj[v][b];
            }
            resetBit(cand, v);
        }
    }

    vector<int> greedyRandomClique() {
        vector<int> clique;
        expand(clique);
        return clique;
    }

    vector<int> perturb(vector<int> clique, double temperature) {
        if (clique.empty()) {
            expand(clique);
            return clique;
        }

        shuffle(clique.begin(), clique.end(), rng);
        int maxRemove = max(1, (int)sqrt((double)clique.size()));
        int removeCount = uniform_int_distribution<int>(1, maxRemove)(rng);

        if (temperature > 1.0 && (int)clique.size() > maxRemove) {
            removeCount = min<int>(clique.size() - 1, removeCount + 1);
        }

        removeCount = min<int>(removeCount, clique.size());
        clique.resize((int)clique.size() - removeCount);
        expand(clique);
        return clique;
    }

    int solve() {
        auto start = chrono::steady_clock::now();
        const double timeLimit = 0.85;

        vector<int> current = greedyRandomClique();
        vector<int> best = current;

        double temperature = 3.0;
        uniform_real_distribution<double> realDist(0.0, 1.0);

        while (true) {
            double elapsed = chrono::duration<double>(chrono::steady_clock::now() - start).count();
            if (elapsed >= timeLimit) {
                break;
            }

            vector<int> next;
            if (realDist(rng) < 0.15) {
                next = greedyRandomClique();
            } else {
                next = perturb(current, temperature);
            }

            int delta = (int)next.size() - (int)current.size();
            if (delta >= 0 || realDist(rng) < exp(delta / temperature)) {
                current = move(next);
            }

            if (current.size() > best.size()) {
                best = current;
            }

            temperature *= 0.9995;
            if (temperature < 0.05) {
                temperature = 0.05;
            }
        }

        return (int)best.size();
    }
};

int main() {
    cin.tie(nullptr)->sync_with_stdio(false);

    int n, m;
    cin >> n >> m;

    MaxCliqueAnnealing solver(n);
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        --u;
        --v;
        solver.addEdge(u, v);
    }

    cout << solver.solve() << '\n';

    return 0;
}
