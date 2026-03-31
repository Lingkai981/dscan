#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="/home/data/mlk/code/index-new2"
DATA_ROOT="/home/data/mlk/data"

# User-requested defaults.
MU_LIST="2,4,6,8,10"
CF_LIST="0.1,0.2,0.4,0.5,0.6,0.8"
K_LIST=(2 4 10 20 50 100 200 500 1000)
DATASETS=(wiki web-Google pokec soc enwiki-2020 uk-2002 uk-2005 webbase friend_sample1 friend_sample2 friend_sample3 friend_ratio10 friend_ratio20 friend_ratio30 friend_ratio40 friend)
FORCE=0
SKIP_BUILD=0
SMOKE=0
FAILED_FILE=""

# dataset_tag -> graph path
# Output: one CSV per (dataset, k)
declare -A GRAPH_PATHS=(
  [wiki]="${DATA_ROOT}/wiki/wiki"
  [web-Google]="${DATA_ROOT}/web-Google/web-Google"
  [pokec]="${DATA_ROOT}/relationships/relationships"
  [soc]="${DATA_ROOT}/soc/soc"
  [enwiki-2020]="${DATA_ROOT}/enwiki-2020/enwiki-2020"
  [uk-2002]="${DATA_ROOT}/uk-2002/uk-2002"
  [uk-2005]="${DATA_ROOT}/uk-2005/uk-2005"
  [webbase]="${DATA_ROOT}/webbase-2001/webbase-2001"
  [friend_sample1]="${DATA_ROOT}/friend_sample1/friend_sample1"
  [friend_sample2]="${DATA_ROOT}/friend_sample2/friend_sample2"
  [friend_sample3]="${DATA_ROOT}/friend_sample3/friend_sample3"
  [friend_ratio10]="${DATA_ROOT}/friend_ratio10/friend_ratio10"
  [friend_ratio20]="${DATA_ROOT}/friend_ratio20/friend_ratio20"
  [friend_ratio30]="${DATA_ROOT}/friend_ratio30/friend_ratio30"
  [friend_ratio40]="${DATA_ROOT}/friend_ratio40/friend_ratio40"
  [friend]="${DATA_ROOT}/friend/friend"
)

usage() {
  cat <<'EOF'
Usage:
  ./run_all_params_all_experiments.sh [options]

Options:
  --datasets d1 d2 ...      Dataset tags to run (default: wiki web-Google)
  --k-list k1 k2 ...        k list (default: 2 4 10 20 50 100 200 500 1000)
  --mu-list csv             mu list csv (default: 2,4,6,8,10)
  --cf-list csv             c/f value list csv (default: 0.1,0.2,0.4,0.5,0.6,0.8)
  --force                   Re-run even if output CSV exists
  --skip-build              Skip rebuilding binaries
  --smoke                   Small-range test: first dataset + first k + reduced params
  -h, --help                Show help

Behavior:
  - For each (dataset, k), output file is:
    /home/data/mlk/code/index-new2/<dataset>/<dataset>-grid-k<k>.csv
  - If output exists and --force is not set, that (dataset, k) is skipped.
  - This acts as "index/result exists -> no rebuild" checkpointing.
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
    --k-list)
      shift
      K_LIST=()
      while [[ $# -gt 0 && "$1" != --* ]]; do
        K_LIST+=("$1")
        shift
      done
      ;;
    --mu-list)
      MU_LIST="$2"
      shift 2
      ;;
    --cf-list)
      CF_LIST="$2"
      shift 2
      ;;
    --force)
      FORCE=1
      shift
      ;;
    --skip-build)
      SKIP_BUILD=1
      shift
      ;;
    --smoke)
      SMOKE=1
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

if [[ ${#DATASETS[@]} -eq 0 || ${#K_LIST[@]} -eq 0 ]]; then
  echo "[ERROR] datasets or k-list is empty." >&2
  exit 1
fi

if [[ "$SMOKE" -eq 1 ]]; then
  DATASETS=("${DATASETS[0]}")
  K_LIST=("${K_LIST[0]}")
  MU_LIST="2"
  CF_LIST="0.5"
fi

cd "$ROOT_DIR"

FAILED_FILE="$ROOT_DIR/failed_cases.csv"
if [[ "$FORCE" -eq 1 ]]; then
  rm -f "$FAILED_FILE"
fi
if [[ ! -f "$FAILED_FILE" ]]; then
  echo "dataset,k,mu_list,cf_list,exit_code,mode" > "$FAILED_FILE"
fi

IFS=',' read -r -a MU_ARR <<< "$MU_LIST"

if [[ "$SKIP_BUILD" -eq 0 ]]; then
  echo "[INFO] Building binaries (main, main_grid) ..."
  /usr/bin/g++ -O3 -std=c++11 -pthread -o main main.cpp Graph.cpp BitMap.cpp HashSet.cpp
  /usr/bin/g++ -O3 -std=c++11 -pthread -o main_grid main_grid.cpp Graph.cpp BitMap.cpp HashSet.cpp
else
  echo "[INFO] Skip build (--skip-build)."
fi

echo "[INFO] datasets: ${DATASETS[*]}"
echo "[INFO] k-list: ${K_LIST[*]}"
echo "[INFO] mu-list: ${MU_LIST}"
echo "[INFO] cf-list: ${CF_LIST}"
echo "[INFO] smoke: ${SMOKE}"

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

  for k in "${K_LIST[@]}"; do
    out_csv="$ROOT_DIR/$dataset/${dataset}-grid-k${k}.csv"

    if [[ "$FORCE" -eq 1 ]]; then
      rm -f "$out_csv"
    fi

    if [[ "$FORCE" -eq 0 && -s "$out_csv" ]]; then
      echo "[SKIP] dataset=$dataset k=$k output exists: $out_csv"
      continue
    fi

    echo "[RUN] dataset=$dataset k=$k"
    if ./main_grid "$graph_path" "$k" "$dataset" "$MU_LIST" "$CF_LIST" "$out_csv"; then
      :
    else
      code=$?
      echo "[FAIL] dataset=$dataset k=$k exit_code=$code; retry by single-mu chunks"
      echo "$dataset,$k,$MU_LIST,$CF_LIST,$code,batch" >> "$FAILED_FILE"

      # Retry strategy: split by mu to reduce peak memory/fragmentation.
      # Keep partial successful results and only record failed chunks.
      rm -f "$out_csv"
      for mu in "${MU_ARR[@]}"; do
        echo "[RETRY] dataset=$dataset k=$k mu=$mu"
        if ./main_grid "$graph_path" "$k" "$dataset" "$mu" "$CF_LIST" "$out_csv"; then
          :
        else
          sub_code=$?
          echo "[FAIL] dataset=$dataset k=$k mu=$mu exit_code=$sub_code"
          echo "$dataset,$k,$mu,$CF_LIST,$sub_code,mu-chunk" >> "$FAILED_FILE"
        fi
      done
      continue
    fi
  done
done

echo "[INFO] Done. Outputs: $ROOT_DIR/<dataset>/<dataset>-grid-k<k>.csv"

3038488