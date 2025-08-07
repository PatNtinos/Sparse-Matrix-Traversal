# Sparse-Matrix-Traversal
This C program implements a sparse matrix data structure with graph traversal capabilities using Breadth First Search (BFS) while also detecting cycles in the BFS tree.

## Features

+ Sparse matrix implementation for graph representation
+ Breadth-First Search (BFS) traversal
+ Cycle detection via identification of non-tree edges


## How It Works

1. The graph is represented as a sparse matrix.

2. BFS is performed starting from a chosen root node.

3. During traversal, the program tracks:
  1. Visited nodes
  2. Parent-child relationships

4. Non-tree edges are identified.

5. These non-tree edges are analyzed to trace back to a common ancestor, helping detect cycles in the graph.
