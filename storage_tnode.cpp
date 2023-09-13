#include "storage_tnode.hpp"
#include "tree_node.hpp"

#include <utility>
#include <map>
#include <vector>

namespace fr::tree {

namespace {
  std::map<void *, std::string> storage_regular_;
  std::vector<tnode> storage_tnode_;
  std::string empty_string_;
}

std::vector<tnode> &StorageTNode::isStorageTNode() { return storage_tnode_;}

bool StorageTNode::rememberRegular(void *node_end, const std::string &regular) {
  return storage_regular_.emplace(std::make_pair(node_end, regular)).second;
}

const std::string &StorageTNode::getRegular(tnode *node_end) {
  auto search = storage_regular_.find(node_end);
  if(search != storage_regular_.end()) {
    return search->second;
  }

  return empty_string_;
}

tnode &StorageTNode::createTNode(uint8_t symbol) {
  tnode node;
  node.symbol = symbol;
  storage_tnode_.emplace_back(std::move(node));

  return storage_tnode_.back();
}

}