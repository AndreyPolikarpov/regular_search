#include "tree_regular.hpp"
#include "storage_tnode.hpp"
#include "tree_node.hpp"
#include "search_tree.hpp"
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <tuple>
#include <vector>

#include "gtest/gtest.h"

namespace {

  std::string ReadStorageTNode() {
    std::string stirng_storage;
    for(const auto &node : fr::tree::StorageSymbol::isStorageTNode())
      stirng_storage += node.symbol;
    
    return stirng_storage;
  }

  std::string ReadStorageSpecialSymbol() {
    std::string stirng_storage;
    for(const auto &node : fr::tree::StorageSymbol::isStorageQuantifier())
      stirng_storage += node.symbol;
    
    return stirng_storage;
  }

TEST(CreateTree, Simpl) {

  std::string input_string{"Regular"};
  fr::tree::TreeRegular tr;
  tr.addRegularExpresion(input_string);

  EXPECT_TRUE(!fr::tree::StorageSymbol::isStorageTNode().empty()) << "Storage node is empty";
  EXPECT_STREQ(input_string.c_str(), ReadStorageTNode().c_str());
  
  fr::tree::StorageSymbol::ClearAllStorage();
  
  std::string input_string_2{"Reg*ul?a.r"};
  fr::tree::TreeRegular tr_2;
  tr_2.addRegularExpresion(input_string_2);

  EXPECT_TRUE(!fr::tree::StorageSymbol::isStorageTNode().empty()) << "Storage node is empty";
  EXPECT_STREQ(input_string.c_str(), ReadStorageTNode().c_str());
  EXPECT_STREQ("*?.", ReadStorageSpecialSymbol().c_str());

  fr::tree::StorageSymbol::ClearAllStorage();
  
  std::string input_string_3{"Reg..."};
  fr::tree::TreeRegular tr_3;
  tr_3.addRegularExpresion(input_string_3);

  EXPECT_TRUE(!fr::tree::StorageSymbol::isStorageTNode().empty()) << "Storage node is empty";
  EXPECT_STREQ("Reg", ReadStorageTNode().c_str());
  EXPECT_STREQ(".", ReadStorageSpecialSymbol().c_str());

  fr::tree::StorageSymbol::ClearAllStorage();

  std::string input_string_4{"Reg...r"};
  fr::tree::TreeRegular tr_4;
  tr_4.addRegularExpresion(input_string_4);

  EXPECT_TRUE(!fr::tree::StorageSymbol::isStorageTNode().empty()) << "Storage node is empty";
  EXPECT_STREQ("Regr", ReadStorageTNode().c_str());
  EXPECT_STREQ(".", ReadStorageSpecialSymbol().c_str());

  fr::tree::StorageSymbol::ClearAllStorage();

  std::string input_string_5{"Reg./?.r"};
  fr::tree::TreeRegular tr_5;
  tr_5.addRegularExpresion(input_string_5);

  EXPECT_TRUE(!fr::tree::StorageSymbol::isStorageTNode().empty()) << "Storage node is empty";
  EXPECT_STREQ("Reg?r", ReadStorageTNode().c_str());
  EXPECT_STREQ("..", ReadStorageSpecialSymbol().c_str());

  fr::tree::StorageSymbol::ClearAllStorage();

  std::string input_string_6{"Reg./r"};
  fr::tree::TreeRegular tr_6;
  tr_6.addRegularExpresion(input_string_6);

  EXPECT_TRUE(!fr::tree::StorageSymbol::isStorageTNode().empty()) << "Storage node is empty";
  EXPECT_STREQ("Reg/r", ReadStorageTNode().c_str());
  EXPECT_STREQ(".", ReadStorageSpecialSymbol().c_str());
}

TEST(SearchWithoutQuantifier, Simpl) {
  std::string regular1{"Regular"};
  //std::string regular2{"Reg/.lar"};
  //std::string regular3{"Regular"};
  fr::tree::TreeRegular tr;
  fr::tree::Searcher searcher;
  tr.addRegularExpresion(regular1);
  //tr.addRegularExpresion(regular2);
  //tr.addRegularExpresion(regular3);

  void *memory_test1 = nullptr;
  size_t offset_test1 = 0;
  std::string regular1_test1;

  std::tie(memory_test1, offset_test1, regular1_test1) = 
        searcher.search(fr::tree::StorageSymbol::isRootTree(),
                          reinterpret_cast<uint8_t*>(regular1.data()), regular1.size());

  EXPECT_STREQ(regular1.c_str(), regular1_test1.c_str());

  fr::tree::StorageSymbol::ClearAllStorage();

  std::string regular2("ABC");
  tr.addRegularExpresion("C");

  void *memory_test2 = nullptr;
  size_t offset_test2 = 0;
  std::string regular1_test2;


  std::tie(memory_test2, offset_test2, regular1_test2) = 
        searcher.search(fr::tree::StorageSymbol::isRootTree(),
                          reinterpret_cast<uint8_t*>(regular2.data()), regular2.size());

  EXPECT_STREQ("C", regular1_test2.c_str());

  std::tie(memory_test2, offset_test2, regular1_test2) = 
        searcher.search(fr::tree::StorageSymbol::isRootTree(),
                          reinterpret_cast<uint8_t*>(regular1.data()), regular1.size());

  EXPECT_TRUE(regular1_test2.empty());
}
}