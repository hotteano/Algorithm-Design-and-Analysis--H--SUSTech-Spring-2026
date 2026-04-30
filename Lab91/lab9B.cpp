#include <bits/stdc++.h>
using namespace std;

const int N = 105;
const int INF = 0x3f3f3f3f;

int dp[N][N][2]; 

int main(){
    string s;
    if (!(cin >> s)) return 0; 
    
    int n = s.length();
    
    memset(dp, 0x3f, sizeof(dp));

    // 区间dp, dp[i][j][1]表示区间不新加B的最小长度，dp[i][j][0]表示区间的最小压缩长度
    // 注意到，如果区间里面有B，那么B左边的和B右边的部分互不相关，我们枚举最后一个B的位置即可，右侧区间用dp[i][j][1]来表示，因为它不包含B了
    // 如果区间里面没有B，那么如果它可以被压缩，它只能是一个偶数长度的字符串，并且前半部分和后半部分相同
    
    for (int len = 1; len <= n; len++) {
        for (int i = 0; i <= n - len; i++) {
            int j = i + len - 1; 
            
            if (len == 1) {
                dp[i][j][0] = dp[i][j][1] = 1;
                continue;
            }

            dp[i][j][1] = dp[i][j-1][1] + 1;
            
            if (len % 2 == 0) {
                int mid = i + len / 2 - 1;
                string left_half = s.substr(i, len / 2);
                string right_half = s.substr(mid + 1, len / 2);
                if (left_half == right_half) {
                    dp[i][j][1] = min(dp[i][j][1], dp[i][mid][1] + 1);
                }
            }
            
            dp[i][j][0] = dp[i][j][1]; 
            
            for (int k = i; k < j; k++) {
                dp[i][j][0] = min(dp[i][j][0], dp[i][k][0] + dp[k+1][j][1] + 1);
            }
        }
    }
    
    cout << dp[0][n-1][0] << endl;
    return 0;
}