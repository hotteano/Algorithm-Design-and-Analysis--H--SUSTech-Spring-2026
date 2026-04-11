# NTT（数论变换）数学原理详解

## 1. 从 FFT 到 NTT

### 1.1 FFT 的核心问题
快速傅里叶变换（FFT）使用**复数单位根** $\omega_n = e^{2\pi i/n}$，在浮点数运算中会产生精度误差。

### 1.2 NTT 的解决思路
数论变换（NTT）将运算迁移到**有限域** $\mathbb{Z}_p$（模 $p$ 的整数域）中，使用**原根**代替复数单位根，实现**精确整数运算**。

---

## 2. 原根：NTT 的数学基础

### 2.1 定义
对于素数 $p$，若整数 $g$ 满足：
$$g^{p-1} \equiv 1 \pmod{p}$$
且对于任意 $1 \leq k < p-1$ 都有 $g^k \not\equiv 1 \pmod{p}$，则称 $g$ 是模 $p$ 的**原根**。

### 2.2 单位根的构造
若 $p = c \cdot 2^k + 1$，定义：
$$\omega_{2^k} = g^c \pmod{p}$$

**性质验证：**
$$(\omega_{2^k})^{2^k} = (g^c)^{2^k} = g^{c \cdot 2^k} = g^{p-1} \equiv 1 \pmod{p}$$

### 2.3 常用 NTT 模数

| 模数 $p$ | 分解形式 | 最大 $2^k$ | 原根 $g$ |
|---------|---------|-----------|---------|
| 998244353 | $119 \times 2^{23} + 1$ | $2^{23}$ | 3 |
| 1004535809 | $479 \times 2^{21} + 1$ | $2^{21}$ | 3 |
| 469762049 | $7 \times 2^{26} + 1$ | $2^{26}$ | 3 |

---

## 3. 离散傅里叶变换（DFT）的模形式

### 3.1 正变换（NTT）
对于长度为 $n$（$n$ 是 $2$ 的幂）的序列 $a = (a_0, a_1, \ldots, a_{n-1})$：

$$A_k = \sum_{j=0}^{n-1} a_j \cdot \omega_n^{jk} \pmod{p}, \quad k = 0, 1, \ldots, n-1$$

其中 $\omega_n = g^{(p-1)/n}$ 是 $n$ 次单位根。

### 3.2 逆变换（INTT）
$$a_j = \frac{1}{n} \sum_{k=0}^{n-1} A_k \cdot \omega_n^{-jk} \pmod{p}$$

**关键性质**：$\omega_n^{-1} \equiv \omega_n^{n-1} \equiv g^{(p-1)(n-1)/n} \pmod{p}$

### 3.3 逆元的处理
在模 $p$ 域中，除以 $n$ 等价于乘以 $n$ 的模逆元：
$$n^{-1} \equiv n^{p-2} \pmod{p} \quad \text{（费马小定理）}$$

---

## 4. 快速变换：蝴蝶操作（Butterfly Operation）

### 4.1 分治思想
DFT 的直接计算复杂度为 $O(n^2)$。利用单位根的周期性，可将问题分解：

将多项式 $A(x) = \sum_{j=0}^{n-1} a_j x^j$ 按奇偶拆分：
$$A(x) = A_{even}(x^2) + x \cdot A_{odd}(x^2)$$

### 4.2 蝴蝶公式推导
对于 $k < n/2$：

$$A_k = \sum_{j=0}^{n/2-1} a_{2j} \cdot \omega_n^{2jk} + \omega_n^k \sum_{j=0}^{n/2-1} a_{2j+1} \cdot \omega_n^{2jk}$$

注意到 $\omega_n^2 = \omega_{n/2}$，因此：
$$A_k = E_k + \omega_n^k \cdot O_k$$

对于 $k + n/2$：
$$A_{k+n/2} = E_k - \omega_n^k \cdot O_k \quad \text{（利用 } \omega_n^{k+n/2} = -\omega_n^k\text{）}$$

