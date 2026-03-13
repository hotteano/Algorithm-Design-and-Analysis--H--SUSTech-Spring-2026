import itertools
import random

def check_valid_and_simulate(queue_str):
    """
    完全模拟：严格按照题目的顺序判定和时间流逝判断
    """
    N = len(queue_str) // 2
    queue = list(queue_str)
    
    # 我们有 N 分钟。每分钟里我们需要让两个机器都处理包裹。
    for minute in range(N):
        if not queue:
            break
            
        machine_A_item_idx = -1
        machine_B_item_idx = -1
        
        front = queue[0]
        
        if front == 'A':
            # 队首是 A，机器 A 空闲，A 有优先级。机器 A 吃掉这个队首包裹。
            machine_A_item_idx = 0
            
            # 此时机器 B 需要一个包裹，它只能吃后续新变成队首的包裹
            if len(queue) > 1:
                machine_B_item_idx = 1
            else:
                return False # B 没东西吃，无法在 N 内完成
                
        else:
            # 队首是 B。B 只能用机器 B。机器 B 忙碌处理它。
            machine_B_item_idx = 0
            
            # 此时机器 A 空闲，且机器 B 忙。第一个 A 抢占机器 A。
            for i in range(1, len(queue)):
                if queue[i] == 'A':
                    machine_A_item_idx = i
                    break
                    
            if machine_A_item_idx == -1:
                return False # 找不到 A 给机器 A 处理！停摆。
                
        # 从队列里弹出这两个处理过的物品
        idx_to_remove = sorted([machine_A_item_idx, machine_B_item_idx], reverse=True)
        for idx in idx_to_remove:
            queue.pop(idx)

    return len(queue) == 0

def calc_delay(orig_with_id, p):
    max_d = 0
    # 枚举目前在 permutation 中的每一个位置
    for new_pos, (char, old_pos) in enumerate(p):
        delay = max(0, new_pos - old_pos)
        if delay > max_d:
            max_d = delay
    return max_d

def real_brute_force(seq):
    if seq.count('A') < seq.count('B'): return -1
    
    orig_with_id = [(c, i) for i, c in enumerate(seq)]
    best_delay = 999999
    found = False
    
    # 穷举排列并结合“序列改变+真实模拟过程”
    for p in set(itertools.permutations(orig_with_id)):
        queue_str = "".join(x[0] for x in p)
        
        if check_valid_and_simulate(queue_str):
            found = True
            delay = calc_delay(orig_with_id, p)
            if delay < best_delay:
                best_delay = delay
                
    return best_delay if found else -1

def formula(seq):
    N = len(seq) // 2
    countA = seq.count('A')
    countB = seq.count('B')
    if countA < countB: return -1
    
    K = countA - N
    max_val = -float('inf')
    bc = 0
    for i, c in enumerate(seq):
        if c == 'B':
            bc += 1
            max_val = max(max_val, (i + 1) - 2 * bc)
            
    if bc == 0: return 0
    return max(0, max_val - 2 * K)

def rle_formula(blocks):
    # blocks is list of (length, string)
    countA = 0
    countB = 0
    for x, s in blocks:
        countA += s.count('A') * x
        countB += s.count('B') * x
        
    if countA < countB: return -1
    
    N = (countA + countB) // 2
    K = countA - N
    
    ans_val = -float('inf')
    CurP = 0
    CurK = 0
    
    for x, s in blocks:
        if x == 0 or not s: continue
        
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

print("Starting tests comparing absolute brute-force with C++ RLE Formula...")
test_cases = [
    # (blocks, expected_description)
    ([(3, "AB"), (2, "A"), (2, "B")], "Mixed blocks"),
    ([(2, "A"), (2, "B"), (2, "A"), (2, "B")], "AABBAABB via blocks"),
    ([(1, "A"), (1, "A"), (1, "A"), (5, "B"), (2, "A")], "AAABBBBBAA via blocks"),
    ([(10, "A")], "All A's"),
    ([(5, "B")], "All B's (Impossible)"),
    ([(2, "BA"), (2, "AB")], "BABAABAB"),
    ([(1, "ABBA"), (1, "BBAA"), (1, "A")], "Complex 1"),
]

for idx, (blocks, desc) in enumerate(test_cases):
    # Flatten blocks to string for brute force
    seq = "".join(s * x for x, s in blocks)
    
    # Needs to be even length for 2N
    if len(seq) % 2 != 0:
        continue
        
    truth = real_brute_force(seq)
    calc = rle_formula(blocks)
    
    print(f"Test {idx} [{desc}]: len={len(seq)} | Brute={truth} | RLE_Formula={calc}", end=" ")
    
    if truth != calc:
        print(" [FAILED!]")
        break
    else:
        print(" [OK]")

