#include "tree_node.hpp"

namespace fr::tree {

namespace {
  tnode g_empty_tnode;
}

tnode &isEmptyTNode() {return g_empty_tnode;};

}