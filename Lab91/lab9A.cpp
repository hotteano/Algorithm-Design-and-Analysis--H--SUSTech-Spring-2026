#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdint>

using namespace std;

const long long INF = 1e18;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s, t;
    if (!(cin >> s >> t)) return 0;

    int m = s.length();
    int n = t.length();

    long long cost_delete, cost_replace, cost_copy, cost_insert, cost_swap;
    cin >> cost_delete >> cost_replace >> cost_copy >> cost_insert >> cost_swap;

    vector<vector<long long>> dp(3, vector<long long>(n + 1, INF));
    
    vector<int8_t> choice((m + 1) * (n + 1), -1);

    dp[0][0] = 0;

    long long min_total_cost = INF;
    int best_end_i = -1;

    for (int i = 0; i <= m; ++i) {
        int r = i % 3;
        int r1 = (i + 2) % 3; // i - 1
        int r2 = (i + 1) % 3; // i - 2

        if (i > 0) {
            fill(dp[r].begin(), dp[r].end(), INF);
        }

        for (int j = 0; j <= n; ++j) {
            if (i == 0 && j == 0) continue;

            int idx = i * (n + 1) + j;

            // 0: delete
            if (i > 0 && dp[r1][j] + cost_delete < dp[r][j]) {
                dp[r][j] = dp[r1][j] + cost_delete;
                choice[idx] = 0;
            }
            // 1: insert
            if (j > 0 && dp[r][j - 1] + cost_insert < dp[r][j]) {
                dp[r][j] = dp[r][j - 1] + cost_insert;
                choice[idx] = 1;
            }
            // 2: replace
            if (i > 0 && j > 0 && dp[r1][j - 1] + cost_replace < dp[r][j]) {
                dp[r][j] = dp[r1][j - 1] + cost_replace;
                choice[idx] = 2;
            }
            // 3: copy
            if (i > 0 && j > 0 && s[i - 1] == t[j - 1] && dp[r1][j - 1] + cost_copy < dp[r][j]) {
                dp[r][j] = dp[r1][j - 1] + cost_copy;
                choice[idx] = 3;
            }
            // 4: swap
            if (i >= 2 && j >= 2 && s[i - 1] == t[j - 2] && s[i - 2] == t[j - 1] && dp[r2][j - 2] + cost_swap < dp[r][j]) {
                dp[r][j] = dp[r2][j - 2] + cost_swap;
                choice[idx] = 4;
            }
        }

        long long current_cost = dp[r][n];
        if (current_cost != INF) {
            if (i < m) {
                current_cost += 1LL * (m - i) * cost_delete - 1;
            }
            if (current_cost < min_total_cost) {
                min_total_cost = current_cost;
                best_end_i = i;
            }
        }
    }

    cout << min_total_cost << "\n";

    vector<string> ops;
    if (best_end_i < m) {
        ops.push_back("finish");
    }

    int curr_i = best_end_i;
    int curr_j = n;

    while (curr_i > 0 || curr_j > 0) {
        int c = choice[curr_i * (n + 1) + curr_j];
        if (c == 0) {
            ops.push_back("delete");
            curr_i--;
        } else if (c == 1) {
            ops.push_back("insert");
            curr_j--;
        } else if (c == 2) {
            ops.push_back("replace");
            curr_i--;
            curr_j--;
        } else if (c == 3) {
            ops.push_back("copy");
            curr_i--;
            curr_j--;
        } else if (c == 4) {
            ops.push_back("swap");
            curr_i -= 2;
            curr_j -= 2;
        }
    }

    reverse(ops.begin(), ops.end());
    for (const string& op : ops) {
        cout << op << "\n";
    }

    return 0;
}