### 4.3 蝴蝶操作
$$\begin{cases}
u = a_i + \omega \cdot a_{i+n/2} \\
v = a_i - \omega \cdot a_{i+n/2}
\end{cases}$$

每层 $O(n)$，共 $\log_2 n$ 层，总复杂度 $O(n \log n)$。

---

## 5. 卷积定理（Convolution Theorem）

### 5.1 定理陈述
对于两个序列 $a$ 和 $b$，设其卷积为 $c = a * b$：
$$c_k = \sum_{i=0}^{k} a_i \cdot b_{k-i}$$

**定理**：
$$\text{NTT}(c) = \text{NTT}(a) \odot \text{NTT}(b)$$

其中 $\odot$ 表示逐元素乘积（Hadamard 积）。

### 5.2 证明概要
$$\begin{aligned}
C_k &= \sum_{j=0}^{n-1} c_j \cdot \omega_n^{jk} \\
&= \sum_{j=0}^{n-1} \left(\sum_{i=0}^{j} a_i b_{j-i}\right) \omega_n^{jk} \\
&= \sum_{i=0}^{n-1} a_i \omega_n^{ik} \sum_{j=i}^{n-1} b_{j-i} \omega_n^{(j-i)k} \\
&= A_k \cdot B_k
\end{aligned}$$

### 5.3 算法流程
1. **补零**：将 $a, b$ 补长到 $n \geq \text{len}(a) + \text{len}(b) - 1$（$n$ 为 $2$ 的幂）
2. **正变换**：计算 $\hat{a} = \text{NTT}(a)$, $\hat{b} = \text{NTT}(b)$
3. **点乘**：$\hat{c}_k = \hat{a}_k \cdot \hat{b}_k \pmod{p}$
4. **逆变换**：$c = \text{INTT}(\hat{c})$
5. **截断**：保留前 $\text{len}(a) + \text{len}(b) - 1$ 项

---

## 6. 复杂度分析

| 操作 | 复杂度 | 说明 |
|-----|-------|------|
| 位逆序置换 | $O(n)$ | 常数较小 |
| 每层蝴蝶变换 | $O(n)$ | $n/2$ 对蝴蝶，每对 $O(1)$ |
| 总层数 | $O(\log n)$ | 二分结构 |
| **总复杂度** | **$O(n \log n)$** | 优于暴力 $O(n^2)$ |
| 空间复杂度 | $O(n)$ | 原地变换可优化至 $O(1)$ 辅助空间 |

---

## 7. 与字符串匹配的关联

在 Peer Prefix 问题中，我们需要检测**前缀与后缀的字符冲突**。

### 7.1 冲突检测建模
定义指示数组：
- $A_0[i] = \mathbb{1}[s[i] = \text{'0'}]$
- $B_1[i] = \mathbb{1}[s[n-1-i] = \text{'1'}]$（反转）

### 7.2 卷积的语义
$$C_{01}[k] = \sum_{i=0}^{k} A_0[i] \cdot B_1[k-i]$$

当 $k = \text{len}-1$ 时：
- $A_0[i] = 1$ ⟺ 前缀位置 $i$ 是 '0'
- $B_1[\text{len}-1-i] = 1$ ⟺ 后缀位置 $(n-\text{len}+i)$ 是 '1'

**因此 $C_{01}[\text{len}-1]$ 精确统计了 "前缀 '0' 对上后缀 '1'" 的冲突次数！**

---

## 8. 实现要点总结

1. **模数选择**：确保 $p = c \cdot 2^k + 1$ 且 $n \leq 2^k$
2. **原根计算**：$\omega_n = g^{(p-1)/n} \pmod{p}$
3. **位逆序**：从 $i=1$ 开始，正确计算二进制反转
4. **逆变换**：记得最后乘以 $n^{p-2} \pmod{p}$ 进行归一化
5. **类型安全**：全程使用 `long long` 防止溢出
