#include "tree_regular.hpp"
#include "storage_tnode.hpp"
#include "tree_node.hpp"
#include <iostream>
#include <vector>

#include "gtest/gtest.h"

namespace {

  std::string ReadStorageTNode() {
    std::string stirng_storage;
    for(const auto &node : fr::tree::StorageTNode().isStorageTNode())
      stirng_storage += node.symbol;
    
    return stirng_storage;
  }

TEST(CreateTree, Simpl) {

  std::string input_string{"Regular"};
  fr::tree::TreeRegular tr;
  tr.addRegularExpresion(input_string);

  EXPECT_TRUE(!fr::tree::StorageTNode().isStorageTNode().empty()) << "Storage node is empty";
  EXPECT_STREQ(input_string.c_str(), ReadStorageTNode().c_str());
  
  fr::tree::StorageTNode::ClearAllStorage();
  
  std::string input_string_2{"Reg*ul?a.r"};
  fr::tree::TreeRegular tr_2;
  tr_2.addRegularExpresion(input_string_2);

  EXPECT_TRUE(!fr::tree::StorageTNode().isStorageTNode().empty()) << "Storage node is empty";
  EXPECT_STREQ(input_string.c_str(), ReadStorageTNode().c_str());

  fr::tree::StorageTNode::ClearAllStorage();
  
  std::string input_string_3{"Reg..."};
  fr::tree::TreeRegular tr_3;
  tr_3.addRegularExpresion(input_string_3);

  EXPECT_TRUE(!fr::tree::StorageTNode().isStorageTNode().empty()) << "Storage node is empty";
  EXPECT_STREQ("Reg", ReadStorageTNode().c_str());

  fr::tree::StorageTNode::ClearAllStorage();

  std::string input_string_4{"Reg...r"};
  fr::tree::TreeRegular tr_4;
  tr_4.addRegularExpresion(input_string_4);

  EXPECT_TRUE(!fr::tree::StorageTNode().isStorageTNode().empty()) << "Storage node is empty";
  EXPECT_STREQ("Regr", ReadStorageTNode().c_str());
}

}