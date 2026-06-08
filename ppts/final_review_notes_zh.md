# CS216 Algorithm Design and Analysis (H) 期末复习 Notes

来源：`ppts` 文件夹中的 Review、各章 PPT、Exam Paper。  
题型参考：10 道不定项选择题 + 6 道大题。大题通常不是背原题，而是要求把新叙述翻译成某个经典模型，并写出算法、正确性证明、复杂度。

## 0. 考前使用路线

### 选择题优先背

1. 各类定义：$O,\Omega,\Theta$，stable matching，flow/cut，NP/NP-hard/NP-complete，Monte Carlo/Las Vegas。
2. 经典算法的选择准则：interval scheduling 选最早结束，Dijkstra 要非负边，Huffman 合并最低频，Ford-Fulkerson 用 residual network。
3. 复杂度：GS $O(n^2)$，weighted interval scheduling $O(n\log n)$，inversion/closest pair/FFT $O(n\log n)$，Bellman-Ford $O(mn)$，Ford-Fulkerson 整数容量 $O(m|f^\*|)$。
4. 定理条件：Max-flow Min-cut，integrality theorem，Master theorem，Chernoff bound，NP-complete 证明三步。

### 六道大题优先练

Exam Paper 的六道大题风格：

1. Greedy：设计贪心并证明 exchange / stays ahead。
2. Divide and Conquer：递归地缩小规模，证明每步不会丢掉答案。
3. Dynamic Programming：定义状态、转移、边界、计算顺序、复杂度。
4. Polynomial Reduction：证明两个问题可互相调用，或把一个 NP-complete 问题归约到新问题。
5. Network Flow：建图，把目标函数转成 cut/flow/circulation。
6. Randomized：算期望或成功概率，用线性期望、union bound、Chernoff、放大。

答大题时最好固定写五段：

1. Model：把题目翻译成哪个经典问题。
2. Algorithm：清楚写步骤。
3. Correctness：说明为什么可行且最优/满足要求。
4. Complexity：时间和空间。
5. Edge cases：不可行、强制约束、负权、容量下界等。

---

## 1. Algorithm Analysis

### 1.1 多项式时间与 tractability

课程采用的工作定义：

- Efficient algorithm：最坏情况多项式时间。
- Polynomial time：存在常数 $a,b>0$，使运行时间 $T(n)\le a n^b$。
- Worst-case analysis：对所有规模为 $n$ 的输入取最坏时间。
- Average-case analysis：需要输入分布，考试中常考它不一定贴近真实输入。
- Randomized analysis：算法本身随机，分析期望时间或成功概率。
- Amortized analysis：一串操作总时间的最坏情况均摊。

常见增长顺序：

$$
\log n \ll n \ll n\log n \ll n^2 \ll n^3 \ll n^k \ll c^n \ll n!
$$

重要关系：

$$
\log_a n = \Theta(\log_b n)
$$

$$
\forall c>0,\ a>1,\quad \log^a n = O(n^c)
$$

$$
\forall r>1,\ c>0,\quad n^c = O(r^n)
$$

$$
n! = 2^{\Theta(n\log n)}
$$

### 1.2 $O,\Omega,\Theta$

定义：

$$
T(n)=O(f(n)) \iff \exists c>0,n_0,\ \forall n\ge n_0,\ 0\le T(n)\le c f(n)
$$

$$
T(n)=\Omega(f(n)) \iff \exists c>0,n_0,\ \forall n\ge n_0,\ T(n)\ge c f(n)\ge 0
$$

$$
T(n)=\Theta(f(n)) \iff T(n)=O(f(n))\ \text{and}\ T(n)=\Omega(f(n))
$$

易错点：

- $O$ 是上界，$\Omega$ 是下界，$\Theta$ 是紧界。
- “任何 comparison sort 至少需要 $O(n\log n)$ 次比较”是错说法，应为 $\Omega(n\log n)$。
- $T(n)=O(f(n))$ 严格说是 $T(n)\in O(f(n))$，不是等号关系。

### 1.3 代表问题

课件用 independent set 主题串联：

- Interval Scheduling：可贪心，$O(n\log n)$。
- Weighted Interval Scheduling：DP，$O(n\log n)$。
- Bipartite Matching：flow，常见 $O(nk)$ 或依实现而定。
- Independent Set：NP-complete。
- Competitive Facility Location：PSPACE-complete。

---

## 2. Stable Matching

### 2.1 定义

One-to-one stable matching：

- Perfect matching：每个男人/女人恰好匹配一个对象。
- Unstable pair：未匹配的一对 $m,w$，彼此都更喜欢对方胜过当前伴侣。
- Stable matching：perfect matching 且没有 unstable pair。

一般 roommate 问题不是二分图，stable matching 不一定存在。婚姻问题中 stable matching 总存在。

### 2.2 Gale-Shapley Algorithm

男方 proposal 版本：

1. 所有人初始 free。
2. 当存在 free man 且他还没向所有 woman 求过：
   - $m$ 向自己列表中尚未求过的最喜欢的 $w$ 求。
   - 若 $w$ free，则订婚。
   - 若 $w$ 更喜欢 $m$ 胜过当前 fiancé $m'$，则 $w$ 改和 $m$ 订婚，$m'$ free。
   - 否则 $w$ 拒绝 $m$。

复杂度：

- 最多 $n^2$ 次 proposal。
- 用 inverse preference array 可 $O(1)$ 比较 woman 对两个 man 的偏好。
- 总时间 $O(n^2)$，空间 $O(n^2)$。

正确性套路：

1. Termination：每轮产生一个新的 proposal，总数最多 $n^2$。
2. Perfect：若某个 man 未匹配，则某个 woman 未匹配；但该 man 必然已向所有 woman 求过，而 woman 一旦被求过就不会再 unmatched，矛盾。
3. Stability：若存在 unstable pair $Z,A$：
   - 若 $Z$ 没向 $A$ 求过，则 $Z$ 更喜欢当前对象，矛盾。
   - 若求过但被拒绝，$A$ 后来只会 trade up，所以 $A$ 更喜欢当前对象，矛盾。

### 2.3 最优性

男方 proposal 的 GS：

- 得到 man-optimal stable matching。
- 同时是 woman-pessimal stable matching。
- 若让 woman proposal，则反过来。

### 2.4 One-to-many

