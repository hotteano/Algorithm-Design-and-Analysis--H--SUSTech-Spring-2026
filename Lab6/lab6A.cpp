#include <bits/stdc++.h>
using namespace std;

struct Point {
    long long x, y, z;
};

double dist(const Point& a, const Point& b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    double dz = a.z - b.z;
    return sqrt(dx * dx + dy * dy + dz * dz);
}

double brute_force(vector<Point>& p, int l, int r) {
    double res = 1e300;
    for (int i = l; i <= r; i++) {
        for (int j = i + 1; j <= r; j++) {
            res = min(res, dist(p[i], p[j]));
        }
    }
    return res;
}

double closest_pair(vector<Point>& p, int l, int r) {
    if (r - l <= 30) {
        return brute_force(p, l, r);
    }

    long long min_x = p[l].x, max_x = p[l].x;
    long long min_y = p[l].y, max_y = p[l].y;
    long long min_z = p[l].z, max_z = p[l].z;
    for (int i = l + 1; i <= r; i++) {
        if (p[i].x < min_x) min_x = p[i].x; else if (p[i].x > max_x) max_x = p[i].x;
        if (p[i].y < min_y) min_y = p[i].y; else if (p[i].y > max_y) max_y = p[i].y;
        if (p[i].z < min_z) min_z = p[i].z; else if (p[i].z > max_z) max_z = p[i].z;
    }
    
    long long dx = max_x - min_x;
    long long dy = max_y - min_y;
    long long dz = max_z - min_z;
    
    int dim = 0; // 0=x, 1=y, 2=z
    if (dy >= dx && dy >= dz) dim = 1;
    else if (dz >= dx && dz >= dy) dim = 2;

    auto cmp = [dim](const Point& a, const Point& b) {
        if (dim == 0) return a.x < b.x;
        if (dim == 1) return a.y < b.y;
        return a.z < b.z;
    };

    int mid = (l + r) >> 1;
    nth_element(p.begin() + l, p.begin() + mid, p.begin() + r + 1, cmp);

    long long mid_val = (dim == 0) ? p[mid].x : ((dim == 1) ? p[mid].y : p[mid].z);

    double d = min(closest_pair(p, l, mid), closest_pair(p, mid + 1, r));

    vector<Point> strip;
    strip.reserve(r - l + 1);
    for (int i = l; i <= r; i++) {
        long long val = (dim == 0) ? p[i].x : ((dim == 1) ? p[i].y : p[i].z);
        if (abs(val - mid_val) < d) {
            strip.push_back(p[i]);
        }
    }


    int sort_dim = (dim + 1) % 3;
    auto cmp2 = [sort_dim](const Point& a, const Point& b) {
        if (sort_dim == 0) return a.x < b.x;
        if (sort_dim == 1) return a.y < b.y;
        return a.z < b.z;
    };
    sort(strip.begin(), strip.end(), cmp2);

    for (size_t i = 0; i < strip.size(); i++) {
        for (size_t j = i + 1; j < strip.size(); j++) {
            long long val_i = (sort_dim == 0) ? strip[i].x : ((sort_dim == 1) ? strip[i].y : strip[i].z);
            long long val_j = (sort_dim == 0) ? strip[j].x : ((sort_dim == 1) ? strip[j].y : strip[j].z);
            if (val_j - val_i >= d) break;
            d = min(d, dist(strip[i], strip[j]));
        }
    }

    return d;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int T;
    if (!(cin >> T)) return 0;
    cout << fixed << setprecision(2);
    
    while (T--) {
        int n;
        cin >> n;
        vector<Point> p(n);
        for (int i = 0; i < n; i++) {
            cin >> p[i].x >> p[i].y >> p[i].z;
        }

        if (n <= 1) {
            cout << 0.00 << "\n";
            continue;
        }

        double ans = closest_pair(p, 0, n - 1);
        cout << ans << "\n";
    }
    
    return 0;
}