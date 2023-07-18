#pragma once

// Find the root of the tree of node i
template <typename LabelT>
inline static LabelT FindRoot(const LabelT *P, LabelT i) {
  LabelT root = i;
  while (P[root] < root) {
    root = P[root];
  }
  return root;
}

// Make all nodes in the path of node i point to root
template <typename LabelT>
inline static void SetRoot(LabelT *P, LabelT i, LabelT root) {
  while (P[i] < i) {
    LabelT j = P[i];
    P[i] = root;
    i = j;
  }
  P[i] = root;
}

// Find the root of the tree of the node i and compress the path in the process
template <typename LabelT> inline static LabelT Find(LabelT *P, LabelT i) {
  LabelT root = FindRoot(P, i);
  SetRoot(P, i, root);
  return root;
}

// unite the two trees containing nodes i and j and return the new root
template <typename LabelT>
inline static LabelT SetUnion(LabelT *P, LabelT i, LabelT j) {
  LabelT root = FindRoot(P, i);
  if (i != j) {
    LabelT rootj = FindRoot(P, j);
    if (root > rootj) {
      root = rootj;
    }
    SetRoot(P, j, root);
  }
  SetRoot(P, i, root);
  return root;
}

// Flatten the Union Find tree and relabel the components
template <typename LabelT>
inline static LabelT Flatten(LabelT *P, LabelT length) {
  LabelT k = 1;
  for (LabelT i = 1; i < length; ++i) {
    if (P[i] < i) {
      P[i] = P[P[i]];
    } else {
      P[i] = k;
      k = k + 1;
    }
  }
  return k - 1;
}
