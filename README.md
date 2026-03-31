# DSCAN — Directed Structural Clustering

DSCAN (Directed Structural Clustering on Networks) 是一种面向有向图的结构聚类算法，支持索引构建、参数化聚类查询、动态更新以及可扩展性测试。

## 项目结构

```
dscan/
├── main.cpp            # 主程序（索引/查询/更新）
├── Graph.cpp / .hpp    # 图数据结构与算法核心
├── BitMap.cpp / .hpp   # 位图辅助结构
├── HashSet.cpp / .hpp  # 哈希集合辅助结构
├── run.sh              # 一键运行脚本
├── extension/          # 扩展代码：k-离散化索引
│   ├── main.cpp             # 主程序入口
│   ├── main_grid.cpp        # 参数网格搜索入口
│   ├── Graph.cpp / .hpp     # 扩展图结构（含 k-index）
│   ├── BitMap.cpp / .hpp
│   └── HashSet.cpp / .hpp
└── README.md
```

## 环境要求

- **C++ 编译器**：支持 C++17（基础代码）/ C++11（扩展代码），如 `g++` 或 `clang++`
- **操作系统**：Linux / macOS

---

## 基础代码使用方法

### 编译

```bash
g++ -O3 -std=c++17 -o dscan main.cpp Graph.cpp BitMap.cpp HashSet.cpp
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
```

**示例：**

```bash
./dscan data/cora index
./dscan data/cora query 0.1 0.1 2
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

### 运行

```bash
./main <图数据目录> <k> <eps_c> <eps_f> <mu>
```

**参数说明：**
| 参数 | 说明 |
|------|------|
| `图数据目录` | 数据目录路径（与基础代码格式相同） |
| `k` | 离散化参数，值越大粒度越细 |
| `eps_c` | cycle 相似度阈值，范围 0.0–1.0 |
| `eps_f` | flow 相似度阈值，范围 0.0–1.0 |
| `mu` | 聚类密度阈值 |

**示例：**

```bash
./main /data/wiki 10 0.5 0.5 2
./main /data/web-Google 20 0.4 0.6 5
```

**输出：** 聚类结果保存在 `<图数据目录>/output/` 下。

---
