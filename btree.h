#pragma once

#include <cstddef>
#include <iostream>
#include <algorithm>

template<typename Tp, std::size_t Order>
struct TreeNode {
  TreeNode *parent_;
  Tp k_[Order];
  std::size_t count_;
  TreeNode<Tp, Order> **childs_;
  bool leaf_;

  TreeNode() : parent_(nullptr), childs_(nullptr), count_(0), leaf_(true) {
    std::fill(k_, k_ + Order, 0);
  }
  ~TreeNode() {
    if (childs_) {
      delete[] *childs_;
    }

    if (parent_) {
      delete parent_;
    }
  }
};

template<typename Tp, std::size_t Order>
void insert(TreeNode<Tp, Order> root, const Tp &val);

template<typename Tp, std::size_t Order>
Tp find_median(TreeNode<Tp, Order> *node) {
  Tp median;
  std::nth_element(node->k_, node->k_ + Order/2, node->k_+Order);
  return node->k_[Order/2];
}

template<typename Tp, std::size_t Order>
void print_bfs(TreeNode<Tp, Order> node, int layer);
// B树分裂
template<typename Tp, std::size_t Order>
void split(TreeNode<Tp, Order> *node, const Tp &val);

bool is_full(const std::size_t &k_num, const std::size_t order) {
  return k_num >= order - 1;
}

template<typename Tp, std::size_t Order>
TreeNode<Tp, Order> *create_btree() {
  TreeNode<Tp, Order> *node = new TreeNode<Tp, Order>();
  node->childs_ = new TreeNode<Tp, Order> *[Order - 1];
  return node;
}

template<typename Tp, std::size_t Order>
TreeNode<Tp, Order> *find_node_tobe_inserted(TreeNode<Tp, Order> *node, const Tp &val) {
  if (node->leaf_) {
    return node;
  }
  for (std::size_t idx = 0; idx <= node->count_; ++idx) {
    if (idx == node->count_ || val < node->k_[idx]) {
      assert(node->childs_[idx]);
      return find_node_tobe_inserted<Tp, Order>(node->childs_[idx], val);
    }
  }
  return node;
}

// find find less than e.g.
// @ks [1, 2, 9, 11]
// @val 7
// return 2
template<typename Tp, std::size_t Order>
std::size_t insert_key(const std::size_t count, const Tp &val, Tp *ks) {
  std::size_t idx = 0;
  for (idx = 0; idx < Order - 1 & idx < count; ++idx) {
    if (val < ks[idx]) {
      break;
    }
  }
  if (idx == Order - 1) {
    ks[idx] = val;
  } else {
    std::copy(ks + idx, ks + Order, ks + idx + 1);
    ks[idx] = val;
  }
  return idx;
}

template<typename Tp, std::size_t Order>
void insert(TreeNode<Tp, Order> *root, const Tp &val) {
  TreeNode<Tp, Order> *node = find_node_tobe_inserted<Tp, Order>(root, val);
  if (is_full(node->count_, Order)) {
    node->k_[Order - 1] = val;
    split(node);
  } else {
    insert_key<Tp, Order>(node->count_, val, node->k_);
    node->count_ += 1;
  }
}

template<typename Tp, std::size_t Order>
void add_median_to_parent(TreeNode<Tp, Order> *node,
                          const Tp &median,
                          TreeNode<Tp, Order> *lhs,
                          TreeNode<Tp, Order> *rhs) {
  std::size_t pos = insert_key<Tp, Order>(node->count_, median, node->k_);
  node->childs_[pos] = lhs;
  node->childs_[pos+1] = rhs;
}

template<typename Tp, std::size_t Order>
void split(TreeNode<Tp, Order> *node) {
  // when root split node is nullptr
  if (!is_full(node->count_, Order) || node == nullptr) {
    return;
  }

  Tp median = find_median<Tp, Order>(node);
  TreeNode<Tp, Order> *left = create_btree<Tp, Order>();
  TreeNode<Tp, Order> *right = create_btree<Tp, Order>();

  // no parent. must be root node
  if (!node->parent_) {
    memset(node->k_, 0, sizeof(node->k_));
    node->count_ = 1;
    node->childs_[0] = left;
    node->childs_[1] = right;
    node->k_[0] = median;
    std::cout << "median " << median << std::endl;
    node->leaf_ = false;
    return;
  } else {
    add_median_to_parent(node->parent_, median, left, right);
  }
  return split(node->parent_);
}

template<typename Tp, std::size_t Order>
void print_bfs(TreeNode<Tp, Order> *node, int layer) {
  if (!node) return;
  std::cout << "count " << node->count_ << std::endl;
  for (std::size_t idx = 0; idx < node->count_; ++idx) {
    std::cout << node->k_[idx] << " ";
  }
  /*
  for (std::size_t idx = 0; idx <= node->count_; ++idx) {
    print_bfs(node->childs_[idx], layer + 1);
  }
   */
  std::cout << std::endl;
}