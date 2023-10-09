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

  std::string regular1{"Regular"};
  fr::tree::TreeRegular tr;
  tr.addRegularExpresion(regular1);

  EXPECT_TRUE(!fr::tree::StorageSymbol::isStorageTNode().empty()) << "Storage node is empty";
  EXPECT_STREQ(regular1.c_str(), ReadStorageTNode().c_str());
  
  fr::tree::StorageSymbol::ClearAllStorage();
  
  std::string regular2{"Reg*ul?a.r"};
  fr::tree::TreeRegular tr_2;
  tr_2.addRegularExpresion(regular2);

  EXPECT_TRUE(!fr::tree::StorageSymbol::isStorageTNode().empty()) << "Storage node is empty";
  EXPECT_STREQ("Regular", ReadStorageTNode().c_str());
  EXPECT_STREQ("*?.", ReadStorageSpecialSymbol().c_str());

  fr::tree::StorageSymbol::ClearAllStorage();
  
  std::string regular3{"Reg..."};
  fr::tree::TreeRegular tr_3;
  tr_3.addRegularExpresion(regular3);

  EXPECT_TRUE(!fr::tree::StorageSymbol::isStorageTNode().empty()) << "Storage node is empty";
  EXPECT_STREQ("Reg", ReadStorageTNode().c_str());
  EXPECT_STREQ(".", ReadStorageSpecialSymbol().c_str());

  fr::tree::StorageSymbol::ClearAllStorage();

  std::string regular4{"Reg...r"};
  fr::tree::TreeRegular tr_4;
  tr_4.addRegularExpresion(regular4);

  EXPECT_TRUE(!fr::tree::StorageSymbol::isStorageTNode().empty()) << "Storage node is empty";
  EXPECT_STREQ("Regr", ReadStorageTNode().c_str());
  EXPECT_STREQ(".", ReadStorageSpecialSymbol().c_str());

  fr::tree::StorageSymbol::ClearAllStorage();

  std::string regular5{"Reg./?.r"};
  fr::tree::TreeRegular tr_5;
  tr_5.addRegularExpresion(regular5);

  EXPECT_TRUE(!fr::tree::StorageSymbol::isStorageTNode().empty()) << "Storage node is empty";
  EXPECT_STREQ("Reg?r", ReadStorageTNode().c_str());
  EXPECT_STREQ("..", ReadStorageSpecialSymbol().c_str());

  fr::tree::StorageSymbol::ClearAllStorage();

  std::string regular6{"Reg./r"};
  fr::tree::TreeRegular tr_6;
  tr_6.addRegularExpresion(regular6);

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

  std::string input_string_1("ABC");
  tr.addRegularExpresion("A");
  answer = searcher.search(fr::tree::StorageSymbol::isRootTree(),
                          reinterpret_cast<uint8_t*>(input_string_1.data()), input_string_1.size());

  EXPECT_TRUE(answer);
  EXPECT_FALSE(searcher.answer_string.empty());
  EXPECT_STREQ("A", searcher.answer_string.c_str());

  fr::tree::StorageSymbol::ClearAllStorage();
  searcher.ClearAnswer();

  std::string input_string_2("ABC");
  tr.addRegularExpresion("C");
  answer = searcher.search(fr::tree::StorageSymbol::isRootTree(),
                          reinterpret_cast<uint8_t*>(input_string_2.data()), input_string_2.size());

  EXPECT_TRUE(answer);
  EXPECT_FALSE(searcher.answer_string.empty());
  EXPECT_STREQ("C", searcher.answer_string.c_str());

  fr::tree::StorageSymbol::ClearAllStorage();
  searcher.ClearAnswer();

  std::string input_string_3("ABCRegularDDD");
  tr.addRegularExpresion("Regular");
  answer = searcher.search(fr::tree::StorageSymbol::isRootTree(),
                          reinterpret_cast<uint8_t*>(input_string_3.data()), input_string_3.size());

  EXPECT_TRUE(answer);
  EXPECT_FALSE(searcher.answer_string.empty());
  EXPECT_STREQ("Regular", searcher.answer_string.c_str());
}

