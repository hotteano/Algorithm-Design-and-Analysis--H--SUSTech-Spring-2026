#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

const int MAXN = 200010;

int n, root;
ll C[MAXN];                    // 每个城市的成本系数
vector<int> children[MAXN];     // 邻接表存储树结构（父指向子）
ll sz[MAXN];                    // 子树节点数量
ll sum[MAXN];                   // 子树中所有 C[i] 之和
ll ans = 0;                     // 最终答案
int curTime = 0;                // 当前时间戳（全局）

// 第一遍 DFS：计算每棵子树的大小 sz 和权重和 sum
void dfs1(int u) {
    sz[u] = 1;
    sum[u] = C[u];
    for (int v : children[u]) {
        dfs1(v);
        sz[u] += sz[v];
        sum[u] += sum[v];
    }
}

// 第二遍 DFS：按平均成本(sum/sz)降序访问子节点，计算答案
void dfs2(int u) {
    curTime++;                  // 访问当前节点，时间+1
    ans += C[u] * curTime;      // 累加成本：C_u * T_u
    
    // 按子树平均成本 (sum/sz) 从大到小排序子节点
    // 使用交叉相乘避免浮点数精度问题
    sort(children[u].begin(), children[u].end(), [&](int a, int b) {
        return sum[a] * sz[b] > sum[b] * sz[a];
    });
    
    // 按排序后的顺序递归访问子节点
    for (int v : children[u]) {
        dfs2(v);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    // 读入 n 和根节点 F
    cin >> n >> root;
    
    // 读入成本系数（1-indexed）
    for (int i = 1; i <= n; i++) {
        cin >> C[i];
    }
    
    // 读入 n-1 条边，建立有根树（x 是 y 的父节点）
    for (int i = 1; i < n; i++) {
        int x, y;
        cin >> x >> y;
        children[x].push_back(y);
    }
    
    // 第一遍 DFS：自底向上计算子树信息
    dfs1(root);
    
    // 第二遍 DFS：自顶向下按最优顺序访问并计算答案
    dfs2(root);
    
    cout << ans << "\n";
    
    return 0;
}