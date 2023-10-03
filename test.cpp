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
  fr::tree::StorageSymbol::ClearAllStorage();

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
  fr::tree::StorageSymbol::ClearAllStorage();

  fr::tree::TreeRegular tr;
  fr::tree::Searcher searcher;


  std::string regular1{"Regular"};
  tr.addRegularExpresion(regular1);

  bool answer = 
        searcher.search(fr::tree::StorageSymbol::isRootTree(),
                          reinterpret_cast<uint8_t*>(regular1.data()), regular1.size());

  EXPECT_TRUE(answer);
  EXPECT_FALSE(searcher.answer_string.empty());
  EXPECT_STREQ(regular1.c_str(), searcher.answer_string.c_str());

  fr::tree::StorageSymbol::ClearAllStorage();
  searcher.ClearAnswer();

  std::string regular2("ABC");
  tr.addRegularExpresion("A");
  answer = searcher.search(fr::tree::StorageSymbol::isRootTree(),
                          reinterpret_cast<uint8_t*>(regular2.data()), regular2.size());

  EXPECT_TRUE(answer);
  EXPECT_FALSE(searcher.answer_string.empty());
  EXPECT_STREQ("A", searcher.answer_string.c_str());

  fr::tree::StorageSymbol::ClearAllStorage();
  searcher.ClearAnswer();

  std::string regular3("ABC");
  tr.addRegularExpresion("C");
  answer = searcher.search(fr::tree::StorageSymbol::isRootTree(),
                          reinterpret_cast<uint8_t*>(regular3.data()), regular3.size());

  EXPECT_TRUE(answer);
  EXPECT_FALSE(searcher.answer_string.empty());
  EXPECT_STREQ("C", searcher.answer_string.c_str());
}

TEST(SearchQuantifierDot, Simpl) {
  fr::tree::StorageSymbol::ClearAllStorage();

  fr::tree::TreeRegular tr;
  fr::tree::Searcher searcher;


  std::string regular1{"Regular"};
  tr.addRegularExpresion("Reg.");

  bool answer = 
        searcher.search(fr::tree::StorageSymbol::isRootTree(),
                          reinterpret_cast<uint8_t*>(regular1.data()), regular1.size());

  EXPECT_TRUE(answer);
  EXPECT_FALSE(searcher.answer_string.empty());
  EXPECT_STREQ("Regu", searcher.answer_string.c_str());
  EXPECT_STREQ("Reg", ReadStorageTNode().c_str());
  EXPECT_STREQ(".", ReadStorageSpecialSymbol().c_str());


}
}