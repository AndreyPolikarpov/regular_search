#include "storage_tnode.hpp"
#include "tree_node.hpp"

#include <utility>
#include <map>
#include <vector>

namespace fr::tree {

namespace {
  std::map<void *, std::string> storage_regular_;
  tnode root_;
  std::vector<tnode> storage_tnode_;
  std::vector<SpecialSymbol> storage_quantifier_;
}

std::vector<tnode> &StorageSymbol::isStorageTNode() { return storage_tnode_;}
std::vector<SpecialSymbol> &StorageSymbol::isStorageQuantifier() { return storage_quantifier_;}

bool StorageSymbol::rememberRegular(void *node_end, const std::string &regular) {
  return storage_regular_.emplace(std::make_pair(node_end, regular)).second;
}

tnode &StorageSymbol::createTNode(uint8_t symbol) {
  tnode node;
  node.symbol = symbol;
  storage_tnode_.emplace_back(std::move(node));

  return storage_tnode_.back();
}

SpecialSymbol &StorageSymbol::createSpecialSymbol(uint8_t symbol) {
  SpecialSymbol special;
  special.symbol = symbol;
  storage_quantifier_.emplace_back(std::move(special));

  return storage_quantifier_.back();
}

void StorageSymbol::ClearAllStorage() {
  storage_regular_.clear();
  storage_tnode_.clear();  
  storage_quantifier_.clear();
  root_ = *isEmptyTNode();
}

tnode *StorageSymbol::isRootTree() {
  return &root_;
}

std::string &StorageSymbol::RegularExpressionMemorized(void *node_end) {
  if(storage_regular_.contains(node_end))
    return storage_regular_.find(node_end)->second;

  return isEmptyRegular();
}
}