#!/bin/bash
# DSCAN 一键运行脚本: 预处理 → 建索引 → 聚类查询 → 评估
#
# 用法:
#   ./run.sh <数据目录> <eps_c> <eps_f> <mu>
#
# 示例:
#   ./run.sh data/cora 0.1 0.1 2
#   ./run.sh data/blog 0.2 0.2 3

set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$SCRIPT_DIR"

# ─── 参数检查 ───
if [ $# -lt 4 ]; then
    echo "DSCAN 一键运行脚本"
    echo ""
    echo "用法: $0 <数据目录> <eps_c> <eps_f> <mu>"
    echo ""
    echo "示例:"
    echo "  $0 data/cora 0.1 0.1 2"
    echo "  $0 data/blog 0.2 0.2 3"
    echo ""
    echo "可用数据集:"
    for d in data/*/; do
        [ -f "$d/edges.txt" ] && echo "  $d"
    done
    exit 1
fi

DATA_DIR="$1"
EPS_C="$2"
EPS_F="$3"
MU="$4"

echo "═══════════════════════════════════════"
echo "  DSCAN Pipeline"
echo "  数据集: $DATA_DIR"
echo "  参数:   eps_c=$EPS_C, eps_f=$EPS_F, mu=$MU"
echo "═══════════════════════════════════════"

# ─── 1. 编译（如果可执行文件不存在或源码更新了）───
NEED_COMPILE=0
if [ ! -f ./dscan ]; then
    NEED_COMPILE=1
else
    for src in main.cpp Graph.cpp BitMap.cpp HashSet.cpp Graph.hpp BitMap.hpp HashSet.hpp; do
        if [ "$src" -nt ./dscan ]; then
            NEED_COMPILE=1
            break
        fi
    done
fi

if [ $NEED_COMPILE -eq 1 ]; then
    echo ""
    echo ">>> [1/5] 编译 DSCAN..."
    g++ -O2 -std=c++17 -o dscan main.cpp Graph.cpp BitMap.cpp HashSet.cpp
    echo "    编译完成 ✓"
else
    echo ""
    echo ">>> [1/5] 编译 DSCAN... 已是最新 ✓"
fi

# ─── 2. 原始数据 → edges.txt（如果 edges.txt 不存在）───
if [ ! -f "$DATA_DIR/edges.txt" ]; then
    echo ""
    echo ">>> [2/5] 原始数据 → edges.txt..."
    python preprocess.py "$DATA_DIR" --from-raw ${RAW_ADJ:+$RAW_ADJ} ${RAW_LABELS:+--labels $RAW_LABELS}
    # preprocess.py 会自动检测 raw/ 下的文件
    echo "    原始数据转换完成 ✓"
else
    echo ""
    echo ">>> [2/5] edges.txt... 已存在 ✓"
fi

# ─── 3. 预处理: edges.txt → DSCAN 输入格式（如果 degree.txt 不存在）───
if [ ! -f "$DATA_DIR/degree.txt" ]; then
    echo ""
    echo ">>> [3/5] edges.txt → DSCAN 格式..."
    python preprocess.py "$DATA_DIR"
    echo "    预处理完成 ✓"
else
    echo ""
    echo ">>> [3/5] DSCAN 格式... 已存在 ✓"
fi

# ─── 4. 建索引（如果 index/ 目录下没有索引文件）───
if [ ! -f "$DATA_DIR/index/degree.txt" ]; then
    echo ""
    echo ">>> [4/5] 建立索引..."
    ./dscan "$DATA_DIR" index
    echo "    索引构建完成 ✓"
else
    echo ""
    echo ">>> [4/5] 建立索引... 已存在 ✓"
fi

# ─── 5. 聚类查询 + 评估 ───
echo ""
echo ">>> [5/5] 聚类查询..."
./dscan "$DATA_DIR" query "$EPS_C" "$EPS_F" "$MU"

echo ""
echo ">>> 评估结果:"
echo "─────────────────────────────────────"
python evaluate.py "$DATA_DIR" "$EPS_C" "$EPS_F" "$MU"
echo "═══════════════════════════════════════"
