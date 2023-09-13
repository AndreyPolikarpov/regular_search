#ifndef STORAGE_TNODE_H
#define STORAGE_TNODE_H

#include <cstdint>
#include <string>
#include <vector>
#include "tree_node.hpp"

namespace fr::tree {

class StorageTNode {
public:
  static bool rememberRegular(void *node_end, const std::string &regular);
  static const std::string &getRegular(tnode *node_end);

  static tnode &createTNode(uint8_t symbol);

  static std::vector<tnode> &isStorageTNode();
};

}

#endif // STORAGE_TNODE