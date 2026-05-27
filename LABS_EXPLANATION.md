# Lab 题目整理与题解

本文按仓库中的 `Lab1` 到 `Lab13` 整理。  
“原题定位”一栏里，能从代码注释、README 或网络搜索明确确认的会直接写出；不能确认的，按当前代码实现归纳为经典模型。

## 总览

| 文件 | 经典模型 | 原题/相关题定位 |
| --- | --- | --- |
| `Lab1/lab1A.cpp` | 取石子博弈，SG/必胜态周期 | 模 6 Nim Game |
| `Lab1/lab2B.cpp` | 找规律，数学归纳 | ZJOI2009 函数 |
| `Lab2/lab2A.cpp` | 多对一稳定匹配 | Hospitals/Residents，Gale-Shapley |
| `Lab2/lab2B.cpp` | 稳定匹配构造博弈策略 | Codeforces 1147F Zigzag Game |
| `Lab3/lab3A.cpp` | 单机截止期调度，最小罚款 | Job sequencing with deadlines |
| `Lab3/lab3B.cpp` | RLE 调度，前缀约束，线性扫描 | Smart Warehouse Scheduling，仓库调度类题 |
| `Lab4/lab4A.cpp` | LFU Cache + TTL + LRU tie-break | 缓存模拟 |
| `Lab4/lab4B.cpp` | IDA* 搜索 | SCOI2005 骑士精神 |
| `Lab5/lab5A.cpp` | 并查集，阈值连通块 | Kruskal 思想 |
| `Lab5/lab5B_submit.cpp` | 有向最小生成树 | Chu-Liu/Edmonds arborescence |
| `Lab6/lab6A.cpp` | 三维最近点对 | 分治最近点对 |
| `Lab6/lab6B.cpp` | 树上带前置约束调度 | Smith rule + DSU 合并 |
| `Lab7/lab7A.cpp` | 大整数乘法 | Karatsuba |
| `Lab7/lab7B.cpp` | 字符串 border 检测 | NTT 卷积 |
| `Lab8/lab8A.cpp` | 多重背包 | 二进制拆分转 01 背包 |
| `Lab8/lab8B.cpp` | 路径高度范围计数 | 四维 DP/滚动数组 |
| `Lab9/lab9A.cpp` | 编辑距离变形 | CLRS edit distance with copy/replace/delete/insert/twiddle/kill |
| `Lab9/lab9B.cpp` | 字符串压缩 | 区间 DP |
| `Lab10/lab10A.cpp` | 最大比例环 | 二分答案 + 正环检测 |
| `Lab10/lab10B.cpp` | 树上选中心 | 树形 DP |
| `Lab11/lab11a.CPP` | 最小费用最大流 | 点容量限制的最短多路路径 |
| `Lab11/lab11B.cpp` | 最大权闭合图/最小割 | 二值图像/网格标号最大收益 |
| `Lab12/lab12A.cpp` | 有上下界最大流 | 工人维护设备分配 |
| `Lab12/lab12B.cpp` | DAG 点容量最大流 | 最长非降子序列抽取数 |
| `Lab13/lab13A.cpp` | 最大团随机算法 | Maximum Clique + 模拟退火 |
| `Lab13/lab13B.cpp` | 基数估计 | HyperLogLog 集合并 |

## Lab1A: 模 6 取石子博弈

题意可以抽象为：给定石子数 `n`，两人轮流操作，无法操作者失败。代码给出的结论是：

```text
n < 6      Alice win
n % 6 == 0 Bob win
otherwise  Alice win
```

这是典型的取石子游戏周期性结论。如果每次可以取 `1..5` 个，那么 `0, 6, 12, ...` 是必败态。  
证明方式是：

1. `1..5` 可以一步取完，先手胜。
2. `6` 无论取 `1..5`，都会把局面交给对方一个 `1..5` 的必胜态，所以先手败。
3. 对于 `6k + r`，`r != 0`，先手取走 `r` 个，把局面变成 `6k`。
4. 对于 `6k`，无论先手取 `x`，后手取 `6 - x`，总能回到 `6(k-1)`。

复杂度 `O(T)`。

## Lab1B: ZJOI2009 函数

代码：

```cpp
if (n == 1) cout << 1;
else cout << 2 * min(k, n - k + 1);
```

