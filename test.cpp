#include "tree_regular.hpp"
#include "storage_tnode.hpp"
#include "tree_node.hpp"
#include <iostream>
#include <vector>

#include "gtest/gtest.h"

namespace {

TEST(CreateTree, Simpl) {

  fr::tree::TreeRegular tr;
  tr.addRegularExpresion(std::string("Regular"));

  bool empty = !fr::tree::StorageTNode().isStorageTNode().empty();

  for(const auto &node : fr::tree::StorageTNode().isStorageTNode()) {
    std::cout << node.symbol;
  }
  std::cout << std::endl;

  EXPECT_TRUE(empty) << "Storage node is empty";
  //EXPECT_EQ(2, Factorial(2));
  //EXPECT_EQ(6, Factorial(3));
  //EXPECT_EQ(40320, Factorial(8));
}

}