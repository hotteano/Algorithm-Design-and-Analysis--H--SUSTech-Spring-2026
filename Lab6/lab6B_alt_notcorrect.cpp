#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef __int128 i128;

const int MAXN = 200010;

int n, root;
ll C[MAXN];
vector<int> children[MAXN];
ll sz[MAXN], sum[MAXN];
ll ans = 0;
int curTime = 0;

void dfs1(int u) {
    sz[u] = 1;
    sum[u] = C[u];
    for (int v : children[u]) {
        dfs1(v);
        sz[u] += sz[v];
        sum[u] += sum[v];
    }
}

bool cmp(int a, int b) {
    i128 left = (i128)sum[a] * sz[b];
    i128 right = (i128)sum[b] * sz[a];
    return left > right;  
}

void dfs2(int u) {
    curTime++;
    ans += C[u] * curTime;
    sort(children[u].begin(), children[u].end(), cmp);
    for (int v : children[u]) dfs2(v);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n >> root;
    for (int i = 1; i <= n; i++) cin >> C[i];
    for (int i = 1; i < n; i++) {
        int x, y; cin >> x >> y;
        children[x].push_back(y);  
    }
    dfs1(root);
    dfs2(root);
    cout << ans << "\n";
    return 0;
}