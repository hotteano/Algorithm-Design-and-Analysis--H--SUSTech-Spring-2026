#include <bits/stdc++.h>
using namespace std;

const int N = 1e5 + 5;

int n, C;
int c[N], v[N], q[N];
vector<int> dp(N, 0);

void solve(){
    // 预处理weight 
    vector<pair<int, int>> items;

    for (int i = 0; i < n; ++i) {
        int cnt = q[i];
        int k = 1;
        while (cnt > 0) {
            int take = min(k, cnt);
            items.push_back({c[i] * take, v[i] * take});
            cnt -= take;
            k <<= 1;  // 倍增
        }
    }


    for(auto &[w, val]: items){
        for(int j = C; j >= w; j--){
            dp[j] = max(dp[j], dp[j - w] + val);
        }
    }

    cout << dp[C] << endl;
}

int main(){
    cin >> n >> C;
    for(int i = 0; i < n; i++) cin >> c[i] >> v[i] >> q[i];
    solve();
    return 0;
}