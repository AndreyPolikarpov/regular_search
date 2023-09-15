#include "tree_node.hpp"

namespace fr::tree {

namespace {
  tnode g_empty_tnode;
  SpecialSymbol g_empty_quantifier;
}

tnode *isEmptyTNode() {return &g_empty_tnode;};
SpecialSymbol *isEmptySpecialSymbol() {return &g_empty_quantifier;};

}