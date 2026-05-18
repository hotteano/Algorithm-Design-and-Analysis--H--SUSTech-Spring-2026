# Lab 11B — Data Center OS Selection (Minimum Cut)

## Problem Statement

Given an `n x m` grid of servers, each server must be installed with either **Linux** or **Windows**.

- Installing Linux on server `(i, j)` yields score `L[i][j]`
- Installing Windows on server `(i, j)` yields score `W[i][j]`
- If two **vertically adjacent** servers both run Linux, gain extra score `VL[i][j]`
- If two **vertically adjacent** servers both run Windows, gain extra score `VW[i][j]`
- If two **horizontally adjacent** servers both run Linux, gain extra score `HL[i][j]`
- If two **horizontally adjacent** servers both run Windows, gain extra score `HW[i][j]`

Find the maximum total performance score.

## Algorithm — Dinic's Max-Flow / Min-Cut

This problem is a **binary labeling problem with submodular pairwise potentials**, which can be solved exactly by a **minimum s-t cut** (equivalently, maximum flow).

### Key Insight: Total Score = Constant - Minimum Cut

1. Let `C` be the sum of **all** input values (as if every server got both OS scores and every adjacent pair got both bonuses).
2. This is impossible, so we must "give up" some values. The amount given up is exactly the capacity of an `s-t` cut.
3. **Max Score = C - Min Cut**

### Graph Construction

| Component | Edges | Capacity | Meaning |
|-----------|-------|----------|---------|
| Linux base score | `S -> cell(i,j)` | `L[i][j]` | If cell chooses Windows (cut to T side), we lose `L` |
| Windows base score | `cell(i,j) -> T` | `W[i][j]` | If cell chooses Linux (cut to S side), we lose `W` |
| Vertical Linux bonus | `S -> vL`, `vL -> A` INF, `vL -> B` INF | `VL` | Only keep `VL` if both A,B choose Linux |
| Vertical Windows bonus | `A -> vW` INF, `B -> vW` INF, `vW -> T` | `VW` | Only keep `VW` if both A,B choose Windows |
| Horizontal bonuses | Same pattern as vertical | | |

**Why INF edges work:** If a bonus node's assignment would force an `INF` edge to cross `S -> T`, the minimum cut will never choose it. Thus the bonus is only kept when the adjacency condition is satisfied.

### Submodularity Proof

For any adjacent pair with Linux bonus `a` and Windows bonus `b`, the energy (loss) matrix is:

|  | y=Linux | y=Windows |
|--|---------|-----------|
| x=Linux | `b` | `a+b` |
| x=Windows | `a+b` | `a` |

Check: `E(0,0) + E(1,1) = a + b <= 2(a+b) = E(0,1) + E(1,0)`.  
Since `a, b >= 0`, the **submodular inequality** holds. By the Kolmogorov-Zabih theorem, this binary labeling problem is exactly solvable by graph cut.

## Complexity Analysis

- **Nodes:** `O(n * m)` grid cells + `O(n * m)` bonus auxiliary nodes
- **Edges:** Each cell connects to S/T; each adjacency introduces an auxiliary node with 3 edges
- **Dinic Complexity:** `O(E * sqrt(V))` for unit capacities, `O(V^2 * E)` worst case. In practice runs fast for grid sizes within typical contest limits.

## Input Format

```
n m
[ n x m matrix: L[i][j] ]
[ n x m matrix: W[i][j] ]
[ (n-1) x m matrix: vertical Linux bonuses ]
[ (n-1) x m matrix: vertical Windows bonuses ]
[ n x (m-1) matrix: horizontal Linux bonuses ]
[ n x (m-1) matrix: horizontal Windows bonuses ]
```

## Output Format

A single integer: the maximum total performance score.

## Compilation

```bash
g++ -std=c++17 -O2 lab11B.cpp -o lab11B
./lab11B < input.txt
```

## Recovering the Assignment (Optional)

After running `max_flow(S, T)`, perform a BFS/DFS from `S` on the **residual graph** (edges with `cap - flow > 0`). Nodes reachable from `S` are in the Linux set; unreachable nodes are in the Windows set.

```cpp
// After max_flow
vector<bool> vis(n_nodes, false);
queue<int> q;
q.push(S); vis[S] = true;
while (!q.empty()) {
    int v = q.front(); q.pop();
    for (auto& e : adj[v]) {
        if (e.cap - e.flow > 0 && !vis[e.to]) {
            vis[e.to] = true;
            q.push(e.to);
        }
    }
}
// vis[cell_id] == true  -> Linux
// vis[cell_id] == false -> Windows
```

## Notes

- The graph construction does **not** enforce connectivity of either OS region. The optimal solution may contain multiple disconnected Linux or Windows islands if the base scores dictate so.
- All capacities use `long long` (`INF = 1e18`) to prevent overflow.
- The problem assumes all input values are non-negative.