这是 ZJOI2009「函数」的规律题。问题本质是问第 `k` 层在若干函数图像下被分成多少段。  
由于图形上下对称，第 `k` 层与第 `n-k+1` 层答案相同，所以只需要考虑靠近边界的一侧。

核心结论：

```text
ans = 1                       if n == 1
ans = 2 * min(k, n-k+1)       otherwise
```

证明可以用归纳：

1. 第 1 层只有左右两端两段，所以答案是 `2`。
2. 从第 `k-1` 层到第 `k` 层，原来的每一段向上延伸，仍然形成 `2(k-1)` 段。
3. 因为最左和最右两侧还会各新出现一段，所以多出 `2` 段。
4. 因此第 `k` 层为 `2k`。
5. 用对称性处理 `k > n/2` 的情况。

复杂度 `O(1)`。

## Lab2A: 多对一稳定匹配

题意抽象：

有 `N` 个学生和 `M` 个学院。学院有容量，学生对学院有满意度，学院也对学生有评价。满意度非正的学院不会被学生主动申请。要求输出一个稳定录取方案。

经典模型是 Hospitals/Residents，即带容量的 Gale-Shapley 稳定匹配。

算法：

1. 对每个学生，把所有满意度为正的学院按满意度从高到低排序，得到申请列表。
2. 所有学生入队，表示当前未匹配。
3. 队首学生向自己还没申请过的最高偏好学院申请。
4. 学院如果没满，则暂时接收。
5. 学院如果已满，则比较新学生和当前最差学生；若新学生更好，踢出最差学生，否则拒绝新学生。
6. 被拒绝或被踢出的学生继续入队申请下一个学院。

数据结构：

- 学生侧：`nxt[i]` 表示下一个要申请的学院。
- 学院侧：小根堆维护当前录取学生，堆顶是学院最不喜欢的学生。

复杂度主要来自排序和申请过程：

```text
O(NM log M + NM log capacity)
```

## Lab2B: Codeforces 1147F Zigzag Game

这是交互博弈题的 hack 版。题面来自 Codeforces 1147F。图是完全二分图，边权互不相同，玩家选择递增或递减规则移动棋子。

代码策略是把问题转成稳定匹配：

1. 二分图左侧点和右侧点之间有唯一边权。
2. 若当前规则是递增，则偏好较大边权；若递减，则偏好较小边权。
3. 通过 Gale-Shapley 求一个稳定匹配。
4. 稳定匹配给出一种“回应策略”：对方走到某点时，我方沿匹配边走回。

为什么稳定匹配能赢：

- 对手从 `u` 走到 `v`，边权满足当前单调规则。
- 我方从 `v` 按匹配边走到 `match[v]`。
- 稳定性保证不存在会让对方持续占优的阻塞边。
- 因此在有限图中，回应策略能避免先无路可走。

复杂度 `O(n^2 log n)`，因为要排序偏好列表并跑稳定匹配。

## Lab3A: 截止期任务调度最小罚款

题意抽象：

有 `n` 个任务，每个任务耗时 1，有截止时间 `ddl[i]` 和罚款 `punish[i]`。若任务没能在截止时间前完成，则产生罚款。求最小总罚款。

等价转化：

```text
最小未完成罚款 = 总罚款 - 最大按时完成任务罚款和
```

贪心做法：

1. 按截止时间从小到大排序。
2. 维护一个小根堆，表示当前选择按时完成的任务罚款。
3. 遍历任务 `(d, p)`：
   - 如果当前已选任务数 `< d`，直接选。
   - 否则如果堆顶罚款小于 `p`，用当前任务替换掉罚款最小的任务。
4. 堆中留下的任务就是在所有截止约束下能保留的最大罚款和。

正确性是经典交换论证：在任何前缀内，如果只能选 `d` 个任务，就应该保留罚款最大的那些。

复杂度 `O(n log n)`。

## Lab3B: RLE 仓库双机器调度

题意按 README 和代码可归纳为：

有一串由 `A/B` 组成的包裹序列，用 RLE 形式给出。两台机器每分钟各处理一个包裹，总共需要在 `N` 分钟内处理 `2N` 个包裹。机器 A 必须有足够的 `A`，机器 B 优先消耗 `B`，多余的 `A` 可作为缓冲。目标是最小化最大延迟。

关键判断：

```text
countA < N  => 不可行，输出 -1
```

令：

```text
K = countA - N
```

