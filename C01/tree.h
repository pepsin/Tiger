#include "util.h"
typedef struct tree *T_tree;
struct tree {
  T_tree left;
  string key;
  void *binding;
  T_tree right;
};

T_tree Tree(T_tree l, string key, void *binding, T_tree r);
T_tree tree_insert(string key, void *binding, T_tree t);
void *tree_lookup(string key, T_tree t);
int member(string key, T_tree t);