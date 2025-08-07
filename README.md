# Sparse-Matrix-Traversal
This C program implements a sparse matrix data structure with graph traversal capabilities using Breadth First Search (BFS) while also detecting cycles in the BFS tree.

## Features

+ Sparse matrix implementation for graph representation
+ Breadth-First Search (BFS) traversal
+ Cycle detection via identification of non-tree edges


## How It Works

- The graph is represented as a **sparse matrix**.
- **Breadth-First Search (BFS)** is performed starting from a chosen root node.
- During traversal, the program tracks:
  - Visited nodes
  - Parent-child relationships
- **Non-tree edges** are identified.
- These non-tree edges are analyzed to trace back to a **common ancestor**, helping detect **cycles** in the graph.