表示多余的 `A` 数量，也就是可以给系统提供的缓冲。对第 `k` 个 `B`，原始位置为 `Pos(k)`。如果 `B` 太晚出现，会迫使前面的 `A` 往后推迟，延迟下界为：

```text
Pos(k) - 2k - 2K
```

答案：

```text
max(0, max_k(Pos(k) - 2k) - 2K)
```

由于输入是 RLE，不能展开。对每个重复块 `s * x`：

1. 在一个周期 `s` 内扫描所有 `B`，求局部最大值。
2. 周期之间的增量是线性的：

```text
slope = len(s) - 2 * countB(s)
```

3. 线性函数的最大值只会出现在第一轮或最后一轮，所以只检查这两个端点。

复杂度 `O(sum |s| + M)`。

## Lab4A: LFU Cache + TTL

题意抽象：

实现容量为 `N` 的缓存，支持：

- `get(key)`：存在且未过期则返回值，并刷新使用信息；否则返回 `-1`。
- `put(key, value)`：插入或更新；容量满时淘汰。
- 每次操作前清理过期键。
- 结束时按更新时间顺序输出缓存中的值，空则输出 `empty`。

淘汰规则：

1. 优先淘汰使用频率最低的键。
2. 同频率下淘汰最久未使用的键。
3. TTL 超过限制的键在操作前删除。

数据结构：

- `keyToNode`：key 到节点。
- `freqToList[f]`：频率为 `f` 的节点链表，链表头是同频最久未用。
- `recency`：全局更新时间顺序，用于最终输出。
- `expireBuckets[time]`：某时刻需要检查过期的候选节点。

由于一个节点可能被多次刷新 TTL，同一个节点会出现在多个过期桶里，因此清理时必须校验：

```text
key 仍存在
map 中指针仍指向该节点
node.expireTime <= now
```

均摊复杂度 `O(1)`。

## Lab4B: SCOI2005 骑士精神

这是经典 5x5 棋盘搜索题。棋盘上有 `0/1/*`，`*` 是空格，每一步空格按马走日交换位置。求变成目标状态的最少步数。原题 P2324 的限制是 15 步，当前代码枚举到 20 步；若按原题提交，应把上限改回 15。

目标棋盘：

```text
11111
01111
00*11
00001
00000
```

算法是 IDA*：

1. 枚举最大深度 `limit = 0..20`。
2. 做 DFS，当前深度为 `g`。
3. 估价函数 `h` 为当前棋盘中不匹配目标棋盘的位置数，不计空格。
4. 若 `g + h > limit`，剪枝。
5. 枚举空格的 8 个马步位置，递归交换。

为什么可行：深度上限只有 20，启发式虽然不强，但足够剪掉大量状态。

## Lab5A: 阈值连通块

题意从代码看是：

给定 `n` 个点、`m` 条无向边和阈值 `T`。若边权 `w < T`，则这条边可以连通两个区域。求最终连通块数量。

做法：

1. 初始化并查集。
2. 遍历所有边，若 `w < T` 且两端不在同一集合，则合并。
3. 每次成功合并，连通块数减一。

代码中虽然按边权降序排序，但对这个判定并非必要；直接扫描所有边也可以。

复杂度 `O(m alpha(n))`，若保留排序则是 `O(m log m)`。

## Lab5B: 有向最小生成树

经典模型：Directed MST / Minimum Arborescence。  
给定有向带权图，要求选择一个根，使得从根能到达所有点，并且总边权最小。代码通过增加超级根，把“不确定根”转化成固定根问题。

Chu-Liu/Edmonds 算法步骤：

1. 对每个非根节点，选择入边权最小的边。
2. 如果这些边没有形成环，则它们就是最优入树。
3. 如果形成环，把环缩成一个超级点。
4. 对所有进入环的边做权值调整：

```text
newWeight(u -> cycle) = w(u -> v) - in[v]
```

其中 `in[v]` 是环中节点 `v` 之前选中的最小入边权。
5. 在缩点图上递归求解。
6. 递归返回后展开环，恢复父边。

代码的超级根技巧：

1. 令 `super_weight = sum(edge weights) + 1`。
2. 超级根向每个原点连一条代价为 `super_weight` 的边。
3. 若最终树中恰好使用一条超级根边，则这条边指向真正根。
4. 答案为 `mst_cost - super_weight`。
5. 若使用多条超级根边，说明原图无法由单根到达所有点，输出 `impossible`。

