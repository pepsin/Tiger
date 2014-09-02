#include "impl.h"
int exp_list_length(A_expList list)
{
  if (list->kind == A_lastExpList) {
    return 1;
  } else {
    return 1 + exp_list_length(list->u.pair.tail);
  }
}

int maxargs_exp(A_exp exp)
{
  switch (exp->kind) {
    case A_idExp:
      return 0;
      
    case A_numExp:
      return 0;
      
    case A_opExp:
      return max(maxargs_exp(exp->u.op.left),
                 maxargs_exp(exp->u.op.right));
                 
    case A_eseqExp:
      return max(maxargs(exp->u.eseq.stm),
                 maxargs_exp(exp->u.eseq.exp));
  }
}

int maxargs_explist(A_expList list)
{
  if (list->kind == A_pairExpList) {
    return max(maxargs_exp(list->u.pair.head),
               maxargs_explist(list->u.pair.tail));
  } else {
    return maxargs_exp(list->u.pair.head);
  }
}

int maxargs(A_stm stm)
{
  switch (stm->kind) {
    case A_compoundStm:
      return max(maxargs(stm->u.compound.stm1),
                 maxargs(stm->u.compound.stm2));
                 
    case A_assignStm:
      return maxargs_exp(stm->u.assign.exp);

    case A_printStm:
      return max(exp_list_length(stm->u.print.exps),
                 maxargs_explist(stm->u.print.exps));
  }
}

Table_ Table(string id, int value, Table_ tail)
{
  Table_ t = (Table_)checked_malloc(sizeof(struct table));
  t->id = id;
  t->value = value;
  t->tail = tail;
  return t;
}

int lookup(Table_ t, string key)
{
  while (t && t->id != key)
    t = t->tail;

  assert(t);
  return t->value;
}

Table_ update(Table_ t, string id, int value)
{
  while (t && t->id != id)
    t = t->tail;

  if (t) {
    t->value = value;
    return t;
  }
  else
    return Table(id, value, t);
}

void interp(A_stm s)
{
  interpStm(s, NULL);
}

Table_ print(A_expList explist, Table_ t)
{
  struct int_and_table t2;
  switch (explist->kind) {
    case A_pairExpList:
      t2 = interpExp(explist->u.pair.head, t);
      printf("%s = %d ", t2.t->id, t2.value);
      return print(explist->u.pair.tail, t2.t);
    case A_lastExpList:
      t2 = interpExp(explist->u.last, t);
      printf("%s = %d\n", t2.t->id, t2.value);
      return t2.t;
  }
}

Table_ interpStm(A_stm s, Table_ t)
{
  struct int_and_table t2;
  switch (s->kind) {
    case A_compoundStm:
      return interpStm(s->u.compound.stm2,
                       interpStm(s->u.compound.stm1, t));
    case A_assignStm:
      t2 = interpExp(s->u.assign.exp, t);
      return update(t2.t, s->u.assign.id, t2.value);
    case A_printStm:
      return print(s->u.print.exps, t);
  }
}

struct int_and_table interpExp(A_exp e, Table_ t)
{
  struct int_and_table it;
  struct int_and_table t1;
  struct int_and_table t2;
  switch (e->kind) {
    case A_idExp:
      it.value = lookup(t, e->u.id);
      it.t = t;
      break;

    case A_numExp:
      it.value = e->u.num;
      it.t = t;
      break;

    case A_opExp:
      t1 = interpExp(e->u.op.left, t);
      t2 = interpExp(e->u.op.right, t1.t);
      switch (e->u.op.oper) {
        case A_plus:
          it.value = t1.value + t2.value;
          break;
        case A_minus:
          it.value = t1.value - t2.value;
          break;
        case A_times:
          it.value = t1.value * t2.value;
          break;
        case A_div:
          it.value = t1.value / t2.value;
          break;
      }
      it.t = t2.t;
      break;

    case A_eseqExp:
      it = interpExp(e->u.eseq.exp,
                     interpStm(e->u.eseq.stm, t));
      break;
  }

  return it;
}