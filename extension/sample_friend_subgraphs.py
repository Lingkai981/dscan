#!/usr/bin/env python3
from __future__ import annotations

import argparse
import random
from array import array
from pathlib import Path


def edge_token_iter(path: Path):
    with path.open("r", encoding="utf-8", errors="ignore") as f:
        for line in f:
            for tok in line.split():
                yield int(tok)


def read_degree_header_and_rows(graph_dir: Path):
    degree_path = graph_dir / "degree.txt"

    with degree_path.open("r", encoding="utf-8") as f:
        first = f.readline().strip().split()
        n = int(first[0])
        m = int(first[1])
    return degree_path, n, m


def degree_bucket(total_deg: int) -> int:
    # Coarse stratification to preserve low/mid/high-degree vertices.
    if total_deg <= 4:
        return 0
    if total_deg <= 32:
        return 1
    if total_deg <= 256:
        return 2
    return 3


def select_vertices(degree_path: Path, n: int, ratio: float, seed: int):
    selected = bytearray(n)
    mapping = array("I", [0]) * n

    def keep(v: int, bucket: int) -> bool:
        # Deterministic pseudo-random by vertex id + bucket.
        h = (v * 11400714819323198485 + seed * 1315423911 + bucket * 2654435761) & ((1 << 64) - 1)
        return (h / float(1 << 64)) < ratio

    selected_cnt = 0
    with degree_path.open("r", encoding="utf-8", errors="ignore") as f:
        _ = f.readline()
        for v in range(n):
            line = f.readline()
            if not line:
                raise RuntimeError(f"degree.txt ended early at vertex {v}")
            out_d, in_d = map(int, line.split())
            b = degree_bucket(out_d + in_d)
            if keep(v, b):
                selected[v] = 1
                mapping[v] = selected_cnt
                selected_cnt += 1

            if v and v % 10000000 == 0:
                print(f"[INFO] selected pass progress: {v}/{n}")

    return selected, mapping, selected_cnt


def count_retained_degrees(degree_path: Path, out_edges_path: Path, in_edges_path: Path, n: int, selected: bytearray, mapping: array, n_new: int):
    out_cnt = array("I", [0]) * n_new
    in_cnt = array("I", [0]) * n_new

    out_tokens = edge_token_iter(out_edges_path)
    in_tokens = edge_token_iter(in_edges_path)

    # Skip the first token in edge files (header can be malformed/overflowed).
    next(out_tokens, None)
    next(in_tokens, None)

    with degree_path.open("r", encoding="utf-8", errors="ignore") as f:
        _ = f.readline()
        for v in range(n):
            line = f.readline()
            if not line:
                raise RuntimeError(f"degree.txt ended early at vertex {v}")
            out_d, in_d = map(int, line.split())

            if selected[v]:
                new_v = mapping[v]
                c = 0
                for _ in range(out_d):
                    u = next(out_tokens)
                    if 0 <= u < n and selected[u]:
                        c += 1
                out_cnt[new_v] = c
            else:
                for _ in range(out_d):
                    _ = next(out_tokens)

            if selected[v]:
                new_v = mapping[v]
                c = 0
                for _ in range(in_d):
                    u = next(in_tokens)
                    if 0 <= u < n and selected[u]:
                        c += 1
                in_cnt[new_v] = c
            else:
                for _ in range(in_d):
                    _ = next(in_tokens)

            if v and v % 5000000 == 0:
                print(f"[INFO] count pass progress: {v}/{n}")

    return out_cnt, in_cnt


def write_induced_subgraph_stream(degree_path: Path, out_edges_path: Path, in_edges_path: Path, n: int, selected: bytearray, mapping: array, n_new: int, out_dir: Path, out_cnt: array, in_cnt: array):
    out_dir.mkdir(parents=True, exist_ok=True)
    (out_dir / "index").mkdir(exist_ok=True)
    (out_dir / "output").mkdir(exist_ok=True)

    out_sum = int(sum(out_cnt))
    in_sum = int(sum(in_cnt))
    m_new = max(out_sum, in_sum)

    with (out_dir / "degree.txt").open("w", encoding="utf-8") as f:
        f.write(f"{n_new} {m_new}\n")
        for v in range(n_new):
            f.write(f"{out_cnt[v]} {in_cnt[v]}\n")

    out_tokens = edge_token_iter(out_edges_path)
    in_tokens = edge_token_iter(in_edges_path)
    next(out_tokens, None)
    next(in_tokens, None)

    with degree_path.open("r", encoding="utf-8", errors="ignore") as fdeg, \
         (out_dir / "out_edges.txt").open("w", encoding="utf-8") as fout, \
         (out_dir / "in_edges.txt").open("w", encoding="utf-8") as fin:
        _ = fdeg.readline()
        fout.write(f"{n_new}\n")
        fin.write(f"{n_new}\n")

        for v in range(n):
            line = fdeg.readline()
            if not line:
                raise RuntimeError(f"degree.txt ended early at vertex {v}")
            out_d, in_d = map(int, line.split())

            if selected[v]:
                for _ in range(out_d):
                    u = next(out_tokens)
                    if 0 <= u < n and selected[u]:
                        fout.write(f"{mapping[u]}\n")
            else:
                for _ in range(out_d):
                    _ = next(out_tokens)

            if selected[v]:
                for _ in range(in_d):
                    u = next(in_tokens)
                    if 0 <= u < n and selected[u]:
                        fin.write(f"{mapping[u]}\n")
            else:
                for _ in range(in_d):
                    _ = next(in_tokens)

            if v and v % 5000000 == 0:
                print(f"[INFO] write pass progress: {v}/{n}")

def main():
    parser = argparse.ArgumentParser(description="Generate stratified induced subgraphs for friend dataset.")
    parser.add_argument("--input", default="/home/data/mlk/data/friend/friend")
    parser.add_argument("--output-root", default="/home/data/mlk/data")
    parser.add_argument("--ratios", default="10,20,30,40")
    parser.add_argument("--seed", type=int, default=20260316)
    args = parser.parse_args()

    graph_dir = Path(args.input)
    output_root = Path(args.output_root)
    ratio_vals = [int(x) for x in args.ratios.split(",") if x.strip()]

    degree_path, n, m = read_degree_header_and_rows(graph_dir)
    out_edges_path = graph_dir / "out_edges.txt"
    in_edges_path = graph_dir / "in_edges.txt"
    print(f"[INFO] loaded friend graph header: n={n} m={m}")

    for ratio_pct in ratio_vals:
        ratio = ratio_pct / 100.0
        name = f"friend_ratio{ratio_pct}"
        out_dir = output_root / name / name
        print(f"[INFO] start {name} ratio={ratio:.2f}")
        selected, mapping, n_new = select_vertices(degree_path, n, ratio, args.seed + ratio_pct)
        print(f"[INFO] selected vertices for {name}: |V|={n_new}")
        out_cnt, in_cnt = count_retained_degrees(degree_path, out_edges_path, in_edges_path, n, selected, mapping, n_new)
        write_induced_subgraph_stream(degree_path, out_edges_path, in_edges_path, n, selected, mapping, n_new, out_dir, out_cnt, in_cnt)
        print(f"[INFO] wrote {name}: |V|={n_new} path={out_dir}")


if __name__ == "__main__":
    main()