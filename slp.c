#include "util.h"
#include "slp.h"

A_stm A_CompoundStm(A_stm stm1, A_stm stm2) {
  A_stm s = checked_malloc(sizeof *s);
  s->kind = A_compoundStm;
  s->u.compound.stm1 = stm1;
  s->u.compound.stm2 = stm2;
  return s;
}

A_stm A_AssignStm(string id, A_exp exp) {
  A_stm s = checked_malloc(sizeof *s);
  s->kind = A_assignStm;
  s->u.assign.id = id;
  s->u.assign.exp = exp;
  return s;
}

A_stm A_PrintStm(A_expList exps) {
  A_stm s = checked_malloc(sizeof *s);
  s->kind = A_printStm;
  s->u.print.exps = exps;
  return s;
}

A_exp A_IdExp(string id) {
  A_exp e = checked_malloc(sizeof *e);
  e->kind = A_idExp;
  e->u.id = id;
  return e;
}

A_exp A_NumExp(int num) {
  A_exp e = checked_malloc(sizeof *e);
  e->kind = A_numExp;
  e->u.num = num;
  return e;
}

A_exp A_OpExp(A_exp left, A_binop oper, A_exp right) {
  A_exp e = checked_malloc(sizeof *e);
  e->kind = A_opExp;
  e->u.op.left = left;
  e->u.op.oper = oper;
  e->u.op.right = right;
  return e;
}

A_exp A_EseqExp(A_stm stm, A_exp exp) {
  A_exp e = checked_malloc(sizeof *e);
  e->kind = A_eseqExp;
  e->u.eseq.stm = stm;
  e->u.eseq.exp = exp;
  return e;
}

A_expList A_PairExpList(A_exp head, A_expList tail) {
  A_expList e = checked_malloc(sizeof *e);
  e->kind = A_pairExpList;
  e->u.pair.head = head;
  e->u.pair.tail = tail;
  return e;
}

A_expList A_LastExpList(A_exp last) {
  A_expList e = checked_malloc(sizeof *e);
  e->kind = A_lastExpList;
  e->u.last = last;
  return e;
}

int maxargs(A_stm stm, int count)
{
  if (stm->kind == A_compoundStm) {
    int stm1_count = maxargs(stm->u.compound.stm1, count);
    int stm2_count = maxargs(stm->u.compound.stm2, count);
    if (stm1_count > stm2_count) {
      if (stm1_count > count) {
        return stm1_count;
      }
    } else {
      if (stm2_count > count) {
        return stm2_count;
      }
    }
    return count;
  } else if (stm->kind == A_printStm) {
    int new_count = exp_list_count(stm->u.print.exps, 0);
    if (new_count > count) {
      return new_count;
    }
    return count;
  } else if (stm->kind == A_assignStm){
    if (stm->u.assign.exp->kind == A_eseqExp) {
      A_stm s = stm->u.assign.exp->u.eseq.stm;
      int stm_count = maxargs(s, count);
      if (stm_count > count) {
        return stm_count;
      }
      return count;
    }
  }
}

int exp_list_count(A_expList list, int count)
{
  if (list->kind == A_lastExpList) {
    return count + 1;
  } else {
    return exp_list_count(list->u.pair.tail, count + 1);
  }
}