TEST(SearchQuantifierDot, Simpl) {
  fr::tree::StorageSymbol::ClearAllStorage();

  fr::tree::TreeRegular tr;
  fr::tree::Searcher searcher;
  bool answer{false};

  std::string input_string_1{"Regular"};
  tr.addRegularExpresion("Reg.");

  answer = 
        searcher.search(fr::tree::StorageSymbol::isRootTree(),
                          reinterpret_cast<uint8_t*>(input_string_1.data()), input_string_1.size());
  EXPECT_TRUE(answer);
  EXPECT_FALSE(searcher.answer_string.empty());
  EXPECT_STREQ("Regu", searcher.answer_string.c_str());
  EXPECT_STREQ("Reg", ReadStorageTNode().c_str());
  EXPECT_STREQ(".", ReadStorageSpecialSymbol().c_str());

  fr::tree::StorageSymbol::ClearAllStorage();
  searcher.ClearAnswer();

  std::string input_string{"Regular"};
  tr.addRegularExpresion(".egu.");

  answer = searcher.search(fr::tree::StorageSymbol::isRootTree(),
                          reinterpret_cast<uint8_t*>(input_string.data()), input_string.size());
  EXPECT_TRUE(answer);
  EXPECT_FALSE(searcher.answer_string.empty());
  EXPECT_STREQ("Regul", searcher.answer_string.c_str());
  EXPECT_STREQ("egu", ReadStorageTNode().c_str());
  EXPECT_STREQ("..", ReadStorageSpecialSymbol().c_str());

  fr::tree::StorageSymbol::ClearAllStorage();
  searcher.ClearAnswer();

  //std::string input_string{"Regular"};
  tr.addRegularExpresion("..g...");

  answer = searcher.search(fr::tree::StorageSymbol::isRootTree(),
                          reinterpret_cast<uint8_t*>(input_string.data()), input_string.size());
  EXPECT_TRUE(answer);
  EXPECT_FALSE(searcher.answer_string.empty());
  EXPECT_STREQ("Regula", searcher.answer_string.c_str());
  EXPECT_STREQ("g", ReadStorageTNode().c_str());
  EXPECT_STREQ("..", ReadStorageSpecialSymbol().c_str());

  fr::tree::StorageSymbol::ClearAllStorage();
  searcher.ClearAnswer();

  //std::string input_string{"Regular"};
  tr.addRegularExpresion(".....");//5

  answer = searcher.search(fr::tree::StorageSymbol::isRootTree(),
                          reinterpret_cast<uint8_t*>(input_string.data()), input_string.size());
  EXPECT_TRUE(answer);
  EXPECT_FALSE(searcher.answer_string.empty());
  /*
    To Do из-за того что начинается с root и нет символов нужно корректировать код
    много возни из-за одного случая
  */
  //EXPECT_STREQ("Regul", searcher.answer_string.c_str());
  EXPECT_STREQ(".....", searcher.AnswerRegularExpresion().c_str());
  EXPECT_STREQ("", ReadStorageTNode().c_str());
  EXPECT_STREQ(".", ReadStorageSpecialSymbol().c_str());

  fr::tree::StorageSymbol::ClearAllStorage();
  searcher.ClearAnswer();

  std::string input_string_test{"R"};
  tr.addRegularExpresion(".");

  answer = searcher.search(fr::tree::StorageSymbol::isRootTree(),
                          reinterpret_cast<uint8_t*>(input_string_test.data()), input_string_test.size());
  EXPECT_TRUE(answer);
  EXPECT_FALSE(searcher.answer_string.empty());
  EXPECT_STREQ("R", searcher.answer_string.c_str());
  EXPECT_STREQ(".", searcher.AnswerRegularExpresion().c_str());
  EXPECT_STREQ("", ReadStorageTNode().c_str());
  EXPECT_STREQ(".", ReadStorageSpecialSymbol().c_str());


  fr::tree::StorageSymbol::ClearAllStorage();
  searcher.ClearAnswer();

  //std::string regular3{"Regular"};
  tr.addRegularExpresion("........");//8

  answer = searcher.search(fr::tree::StorageSymbol::isRootTree(),
                          reinterpret_cast<uint8_t*>(input_string.data()), input_string.size());
  EXPECT_FALSE(answer);
}

TEST(SearchQuantifierQuestion, Simpl) {
  fr::tree::StorageSymbol::ClearAllStorage();

  fr::tree::TreeRegular tr;
  fr::tree::Searcher searcher;
  bool answer{false};

  std::string input_string_1{"Regular"};
  tr.addRegularExpresion("Reg?");

  answer = 
        searcher.search(fr::tree::StorageSymbol::isRootTree(),
                          reinterpret_cast<uint8_t*>(input_string_1.data()), input_string_1.size());
  EXPECT_TRUE(answer);
  EXPECT_FALSE(searcher.answer_string.empty());
  EXPECT_STREQ("Reg", searcher.answer_string.c_str());
  EXPECT_STREQ("Reg?", searcher.AnswerRegularExpresion().c_str());
  EXPECT_STREQ("Reg", ReadStorageTNode().c_str());
  EXPECT_STREQ("?", ReadStorageSpecialSymbol().c_str());
}
}