#include <bits/stdc++.h>
using namespace std;
const int MOD = 12345678;

static int dp[2][105][105][105];

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, m, k;
    if (!(cin >> n >> m >> k)) return 0;
    
    int cur = 0;
    dp[cur][0][0][0] = 1;  // i=0, j=0
    
    for (int i = 0; i <= n; i++) {
        int pre = cur;
        cur ^= 1;
        
        // 清空当前层
        for (int j = 0; j <= m; j++)
            for (int d = 0; d <= k; d++)
                for (int r = 0; r <= k; r++)
                    dp[cur][j][d][r] = 0;
        
        for (int j = 0; j <= m; j++) {
            if (i == 0 && j == 0) {
                dp[cur][0][0][0] = 1;
                continue;
            }
            
            // 上一个放男生：从 pre 层 (i-1, j) 转移
            if (i > 0) {
                for (int d = 0; d <= k; d++) {
                    for (int r = 0; r <= k; r++) {
                        int val = dp[pre][j][d][r];
                        if (!val) continue;
                        if (d < r) {
                            int &ref = dp[cur][j][d + 1][r];
                            ref = (ref + val) % MOD;
                        } else if (d == r) {
                            if (r + 1 <= k) {
                                int &ref = dp[cur][j][d + 1][r + 1];
                                ref = (ref + val) % MOD;
                            }
                        }
                    }
                }
            }
            
            // 上一个放女生：从 cur 层 (i, j-1) 转移
            if (j > 0) {
                for (int d = 0; d <= k; d++) {
                    for (int r = 0; r <= k; r++) {
                        int val = dp[cur][j - 1][d][r];
                        if (!val) continue;
                        if (d > 0) {
                            int &ref = dp[cur][j][d - 1][r];
                            ref = (ref + val) % MOD;
                        } else { // d == 0
                            if (r + 1 <= k) {
                                int &ref = dp[cur][j][0][r + 1];
                                ref = (ref + val) % MOD;
                            }
                        }
                    }
                }
            }
        }
    }
    
    long long ans = 0;
    for (int d = 0; d <= k; d++)
        for (int r = 0; r <= k; r++)
            ans = (ans + dp[cur][m][d][r]) % MOD;
    
    cout << ans << '\n';
    return 0;
}