复杂度朴素实现约 `O(VE)`。

## Lab6A: 三维最近点对

题意：

多组测试，每组给出 `n` 个三维点，输出最近两点的欧氏距离，保留两位小数。

代码使用分治：

1. 如果区间点数小于等于 30，暴力 `O(k^2)`。
2. 否则找当前区间跨度最大的坐标轴作为划分维度。
3. 用 `nth_element` 按该维度划分为左右两半。
4. 递归求左右最小距离 `d`。
5. 收集距离划分平面小于 `d` 的点形成 strip。
6. 按另一个维度排序，在 strip 中检查可能更近的点对。

这是二维最近点对分治在三维上的工程化变形。严格最坏复杂度较难保证，但随机/均匀数据下通常接近 `O(n log n)`。

## Lab6B: 树上前置约束调度

题意抽象：

有一棵以 `F` 为根的树，每个节点有权重 `C[i]`。执行一个节点前必须先执行其父节点。每个节点耗时 1，求最小化：

```text
sum C[i] * completion_time[i]
```

经典思想是 Smith rule 的树上版本。若两个任务块 A、B 都已经满足前置条件，应优先执行平均权重更大的块：

```text
W_A / size_A > W_B / size_B
```

算法：

1. 每个节点初始是一个块，`W = C[i]`，`Size = 1`。
2. 除根外所有块放入优先队列，按 `W / Size` 从大到小排序。
3. 每次取平均权重最大的块 `v`。
4. 找到它当前父亲所在的并查集块 `u`。
5. 把 `v` 合并到 `u` 后面。
6. 合并代价增加：

```text
Size[u] * W[v]
```

因为 `v` 块整体被 `u` 块推迟了 `Size[u]` 个时间单位。

复杂度 `O(n log n)`。

## Lab7A: Karatsuba 大整数乘法

题意：

输入两个大整数，输出乘积。支持负号。

做法：

1. 用 `BASE = 10000`，每 4 位十进制作为一个块，低位在前。
2. 小规模时用普通 `O(n^2)` 乘法。
3. 大规模时用 Karatsuba：

```text
A = A0 + A1 * B^k
C = C0 + C1 * B^k

A*C =
z0 + (z1 - z0 - z2) * B^k + z2 * B^(2k)

z0 = A0*C0
z2 = A1*C1
z1 = (A0+A1)(C0+C1)
```

4. 最后统一处理进位并恢复符号。

复杂度约 `O(n^log2 3)`。

## Lab7B: NTT 求二进制串 border

题意从代码可归纳为：

给定一个二进制串 `s`，找所有长度 `len <= n/2` 的 border，即：

```text
s[0..len-1] == s[n-len..n-1]
```

对所有满足条件的 `len`，把 `len^2` 异或进答案。

暴力比较所有 `len` 会是 `O(n^2)`。代码用 NTT 做字符串匹配：

1. 构造：

```text
A0[i] = 1 iff s[i] == '0'
A1[i] = 1 iff s[i] == '1'
B0[i] = 1 iff reversed(s)[i] == '0'
B1[i] = 1 iff reversed(s)[i] == '1'
```

2. 计算卷积：

```text
C01 = A0 * B1
C10 = A1 * B0
```

3. 对于某个 `len`，`C01[len-1]` 表示前缀长度 `len` 与后缀长度 `len` 中 `0/1` 不匹配的数量之一，`C10[len-1]` 表示另一类不匹配。
4. 若两者都为 0，则该长度是 border。

复杂度 `O(n log n)`。

## Lab8A: 多重背包

题意：

有 `n` 种物品，背包容量为 `C`。第 `i` 种物品体积 `c[i]`、价值 `v[i]`、数量上限 `q[i]`。求最大价值。

做法是二进制拆分：

```text
q = 1 + 2 + 4 + ... + rest
```

把每种物品拆成若干个 01 物品：

```text
weight = c[i] * take
value  = v[i] * take
```

然后跑 01 背包：

```cpp
for item:
    for cap from C down to weight:
        dp[cap] = max(dp[cap], dp[cap-weight] + value)
```

复杂度：

```text
O(C * sum log q[i])
```

## Lab8B: 高度范围受限的排列计数

题意可抽象为：

