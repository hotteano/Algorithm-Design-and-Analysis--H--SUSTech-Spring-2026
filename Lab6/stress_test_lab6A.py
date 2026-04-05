import argparse
import math
import random
import subprocess
import sys
from typing import List, Tuple

Point = Tuple[int, int, int]


def brute_closest(points: List[Point]) -> float:
    n = len(points)
    if n < 2:
        return 0.0
    best2 = float("inf")
    for i in range(n):
        x1, y1, z1 = points[i]
        for j in range(i + 1, n):
            x2, y2, z2 = points[j]
            dx = x1 - x2
            dy = y1 - y2
            dz = z1 - z2
            d2 = dx * dx + dy * dy + dz * dz
            if d2 < best2:
                best2 = d2
    return math.sqrt(best2)


def run_fast_solver(exe_path: str, points: List[Point]) -> float:
    lines = ["1", str(len(points))]
    lines.extend(f"{x} {y} {z}" for x, y, z in points)
    input_data = "\n".join(lines) + "\n"

    proc = subprocess.run(
        [exe_path],
        input=input_data,
        text=True,
        capture_output=True,
        check=False,
    )
    if proc.returncode != 0:
        raise RuntimeError(
            f"Executable failed with code {proc.returncode}\n"
            f"STDOUT:\n{proc.stdout}\nSTDERR:\n{proc.stderr}"
        )

    out = proc.stdout.strip().split()
    if not out:
        raise RuntimeError("Executable produced empty output")
    return float(out[-1])


def generate_case(n: int, coord_abs: int) -> List[Point]:
    points: List[Point] = []
    for _ in range(n):
        x = random.randint(-coord_abs, coord_abs)
        y = random.randint(-coord_abs, coord_abs)
        z = random.randint(-coord_abs, coord_abs)
        points.append((x, y, z))
    return points


def main() -> int:
    parser = argparse.ArgumentParser(description="Stress test for Lab6 3D closest pair")
    parser.add_argument("--exe", default="./lab6A.exe", help="Path to compiled fast solver")
    parser.add_argument("--rounds", type=int, default=2000, help="Number of random tests")
    parser.add_argument("--max-n", type=int, default=80, help="Maximum points per test")
    parser.add_argument("--coord", type=int, default=1000, help="Coordinate absolute bound")
    parser.add_argument("--seed", type=int, default=20260402, help="Random seed")
    parser.add_argument("--eps", type=float, default=0.011, help="Allowed absolute error")
    args = parser.parse_args()

    random.seed(args.seed)

    for t in range(1, args.rounds + 1):
        n = random.randint(2, args.max_n)
        points = generate_case(n, args.coord)

        brute = brute_closest(points)
        fast = run_fast_solver(args.exe, points)

        if abs(brute - fast) > args.eps:
            print("Mismatch found!")
            print(f"Round: {t}")
            print(f"n = {n}")
            print(f"Brute = {brute:.10f}")
            print(f"Fast  = {fast:.10f}")
            print("Input for replay:")
            print(1)
            print(n)
            for p in points:
                print(p[0], p[1], p[2])
            return 1

        if t % 200 == 0:
            print(f"Passed {t} rounds...")

    print(f"All {args.rounds} rounds passed.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
