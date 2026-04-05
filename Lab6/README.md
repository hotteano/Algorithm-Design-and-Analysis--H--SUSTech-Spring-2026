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
