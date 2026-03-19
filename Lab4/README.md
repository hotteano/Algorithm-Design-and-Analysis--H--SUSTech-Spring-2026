# Lab4A - LFU Cache with TTL

## 题目目标
需要实现一个容量为 `N` 的缓存，支持 `get/put` 两种操作，并满足：

1. 先按 LFU 淘汰：访问频次最小的 key 先淘汰。
2. 若频次相同，再按 LRU 淘汰：更久没被访问的先淘汰。
3. 每次操作前，先删除所有超时 key：若某 key 超过 `T` 个时间单位未被使用，则必须在本次操作前清理。
4. `get/put` 需要做到常数级更新（摊还 O(1)）。

当前实现位于 `lab4A.cpp`。

## 核心数据结构

### 1) `Node`
每个缓存项用一个节点表示，包含：

- `key`, `value`
- `freq`：当前访问频次
- `lastUsed`：最近一次使用时间
- `expireTime`：过期时刻，定义为 `lastUsed + T + 1`
- `freqIt`：该节点在“同频次链表”中的迭代器
- `recIt`：该节点在“全局更新时间链表”中的迭代器

### 2) `keyToNode: unordered_map<int, Node*>`
- 通过 key O(1) 定位节点。
- `get/put` 命中都依赖它。

### 3) `freqToList: unordered_map<int, list<Node*>>`
- 每个频次对应一个双向链表。
- 链表内按“最近访问顺序”维护：
  - 头部：该频次下最久未访问
  - 尾部：该频次下最近访问
- 淘汰 LFU+LRU 时，从 `minFreq` 对应链表头部删除即可。

### 4) `minFreq`
- 维护当前缓存中的最小频次。
- 使得淘汰候选可以 O(1) 定位。

### 5) `recency: list<Node*>`
- 维护“全局更新时间顺序”。
- 每次节点被更新/访问后移动到尾部。
- 最终输出时按该链表从前到后输出 value。

### 6) `expireBuckets: unordered_map<long long, vector<Node*>>`
- 按“过期时刻”分桶。
- 某节点过期时间为 `x`，就把节点指针加入 `expireBuckets[x]`。
- 每次操作前只处理 `expireBuckets[now]`，避免全表扫描。

说明：同一个节点会被多次“重新调度过期”，因此某些旧指针会残留在旧桶里。清理时通过
`keyToNode[node->key] == node && node->expireTime <= now` 做有效性校验，旧记录会被自动忽略。

## 关键操作流程

### A. `expireBeforeOp()`
每次 `get/put` 的第一步：

1. 找到 `expireBuckets[now]`。
2. 枚举桶中节点：
   - 如果该节点仍在缓存中且当前确实到期，则删除。
3. 删除该时间桶。

这保证了“每次操作前必须先清理超时项”的题意。

### B. `get(key)`
1. 先过期清理。
2. 若 key 不存在，返回 `-1`。
3. 若存在：
   - 读取 value
   - 调用 `touch(node)`，更新频次和最近使用信息
4. 时间 `now++`。

### C. `put(key, value)`
1. 先过期清理。
2. 若容量 `cap == 0`，直接 `now++` 返回。
3. 若 key 已存在：
   - 更新 value
   - `touch(node)`
4. 若 key 不存在：
   - 若已满，调用 `evictLFU()` 淘汰一个
   - 插入新节点，频次置 1，更新时间与过期时间
   - 放入各索引结构（`keyToNode/freqToList/recency/expireBuckets`）
   - `minFreq = 1`
5. 时间 `now++`。

### D. `touch(node)`
表示一次“使用”：

1. 从旧频次链表删除。
2. 若旧链表空了，必要时更新 `minFreq`。
3. `freq++` 后加入新频次链表尾部。
4. 在 `recency` 中移动到尾部。
5. 刷新 `lastUsed/expireTime` 并重新加入过期桶。

### E. `evictLFU()`
1. 找到 `minFreq` 对应非空链表。
2. 取链表头（同频次下最久未访问）作为受害者。
3. 调用 `removeNode(victim)` 删除。

## 复杂度分析

