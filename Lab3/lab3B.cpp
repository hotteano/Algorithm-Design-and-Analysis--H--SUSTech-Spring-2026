#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll N;
    int M;
    cin >> N >> M;

    // 输入是字符串 S_i 重复 K_i 次
    // 我们需要把它转换成 (type, count, start_pos) 的块列表
    // 由于 K_i 可达 10^18，不能展开循环
    
    // 先把所有输入读入，转成连续块的形式
    // 块: (type, count)，然后计算前缀和得到位置
    
    vector<pair<char, ll>> blocks; // (type, count)
    
    for (int i = 0; i < M; i++) {
        string s;
        ll k;
        cin >> s >> k;
        
        // 将 s 内部分成连续段
        vector<pair<char, ll>> segs;
        for (char c : s) {
            if (segs.empty() || segs.back().first != c) {
                segs.push_back({c, 1});
            } else {
                segs.back().second++;
            }
        }
        
        int n_segs = segs.size();
        
        if (n_segs == 1) {
            // 整个 s 只有一种字符
            ll total = segs[0].second * k;
            if (!blocks.empty() && blocks.back().first == segs[0].first) {
                blocks.back().second += total;
            } else {
                blocks.push_back({segs[0].first, total});
            }
        } else if (segs.front().first != segs.back().first) {
            // 首尾字符不同，k 次重复不会在边界合并
            // 每种字符的总数 = 该字符在 s 中的数量 * k
            for (auto& [c, cnt] : segs) {
                ll total = cnt * k;
                if (!blocks.empty() && blocks.back().first == c) {
                    blocks.back().second += total;
                } else {
                    blocks.push_back({c, total});
                }
            }
        } else {
            // 首尾字符相同，边界会合并
            // 结构：第一次完整 + (k-1) 次的中间部分合并
            
            // 第一次：加入所有段
            for (auto& [c, cnt] : segs) {
                if (!blocks.empty() && blocks.back().first == c) {
                    blocks.back().second += cnt;
                } else {
                    blocks.push_back({c, cnt});
                }
            }
            
            if (k > 1) {
                // 后续 (k-1) 次：segs.back() 和 segs.front() 会合并
                // 合并后的块大小 = segs.back().second + segs.front().second
                // 这会重复 (k-1) 次
                
                // blocks.back() 当前是 segs.back()
                // 加上 (k-1) 次的 segs.front()
                blocks.back().second += (k - 1) * segs[0].second;
                
                // 中间的段 segs[1] 到 segs[n-2] 各加 (k-1) 次
                for (int j = 1; j < n_segs - 1; j++) {
                    blocks.push_back({segs[j].first, segs[j].second * (k - 1)});
                }
                
                // segs.back() 加 (k-1) 次
                blocks.push_back({segs[n_segs - 1].first, segs[n_segs - 1].second * (k - 1)});
            }
        }
    }
    
    // 合并相邻相同块
    vector<pair<char, ll>> merged;
    for (auto& [c, cnt] : blocks) {
        if (!merged.empty() && merged.back().first == c) {
            merged.back().second += cnt;
        } else {
            merged.push_back({c, cnt});
        }
    }
    blocks = merged;
    
    // 计算每个块的起始位置
    vector<tuple<char, ll, ll>> blocks_with_pos; // (type, count, start_pos)
    ll pos = 0;
    for (auto& [c, cnt] : blocks) {
        blocks_with_pos.push_back({c, cnt, pos});
        pos += cnt;
    }
    
    // 检查总数
    ll totalA = 0, totalB = 0;
    for (auto& [c, cnt, p] : blocks_with_pos) {
        if (c == 'A') totalA += cnt;
        else totalB += cnt;
    }
    
    if (totalA != N || totalB != N) {
        cout << -1 << endl;
        return 0;
    }
    
    // 贪心算法（块级别）：
    // 从后往前遍历，维护 surplus = 后缀中 A 比 B 多的数量
    // 如果 surplus < 0，需要从前面借 A
    // 记录借 A 时产生的最大延迟
    
    ll surplus = 0;
    ll ans = 0;
    
    // 前面的 A 块栈：(块的结束位置, 剩余数量)
    vector<pair<ll, ll>> a_stack;
    for (auto& [c, cnt, p] : blocks_with_pos) {
        if (c == 'A') {
            a_stack.push_back({p + cnt - 1, cnt}); // 结束位置（0-indexed）
        }
    }
    
    // 需要 A 填补的位置（当后缀缺 A 时，B 块的起始位置就是需要 A 来的地方）
    ll need_pos = 2 * N;
    
    for (int i = (int)blocks_with_pos.size() - 1; i >= 0; i--) {
        auto [c, cnt, start] = blocks_with_pos[i];
        
        if (c == 'A') {
            surplus += cnt;
            // 这个 A 块本身在后缀中，不需要借，从栈中移除
            while (!a_stack.empty() && a_stack.back().first >= start) {
                a_stack.pop_back();
            }
        } else { // c == 'B'
            surplus -= cnt;
            need_pos = start; // B 块的起始位置
        }
        
        // 如果 surplus < 0，需要从前面借 A
        while (surplus < 0 && !a_stack.empty()) {
            auto& [a_end, a_cnt] = a_stack.back();
            ll borrow = min(a_cnt, -surplus);
            
            // 这些 A 从 (a_end - borrow + 1) ~ a_end 移动到 need_pos 附近
            // 最大延迟：最靠前的 A (位置 a_end - borrow + 1) 移到 need_pos
            ll delay = need_pos - (a_end - borrow + 1);
            ans = max(ans, delay);
            
            surplus += borrow;
            a_cnt -= borrow;
            a_end -= borrow;
            
            if (a_cnt == 0) a_stack.pop_back();
        }
        
        if (surplus < 0) {
            cout << -1 << endl;
            return 0;
        }
    }
    
    cout << ans << endl;
    return 0;
}