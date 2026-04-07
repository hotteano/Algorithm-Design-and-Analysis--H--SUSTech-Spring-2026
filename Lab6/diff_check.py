import subprocess
import random

def gen_test():
    random.seed()
    n = random.randint(1, 9)
    root = random.randint(1, n)
    
    lines = [f'{n} {root}']
    C = [random.randint(1, 20) for _ in range(n)]
    lines.append(' '.join(map(str, C)))
    
    parent = [0] * (n + 1)
    parent[root] = -1
    nodes = [root]
    
    for i in range(1, n + 1):
        if i == root:
            continue
        p = random.choice(nodes)
        parent[i] = p
        nodes.append(i)
    
    for i in range(1, n + 1):
        if i != root:
            lines.append(f'{parent[i]} {i}')
    
    return '\n'.join(lines) + '\n'

def run_exe(exe, input_data):
    result = subprocess.run([exe], input=input_data, capture_output=True, text=True)
    return result.stdout.strip()

# 编译两个程序
print("编译程序...")
subprocess.run(["g++", "-std=c++17", "-O2", "lab6B.cpp", "-o", "lab6B.exe"])
subprocess.run(["g++", "-std=c++17", "-O2", "lab6B_alt.cpp", "-o", "lab6B_alt.exe"])

print("开始对拍...")
cnt = 0
while True:
    cnt += 1
    input_data = gen_test()
    
    res1 = run_exe("lab6B.exe", input_data)
    res2 = run_exe("lab6B_alt.exe", input_data)
    
    if res1 != res2:
        print(f"\n发现差异！Test {cnt}")
        print(f"标准程序 (lab6B): {res1}")
        print(f"DFS程序 (alt): {res2}")
        print(f"\n输入数据:\n{input_data}")
        break
    
    if cnt % 100 == 0:
        print(f"已测试 {cnt} 组...")