- `get` 命中/未命中：哈希查询 + 常数次链表操作，摊还 O(1)
- `put` 更新/插入：同样摊还 O(1)
- `evictLFU`：O(1)
- 过期清理：单次可能清理多个键，但每个节点的每次调度记录只会被处理一次，整体摊还可视为 O(1) 每操作

## 过期边界说明

实现使用 `expireTime = lastUsed + T + 1`，并在每次操作开始时按当前 `now` 清理。
这等价于：

- 在时间差 `<= T` 时仍有效
- 在时间差 `> T` 时，于下一次操作前被淘汰

与题意“超过 `T` 时间单位未使用则淘汰”一致。

## 最终输出规则

程序先输出所有 `get` 的结果（每次一行），然后：

- 若缓存为空，输出 `empty`
- 否则按 `recency`（从最久未更新到最近更新）输出所有 value（空格分隔）

## 实现细节注意点

1. `removeNode` 不释放 Node 内存（由 `pool` 持有，程序结束统一释放），避免悬挂指针。
2. 过期桶中存在旧记录是允许的，靠有效性校验过滤。
3. `minFreq` 在删除后可能失效，需要在删除逻辑里修复。
4. `cap == 0` 是必须单独处理的边界。

## 方法级步骤详解

下面按 `lab4A.cpp` 中的方法顺序，逐个描述每个方法“做了什么、为什么这么做”。

### 1) LFUCache 构造函数

方法：`LFUCache(int capacity, long long ttl)`

步骤：

1. 保存缓存容量 `cap = capacity`。
2. 保存超时阈值 `T = ttl`。
3. 将 `minFreq` 初始化为 `0`：表示当前缓存为空，还没有最小频次概念。
4. 将全局时间 `now` 初始化为 `0`：第一条操作发生在时间 0 前的检查阶段。

作用：完成所有运行期状态的初始设置。

### 2) get

方法：`int get(int key)`

步骤：

1. 调用 `expireBeforeOp()`：在本次读操作前先清掉所有到期项。
2. 令答案默认值 `ans = -1`。
3. 在 `keyToNode` 中查找 `key`。
4. 若未命中：保持 `ans = -1`。
5. 若命中：
6. 读取节点 value 到 `ans`。
7. 调用 `touch(node)`，把这次 get 记为一次“使用”：频次 +1、刷新最近使用时间、刷新过期时间、更新顺序结构。
8. 操作结束后 `now++`，表示该操作消耗 1 个时间单位。
9. 返回 `ans`。

关键点：

1. get 命中会改变缓存状态（频次与时效），不是纯查询。
2. 过期清理一定在查 key 之前进行，保证题意一致。

### 3) put

方法：`void put(int key, int value)`

步骤：

1. 调用 `expireBeforeOp()`，先做本次操作前的到期清理。
2. 处理边界：若 `cap == 0`，缓存无法存任何键。
3. 若容量为 0：仅执行 `now++`，然后返回。
4. 在 `keyToNode` 查找 key。
5. 若 key 已存在：
6. 直接更新节点 `value`。
7. 调用 `touch(node)`，把这次 put 也视为使用行为。
8. 执行 `now++`，返回。
9. 若 key 不存在：
10. 若当前缓存已满（`size >= cap`），先调用 `evictLFU()` 淘汰一个受害者。
11. 创建新节点，填充 `key/value/freq/lastUsed/expireTime`。
12. 将节点加入 `keyToNode`，建立 key 索引。
13. 将节点放入 `freqToList[1]` 的尾部，并记录 `freqIt`。
14. 将节点放入 `recency` 尾部，并记录 `recIt`。
15. 设置 `minFreq = 1`（新插入节点频次固定为 1）。
16. 调用 `scheduleExpire(node)` 记录过期调度。
17. 执行 `now++` 结束。

关键点：

1. put 命中与 get 命中一样会“续命”和“加频次”。
2. put 新插入前先淘汰，避免超容量。

### 4) finalValuesByUpdateTime

方法：`vector<int> finalValuesByUpdateTime() const`

步骤：

1. 新建结果数组 `vals`。
2. 预留容量为 `recency.size()`，减少扩容。
3. 从 `recency` 头到尾遍历节点指针。
4. 依次把每个节点的 value 压入 `vals`。
5. 返回 `vals`。

