import random
from itertools import permutations

def simulate_sorting(perm_with_id, N):
    """
    模拟排序过程
    perm_with_id: [(type, id), ...] 带有唯一ID的排列
    N: 时间限制
    返回 (是否可行, 最大delay)
    """
    n = len(perm_with_id)
    # 提取类型序列和ID序列
    perm = [t for t, _ in perm_with_id]
    ids = [i for _, i in perm_with_id]
    
    # 记录每个ID的完成时间
    finish_time = {}
    
    # 机器状态：下次空闲时间
    freeA = 0
    freeB = 0
    
    # 队列（存储在perm_with_id中的索引）
    queue = list(range(n))
    
    time = 0
    max_time = 2 * N  # 设置一个足够大的上限
    
    while queue and time <= max_time:
        processed_this_minute = []
        
        # 先处理抢占情况（规则2后半部分）
        # 如果机器A空闲，机器B忙，且队首是B，找队列中第一个A抢占
        if freeA <= time and freeB > time and queue:
            front_idx = queue[0]
            if perm[front_idx] == 'B':
                # 找队列中第一个A
                preempt_pos = -1
                for i, q_idx in enumerate(queue):
                    if perm[q_idx] == 'A':
                        preempt_pos = i
                        break
                if preempt_pos != -1:
                    # A抢占机器A
                    pkg_idx = queue.pop(preempt_pos)
                    pkg_id = ids[pkg_idx]
                    finish_time[pkg_id] = time + 1
                    freeA = time + 1
                    processed_this_minute.append(pkg_id)
        
        # 处理队首包裹（如果没有被抢占处理过）
        if queue:
            front_idx = queue[0]
            front_type = perm[front_idx]
            
            if front_type == 'A':
                # Fragile，优先用机器A
                if freeA <= time:
                    queue.pop(0)
                    pkg_id = ids[front_idx]
                    finish_time[pkg_id] = time + 1
                    freeA = time + 1
                    processed_this_minute.append(pkg_id)
                elif freeB <= time:
                    # 机器A忙，用机器B
                    queue.pop(0)
                    pkg_id = ids[front_idx]
                    finish_time[pkg_id] = time + 1
                    freeB = time + 1
                    processed_this_minute.append(pkg_id)
            else:
                # Material，只能用机器B
                if freeB <= time:
                    queue.pop(0)
                    pkg_id = ids[front_idx]
                    finish_time[pkg_id] = time + 1
                    freeB = time + 1
                    processed_this_minute.append(pkg_id)
        
        time += 1
    
    # 检查是否所有包裹都在N分钟内完成
    if queue:  # 还有未完成
        return False, float('inf')
    
    for t in finish_time.values():
        if t > N:
            return False, float('inf')
    
    # 计算delay
    # 对于每个ID，计算：新位置 - 原位置
    # 新位置：在perm_with_id中的索引
    # 原位置：ID本身就是原位置（因为我们构造时ID就是原始索引）
    max_delay = 0
    for new_pos, (t, orig_id) in enumerate(perm_with_id):
        delay = new_pos - orig_id
        if delay > max_delay:
            max_delay = delay
    
    return True, max_delay


def brute_force(seq):
    """
    暴力枚举所有排列，返回最小最大delay
    seq: 原始序列字符串
    """
    n = len(seq)
    if n % 2 != 0:
        n += 1  # 确保是偶数
    N = n // 2
    
    countA = seq.count('A')
    countB = seq.count('B')
    
    if countA < countB:
        return -1
    
    # 构造带ID的原始序列
    original = [(seq[i], i) for i in range(len(seq))]
    
    min_max_delay = float('inf')
    found_valid = False
    
    # 枚举所有排列
    for perm in permutations(original):
        feasible, delay = simulate_sorting(list(perm), N)
        if feasible:
            found_valid = True
            if delay < min_max_delay:
                min_max_delay = delay
                if min_max_delay == 0:
                    break
    
    return min_max_delay if found_valid else -1


def flat_formula(seq):
    """
    基于数学公式的解法（逐元素扫描）
    """
    n = len(seq)
    N = n // 2
    countA = seq.count('A')
    countB = seq.count('B')
    if countA < countB:
        return -1
    
    K = countA - N
    max_val = -float('inf')
    bc = 0
    for i, c in enumerate(seq):
        if c == 'B':
            bc += 1
            max_val = max(max_val, (i + 1) - 2 * bc)
            
    if bc == 0:
        return 0
    return max(0, max_val - 2 * K)


def rle_formula(blocks):
    """
    RLE优化版公式解法
    """
    countA = 0
    countB = 0
    for s, x in blocks:
        countA += s.count('A') * x
        countB += s.count('B') * x
        
    if countA < countB:
        return -1
    
    N = (countA + countB) // 2
    K = countA - N
    
    ans_val = -float('inf')
    CurP = 0
    CurK = 0
    
    for s, x in blocks:
        if x == 0 or not s:
            continue
        
        bc = 0
        max_local = -float('inf')
        for j, c in enumerate(s):
            if c == 'B':
                bc += 1
                local_v = (j + 1) - 2 * bc
                max_local = max(max_local, local_v)
        
        if bc == 0:
            CurP += x * len(s)
            continue
            
        slope = len(s) - 2 * bc
        val0 = CurP - 2 * CurK + max_local
        val_end = CurP - 2 * CurK + (x - 1) * slope + max_local
        
        ans_val = max(ans_val, val0, val_end)
        
        CurP += x * len(s)
        CurK += x * bc
        
    if ans_val != -float('inf'):
        return max(0, ans_val - 2 * K)
    return 0


# 测试
print("Testing with brute force enumeration...")
print()

passed = 0
test_cases = 50  # 由于排列数量增长很快，测试少一些

for i in range(test_cases):
    # 生成随机序列，长度6或8（N=3或4）
    n = random.choice([6, 8])
    seq = "".join(random.choices(['A', 'B'], k=n))
    
    print(f"Test {i+1}: seq={seq}", end=" ")
    
    brute_result = brute_force(seq)
    formula_result = flat_formula(seq)
    
    if brute_result != formula_result:
        print(f"[FAILED]")
        print(f"  Brute force: {brute_result}")
        print(f"  Formula:     {formula_result}")
        break
    else:
        print(f"[OK] delay={brute_result}")
        passed += 1

print()
if passed == test_cases:
    print(f"[SUCCESS] All {passed} test cases passed!")
    print("The formula matches brute force enumeration.")
else:
    print(f"Failed after {passed} passed tests")
