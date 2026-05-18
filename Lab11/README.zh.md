# Lab 11B — 数据中心服务器操作系统选择问题

## 目录

1. [问题重述](#问题重述)
2. [设计动机：为什么用最小割](#设计动机为什么用最小割)
3. [数学原理](#数学原理)
   - 3.1 二元标记问题的能量函数框架
   - 3.2 子模性（Submodularity）
   - 3.3 Kolmogorov-Zabih 定理
   - 3.4 本题能量函数的严格推导
4. [建图详解](#建图详解)
   - 4.1 单点项的建图
   - 4.2 成对项的建图：辅助节点法
   - 4.3 INF 边的数学意义
5. [Dinic 算法流程](#dinic-算法流程)
6. [正确性证明](#正确性证明)
7. [复杂度分析](#复杂度分析)
8. [输入输出格式](#输入输出格式)
9. [参考实现](#参考实现)

---

## 问题重述

给定一个 $n \times m$ 的服务器网格，每个服务器必须安装 **Linux** 或 **Windows** 操作系统。

**单点收益：**
- 服务器 $(i, j)$ 安装 Linux 获得基础分 $L_{i,j}$
- 服务器 $(i, j)$ 安装 Windows 获得基础分 $W_{i,j}$

**相邻奖励：**
- 若两个**垂直相邻**服务器都装 Linux，额外获得 $VL_{i,j}$
- 若两个**垂直相邻**服务器都装 Windows，额外获得 $VW_{i,j}$
- 若两个**水平相邻**服务器都装 Linux，额外获得 $HL_{i,j}$
- 若两个**水平相邻**服务器都装 Windows，额外获得 $HW_{i,j}$

目标：最大化总收益。

---

## 设计动机：为什么用最小割

### 直接搜索不可行

每个服务器有 2 种选择，共 $2^{nm}$ 种方案。当 $n, m$ 达到几十时，枚举完全不可行。

### 贪心/动态规划行不通

相邻奖励使得服务器之间存在**耦合**：一个服务器的选择会影响邻居的收益。这种全局依赖关系破坏了贪心选择性质，也使得 DP 的状态设计极为困难（需要记录边界上的所有选择）。

### 关键观察：这是一个二元标记问题

每个服务器只有两种状态（Linux / Windows），且收益只依赖于：
1. 单个服务器的选择（一元项）
2. 相邻一对服务器的选择（二元项）

这种结构在计算机视觉中被称为 **二元标记问题（Binary Labeling）**。而本题的特殊之处在于：所有二元项都**奖励同侧选择**，这恰好对应一种良好的数学性质——**子模性（Submodularity）**。

### 子模性的威力

Kolmogorov 和 Zabih 在 2004 年证明了：

> **一个二元标记问题能被精确归约为最小 s-t 割问题，当且仅当其所有成对势函数满足子模性。**

这意味着，一旦我们验证了子模性，就可以构造一个图，使得：**最小割对应最优解**。

---

## 数学原理

### 3.1 二元标记问题的能量函数框架

对每个服务器 $(i, j)$，引入二元变量：

$$
x_{i,j} = \begin{cases} 0 & \text{安装 Linux} \\ 1 & \text{安装 Windows} \end{cases}
$$

总得分可以写成：

$$
\text{Score}(x) = \underbrace{\sum_{i,j} \left[(1-x_{i,j})L_{i,j} + x_{i,j}W_{i,j}\right]}_{\text{单点项}} + \underbrace{\sum_{\text{相邻对}} \text{bonus}(x_i, x_j)}_{\text{成对项}}
$$

为了使用最小割框架，我们进行等价变换。令 $C$ 为**所有输入值的总和**（假设所有奖励都能拿到），定义**能量函数**：

$$
E(x) = C - \text{Score}(x)
$$

于是：

$$
\max \text{Score}(x) \quad \Longleftrightarrow \quad \min E(x)
$$

### 3.2 子模性（Submodularity）

对于一对相邻变量 $(x, y)$，其成对能量 $V(x, y)$ 满足**子模性**，当且仅当：

$$
V(0, 0) + V(1, 1) \leq V(0, 1) + V(1, 0)
$$

**直观含义**：两个变量取相同值的"代价"，不大于取不同值的"代价"之和。换句话说，**同侧有优惠**。

### 3.3 Kolmogorov-Zabih 定理

> **定理**：二元标记问题 $E(x) = \sum_i D_i(x_i) + \sum_{i<j} V_{ij}(x_i, x_j)$ 能被一个 s-t 割精确表示，当且仅当每个 $V_{ij}$ 都是子模的。

这一定理给出了**充分必要条件**，是图割方法的理论基石。

### 3.4 本题能量函数的严格推导

#### 单点项

对于服务器 $(i, j)$，常数 $C$ 中包含 $L_{i,j} + W_{i,j}$，实际得分包含 $(1-x_{i,j})L_{i,j} + x_{i,j}W_{i,j}$。

因此单点能量：

$$
\begin{aligned}
D_{ij}(x) &= L_{i,j} + W_{i,j} - [(1-x)L_{i,j} + x W_{i,j}] \\
&= (1-x)W_{i,j} + x L_{i,j}
\end{aligned}
$$

即：
- 选 Linux ($x=0$)：能量 = $W_{i,j}$（放弃 Windows 基础分）
- 选 Windows ($x=1$)：能量 = $L_{i,j}$（放弃 Linux 基础分）

#### 成对项（以垂直相邻为例）

设相邻对为 $(A, B)$，Linux 奖励为 $a$，Windows 奖励为 $b$。常数 $C$ 中包含 $a + b$，实际得分包含 $(1-x_A)(1-x_B)a + x_A x_B b$。

因此成对能量：

$$
V(x_A, x_B) = a + b - [(1-x_A)(1-x_B)a + x_A x_B b]
$$

展开四种情况：

| $(x_A, x_B)$ | 得分 | 能量 $V(x_A, x_B)$ |
|:-----------:|:----:|:------------------:|
| $(0, 0)$ | $a$ | $b$ |
| $(0, 1)$ | $0$ | $a + b$ |
| $(1, 0)$ | $0$ | $a + b$ |
| $(1, 1)$ | $b$ | $a$ |

#### 验证子模性

$$
V(0,0) + V(1,1) = b + a = a + b
$$

$$
V(0,1) + V(1,0) = (a+b) + (a+b) = 2(a+b)
$$

由于 $a, b \geq 0$：

$$
a + b \leq 2(a + b) \quad \checkmark
$$

**严格满足子模不等式！** 水平相邻同理。

因此，由 Kolmogorov-Zabih 定理，本题可被精确归约为最小割。

---

## 建图详解

### 4.1 单点项的建图

对每个服务器节点 $u = \text{cell}(i,j)$：

- 源点 $S \to u$，容量为 $L_{i,j}$
  - 若 $u$ 被划分到 $T$ 侧（选 Windows），该边被割，付出代价 $L_{i,j}$
- $u \to$ 汇点 $T$，容量为 $W_{i,j}$
  - 若 $u$ 被划分到 $S$ 侧（选 Linux），该边被割，付出代价 $W_{i,j}$

### 4.2 成对项的建图：辅助节点法

对于一对相邻服务器 $(A, B)$ 和奖励值 $(a, b)$，**引入两个辅助节点** $v_a$（Linux 奖励节点）和 $v_b$（Windows 奖励节点）。

**Linux 奖励 $a$：**

$$
S \xrightarrow{a} v_a \xrightarrow{\infty} A, \quad S \xrightarrow{a} v_a \xrightarrow{\infty} B
$$

**Windows 奖励 $b$：**

$$
A \xrightarrow{\infty} v_b \xrightarrow{b} T, \quad B \xrightarrow{\infty} v_b \xrightarrow{b} T
$$

### 4.3 INF 边的数学意义

设 $\infty$ 是一个大于任何可能答案的数（实现中取 $10^{18}$）。

**分析 Linux 奖励节点 $v_a$：**

| $A$ 的选择 | $B$ 的选择 | $v_a$ 最优位置 | 割掉的边 | 代价 |
|:---------:|:---------:|:-------------:|:--------:|:----:|
| Linux ($S$) | Linux ($S$) | $S$ 侧 | 无 | $0$ |
| Linux ($S$) | Windows ($T$) | $T$ 侧（若留 $S$ 则 $v_a \to B$ 被割，代价 $\infty$）| $S \to v_a$ | $a$ |
| Windows ($T$) | Linux ($S$) | $T$ 侧 | $S \to v_a$ | $a$ |
| Windows ($T$) | Windows ($T$) | $T$ 侧 | $S \to v_a$ | $a$ |

结论：**仅当 $A, B$ 都选 Linux 时，才不付出代价 $a$；否则必须放弃该奖励。**

**分析 Windows 奖励节点 $v_b$（对称）：**

| $A$ 的选择 | $B$ 的选择 | $v_b$ 最优位置 | 割掉的边 | 代价 |
|:---------:|:---------:|:-------------:|:--------:|:----:|
| Linux ($S$) | Linux ($S$) | $S$ 侧（若留 $T$ 则 $A \to v_b$ 被割，代价 $\infty$）| $v_b \to T$ | $b$ |
| Linux ($S$) | Windows ($T$) | $S$ 侧 | $v_b \to T$ | $b$ |
| Windows ($T$) | Linux ($S$) | $S$ 侧 | $v_b \to T$ | $b$ |
| Windows ($T$) | Windows ($T$) | $T$ 侧 | 无 | $0$ |

结论：**仅当 $A, B$ 都选 Windows 时，才不付出代价 $b$。**

### 完整建图总结

| 组件 | 边 | 容量 | 含义 |
|-----|---|------|------|
| Linux 基础分 | $S \to \text{cell}(i,j)$ | $L_{i,j}$ | 选 Windows 时放弃 $L$ |
| Windows 基础分 | $\text{cell}(i,j) \to T$ | $W_{i,j}$ | 选 Linux 时放弃 $W$ |
| 垂直 Linux 奖励 | $S \to v_{VL}$，$v_{VL} \to A$，$v_{VL} \to B$ | $VL$，$\infty$，$\infty$ | 都选 Linux 才保留 |
| 垂直 Windows 奖励 | $A \to v_{VW}$，$B \to v_{VW}$，$v_{VW} \to T$ | $\infty$，$\infty$，$VW$ | 都选 Windows 才保留 |
| 水平 Linux 奖励 | 同垂直 | | |
| 水平 Windows 奖励 | 同垂直 | | |

---

## Dinic 算法流程

Dinic 算法通过**分层图**和**阻塞流**来高效求解最大流。

```
初始化：所有边流量为 0

while (从 S 出发的 BFS 能到达 T) {
    构建分层图（level[] 数组）
    重置当前弧指针 ptr[]
    while (能从 S 向 T 推送流量) {
        在分层图上进行 DFS 多路增广
        更新残量网络
    }
}

返回总流量 = 最小割容量
```

**最终答案：**

$$
\text{answer} = \text{total\_sum} - \text{max\_flow}
$$

---

## 正确性证明

**定理**：上述算法输出的值等于最大总收益。

**证明：**

1. **构造完备性**：由第 4 节的分析，图中每个 $s-t$ 割唯一对应一种操作系统分配方案，且割容量等于该方案下放弃的总收益。

2. **割容量计算**：
   - 单点项：选 Linux 割 $W$，选 Windows 割 $L$，恰好等于放弃的基础分。
   - 成对项：由 4.3 节的分析，同侧保留奖励（代价 0），异侧放弃奖励（代价为对应奖励值）。

3. **常数关系**：设 $C$ 为所有输入值之和，则对任意方案 $x$：
   $$
   \text{Score}(x) = C - \text{cap}(\text{cut}(x))
   $$

4. **最优性**：
   $$
   \max_x \text{Score}(x) = \max_x [C - \text{cap}(\text{cut}(x))] = C - \min_{\text{cut}} \text{cap}(\text{cut})
   $$

   由最大流最小割定理，Dinic 算法求出的 `max_flow` 等于最小割容量。

5. 因此 `total_sum - max_flow` 等于最大总收益。$\square$

---

## 复杂度分析

- **节点数**：$O(nm)$ 个服务器节点 + $O(nm)$ 个奖励辅助节点
- **边数**：每个服务器 2 条边（连 S 和 T）；每个奖励辅助节点 3 条边；总边数 $O(nm)$
- **Dinic 时间复杂度**：
  - 一般情况：$O(V^2 E)$
  - 单位容量网络：$O(\min(V^{2/3}, E^{1/2}) \cdot E)$
  - 本题实际运行效率很高，可轻松处理 $n, m \leq 100$ 规模的网格

---

## 输入输出格式

### 输入

```
n m
L[0][0] L[0][1] ... L[0][m-1]
...
L[n-1][0] ... L[n-1][m-1]

W[0][0] ... W[0][m-1]
...
W[n-1][0] ... W[n-1][m-1]

VL[0][0] ... VL[0][m-1]      // (n-1) 行
...
VL[n-2][0] ... VL[n-2][m-1]

VW[0][0] ... VW[0][m-1]      // (n-1) 行
...

HL[0][0] ... HL[0][m-2]      // n 行，每行 (m-1) 个
...

HW[0][0] ... HW[0][m-2]      // n 行，每行 (m-1) 个
...
```

### 输出

一个整数，表示最大总收益。

---

## 参考实现

见 [lab11B.cpp](lab11B.cpp)。核心要点：

- 使用 `long long` 防止溢出，`INF = 1e18`
- 节点编号：$S = 0, T = 1$，服务器从 2 开始编号
- 奖励辅助节点动态分配（`node_cnt++`）
- 使用当前弧优化（`ptr` 数组）加速 DFS

### 可选扩展：恢复具体分配方案

在求完最大流后，从 $S$ 出发在**残量网络**（`cap - flow > 0` 的边）上做一次 BFS，能访问到的服务器节点即为 **Linux**，访问不到的为 **Windows**。

---

## 参考资料

1. Kolmogorov, V., & Zabih, R. (2004). *What energy functions can be minimized via graph cuts?*. IEEE TPAMI, 26(2), 147-159.
2. Boykov, Y., & Kolmogorov, V. (2004). *An experimental comparison of min-cut/max-flow algorithms for energy minimization in vision*. IEEE TPAMI, 26(9), 1124-1137.