作用：按“最久未更新 -> 最近更新”输出最终缓存值，匹配题目最后一行输出要求。

### 5) scheduleExpire

方法：`void scheduleExpire(Node* node)`

步骤：

1. 读取 `node->expireTime`。
2. 将节点指针追加到 `expireBuckets[expireTime]` 对应的 vector 中。

作用：把节点登记到未来某个时刻要检查的桶里。

说明：

1. 节点每次 touch 后都会重新登记新的过期时刻。
2. 旧登记不需要主动删除，后续由有效性校验自动过滤。

### 6) expireBeforeOp

方法：`void expireBeforeOp()`

步骤：

1. 在 `expireBuckets` 中查找当前时刻 `now` 的桶。
2. 若不存在该桶：直接返回。
3. 若存在：遍历桶中每个节点指针。
4. 对每个节点做“仍然有效且确实到期”校验：
5. `keyToNode` 中仍有该 key。
6. `keyToNode[key]` 仍然指向同一个节点（不是旧记录）。
7. `node->expireTime <= now`。
8. 校验通过则调用 `removeNode(node)` 删除。
9. 当前桶处理完后，执行 `expireBuckets.erase(it)` 删除该时刻桶。

关键点：

1. 只处理当前时刻桶，不扫描全部缓存。
2. 旧调度记录通过双重指针校验安全忽略。

### 7) touch

方法：`void touch(Node* node)`

步骤：

1. 保存旧频次 `oldFreq = node->freq`。
2. 从 `freqToList[oldFreq]` 中用 `freqIt` O(1) 删除节点。
3. 若旧频次链表被删空：
4. 从 `freqToList` 中删除该空链表。
5. 若 `minFreq == oldFreq`，说明最小频次层被清空，执行 `minFreq++`。
6. 将节点频次加一：`node->freq = oldFreq + 1`。
7. 把节点追加到新频次链表尾部，并更新 `freqIt`。
8. 从 `recency` 中删除旧位置，再放到尾部，并更新 `recIt`。
9. 刷新 `lastUsed = now`。
10. 刷新 `expireTime = now + T + 1`。
11. 调用 `scheduleExpire(node)` 写入新的过期调度。

作用：统一实现“被使用一次”后的所有状态迁移。

### 8) removeNode

方法：`void removeNode(Node* node)`

步骤：

1. 读取节点频次 `f = node->freq`。
2. 在 `freqToList[f]` 中按 `freqIt` 删除该节点。
3. 若该频次链表变空，则从 map 删除该链表条目。
4. 在 `recency` 中按 `recIt` 删除该节点。
5. 在 `keyToNode` 中删除该 key。
6. 若删除后缓存为空：
7. 设置 `minFreq = 0` 并返回。
8. 若删除频次正好是 `minFreq` 且该频次层已不存在：
9. 持续 `minFreq++`，直到找到下一个存在的频次层。

作用：统一处理“过期删除”和“淘汰删除”的底层动作。

### 9) evictLFU

方法：`void evictLFU()`

步骤：

1. 若缓存为空，直接返回（保护性判断）。
2. 确保 `minFreq` 指向一个存在且非空的频次链表：
3. 若当前 `minFreq` 无效则不断递增，直到命中有效链表。
4. 取 `freqToList[minFreq].front()` 作为受害者：
5. 该节点满足 LFU（最小频次）与同频 LRU（最久未访问）。
6. 调用 `removeNode(victim)` 完成删除。

### 10) main

方法：`int main()`

步骤：

1. 打开快速 IO。
2. 读入 `N, M, T`，若失败则直接结束。
3. 构造缓存对象 `LFUCache cache(N, T)`。
4. 准备 `outputs` 存所有 get 的返回值。
5. 循环读取 M 条操作：
6. 若 `op == 1`：读 key，调用 `cache.get(key)`，把结果放进 `outputs`。
7. 若 `op == 2`：读 `key, value`，调用 `cache.put(key, value)`。
8. 全部操作后，先逐行输出 `outputs`。
9. 调用 `finalValuesByUpdateTime()` 获取最终缓存中的值序列。
10. 若为空输出 `empty`；否则按空格分隔输出一行。
11. 返回 0。

作用：负责输入解析、驱动缓存执行、按题目格式输出结果。