有 `n` 个上升步和 `m` 个下降步，要求整条路径的高度范围不超过 `k`。输出方案数，模 `12345678`。

代码状态：

```text
dp[i][j][d][r]
```

滚动掉 `i` 后实际为：

- 已放 `i` 个上升步。
- 已放 `j` 个下降步。
- 当前高度相对历史最低点的距离为 `d`。
- 当前历史最高点与最低点的差为 `r`。

转移：

1. 放一个上升步：
   - 如果 `d < r`，只是当前位置升高，`d += 1`。
   - 如果 `d == r`，突破历史最高点，`d += 1, r += 1`。
2. 放一个下降步：
   - 如果 `d > 0`，当前位置下降，`d -= 1`。
   - 如果 `d == 0`，突破历史最低点，整体平移后 `d = 0, r += 1`。

只保留 `r <= k` 的状态。最终答案是：

```text
sum dp[n][m][d][r]
```

复杂度 `O(n*m*k^2)`，空间用滚动数组降为 `O(m*k^2)`。

## Lab9A: 编辑距离变形

题意：

给定源串 `s` 和目标串 `t`，以及操作代价：

- delete
- replace
- copy
- insert
- swap/twiddle
- finish/kill

求最小代价，并输出操作序列。

DP 定义：

```text
dp[i][j] = 把 s 的前 i 个字符变成 t 的前 j 个字符的最小代价
```

转移：

```text
delete:  dp[i][j]   <- dp[i-1][j]   + cost_delete
insert:  dp[i][j]   <- dp[i][j-1]   + cost_insert
replace: dp[i][j]   <- dp[i-1][j-1] + cost_replace
copy:    若 s[i-1] == t[j-1]
         dp[i][j]   <- dp[i-1][j-1] + cost_copy
swap:    若 s[i-2..i-1] 与 t[j-2..j-1] 交叉相等
         dp[i][j]   <- dp[i-2][j-2] + cost_swap
```

finish/kill 操作表示：当目标串已经生成完，可以一次性结束并删除源串剩余部分。代码用：

```text
current_cost = dp[i][n] + (m-i)*delete - 1
```

记录最优结束位置 `best_end_i`，再根据 `choice` 数组回溯操作。

复杂度 `O(|s| * |t|)`；代码用 3 行滚动数组节省 DP 值空间，但仍保存 `choice` 以回溯。

## Lab9B: 字符串压缩区间 DP

题意从 README 和代码可归纳为：

给定字符串，允许某种压缩规则：

1. 普通字符直接保留，长度贡献 1。
2. 如果一个区间可以分成两个完全相同的半区间，可以用前半区间加 1 个标记表示。
3. 还可以通过加入分隔符 `B` 将左右区间组合。

状态：

```text
dp[i][j][1] = 压缩 s[i..j]，且区间内部不新加 B 的最小长度
dp[i][j][0] = 压缩 s[i..j]，允许内部加 B 的最小长度
```

转移：

1. 不加新 `B`：

```text
dp[i][j][1] = dp[i][j-1][1] + 1
```

2. 若区间长度为偶数且左右两半完全相等：

```text
dp[i][j][1] = min(dp[i][j][1], dp[i][mid][1] + 1)
```

3. 允许加 `B`：

```text
dp[i][j][0] = dp[i][j][1]
dp[i][j][0] = min(dp[i][k][0] + dp[k+1][j][1] + 1)
```

复杂度代码当前为 `O(n^4)` 风险，因为每次用 `substr` 比较两半；若要优化，应使用哈希或 LCP 把相等判断降到 `O(1)`。

## Lab10A: 最大比例环

题意：

每条有向边有两个值 `v` 和 `p`。要求找一个有向环，使得：

```text
sum(v) / sum(p)
```

最大。若图中不存在环，输出 `-1`。

二分答案 `r`。判断是否存在环满足：

```text
sum(v) / sum(p) > r
```

等价于：

```text
sum(v - r*p) > 0
```

所以对每条边赋新权：

```text
w = v - r*p
```

只要图中存在正权环，说明答案至少为 `r`。

代码用类似 SPFA 的最长路松弛检测正环：

1. 所有点入队，初始距离为 0。
2. 若能不断松弛并且路径边数达到 `n`，说明存在正环。
3. 二分 `r`，最后输出一位小数。

复杂度约 `O(iterations * SPFA)`。

## Lab10B: 树上选中心 DP

题意抽象：

