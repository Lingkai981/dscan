#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="/home/data/mlk/code/index-new2"
DATA_ROOT="/home/data/mlk/data"

# Defaults: small datasets + reviewer k list + several query parameter groups.
DATASETS=(wiki web-Google)
K_LIST=(2 4 10 20 50 100 200 500 1000)
PARAM_PAIRS=(
  "0.3:0.3"
  "0.4:0.4"
  "0.5:0.5"
  "0.6:0.6"
  "0.7:0.7"
  "0.5:0.3"
  "0.3:0.5"
)
SKIP_BUILD=0

# dataset_tag -> graph path
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

usage() {
  cat <<'EOF'
Usage:
  ./run_accuracy_proxy_multi_params.sh [--datasets d1 d2 ...] [--params c1:f1 c2:f2 ...] [--k-list k1 k2 ...] [--skip-build]

Examples:
  ./run_accuracy_proxy_multi_params.sh
  ./run_accuracy_proxy_multi_params.sh --datasets wiki web-Google
  ./run_accuracy_proxy_multi_params.sh --datasets wiki --params 0.5:0.5 0.6:0.4 --k-list 2 4 10
  ./run_accuracy_proxy_multi_params.sh --datasets wiki web-Google --params 0.5:0.5 --skip-build

Outputs:
  /home/data/mlk/code/index-new2/<dataset>/<dataset>-accuracy-proxy.csv
  /home/data/mlk/code/index-new2/<dataset>/<dataset>-accuracy-proxy-core-nei.csv
EOF
}

while [[ $# -gt 0 ]]; do
  case "$1" in
    --datasets)
      shift
      DATASETS=()
      while [[ $# -gt 0 && "$1" != --* ]]; do
        DATASETS+=("$1")
        shift
      done
      ;;
    --params)
      shift
      PARAM_PAIRS=()
      while [[ $# -gt 0 && "$1" != --* ]]; do
        PARAM_PAIRS+=("$1")
        shift
      done
      ;;
    --k-list)
      shift
      K_LIST=()
      while [[ $# -gt 0 && "$1" != --* ]]; do
        K_LIST+=("$1")
        shift
      done
      ;;
    --skip-build)
      SKIP_BUILD=1
      shift
      ;;
    -h|--help)
      usage
      exit 0
      ;;
    *)
      echo "[ERROR] Unknown argument: $1" >&2
      usage
      exit 1
      ;;
  esac
done

if [[ ${#DATASETS[@]} -eq 0 ]]; then
  echo "[ERROR] Dataset list is empty." >&2
  exit 1
fi

if [[ ${#PARAM_PAIRS[@]} -eq 0 ]]; then
  echo "[ERROR] Parameter pair list is empty." >&2
  exit 1
fi

if [[ ${#K_LIST[@]} -eq 0 ]]; then
  echo "[ERROR] k list is empty." >&2
  exit 1
fi

cd "$ROOT_DIR"

if [[ "$SKIP_BUILD" -eq 0 ]]; then
  echo "[INFO] Building main ..."
  /usr/bin/g++ -O3 -std=c++11 -pthread -o main main.cpp Graph.cpp BitMap.cpp HashSet.cpp
else
  echo "[INFO] Skip build (--skip-build)."
fi

echo "[INFO] datasets: ${DATASETS[*]}"
echo "[INFO] k-list: ${K_LIST[*]}"
echo "[INFO] params: ${PARAM_PAIRS[*]}"

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
  for pair in "${PARAM_PAIRS[@]}"; do
    if [[ "$pair" != *:* ]]; then
      echo "[WARN] Invalid param pair '$pair' (expected c:f), skip"
      continue
    fi

    c="${pair%%:*}"
    f="${pair##*:}"
    echo "  [PARAM] c=$c f=$f"

    for k in "${K_LIST[@]}"; do
      echo "    -> k=$k"
      ./main "$graph_path" "$k" "$dataset" "$c" "$f"
    done
  done
done

echo "[INFO] Done. Check CSVs under: $ROOT_DIR/<dataset>/"
