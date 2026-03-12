#include <bits/stdc++.h>
using namespace std;

int N, M;
vector<pair<int, char>> items;

/*
注意到如果A几乎是很自由的，但是B是受到限制的。A可以进入两个机器，且可以直接从后面被拉过来，因此永远不会被卡住。B只能进入一个机器，并且只能从前面被拉到后面，因此没有那么自由。因此，我们至少需要B个数量的A来保证B不会被卡住。首先，如果二者数量相同，那么显然最优策略就是，如果遇到一个A，那么查看是否有B，如果不是，那就把A和最近的B调换位置，惩罚为i - j;如果遇到B，直接拿一个A就可以了。对于数量不同的A，B，我们首先贪心保证所有的B都被处理，然后处理A；这就是最优策略。
*/

bool check(int D) {
    
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    cin >> N >> M;
    
    int cntA = 0;
    int pos = 0;
    
    for (int i = 0; i < M; i++) {
        char c;
        int x;
        cin >> c >> x;
        for (int j = 0; j < x; j++) {
            items.push_back({pos, c});
            pos++;
        }
        if (c == 'A') cntA += x;
    }
    
    if (cntA < N) {
        cout << -1 << endl;
        return 0;
    }
    
    if (items.size() != 2 * N) {
        cout << -1 << endl;
        return 0;
    }
    
    int lo = 0, hi = 2 * N, ans = -1;
    
    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        if (check(mid)) {
            ans = mid;
            hi = mid - 1;
        } else {
            lo = mid + 1;
        }
    }
    
    cout << ans << endl;
    
    return 0;
}