医院/学生版本：

- 医院有 capacity。
- 可能存在 unacceptable pairs。
- 人数可以不等。
- 扩展 GS 仍可求 stable matching。

选择题易错点：

- stable matching 可以不唯一。
- GS 的结果依赖哪一边 proposal，但同一 proposal side 下结果是 proposer-optimal。
- Rural hospital theorem：某些 rural hospitals 在所有 stable matching 中得到同样学生数量，甚至同样学生集合。

---

## 3. Greedy Algorithms

### 3.1 贪心证明模板

常见证明法：

1. Greedy stays ahead：证明贪心的第 $i$ 步至少不比任意最优解差。
2. Exchange argument：取一个与贪心前缀最长相同的最优解，把它的下一步替换为贪心选择，不变差，得到更长相同前缀，矛盾。
3. Structural bound：先证明任何解都至少需要某个下界，再证明贪心达到下界。
4. Safe choice + induction：证明贪心选择一定存在于某个最优解中，然后递归处理剩余问题。

大题写法：

$$
\text{贪心选择} \Rightarrow \text{可交换/不落后} \Rightarrow \text{归纳得到全局最优}
$$

### 3.2 Selecting Breakpoints

问题：固定路线、油箱容量 $c$，加油站位置 $b_i$，最少停车。

算法：每次开到当前能到达的最远加油站。

正确性：exchange。设贪心停靠 $g_0,g_1,\dots$，最优解 $f_0,f_1,\dots$ 与贪心前 $r$ 个相同且 $r$ 最大。贪心的 $g_{r+1}$ 是从 $g_r$ 能到的最远点，因此 $g_{r+1}\ge f_{r+1}$，把 $f_{r+1}$ 换成 $g_{r+1}$ 不会破坏可行性，矛盾。

### 3.3 Coin Changing

Cashier algorithm：每次取不超过剩余金额的最大面额。

注意：

- 对美国硬币 $1,5,10,25,100$ 是最优。
- 对任意面额不一定最优。例如 $1,10,21,34,70,100,\dots$，金额 $140$，贪心可能不是 $70+70$。

选择题常问：贪心不是天然正确，必须证明面额系统满足性质。

### 3.4 Interval Scheduling

目标：最多选互不重叠区间。

正确贪心：按 finish time $f_j$ 升序，能选就选。

错误贪心：

- earliest start time。
- shortest interval。
- fewest conflicts。

正确性 stays ahead：

令贪心选出的第 $i$ 个 job 结束时间为 $g_i$，任意最优解第 $i$ 个 job 结束时间为 $o_i$。归纳证明：

$$
f(g_i)\le f(o_i)
$$

所以贪心能选至少和最优一样多的 job。

复杂度：排序 $O(n\log n)$，扫描 $O(n)$。

### 3.5 Interval Partitioning

目标：用最少 classroom/resource 安排所有区间。

算法：

1. 按 start time 排序。
2. 用优先队列维护每个 classroom 当前最后结束时间。
3. 若最早空闲 classroom 的结束时间 $\le s_j$，复用；否则新开 classroom。

结构下界：任意时刻重叠的区间数 depth $d$，至少需要 $d$ 个 classroom。

贪心达到 $d$，所以最优。

复杂度：$O(n\log n)$。

### 3.6 Scheduling to Minimize Maximum Lateness

单机调度，每个任务处理时间 $t_j$，deadline $d_j$。目标最小化：

$$
L_{\max}=\max_j\{C_j-d_j\}
$$

算法：按 deadline 升序，即 Earliest Deadline First。

证明：exchange argument。若相邻两个任务 $i,j$ 满足 $d_i>d_j$ 却 $i$ 在 $j$ 前，则交换它们不会增加最大 lateness。重复消除 inversion，得到 EDF。

### 3.7 Caching

Offline caching 的最优算法：Farthest-in-Future。

规则：cache 满且 miss 时，驱逐下一次使用时间最远的 item，若再也不用则优先驱逐。

证明：exchange。把任意最优策略的第一次不同驱逐改成 FF 的驱逐，不增加 miss 数。

Online 常见策略：

- LRU：驱逐最近最少使用。
- LFU：驱逐频率最低。

注意：online 不知道未来，不能保证总是最优。

### 3.8 Dijkstra

适用：非负边权。

算法核心：每次从未确定点中取 $d[v]$ 最小的点 $v$，把它永久确定。

正确性：cut/stays ahead。由于所有边非负，任何经过未确定点再到 $v$ 的路径不会比当前 $d[v]$ 更短。

复杂度依实现：

- 数组：$O(n^2)$。
- binary heap：$O((m+n)\log n)$。
- Fibonacci heap：$O(m+n\log n)$。

易错点：有负边时 Dijkstra 不正确。

### 3.9 MST and Clustering

MST cut property：

对任意 cut，跨越该 cut 的最小权边是 safe edge，属于某棵 MST。

Kruskal：

- 按边权升序，若不成环则加入。
- 并查集实现 $O(m\log n)$。

Prim：

- 从一个点开始，每次加入连接树和外部的最小边。

Boruvka：

- 每个连通块同时选出向外的最小边。

$k$-clustering：

- 用 Kruskal 先合并到剩 $k$ 个 cluster。
- spacing 最大。

### 3.10 Directed MST, Chu-Liu/Edmonds

Minimum-cost arborescence：

- 有向图，根 $r$。
- 除 $r$ 外每个点恰好一条入边。
- 从 $r$ 能到所有点。

算法思路：

1. 对每个 $v\ne r$ 选最小入边。
2. 若无环，得到最优 arborescence。
3. 若有 0-cost cycle，收缩该环，递归求解，再展开。

复杂度：课件基本版 $O(mn)$，高级实现可 $O(m\log n)$。

### 3.11 Huffman Codes

Prefix code：任意字符编码都不是另一个字符编码的前缀。

目标最小化平均编码长度：

$$
ABL(T)=\sum_{x\in S} f_x\cdot depth_T(x)
$$

Huffman algorithm：

1. 每次取频率最低的两个符号 $y,z$。
2. 合并为 meta-letter $\omega$，频率 $f_\omega=f_y+f_z$。
3. 递归构造，再展开。

正确性关键：

- 存在某棵最优树，使两个最低频字符是最深层 siblings。
- 令 $T'$ 为合并后的树：

