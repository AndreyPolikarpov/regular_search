#include "storage_tnode.hpp"
#include "tree_node.hpp"

#include <utility>
#include <map>
#include <vector>

namespace fr::tree {

namespace {
  std::map<void *, std::string> storage_regular_;
  std::vector<tnode> storage_tnode_;
  std::vector<SpecialSymbol> storage_quantifier_;
  std::string empty_string_;
}

std::vector<tnode> &StorageSymbol::isStorageTNode() { return storage_tnode_;}
std::vector<SpecialSymbol> &StorageSymbol::isStorageQuantifier() { return storage_quantifier_;}

bool StorageSymbol::rememberRegular(void *node_end, const std::string &regular) {
  return storage_regular_.emplace(std::make_pair(node_end, regular)).second;
}

const std::string &StorageSymbol::getRegular(tnode *node_end) {
  auto search = storage_regular_.find(node_end);
  if(search != storage_regular_.end()) {
    return search->second;
  }

  return empty_string_;
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
}

}