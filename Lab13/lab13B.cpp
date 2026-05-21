#include <bits/stdc++.h>
using namespace std;

using ull = unsigned long long;

struct HyperLogLog {
    static constexpr int P = 8;
    static constexpr int M = 1 << P;
    static constexpr double ALPHA = 0.7213 / (1.0 + 1.079 / M);

    array<unsigned char, M> reg{};

    static ull splitmix64(ull x) {
        x += 0x9e3779b97f4a7c15ULL;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
        x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
        return x ^ (x >> 31);
    }

    void add(ull value) {
        ull h = splitmix64(value);
        int bucket = h & (M - 1);
        ull rest = h >> P;

        int rho;
        if (rest == 0) {
            rho = 64 - P + 1;
        } else {
            rho = __builtin_ctzll(rest) + 1;
        }

        reg[bucket] = max<unsigned char>(reg[bucket], (unsigned char)rho);
    }

    static HyperLogLog mergeSketch(const HyperLogLog &a, const HyperLogLog &b) {
        HyperLogLog result;
        for (int i = 0; i < M; i++) {
            result.reg[i] = max(a.reg[i], b.reg[i]);
        }
        return result;
    }

    double estimate() const {
        double sum = 0.0;
        int zero = 0;

        for (int i = 0; i < M; i++) {
            sum += ldexp(1.0, -reg[i]);
            if (reg[i] == 0) {
                zero++;
            }
        }

        double raw = ALPHA * M * M / sum;

        if (raw <= 2.5 * M && zero > 0) {
            return M * log((double)M / zero);
        }

        return raw;
    }
};

int main() {
    cin.tie(nullptr)->sync_with_stdio(false);

    int n, m;
    cin >> n >> m;

    vector<HyperLogLog> hll(n + 1);
    for (int i = 1; i <= m; i++) {
        hll[i].add(i);
    }

    for (int i = m + 1; i <= n; i++) {
        int a, b;
        cin >> a >> b;

        hll[i] = HyperLogLog::mergeSketch(hll[a], hll[b]);
        cout << llround(hll[i].estimate()) << '\n';
    }

    return 0;
}