$$
ABL(T)=ABL(T')+f_\omega
$$

复杂度：优先队列 $O(n\log n)$。

### 3.12 Exam Greedy 题模板：区间匹配

Exam Paper 的交易匹配题可抽象为：旧交易 $j$ 对应区间

$$
I_j=[t_j-e_j,\ t_j+e_j]
$$

新交易时间是点 $m_i$。问是否每个区间能分配一个不同点。

贪心算法：

1. 将所有点 $m_i$ 升序排序。
2. 将所有区间按右端点 $R_j=t_j+e_j$ 升序排序。
3. 依次处理区间 $I_j=[L_j,R_j]$，给它分配当前未使用的最小点 $m_i$，要求 $m_i\ge L_j$。
4. 若该点不存在或 $m_i>R_j$，返回 false；否则分配并继续。

若用数组找下一个未用点，$O(n^2)$ 可接受；若用平衡树，$O(n\log n)$。

正确性 exchange：

考虑最早结束的区间 $I$。任何可行解必须给 $I$ 一个点 $x\in I$。贪心给它最小可用点 $g\in I$，且 $g\le x$。把可行解中分给 $I$ 的点换成 $g$，若 $g$ 原来分给了别的区间，则那个区间可改用 $x$，因为它的右端点不早于 $I$ 且 $x\ge g$。因此存在一个可行解与贪心第一步一致。归纳即可。

---

## 4. Divide and Conquer

### 4.1 Master Theorem

若：

$$
T(n)=aT(n/b)+\Theta(n^c)
$$

其中 $a\ge 1,b\ge 2,c\ge 0$，则：

1. 若 $c>\log_b a$：

$$
T(n)=\Theta(n^c)
$$

2. 若 $c=\log_b a$：

$$
T(n)=\Theta(n^c\log n)
$$

3. 若 $c<\log_b a$：

$$
T(n)=\Theta(n^{\log_b a})
$$

常见：

$$
2T(n/2)+O(n)=O(n\log n)
$$

$$
T(n/2)+O(1)=O(\log n)
$$

$$
3T(n/2)+O(n)=O(n^{\log_2 3})
$$

### 4.2 Counting Inversions

定义：数组 $a_1,\dots,a_n$ 中，若 $i<j$ 且 $a_i>a_j$，则 $(i,j)$ 是 inversion。

算法：改造 merge sort。

1. 分成左右两半。
2. 递归统计左、右内部 inversion。
3. merge 时统计 cross inversion。若右边元素 $b_j$ 小于左边当前 $a_i$，则左边剩余所有元素都与 $b_j$ 构成 inversion。

复杂度：

$$
T(n)=2T(n/2)+O(n)=O(n\log n)
$$

### 4.3 Closest Pair of Points

算法：

1. 按 $x$ 坐标分成左右各一半。
2. 递归求左右最近距离 $\delta_L,\delta_R$。
3. 令 $\delta=\min(\delta_L,\delta_R)$。
4. 只考虑距离中线 $L$ 不超过 $\delta$ 的 strip。
5. 按 $y$ 坐标排序，每个点只需检查后面 7 个点。

关键证明：

- 若最近跨越中线，则两个点都在宽度 $2\delta$ 的 strip 内。
- 在 $2\delta\times \delta$ 的矩形中分成 8 个小方格，每格边长 $\delta/2$，每格最多一个点，否则同侧已有更近点。
- 因此按 $y$ 排序后只需看常数个后继。

复杂度：若预先维护 $x$-sorted 和 $y$-sorted list：

$$
T(n)=2T(n/2)+O(n)=O(n\log n)
$$

### 4.4 Integer Multiplication

Grade-school：$O(n^2)$ bit operations。

Karatsuba：

令：

$$
x=x_1 2^{n/2}+x_0,\quad y=y_1 2^{n/2}+y_0
$$

普通分治需要 4 次乘法：

$$
xy=x_1y_1 2^n +(x_1y_0+x_0y_1)2^{n/2}+x_0y_0
$$

Karatsuba 用 3 次乘法：

$$
x_1y_0+x_0y_1=(x_1+x_0)(y_1+y_0)-x_1y_1-x_0y_0
$$

递推：

$$
T(n)=3T(n/2)+O(n)=O(n^{\log_2 3})
$$

### 4.5 Matrix Multiplication

普通算法 $O(n^3)$。

Strassen 用 7 次子矩阵乘法：

$$
T(n)=7T(n/2)+O(n^2)=O(n^{\log_2 7})\approx O(n^{2.807})
$$

选择题常问：Strassen 是渐进更快，但常数和数值稳定性会影响实践。

### 4.6 FFT and Polynomial Multiplication

多项式表示：

- coefficient representation：系数 $a_0,\dots,a_{n-1}$。
- point-value representation：$n$ 个点值 $(x_i,A(x_i))$。

乘法思路：

1. 用 FFT 将 $A,B$ 从 coefficient 转为 point-value。
2. 点值逐点相乘：

$$
C(x_i)=A(x_i)B(x_i)
$$

3. 用 inverse FFT 插值得到 $C$ 的系数。

FFT 核心：

$$
A(x)=A_{\text{even}}(x^2)+xA_{\text{odd}}(x^2)
$$

选择 $n$ 次单位根：

$$
\omega=e^{2\pi i/n},\quad x_k=\omega^k
$$

递推：

$$
T(n)=2T(n/2)+O(n)=O(n\log n)
$$

Inverse FFT 使用：

$$
\omega^{-1}=e^{-2\pi i/n}
$$

最后除以 $n$。

### 4.7 Exam D&C 题模板：完全二叉树局部最小

问题：complete binary tree，从 root 开始，每访问一个 node 才知道值，找 local minimum。local minimum 指其值不大于所有邻居，包括 parent 和 children。

算法：

1. 从 root 当前节点 $v$ 开始。
2. 若 $v$ 是叶子，返回 $v$。
3. 若 $v$ 的值 $\le$ 所有 children，返回 $v$。
4. 否则走向值更小的 child。
5. 重复。

正确性：

- 每一步都沿着值严格下降的边向下走。
- 因为树高是 $O(\log n)$，最终会停。
- 若停在内部节点，则它不大于 children；又因为它是从 parent 沿更小值走来的，所以它也小于 parent。
- 若停在叶子，则它小于 parent 或 root 本身无 parent，所以是 local minimum。

复杂度：complete binary tree 高度 $O(\log n)$，每层访问常数个节点，因此 $O(\log n)$。

---

## 5. Dynamic Programming

### 5.1 DP 答题模板

DP 大题固定五步：

1. State：$dp[\cdots]$ 表示什么。必须语义清楚。
2. Transition：从哪些更小状态转移。
3. Base case：边界值。
4. Order：按什么顺序填表，保证依赖已计算。
5. Complexity：状态数 $\times$ 每个状态转移成本。

若要求输出解，还要写 traceback。

### 5.2 Weighted Interval Scheduling

按 finish time 排序：

$$
f_1\le f_2\le \cdots\le f_n
$$

定义：

$$
p(j)=\max\{i<j:f_i\le s_j\}
$$

状态：

$$
OPT(j)=\text{只考虑前 }j\text{ 个 job 的最大权重}
$$

转移：

$$
OPT(j)=\max\{OPT(j-1),\ w_j+OPT(p(j))\}
$$

复杂度：

- 排序 $O(n\log n)$。
- 二分求所有 $p(j)$，$O(n\log n)$。
- DP $O(n)$。
- 总 $O(n\log n)$。

traceback：

- 若 $OPT(j)=OPT(j-1)$，不选 $j$，转到 $j-1$。
- 否则选 $j$，转到 $p(j)$。

### 5.3 Segmented Least Squares

给定点 $p_1,\dots,p_n$，希望用若干线段拟合，总成本：

$$
E+cL
$$

其中 $E$ 是所有 segment 的 SSE 总和，$L$ 是线段数。

预处理：

$$
e_{ij}=\text{点 }p_i,\dots,p_j\text{ 的最小 SSE}
$$

状态：

$$
OPT(j)=\text{拟合前 }j\text{ 个点的最小成本}
$$

转移：

$$
OPT(j)=\min_{1\le i\le j}\{OPT(i-1)+e_{ij}+c\}
$$

复杂度：

- 朴素预处理 $e_{ij}$：$O(n^3)$。
- 用前缀和可优化到 $O(n^2)$。
- DP 转移 $O(n^2)$。

### 5.4 Knapsack

0-1 knapsack：

物品 $i$ 重量 $w_i$，价值 $v_i$，容量 $W$。

状态：

$$
dp[i][x]=\text{只考虑前 }i\text{ 件物品，容量 }x\text{ 的最大价值}
$$

转移：

$$
dp[i][x]=
\begin{cases}
dp[i-1][x], & x<w_i\\
\max\{dp[i-1][x],\ dp[i-1][x-w_i]+v_i\}, & x\ge w_i
\end{cases}
$$

复杂度：$O(nW)$，这是 pseudo-polynomial，不是输入长度意义下的多项式，因为 $W$ 用二进制编码时输入长度是 $\log W$。

### 5.5 RNA Secondary Structure

典型 interval DP。

状态：

$$
OPT(i,j)=\text{子串 }i,\dots,j\text{ 的最大匹配数}
$$

转移思路：

- $j$ 不配对：

$$
OPT(i,j)=OPT(i,j-1)
$$

- $j$ 与某个 $t$ 配对：

$$
OPT(i,j)=\max_t\{1+OPT(i,t-1)+OPT(t+1,j-1)\}
$$

要求 $t,j$ 可以配对，且间距满足题目限制。

### 5.6 Sequence Alignment and Hirschberg

普通 alignment DP：

状态：

$$
OPT(i,j)=\text{前 }i\text{ 个字符与前 }j\text{ 个字符对齐的最小成本}
$$

转移：

$$
OPT(i,j)=\min
\begin{cases}
OPT(i-1,j-1)+\alpha_{x_i,y_j}\\
OPT(i-1,j)+\delta\\
OPT(i,j-1)+\delta
\end{cases}
$$

时间 $O(mn)$，空间 $O(mn)$。若只要值，可滚动数组 $O(n)$ 空间。Hirschberg 用 DP + divide and conquer 在 $O(mn)$ 时间、$O(m+n)$ 空间恢复解。

### 5.7 Shortest Paths with Negative Edges

Bellman-Ford DP：

令：

$$
OPT(i,v)=\text{从 }v\text{ 到 }t\text{ 使用至多 }i\text{ 条边的最短路长度}
$$

边界：

$$
OPT(0,t)=0,\quad OPT(0,v)=\infty\ (v\ne t)
$$

转移：

$$
OPT(i,v)=\min\left\{OPT(i-1,v),\ \min_{(v,w)\in E}\{OPT(i-1,w)+\ell_{vw}\}\right\}
$$

若无负环，最短简单路最多 $n-1$ 条边，因此 $n-1$ 轮足够。

复杂度：$O(mn)$，空间可用两层数组降为 $O(n)$。

SPFA 是优化实现，通常快，但最坏仍可能差。

### 5.8 Negative Cycle Detection

方法：

1. 加一个新 sink $t$，从所有点连一条 0 长度边到 $t$，保证每个点可达 $t$。
2. 跑 Bellman-Ford $n$ 或 $n+1$ 轮。
3. 若第 $n$ 轮仍有松弛，则存在 negative cycle。

原因：若 $OPT(n,v)<OPT(n-1,v)$，长度为 $n$ 的最短路径必有重复点，形成 cycle；若删掉该 cycle 会变短，说明 cycle 权重为负。

### 5.9 Exam DP 题模板：库存/仓储

问题：每月需求 $c_i$，仓库容量 $W$，若某月购买任意正数量，固定成本 $P$，库存每台每月费用 $F$。目标最小成本。

推荐状态：

$$
dp[i][j]=\text{处理完前 }i\text{ 个月后，仓库剩 }j\text{ 台的最小成本}
$$

其中 $0\le j\le W$。

设上月结束库存为 $k$，本月购买数量为：

$$
x=j+c_i-k
$$

要求 $x\ge 0$。若 $x=0$，本月不购买；若 $x>0$，付固定购买成本 $P$。

若仓储费按月末库存收取，转移为：

$$
dp[i][j]=\min_{0\le k\le W,\ j+c_i-k\ge 0}
\left\{
dp[i-1][k]+P\cdot \mathbf{1}_{j+c_i-k>0}+Fj
\right\}
$$

边界：

$$
dp[0][0]=0,\quad dp[0][j]=\infty\ (j>0)
$$

答案：

$$
\min_{0\le j\le W} dp[n][j]
$$

复杂度：状态 $O(nW)$，每个状态枚举 $k$，总 $O(nW^2)$，是 $n,W$ 的多项式。若题目把仓储费定义为“上月留下的库存本月收费”，把上式的 $Fj$ 改为 $Fk$ 即可。

---

## 6. Network Flow

### 6.1 基本定义

Flow network：

$$
G=(V,E,s,t,c)
$$

容量约束：

$$
0\le f(e)\le c(e)
$$

流守恒：

$$
\sum_{e\in \delta^-(v)} f(e)=\sum_{e\in \delta^+(v)} f(e),\quad v\ne s,t
$$

flow value：

$$
|f|=\sum_{e\in \delta^+(s)}f(e)-\sum_{e\in \delta^-(s)}f(e)
$$

s-t cut：

$$
(A,B),\quad s\in A,\ t\in B
$$

cut capacity：

$$
c(A,B)=\sum_{u\in A,\ v\in B} c(u,v)
$$

只算从 $A$ 到 $B$ 的边，不算 $B$ 到 $A$。

### 6.2 Residual Network

对边 $(u,v)$：

- forward residual capacity：

$$
c_f(u,v)=c(u,v)-f(u,v)
$$

- backward residual capacity：

$$
c_f(v,u)=f(u,v)
$$

Residual network $G_f$ 中只保留 residual capacity $>0$ 的边。

Augmenting path 的瓶颈：

$$
\Delta=\min_{e\in P} c_f(e)
$$

沿 path 增广 $\Delta$。

### 6.3 Ford-Fulkerson and Max-flow Min-cut

Ford-Fulkerson：

1. 初始 $f(e)=0$。
2. 若 residual network 中存在 $s$ 到 $t$ 的 path，则增广。
3. 直到没有 augmenting path。

定理：

以下等价：

1. $f$ 是最大流。
2. residual network 中不存在 augmenting path。
3. 存在 cut $(A,B)$，使 $|f|=c(A,B)$。

于是：

$$
\max_f |f|=\min_{(A,B)} c(A,B)
$$

整数容量定理：

若所有容量为整数，则存在整数最大流；Ford-Fulkerson 每次增广整数流。

复杂度：

- 若容量整数且最大流值为 $F$，每次至少增广 1，朴素为 $O(mF)$。
- Edmonds-Karp 用 BFS 找最短增广路，$O(nm^2)$。
- Dinic 通常更快，复杂度依图类而定。

### 6.4 Bipartite Matching

建图：

- 源点 $s$ 连左侧每个点，容量 1。
- 左侧到右侧按原边连，容量 1。
- 右侧每个点连汇点 $t$，容量 1。

最大匹配大小 = 最大流值。

若最大流为左侧点数，则有 perfect matching。

### 6.5 Disjoint Paths

Edge-disjoint paths：

- 每条边容量设为 1。
- 最大流值 = 最多边不相交路径数。

Vertex-disjoint paths：

- 把每个点 $v$ 拆成 $v_{in}\to v_{out}$，容量 1。
- 原边接 $u_{out}\to v_{in}$。

### 6.6 Circulation with Demands

没有固定源汇，每个点有 demand $d(v)$：

$$
\sum_{e\in \delta^-(v)}f(e)-\sum_{e\in \delta^+(v)}f(e)=d(v)
$$

- $d(v)>0$：需求点。
- $d(v)<0$：供应点。

必要条件：

$$
\sum_{v\in V} d(v)=0
$$

转化成 max flow：

1. 加 source $s$ 和 sink $t$。
2. 若 $d(v)<0$，加边 $s\to v$，容量 $-d(v)$。
3. 若 $d(v)>0$，加边 $v\to t$，容量 $d(v)$。
4. 原边保留。
5. 存在 circulation 当且仅当最大流能 saturate 所有从 $s$ 出发的供应边，也即流值：

$$
D=\sum_{d(v)>0}d(v)=\sum_{d(v)<0}-d(v)
$$

### 6.7 Lower Bounds

边有下界：

$$
\ell(e)\le f(e)\le c(e)
$$

处理方法：

1. 先让每条边发送 $\ell(e)$。
2. 新容量：

$$
c'(e)=c(e)-\ell(e)
$$

3. 更新端点 demand。对边 $e=(u,v)$：

$$
d'(u)=d(u)+\ell(e),\quad d'(v)=d(v)-\ell(e)
$$

然后按 circulation with demands 求可行性。

### 6.8 Survey Design

消费者 $i$ 被问问题数量在 $[c_i,c'_i]$，产品 $j$ 被问数量在 $[p_j,p'_j]$。

建 circulation with lower bounds：

- $s\to$ consumer $i$，容量区间 $[c_i,c'_i]$。
- consumer $i\to$ product $j$，若 $i$ 拥有 $j$，容量 $[0,1]$。
- product $j\to t$，容量区间 $[p_j,p'_j]$。
- $t\to s$，容量 $[0,\infty]$。

存在整数 circulation 等价于可行 survey。

### 6.9 Airline Scheduling

给 $k$ 个 flight，判断 $c$ 个 crew 是否够。

建图：

- 每个 flight $i$ 拆成 $u_i\to v_i$，容量 $[1,1]$，表示必须被一个 crew 服务。
- $s\to u_i$ 容量 $[0,1]$，crew 可以从任意 flight 开始。
- $v_i\to t$ 容量 $[0,1]$，crew 可以在任意 flight 结束。
- 若 flight $i$ 后能接 flight $j$，加 $v_i\to u_j$ 容量 $[0,1]$。
- $s$ supply $c$，$t$ demand $c$，或用等价 circulation 表示。

二分最小 $c$，复杂度课件给出 $O(k^3\log k)$。

### 6.10 Image Segmentation / Maximum Closure 模板

常见目标：

$$
\max_{S\subseteq V}\left(\sum_{i\in S} p_i-\sum_{(i,j)\text{ separated}} d_{ij}\right)
$$

建 min-cut：

- $s\to i$ 容量 $p_i$。若 $i$ 不选，会 cut 这条边，损失 $p_i$。
- 对相互作用 pair $(i,j)$，加 $i\to j$ 和 $j\to i$，容量 $d_{ij}$。若一个选一个不选，会 cut 一条，付 penalty。
- 强制 $i$ 不选：加 $i\to t$ 容量 $\infty$。
- 强制 $i$ 选：加 $s\to i$ 容量 $\infty$。

若 cut 的 $s$ 侧为 $S$，则：

$$
c(A,B)=\sum_{i\notin S}p_i+\sum_{(i,j)\text{ separated}}d_{ij}+\text{forced penalty}
$$

因此最大收益：

$$
\sum_i p_i-\mincut
$$

### 6.11 Exam Flow 题模板：软件迁移

题意：软件 $i$ 迁移到系统 B 获得收益 $p_i$。若 pair $i,j$ 只有一个迁移，损失 $d_{ij}$。软件 1 不能迁移。最大化净收益。

令 $S$ 为迁移的软件集合：

$$
\max_S\left(\sum_{i\in S}p_i-\sum_{i\in S,j\notin S}d_{ij}\right)
$$

建图：

1. 对每个软件建点 $i$，加 $s,t$。
2. 加边 $s\to i$，容量 $p_i$。
3. 对每个 pair $(i,j)$，加两条有向边 $i\to j$ 和 $j\to i$，容量 $d_{ij}$。
4. 为强制软件 1 不迁移，加边 $1\to t$，容量 $M$，其中 $M$ 大于所有可能有限 cut 容量，或记为 $\infty$。
5. 求最小 $s$-$t$ cut。令 $S$ 为 $s$ 侧软件点。

正确性：

- 若 $i\notin S$，边 $s\to i$ 被 cut，表示放弃收益 $p_i$。
- 若 $i,j$ 分属两侧，pair 边恰好贡献一次 $d_{ij}$，表示割裂损失。
- 若 $1\in S$，边 $1\to t$ 被 cut，代价无穷，因此最优 cut 不会选软件 1。
- 因为总收益 $\sum_i p_i$ 是常数，最小化 cut 等价于最大化净收益。

---

## 7. Computational Intractability

### 7.1 Reduction

Cook reduction：

问题 $X$ polynomial-time reduces to $Y$，记作：

$$
X\le_P Y
$$

含义：若有 $Y$ 的 oracle，可以用多项式时间和多项式次 oracle call 解 $X$。

Karp reduction：

把 $X$ 的任意实例 $x$ 多项式时间转成 $Y$ 的实例 $y$，满足：

$$
x\in X \iff y\in Y
$$

考试大多默认 Karp reduction。

方向易错：

- 要证明新问题 $Y$ 很难，应从已知难问题 $X$ 归约到 $Y$：

$$
X\le_P Y
$$

- 意思是：如果 $Y$ 容易，则 $X$ 也容易。因此 $X$ 难推出 $Y$ 难。

### 7.2 Independent Set and Vertex Cover

Independent set：

$$
S\subseteq V,\quad \forall (u,v)\in E,\ \{u,v\}\nsubseteq S
$$

Vertex cover：

$$
C\subseteq V,\quad \forall (u,v)\in E,\ u\in C\ \text{or}\ v\in C
$$

核心等价：

$$
S\text{ is independent} \iff V-S\text{ is vertex cover}
$$

因此：

$$
G\text{ has IS of size }\ge k
\iff
G\text{ has VC of size }\le |V|-k
$$

所以：

$$
INDEPENDENT\text{-}SET\equiv_P VERTEX\text{-}COVER
$$

### 7.3 Vertex Cover to Set Cover

SET-COVER：

给 universe $U$，集合族 $\mathcal{S}$，问能否选 $\le k$ 个集合覆盖 $U$。

归约：

- 给 VC 实例 $G=(V,E),k$。
- 构造 $U=E$。
- 对每个顶点 $v\in V$ 构造集合：

$$
S_v=\{e\in E:e\text{ incident to }v\}
$$

则：

$$
G\text{ has vertex cover of size }k
\iff
(U,\mathcal{S})\text{ has set cover of size }k
$$

### 7.4 3-SAT to Independent Set

3-SAT：

$$
\Phi=C_1\land C_2\land \cdots\land C_k
$$

每个 clause 有 3 个 literal。

构造：

1. 每个 clause 建一个 triangle，三个点对应三个 literal。
2. 同一 triangle 内三点互连，保证最多选一个。
3. 对互为否定的 literal 点连边，保证不同时选。
4. 令目标 independent set 大小为 $k$。

证明：

- 若 $\Phi$ 可满足，每个 clause 选一个 true literal。不会选同一 clause 两个点，也不会选互相矛盾 literal，因此得到大小 $k$ independent set。
- 若图有大小 $k$ independent set，因为每个 triangle 最多选一个，而共有 $k$ 个 triangle，所以每个 clause 恰好选一个 literal；且没有矛盾 literal，可赋值使这些 literal 为 true，因此公式满足。

### 7.5 P, NP, NP-hard, NP-complete

P：可多项式时间求解的 decision problems。

NP：给定 certificate 后可多项式时间验证 yes 的 decision problems。

NP-hard：

$$
\forall X\in NP,\ X\le_P Y
$$

$Y$ 不一定在 NP，甚至可能不是 decision problem。

NP-complete：

$$
Y\in NP\quad\text{and}\quad \forall X\in NP,\ X\le_P Y
$$

证明 $Y$ NP-complete 三步：

1. 证明 $Y\in NP$。
2. 选已知 NP-complete 问题 $X$。
3. 证明 $X\le_P Y$。

若 $Y$ NP-complete 且 $Y\in P$，则：

$$
P=NP
$$

### 7.6 Circuit-SAT and 3-SAT

Cook-Levin：

$$
CIRCUIT\text{-}SAT\text{ is NP-complete}
$$

3-SAT is NP-complete：

- 为 circuit 中每个 wire/gate 建变量。
- 用 clauses 强制每个 gate 的输入输出关系。
- 强制 output 为 true。
- 把短 clause 转成长度 3 的 clause。

选择题重点：

- 3-SAT 是 NP-complete。
- 2-SAT 在 P。
- Independent Set、Vertex Cover、Set Cover、Hamiltonian Cycle、TSP decision、Subset Sum、Knapsack decision 是经典 NP-complete。
- Factoring 目前未知是否在 P，也不认为是 NP-complete。

### 7.7 Search vs Decision

Exam Paper 中 VERTEX-COVER 和 FIND-VERTEX-COVER 等价。

方向 1：Search $\Rightarrow$ Decision。

若能找大小 $k$ 的 vertex cover，直接调用 find，找到则 yes，找不到则 no。

方向 2：Decision $\Rightarrow$ Search。

给 decision oracle $VC(G,k)$。

算法：

1. 先问 $VC(G,k)$，若 no 则不存在。
2. 对每个顶点或边逐步固定选择：
   - 典型 edge branching：取一条边 $(u,v)$。任何 vertex cover 必含 $u$ 或 $v$。
   - 问 oracle：$G-u$ 是否有大小 $k-1$ 的 cover。
   - 若 yes，选择 $u$，令 $G=G-u,k=k-1$。
   - 否则选择 $v$，令 $G=G-v,k=k-1$。
3. 重复直到所有边被覆盖。

每步图规模减少，多项式次 oracle call，因此 search 和 decision 多项式等价。

注意：如果要更严谨处理“必须包含某顶点”，可删除该顶点及其 incident edges，并把 $k$ 减 1。

---

## 8. Randomized Algorithms

### 8.1 基础工具

Linearity of expectation：

$$
E\left[\sum_i X_i\right]=\sum_i E[X_i]
$$

不要求独立。

Union bound：

$$
Pr\left[\bigcup_i A_i\right]\le \sum_i Pr[A_i]
$$

Chernoff bounds：

若 $X=\sum_i X_i$，$X_i$ 独立且取 $0/1$，$\mu=E[X]$。

上尾：

$$
Pr[X>(1+\delta)\mu]<
\left(\frac{e^\delta}{(1+\delta)^{1+\delta}}\right)^\mu
$$

下尾：

$$
Pr[X<(1-\delta)\mu]<e^{-\mu\delta^2/2}
$$

### 8.2 Monte Carlo vs Las Vegas

Monte Carlo：

- 多项式时间保证。
- 答案可能错，但错误概率可控。
- 例：Karger contraction algorithm。

Las Vegas：

- 答案一定正确。
- 运行时间是期望多项式。
- 例：randomized quicksort/quickselect。

可将 Las Vegas 截断成 Monte Carlo；一般不能把 Monte Carlo 转成 Las Vegas。

### 8.3 Contention Resolution

$n$ 个进程无通信抢数据库。每轮每个进程以概率：

$$
p=\frac{1}{n}
$$

尝试访问。

某个进程 $i$ 在某轮成功概率：

$$
Pr[S(i,t)]=\frac{1}{n}\left(1-\frac{1}{n}\right)^{n-1}
$$

有界：

$$
\frac{1}{en}\le Pr[S(i,t)]\le \frac{1}{2n}
$$

经过 $en\cdot c\ln n$ 轮，该进程失败概率 $\le n^{-c}$。Union bound 得到所有进程在 $2en\ln n$ 轮内成功的概率至少 $1-\frac{1}{n}$。

### 8.4 Randomized Quickselect

目标：找第 $k$ 小。

算法：

1. 随机选 pivot $p$。
2. 三路划分 $L,M,R$。
3. 只递归进入包含第 $k$ 小的那一边。

期望复杂度：

$$
E[T(n)]=O(n)
$$

证明要点：随机 pivot 使期望递归规模可控。课件给出强归纳结论：

$$
T(n)\le 4n
$$

选择题易错：quickselect 最坏时间仍是 $O(n^2)$，但期望 $O(n)$。

### 8.5 Karger Global Min Cut

问题：无向连通图中找全局最小 cut。

Contraction algorithm：

1. 均匀随机选一条边。
2. 收缩该边，保留平行边，删除自环。
3. 直到只剩两个 supernodes。
4. 返回这两个 supernodes 对应 cut。

成功概率：

若固定某个 min cut $F^\*$，大小为 $k$。每个点度数至少 $k$，所以：

$$
2|E|\ge kn
$$

第一步选中 cut 边的概率：

$$
\frac{k}{|E|}\le \frac{2}{n}
$$

完整不收缩 $F^\*$ 的概率至少：

$$
\prod_{r=n}^{3}\left(1-\frac{2}{r}\right)=\frac{2}{n(n-1)}\ge \frac{2}{n^2}
$$

重复 $n^2\ln n$ 次后失败概率：

$$
\left(1-\frac{2}{n^2}\right)^{n^2\ln n}\le \frac{1}{n^2}
$$

### 8.6 Load Balancing

$m$ 个 jobs 随机分给 $n$ 个 processors。

若 $m=n$，每个 processor 的 load $X_i$：

$$
E[X_i]=1
$$

用 Chernoff + union bound 可得最大 load 高概率为：

$$
O\left(\frac{\log n}{\log\log n}\right)
$$

若 $m=16n\ln n$，平均 load：

$$
\mu=16\ln n
$$

Chernoff 可证明所有 processor 的 load 高概率在：

$$
\left[\frac{\mu}{2},2\mu\right]
$$

内，概率至少 $1-\frac{2}{n}$。

### 8.7 MAX 3-SAT

随机算法：每个变量独立以概率 $1/2$ 赋 true。

对任意 clause $C_j$，三个 literal 都 false 的概率为：

$$
\frac{1}{8}
$$

所以被满足概率：

$$
Pr[C_j\text{ satisfied}]=\frac{7}{8}
$$

令 $Z_j$ 是 clause $j$ 是否满足的 indicator，$Z=\sum_j Z_j$。

由线性期望：

$$
E[Z]=\sum_j E[Z_j]=\frac{7k}{8}
$$

因此存在一个 assignment 满足至少 $\frac{7k}{8}$ 个 clauses。

Johnson algorithm：重复随机生成 assignment，直到满足至少 $7k/8$ 个 clauses。课件给出成功概率至少 $1/(8k)$，所以期望 trial 数 $O(k)$。

### 8.8 Exam Randomized 题模板：四染色满足边

题意：给图 $G=(V,E)$，给每个点染 4 种颜色。若边两端颜色不同，则边 satisfied。要求至少 $\frac{3}{4}$ 的边 satisfied。

随机算法：

1. 每个点独立均匀随机选择 $\{1,2,3,4\}$ 中一种颜色。
2. 对每条边 $e=(u,v)$，定义 indicator：

$$
X_e=
\begin{cases}
1,& color(u)\ne color(v)\\
0,& color(u)=color(v)
\end{cases}
$$

3. 因为：

$$
Pr[color(u)=color(v)]=\frac{1}{4}
$$

所以：

$$
E[X_e]=Pr[X_e=1]=\frac{3}{4}
$$

4. 总满足边数：

$$
X=\sum_{e\in E}X_e
$$

线性期望：

$$
E[X]=\sum_{e\in E}E[X_e]=\frac{3}{4}|E|
$$

因此存在某个染色满足至少 $\frac{3}{4}|E|$ 条边。

若题目要求“算法保证找到”，可写 conditional expectation 去随机化：

1. 按任意顺序给点染色。
2. 对当前点尝试 4 种颜色，选择使剩余随机染色下的条件期望最大的颜色。
3. 因为 4 种颜色的平均条件期望等于当前条件期望，至少有一种颜色不降低期望。
4. 最终得到确定染色，满足边数至少初始期望 $\frac{3}{4}|E|$。

---

## 9. 六道大题答题骨架

### 9.1 Greedy

写法：

1. 排序准则是什么。
2. 每一步选择什么。
3. 为什么选择是 safe。
4. 复杂度。

常用句：

设 $G$ 为贪心解，$O$ 为最优解。令 $r$ 为二者最长公共前缀长度。由于贪心在第 $r+1$ 步选择了当前可行的最优局部对象，可以将 $O$ 的第 $r+1$ 个选择替换为贪心选择而不破坏可行性，也不降低目标值。这与 $r$ 最大矛盾。因此存在最优解与贪心完全一致。

### 9.2 Divide and Conquer

写法：

1. 说明如何 divide。
2. 说明递归处理哪些子问题。
3. 说明 combine 如何不漏答案。
4. 写 recurrence 并解出复杂度。

常用句：

答案要么完全落在某个子问题中，要么跨越分界。前者由递归保证正确，后者由 combine step 穷尽所有可能候选。因此取三者最优即为全局最优。

### 9.3 Dynamic Programming

写法：

1. $dp$ 语义。
2. base case。
3. transition。
4. table order。
5. answer and complexity。

常用句：

最优解的最后一步只有以下几种互斥情况。删除最后一步后，剩余部分必须是相应子问题的最优解，否则可替换为更优子解从而改进原解，矛盾。

### 9.4 Reduction

写法：

1. 明确要证明 $X\le_P Y$ 还是 $X\equiv_P Y$。
2. 给 construction。
3. 证明 iff。
4. 说明 construction 是 polynomial。

常用句：

给定 $X$ 的任意实例 $x$，我们在多项式时间构造 $Y$ 的实例 $y$。下面证明 $x$ 是 yes-instance 当且仅当 $y$ 是 yes-instance。

若证明等价：

$$
X\le_P Y\quad\text{and}\quad Y\le_P X
$$

两个方向都要写。

### 9.5 Network Flow

写法：

1. 变量代表什么：选/不选，流过/不流过。
2. 节点和边怎么建。
3. 容量为什么这样设。
4. cut 或 flow 与原问题目标的对应关系。
5. integrality 或 max-flow min-cut 保证解可解释。

常用句：

对任意可行方案，可构造一个 cut，其容量等于常数减去方案收益；反过来，对任意 cut，也可读出一个可行方案，且目标值与 cut 容量满足同一关系。因此最小 cut 对应最大收益方案。

### 9.6 Randomized

写法：

1. 随机选择规则。
2. 定义 indicator random variables。
3. 算单个事件概率。
4. 用线性期望/union bound/Chernoff 合并。
5. 若要高概率，写 repetition/amplification。

常用句：

令 $X_i$ 表示第 $i$ 个对象是否成功。虽然这些变量不一定独立，但线性期望仍然成立：

$$
E\left[\sum_i X_i\right]=\sum_i E[X_i]
$$

所以期望达到目标值。由 probabilistic method，至少存在一个随机选择结果达到该期望。

---

## 10. 选择题快速自测

1. $T(n)=32n^2+17n+32$ 是 $\Theta(n^2)$，也是 $O(n^3)$ 和 $\Omega(n)$。
2. GS 中 proposer side 得到最优 stable matching。
3. Interval scheduling 应按 finish time 贪心。
4. Weighted interval scheduling 不能用普通 interval scheduling 的贪心。
5. Dijkstra 需要非负边权。
6. Huffman 每次合并最低频两个符号。
7. FFT 用 $n$ 次单位根，把 polynomial evaluation 降为 $O(n\log n)$。
8. Bellman-Ford 可处理负边，但若存在 reachable negative cycle，则最短路可能不存在。
9. Max-flow value 等于 min-cut capacity。
10. 有整数容量时，存在整数最大流。
11. Bipartite matching 可转成 max flow。
12. Circulation with lower bounds 先扣掉 lower bound，再更新 demand。
13. 要证明新问题 NP-hard，从已知 NP-hard/NP-complete 问题归约到新问题。
14. NP-complete 必须属于 NP，NP-hard 不一定属于 NP。
15. 3-SAT 是 NP-complete，2-SAT 在 P。
16. Search version 和 decision version 很多时候可多项式等价，但要说明如何用 oracle 恢复解。
17. Linearity of expectation 不需要独立。
18. Union bound 不需要独立。
19. Chernoff bound 需要独立的 $0/1$ 变量或相应条件。
20. Monte Carlo 时间有保证但可能错；Las Vegas 答案正确但时间随机。

---

## 11. 最后一天复习建议

按优先级：

1. 背六类大题模板，尤其是 DP 和 Flow 的建模语句。
2. 每章至少能默写一个经典算法和证明关键词。
3. 对 Exam Paper 六题各写一遍英文/中文提纲，重点是 correctness。
4. 选择题刷定义和反例：错误贪心、负边 Dijkstra、NP 归约方向、Monte Carlo/Las Vegas。

考场上，如果题目很新，先问自己：

- 这是“选一批对象”吗？可能是 greedy、DP、flow cut、set cover。
- 有“两个集合配对/容量”吗？可能是 matching/flow/circulation。
- 有“最后一步/第 $i$ 个月/容量 $W$”吗？大概率 DP。
- 有“证明两个问题等价/用一个算法解另一个”吗？reduction。
- 有“随机染色/随机赋值/随机选边”吗？indicator + expectation。
