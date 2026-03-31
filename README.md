# DSCAN — Directed Structural Clustering

DSCAN (Directed Structural Clustering on Networks) 是一种面向有向图的结构聚类算法，支持索引构建、参数化聚类查询、动态更新以及可扩展性测试。

## 项目结构

```
dscan/
├── main.cpp           # 主程序（索引/查询/更新/可扩展性）
├── Graph.cpp / .hpp   # 图数据结构与算法核心
├── BitMap.cpp / .hpp   # 位图辅助结构
├── HashSet.cpp / .hpp  # 哈希集合辅助结构
├── run.sh             # 一键运行脚本（编译→预处理→建索引→查询→评估）
├── extension/         # 扩展代码：k-离散化索引 + 精度评估 + 参数网格搜索
│   ├── main.cpp            # 单参数精度评估入口
│   ├── main_grid.cpp       # 参数网格搜索入口
│   ├── Graph.cpp / .hpp    # 扩展图结构（含 k-index）
│   ├── BitMap.cpp / .hpp
│   ├── HashSet.cpp / .hpp
│   ├── run.sh              # 多数据集批量评估脚本
│   ├── run_accuracy_proxy.sh
│   ├── run_accuracy_proxy_all.sh
│   ├── run_accuracy_proxy_multi_params.sh
│   ├── run_all_params_all_experiments.sh
│   ├── ari.py              # 调整兰德指数 (ARI) 计算
│   ├── csv.py              # CSV 结果聚合
│   └── sample_friend_subgraphs.py  # 图子采样工具
└── README.md
```

## 环境要求

- **C++ 编译器**：支持 C++17（基础代码）/ C++11（扩展代码），如 `g++` 或 `clang++`
- **Python 3**（运行辅助脚本时需要）
- **操作系统**：Linux / macOS

---

## 基础代码使用方法

### 编译

```bash
g++ -O2 -std=c++17 -o dscan main.cpp Graph.cpp BitMap.cpp HashSet.cpp
```

### 数据格式

数据目录需包含以下文件：

```
<data_dir>/
├── degree.txt      # 第一行: n m（顶点数 边数）; 后 n 行每行: out_degree in_degree
├── out_edges.txt   # 第一行: m; 后 m 行每行: 目标顶点 ID
├── in_edges.txt    # 第一行: m; 后 m 行每行: 源顶点 ID
```

### 运行

DSCAN 支持 4 种运行模式：

```bash
# 1. 建立索引
./dscan <数据目录> index

# 2. 聚类查询（需要先建索引）
./dscan <数据目录> query <eps_c> <eps_f> <mu>

# 3. 动态更新测试（1000 次随机插入/删除）
./dscan <数据目录> update

# 4. 可扩展性测试（分阶段计时 + 内存报告）
./dscan <数据目录> scalability [eps_c eps_f mu]
```

**示例：**

```bash
./dscan data/cora index
./dscan data/cora query 0.1 0.1 2
```

### 一键运行

`run.sh` 封装了完整流程（编译 → 预处理 → 建索引 → 聚类查询 → 评估）：

```bash
./run.sh <数据目录> <eps_c> <eps_f> <mu>

# 示例
./run.sh data/cora 0.1 0.1 2
```

### 输出

- 索引文件保存在 `<数据目录>/index/` 下
- 聚类结果保存在 `<数据目录>/output/<eps_c>-<eps_f>-<mu>.txt`，格式：
  - `c <vertex_id> <cluster_id>` — 核心顶点
  - `n <vertex_id> <cluster_id>` — 非核心顶点
  - `h <vertex_id>` — 枢纽顶点

---

## 扩展代码使用方法（extension/）

扩展代码在基础 DSCAN 上引入了 **k-离散化索引**，通过将相似度分数离散化到 k 个区间来加速范围查询，并提供精度评估（precision / recall / F1 / RMSE）功能。

### 编译

```bash
cd extension/

# 编译单参数评估程序
g++ -O3 -std=c++11 -pthread -o main main.cpp Graph.cpp BitMap.cpp HashSet.cpp

# 编译参数网格搜索程序
g++ -O3 -std=c++11 -pthread -o main_grid main_grid.cpp Graph.cpp BitMap.cpp HashSet.cpp
```

### 运行 — 单参数精度评估

```bash
./main <图数据目录> <k> <数据集名> [c] [f]
# 或指定 mu:
./main <图数据目录> <k> <mu> <数据集名> [c] [f]
```

**参数说明：**
| 参数 | 说明 |
|------|------|
| `图数据目录` | 数据目录路径（与基础代码格式相同） |
| `k` | 离散化参数，值越大粒度越细 |
| `mu` | 聚类密度阈值（可选，默认等于 k） |
| `数据集名` | 用于输出 CSV 文件命名 |
| `c`, `f` | 相似度阈值，范围 0.0–1.0（默认 0.5） |

**示例：**

```bash
./main /data/wiki 10 wiki 0.5 0.5
./main /data/web-Google 20 5 web-Google 0.4 0.6
```

**输出：** 将精度指标追加写入 CSV 文件（精度、召回率、F1、RMSE 等）。

### 运行 — 参数网格搜索

```bash
./main_grid <图数据目录> <k> <数据集名> <mu列表> <cf值列表> <输出CSV>
```

**示例：**

```bash
./main_grid /data/wiki 10 wiki "2,5,10" "0.1,0.2,0.5,0.8" wiki-grid-k10.csv
```

这会遍历所有 mu × (c, f) 组合，将结果写入一个 CSV 文件。

### 批量实验脚本

```bash
# 基础批量评估（多 k 值 × 多数据集）
bash run.sh

# 精度评估（多 k 值）
bash run_accuracy_proxy.sh

# 多数据集精度评估
bash run_accuracy_proxy_all.sh

# 多参数组合评估
bash run_accuracy_proxy_multi_params.sh

# 全参数全实验（含检查点恢复、失败重试）
bash run_all_params_all_experiments.sh [--force] [--smoke]
```

> **注意**：批量脚本中的数据路径默认为服务器路径，使用前请修改脚本中的路径配置。

### 辅助 Python 脚本

```bash
# 计算两组聚类结果的调整兰德指数 (ARI)
python ari.py <输出目录>

# 聚合多个 k 值的 CSV 时间结果
python csv.py <目录> <文件前缀>

# 按度分布分层采样生成子图（10%, 20%, 30%, 40%）
python sample_friend_subgraphs.py --ratios 10,20,30,40
```

---

## 基础代码与扩展代码的主要区别

| 特性 | 基础代码 | 扩展代码 (extension/) |
|------|---------|----------------------|
| 索引结构 | 邻居相似度排序 | 邻居排序 + k-离散化二维索引 |
| 查询方式 | 精确阈值查询 | 离散化范围查询（更快） |
| 动态更新 | 支持边的插入/删除 | 不支持（专注查询性能） |
| 输出 | 聚类结果文件 | 精度评估 CSV（precision/recall/F1/RMSE） |
| 额外功能 | 可扩展性测试 | 参数网格搜索、批量实验、ARI 计算 |