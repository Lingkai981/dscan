#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="/home/data/mlk/code/index-new2"
DATA_ROOT="/home/data/mlk/data"

# Default reviewer settings.
K_LIST=(2 4 10 20 50 100 200 500 1000)
PARAMETER_C="0.5"
PARAMETER_F="0.5"

# dataset_tag -> graph file path mapping
# main usage: ./main <graph_dir> <k> <dataset_tag> [parameter_c] [parameter_f]
declare -A GRAPH_PATHS=(
  [wiki]="${DATA_ROOT}/wiki/wiki"
  [web-Google]="${DATA_ROOT}/web-Google/web-Google"
  [pokec]="${DATA_ROOT}/relationships/relationships"
  [soc]="${DATA_ROOT}/soc/soc"
  [enwiki-2020]="${DATA_ROOT}/enwiki-2020/enwiki-2020"
  [uk-2002]="${DATA_ROOT}/uk-2002/uk-2002"
  [uk-2005]="${DATA_ROOT}/uk-2005/uk-2005"
  [webbase]="${DATA_ROOT}/webbase-2001/webbase-2001"
  [friend]="${DATA_ROOT}/friend/friend"
  [friend_sample1]="${DATA_ROOT}/friend_sample1/friend_sample1"
  [friend_sample2]="${DATA_ROOT}/friend_sample2/friend_sample2"
  [friend_sample3]="${DATA_ROOT}/friend_sample3/friend_sample3"
)

# Default full list.
DATASETS=(
  wiki
  web-Google
  pokec
  soc
  enwiki-2020
  uk-2002
  uk-2005
  webbase
  friend_sample1
  friend_sample2
  friend_sample3
  friend
)

if [[ "${1:-}" == "--datasets" ]]; then
  shift
  if [[ $# -eq 0 ]]; then
    echo "[ERROR] --datasets requires at least one dataset tag." >&2
    exit 1
  fi
  DATASETS=("$@")
fi

cd "$ROOT_DIR"

echo "[INFO] Building main ..."
/usr/bin/g++ -O3 -std=c++11 -pthread -o main main.cpp Graph.cpp BitMap.cpp HashSet.cpp

for dataset in "${DATASETS[@]}"; do
  graph_path="${GRAPH_PATHS[$dataset]:-}"
  if [[ -z "$graph_path" ]]; then
    echo "[WARN] Unknown dataset tag: $dataset (skip)"
    continue
  fi

  if [[ ! -e "$graph_path" ]]; then
    echo "[WARN] Graph path not found: $graph_path (skip $dataset)"
    continue
  fi

  mkdir -p "$ROOT_DIR/$dataset"

  echo "[INFO] Running dataset=$dataset graph=$graph_path"
  for k in "${K_LIST[@]}"; do
    echo "  -> k=$k"
    ./main "$graph_path" "$k" "$dataset" "$PARAMETER_C" "$PARAMETER_F"
  done
done

echo "[INFO] Done. CSV outputs are under: $ROOT_DIR/<dataset>/<dataset>-accuracy-proxy.csv"
