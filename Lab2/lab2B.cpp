#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <cstring>
using namespace std;

const int MAXN = 55;

int n, k;
int weight[MAXN][MAXN];      
int rank_col[MAXN][MAXN];  
int proposal[MAXN];         
int match_of[2 * MAXN];      
vector<int> row_pref[MAXN]; 

void stable_matching(int op) {
    memset(proposal, 0, sizeof(proposal));
    memset(match_of, 0, sizeof(match_of));

    queue<int> free_rows;
    for (int i = 1; i <= n; i++) {
        free_rows.push(i);
        row_pref[i].clear();
        for (int j = 1; j <= n; j++) {
            row_pref[i].push_back(j);
        }
        sort(row_pref[i].begin(), row_pref[i].end(), [&](int x, int y) {
            return (weight[i][x] < weight[i][y]) ^ op;
        });
    }

    for (int j = 1; j <= n; j++) {
        vector<int> order;
        for (int i = 1; i <= n; i++) {
            order.push_back(i);
        }
        sort(order.begin(), order.end(), [&](int x, int y) {
            return (weight[x][j] > weight[y][j]) ^ op;
        });
        for (int r = 0; r < n; r++) {
            rank_col[j][order[r]] = r + 1;
        }
    }

    while (!free_rows.empty()) {
        int row = free_rows.front();
        free_rows.pop();
        while (match_of[row] == 0) {
            int col = row_pref[row][proposal[row]++];
            int col_node = col + n;
            if (match_of[col_node] == 0 || rank_col[col][match_of[col_node]] > rank_col[col][row]) {
                int old_row = match_of[col_node];
                if (old_row != 0) {
                    match_of[old_row] = 0;
                    free_rows.push(old_row);
                }
                match_of[col_node] = row;
                match_of[row] = col_node;
            }
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> k;
    while (k--) {
        cin >> n;
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                cin >> weight[i][j];
            }
        }

        cout << "B" << endl;

        char strategy;
        int start;
        cin >> strategy >> start;

        if (strategy == 'D') {
            for (int i = 1; i <= n; i++) {
                for (int j = 1; j <= n; j++) {
                    weight[i][j] = -weight[i][j];
                }
            }
        }

        stable_matching(start > n ? 1 : 0);

        cout << match_of[start] << endl;

        int x;
        cin >> x;
        while (x != -1 && x != -2) {
            cout << match_of[x] << endl;
            cin >> x;
        }
    }
    return 0;
}
