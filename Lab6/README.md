# Lab6

## `lab6A.cpp` 对拍说明

### 1) 编译

```powershell
g++ -O2 -std=c++17 .\lab6A.cpp -o .\lab6A.exe
```

### 2) 运行随机暴力对拍

```powershell
python .\stress_test_lab6A.py --exe .\lab6A.exe --rounds 2000 --max-n 80 --coord 1000
```

### 3) 常用参数

- `--rounds`：随机测试轮数
- `--max-n`：每轮最大点数
- `--coord`：坐标绝对值上界
- `--seed`：随机种子（便于复现）
- `--eps`：浮点误差容忍

如果出现 mismatch，脚本会打印可直接回放的输入数据。

---

## `lab6B.cpp` 算法说明

### 问题背景 (Tree Scheduling / Job Sequencing with Precedence Constraints)
给定 $n$ 个具有权重 $C_i$ 的节点，要求遍历整棵树，每访问一个节点耗时 1 个单位时间。
限制条件为：要访问节点 $v$，必须先访问其父节点 $u$。
目标：找出使得总旅行成本 $\sum C_i \times T_i$ 最小的访问顺序。

### 核心算法 (贪心 + 并查集 + 优先队列)
1. **贪心选择**：优先处理平均权重最大的前趋约束块（即 $\frac{\Sigma C_i}{\Sigma T_i}$ 最大）。
2. **序列合并**：每次从优先队列中取出所有合法节点（非局部根）中平均权重最大的节点 $v$。由于其前趋约束，它必须紧跟在它的父节点 $u$ （或者父节点所在的整个已合并块）之后执行。我们将 $v$ 代表的序列挂在 $u$ 后面。
3. **延迟代价计算**：此时 $v$ 块整体的时间被推迟了 $Size(u)$ 步，所以直接将 $Size(u) \times Weight(v)$ 累加至答案即可。并更新合并后的节点池和大小权重。
4. **数据结构**：使用 **大根堆（Priority Queue）** 维护平均权重，使用 **并查集（DSU）** 快速查找节点当前所挂载合并的根集合。

### 时间复杂度
- $O(N \log N)$，其中 $N$ 是节点数（城市数量）。
- 这主要来源于使用优先队列进行 $N-1$ 次堆合并操作的时间。

### 编译与运行

```powershell
# 编译
g++ -O2 -std=c++17 .\lab6B.cpp -o .\lab6B.exe

# 运行
.\lab6B.exe < input.txt
```
