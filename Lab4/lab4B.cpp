#include <iostream>
#include <vector>
#include <string>
using namespace std;

// 目标棋盘状态，-1表示空格
const int target[5][5] = {
    {1, 1, 1, 1, 1},
    {0, 1, 1, 1, 1},
    {0, 0, -1, 1, 1},
    {0, 0, 0, 0, 1},
    {0, 0, 0, 0, 0}
};

// 马的8个移动方向
int dx[] = {-2, -2, -1, -1, 1, 1, 2, 2};
int dy[] = {-1, 1, -2, 2, -2, 2, -1, 1};

// 启发式函数：计算当前状态与目标状态有几个位置不一致（不包含空格）
int evaluate(const vector<vector<int>>& board) {
    int diff = 0;
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (board[i][j] != -1 && board[i][j] != target[i][j]) {
                diff++;
            }
        }
    }
    return diff;
}

// 深度优先搜索（附带IDA*剪枝）
bool dfs(vector<vector<int>>& board, int x, int y, int g, int max_depth) {
    int h = evaluate(board);
    // 重合，搜索成功
    if (h == 0) return true;
    // 剪枝：已走步数 + 理想最少还需要走的步数 > 限制最大步数
    if (g + h > max_depth) return false;

    // 遍历8个方向
    for (int i = 0; i < 8; ++i) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        if (nx >= 0 && nx < 5 && ny >= 0 && ny < 5) {
            swap(board[x][y], board[nx][ny]); // 走马
            if (dfs(board, nx, ny, g + 1, max_depth)) return true;
            swap(board[x][y], board[nx][ny]); // 回溯
        }
    }
    return false;
}

void solve(){
    vector<vector<int>> board(5, vector<int>(5)); 
    int start_x = -1, start_y = -1;
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
            char m;
            cin >> m;
            if(m == '*') {
                board[i][j] = -1;
                start_x = i;
                start_y = j;
            }
            else {
                board[i][j] = m - '0';
            }
        }
    }

    // 迭代加深，从0步开始枚举到20步
    for (int max_depth = 0; max_depth <= 20; ++max_depth) {
        if (dfs(board, start_x, start_y, 0, max_depth)) {
            cout << max_depth << "\n";
            return;
        }
    }
    // 超过20步都不可达
    cout << -1 << "\n";
}

int main(){
    int T;
    if(cin >> T) {
        while(T--){
            solve();
        }
    }
    return 0;
}