给定一棵有根树，每个节点有权重 `w[i]`，边长 `d[i]`。选择最多/恰好 `m` 个节点作为中心，使所有点到某个选中祖先中心的加权距离总和最小。

代码状态较特殊：

```text
dp[u][d][j]
```

表示在 `u` 的子树中选择 `j` 个中心，且 `u` 子树中未被子树中心覆盖的点由深度为 `d` 的祖先负责时的最小代价。

初始化：

- 若选择 `u` 为中心：

```text
dp[u][depth[u]][1] = 0
```

- 若不在 `u` 及其子树内选择能覆盖 `u` 的中心，则由祖先 `anc[d]` 负责：

```text
dp[u][d][0] = w[u] * (dist[u] - dist[anc[d]])
```

合并子树时做树形背包：

```text
dp = min(dp + child_dp)
```

最后还有一步：

```text
dp[u][d][j] = min(dp[u][d][j], dp[u][depth[u]][j])
```

含义是：如果子树内部已经选择了 `u` 作为中心，那么对更高祖先状态同样可行。

复杂度大致为 `O(n * depth * m^2)`，具体取决于树高和剪枝。

## Lab11A: 点容量限制的最小费用最大流

题意抽象：

给定有向图，从 `1` 到 `n` 找尽可能多条路径，并使总费用最小。中间点容量为 1，即每个中间点最多被一条路径使用。

建图：

1. 对中间点 `i = 2..n-1` 拆点：

```text
i_in -> i_out, cap = 1, cost = 0
```

2. 原边 `u -> v, w`：
   - 如果 `u` 是中间点，则从 `u_out` 出发。
   - 否则从 `u` 出发。
   - 连到 `v_in` 或 `v`，代码中直接连到 `v`，因为中间点入点编号就是原编号。

3. 每条原边容量为 1，费用为 `w`。

然后跑最小费用最大流：

- 每次用 SPFA 找当前最短增广路。
- 沿路增广。
- 累加流量和费用。

输出：

```text
max_flow min_cost
```

## Lab11B: 网格二值标号最大收益

题意抽象：

有一个 `n*m` 网格，每个格子可以选 Linux 或 Windows。选择某系统有基础收益；相邻格子如果同为某系统，还会产生额外收益。求最大总收益。

这是经典最大权闭合图/最小割建模。

基本思想：

```text
最大收益 = 所有正收益之和 - 最小割损失
```

建图：

1. 每个格子一个点。
2. Linux 基础收益 `val`：

```text
S -> cell, cap = val
```

3. Windows 基础收益 `val`：

```text
cell -> T, cap = val
```

4. 相邻两个格子同为 Linux 的收益，新增奖励点 `x`：

```text
S -> x, cap = val
x -> cell1, INF
x -> cell2, INF
```

只有当两个格子都在源侧时，这个奖励才能保留。

5. 相邻两个格子同为 Windows 的收益：

```text
cell1 -> x, INF
cell2 -> x, INF
x -> T, cap = val
```

只有当两个格子都在汇侧时，这个奖励才能保留。

最后：

```text
answer = total_sum - maxflow(S, T)
```

## Lab12A: 带上下界最大流分配

题意：

`m` 个工人、`n` 天。工人 `i` 总共至少维护 `D[i]` 台设备。第 `k` 天最多维护 `T[k]` 台；当天出现的工人 `i` 可以维护 `[L, R]` 台。求最多维护多少台。

原图：

```text
source -> day[k]       [0, T[k]]
day[k] -> worker[i]   [L, R]
worker[i] -> sink     [D[i], INF]
```

这是有源汇的上下界最大流。

去下界：

对每条边 `(u, v, lower, upper)`：

```text
capacity = upper - lower
need[u] -= lower
need[v] += lower
```

加超级源汇：

```text
need[x] > 0: SS -> x
need[x] < 0: x -> TT
```

再加：

```text
sink -> source, INF
```

先跑 `maxflow(SS, TT)` 检查可行性。若不可行输出 `-1`。  
可行后，禁用超级源汇相关边和辅助边，再从 `source` 到 `sink` 继续增广，得到最大答案。

## Lab12B: 最长非降子序列抽取数

题意：

给定数组 `a[1..n]` 和每个位置可被使用次数 `x[i]`。每条子序列中同一位置最多使用一次。设 `S` 为数组的最长非降子序列长度，求最多能抽出多少条长度为 `S` 的非降子序列。

