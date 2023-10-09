#include "tree_node.hpp"
#include <cstdint>
#include <string>

namespace fr::tree {

namespace {
  tnode g_empty_tnode;
  SpecialSymbol g_empty_quantifier;
  uint8_t g_empty_uint8_t{0};
  std::string g_empty_regular{""};
}

tnode *isEmptyTNode() {return &g_empty_tnode;};
SpecialSymbol *isEmptySpecialSymbol() {return &g_empty_quantifier;};
uint8_t *isEmptyMemory() {return &g_empty_uint8_t;};
std::string &isEmptyRegular() {return g_empty_regular;}; 

}