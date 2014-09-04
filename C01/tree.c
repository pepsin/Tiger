#include "tree.h"
#include <string.h>

T_tree Tree(T_tree l, string k, void *b, T_tree r)
{
  T_tree t = checked_malloc(sizeof(*t));
  t->left = l;
  t->key = k;
  t->binding = b;
  t->right = r;
  return t;
}
  
T_tree tree_insert(string key, void *binding, T_tree t)
{
  if (t==NULL) {
    return Tree(NULL, key, binding, NULL);
  } else if (strcmp(key, t->key) < 0) {
    return Tree(tree_insert(key, binding, t->left), t->key, t->binding, t->right);
  } else if (strcmp(key, t->key) > 0) {
    return Tree(t->left, t->key, t->binding, tree_insert(key, binding, t->right));
  } else {
    return Tree(t->left, key, binding, t->right);
  }    
}

void *tree_lookup(string key, T_tree t)
{
  if (t==NULL) {
    return NULL;
  }
  int a = strcmp(key, t->key);
  if (!a) {
    return t->binding;
  } else {
    return last_can_null(tree_lookup(key, t->left), tree_lookup(key, t->right));
  }
}

int member(string key, T_tree t)
{
  if (t==NULL) {
    return FALSE;
  }
  int a = strcmp(key, t->key);
  if (!a) {
    return TRUE;
  } else {
    return member(key, t->left) || member(key, t->right);
  }
}