步骤：

1. 求：

```text
dp[i] = 以 i 结尾的最长非降子序列长度
```

转移：

```text
if j < i and a[j] <= a[i]:
    dp[i] = max(dp[i], dp[j] + 1)
```

2. 拆点限制每个位置的使用次数：

```text
in(i) -> out(i), cap = x[i]
```

3. 建 DAG 流图：

```text
source -> in(i),     if dp[i] == 1
out(i) -> sink,      if dp[i] == S
out(i) -> in(j),     if i < j, a[i] <= a[j], dp[j] == dp[i] + 1
```

4. 最大流就是最多能抽出的最长非降子序列条数。当前代码先输出 `longest`，再输出最大流；如果题目只要求数量，需要删掉第一行输出。

注意：只能连 `dp[j] == dp[i] + 1` 的边，不能只按 `a[i] <= a[j]` 连，否则一条路径可能跳层，不一定长度为 `S`。

## Lab13A: 最大团模拟退火

题意：

给定无向图，求最大点集，使点集中任意两点都有边。这就是 Maximum Clique。

最大团是 NP-hard 问题，所以代码使用随机启发式算法：

1. 状态始终保持为一个合法团 `clique`。
2. 候选集合是所有与当前团内每个点都相连的点。
3. `expand`：从候选点中随机抽若干个，选择度数较高的加入，直到不能加入。
4. `perturb`：随机删除当前团中的若干点，然后重新 `expand`。
5. 若新团更大，直接接受；若更小，以模拟退火概率接受：

```text
prob = exp((newSize - oldSize) / temperature)
```

6. 在时间限制内不断迭代，输出见过的最大团大小。

这是“随机局部搜索 + 模拟退火接受准则”，适合近似判题或随机算法要求，不保证精确最优。

## Lab13B: HyperLogLog 集合并基数估计

题意：

有 `m` 个基础生物，第 `i` 个拥有特征 `{i}`。之后每个新生物由两个已有生物合成，其特征集合为两个集合的并集。对每个新生物输出特征数量。

精确 bitset 做法内存过大：

```text
n <= 6e5, m <= 9e4
n * m bits ≈ 6.75GB
```

代码采用 HyperLogLog 近似估计集合基数。

HLL 思想：

1. 把每个基础特征 hash 成随机 64 位数。
2. 用一部分位决定桶编号。
3. 用剩余位的连续 0 数量作为“稀有程度”。
4. 每个桶只记录见过的最大稀有程度。
5. 集合越大，越可能出现很长的连续 0。

加入元素：

```text
bucket = hash(x) 的低 P 位
rho = trailing_zero_count(hash(x) >> P) + 1
reg[bucket] = max(reg[bucket], rho)
```

合并集合：

```text
reg_union[i] = max(reg_a[i], reg_b[i])
```

估计公式：

```text
E = alpha * M^2 / sum(2^(-reg[i]))
```

代码参数：

```text
P = 8
M = 256
误差约 1.04 / sqrt(M) ≈ 6.5%
```

注意：HLL 是近似算法，不适合普通精确判题；如果题目是 special judge 或要求随机近似算法，则可用。

## 外部参考

- Codeforces 1147F Zigzag Game: https://codeforces.com/problemset/problem/1147/F
- ZJOI2009 函数: https://www.luogu.com.cn/problem/P2591
- ZJOI2009 函数题解参考: https://www.cnblogs.com/encodetalker/p/9791706.html
- Hospitals/Residents 稳定匹配: https://en.wikipedia.org/wiki/Stable_marriage_problem
- Gale-Shapley 算法: https://en.wikipedia.org/wiki/Gale%E2%80%93Shapley_algorithm
- SCOI2005 骑士精神: https://www.luogu.com.cn/problem/P2324
- Chu-Liu/Edmonds 有向最小生成树: https://en.wikipedia.org/wiki/Edmonds%27_algorithm
- CLRS Edit Distance 变形: https://walkccc.me/CLRS/Chap15/Problems/15-5/
- 最大权闭合图与最小割: https://en.wikipedia.org/wiki/Closure_problem
- Maximum Clique 复杂性与启发式算法: https://www.mdpi.com/2073-8994/15/11/1979
- HyperLogLog 并集合并: https://handwiki.org/wiki/HyperLogLog
