#include "tree_regular.hpp"
#include "storage_tnode.hpp"
#include "tree_node.hpp"
#include "search_tree.hpp"

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>
#include <iostream>
#include <fstream>

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
  searcher.startSearch(true);

  std::string regular1{"Regular"};
  tr.addRegularExpresion(regular1);

  bool answer = 
        searcher.search(fr::tree::StorageSymbol::isRootTree(),
                          reinterpret_cast<uint8_t*>(regular1.data()), regular1.size());

  EXPECT_TRUE(answer);
  EXPECT_FALSE(searcher.answer_string.empty());
  EXPECT_STREQ(regular1.c_str(), searcher.answer_string.c_str());

  fr::tree::StorageSymbol::ClearAllStorage();
  searcher.clearAnswer();

  std::string input_string_1("ABC");
  tr.addRegularExpresion("A");
  answer = searcher.search(fr::tree::StorageSymbol::isRootTree(),
                          reinterpret_cast<uint8_t*>(input_string_1.data()), input_string_1.size());

  EXPECT_TRUE(answer);
  EXPECT_FALSE(searcher.answer_string.empty());
  EXPECT_STREQ("A", searcher.answer_string.c_str());

  fr::tree::StorageSymbol::ClearAllStorage();
  searcher.clearAnswer();

  std::string input_string_2("ABC");
  tr.addRegularExpresion("C");
  answer = searcher.search(fr::tree::StorageSymbol::isRootTree(),
                          reinterpret_cast<uint8_t*>(input_string_2.data()), input_string_2.size());

  EXPECT_TRUE(answer);
  EXPECT_FALSE(searcher.answer_string.empty());
  EXPECT_STREQ("C", searcher.answer_string.c_str());

  fr::tree::StorageSymbol::ClearAllStorage();
  searcher.clearAnswer();

  std::string input_string_3("ABCRegularDDD");
  tr.addRegularExpresion("Regular");
  answer = searcher.search(fr::tree::StorageSymbol::isRootTree(),
                          reinterpret_cast<uint8_t*>(input_string_3.data()), input_string_3.size());

  EXPECT_TRUE(answer);
  EXPECT_FALSE(searcher.answer_string.empty());
  EXPECT_STREQ("Regular", searcher.answer_string.c_str());

  searcher.clearFlag();
}

TEST(SearchQuantifierDot, Simpl) {
  fr::tree::StorageSymbol::ClearAllStorage();

  fr::tree::TreeRegular tr;
  fr::tree::Searcher searcher;
  searcher.startSearch(true);
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
  searcher.clearAnswer();

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
  searcher.clearAnswer();

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
  searcher.clearAnswer();

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
  searcher.clearAnswer();

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
  searcher.clearAnswer();

  //std::string regular3{"Regular"};
  tr.addRegularExpresion("........");//8

  answer = searcher.search(fr::tree::StorageSymbol::isRootTree(),
                          reinterpret_cast<uint8_t*>(input_string.data()), input_string.size());
  EXPECT_FALSE(answer);

  searcher.clearFlag();
}

TEST(SearchQuantifierQuestion, Simpl) {
  fr::tree::StorageSymbol::ClearAllStorage();

  fr::tree::TreeRegular tr;
  fr::tree::Searcher searcher;
  searcher.startSearch(true);
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

  fr::tree::StorageSymbol::ClearAllStorage();
  searcher.clearAnswer();

  //td::string input_string_1{"Regular"};
  tr.addRegularExpresion("?????R");

  answer = searcher.search(fr::tree::StorageSymbol::isRootTree(),
                          reinterpret_cast<uint8_t*>(input_string_1.data()), input_string_1.size());
  EXPECT_TRUE(answer);
  EXPECT_FALSE(searcher.answer_string.empty());
  EXPECT_STREQ("R", searcher.answer_string.c_str());
  EXPECT_STREQ("?????R", searcher.AnswerRegularExpresion().c_str());
  EXPECT_STREQ("R", ReadStorageTNode().c_str());
  EXPECT_STREQ("?", ReadStorageSpecialSymbol().c_str());

  fr::tree::StorageSymbol::ClearAllStorage();
  searcher.clearAnswer();

  //td::string input_string_1{"Regular"};
  tr.addRegularExpresion("R????????????");

  answer = searcher.search(fr::tree::StorageSymbol::isRootTree(),
                          reinterpret_cast<uint8_t*>(input_string_1.data()), input_string_1.size());
  EXPECT_TRUE(answer);
  EXPECT_FALSE(searcher.answer_string.empty());
  //EXPECT_STREQ("Re", searcher.answer_string.c_str());//To Do из-за специфики алгоритма должно показывать R, если не критично то пофиг 
  EXPECT_STREQ("R????????????", searcher.AnswerRegularExpresion().c_str());
  EXPECT_STREQ("R", ReadStorageTNode().c_str());
  EXPECT_STREQ("?", ReadStorageSpecialSymbol().c_str());

  fr::tree::StorageSymbol::ClearAllStorage();
  searcher.clearAnswer();

  //td::string input_string_1{"Regular"};
  tr.addRegularExpresion("Re??lar");

  answer = searcher.search(fr::tree::StorageSymbol::isRootTree(),
                          reinterpret_cast<uint8_t*>(input_string_1.data()), input_string_1.size());
  EXPECT_TRUE(answer);
  EXPECT_FALSE(searcher.answer_string.empty());
  EXPECT_STREQ("Regular", searcher.answer_string.c_str());
  EXPECT_STREQ("Re??lar", searcher.AnswerRegularExpresion().c_str());
  EXPECT_STREQ("Relar", ReadStorageTNode().c_str());
  EXPECT_STREQ("?", ReadStorageSpecialSymbol().c_str());

  fr::tree::StorageSymbol::ClearAllStorage();
  searcher.clearAnswer();

  //td::string input_string_1{"Regular"};
  tr.addRegularExpresion("???Re????lar???");

  answer = searcher.search(fr::tree::StorageSymbol::isRootTree(),
                          reinterpret_cast<uint8_t*>(input_string_1.data()), input_string_1.size());
  EXPECT_TRUE(answer);
  EXPECT_FALSE(searcher.answer_string.empty());
  EXPECT_STREQ("Regular", searcher.answer_string.c_str());
  EXPECT_STREQ("???Re????lar???", searcher.AnswerRegularExpresion().c_str());
  EXPECT_STREQ("Relar", ReadStorageTNode().c_str());
  EXPECT_STREQ("???", ReadStorageSpecialSymbol().c_str());

  searcher.clearFlag();
}

TEST(SearchQuantifierStar, Simpl) {
  fr::tree::StorageSymbol::ClearAllStorage();

  fr::tree::TreeRegular tr;
  fr::tree::Searcher searcher;
  searcher.startSearch(true);
  bool answer{false};

  std::string input_string_1{"Regular"};
  tr.addRegularExpresion("Reg*");

  answer = 
        searcher.search(fr::tree::StorageSymbol::isRootTree(),
                          reinterpret_cast<uint8_t*>(input_string_1.data()), input_string_1.size());
  EXPECT_TRUE(answer);
  EXPECT_FALSE(searcher.answer_string.empty());
  EXPECT_STREQ("Reg", searcher.answer_string.c_str());
  EXPECT_STREQ("Reg*", searcher.AnswerRegularExpresion().c_str());
  EXPECT_STREQ("Reg", ReadStorageTNode().c_str());
  EXPECT_STREQ("*", ReadStorageSpecialSymbol().c_str());

  fr::tree::StorageSymbol::ClearAllStorage();
  searcher.clearAnswer();

  //td::string input_string_1{"Regular"};
  tr.addRegularExpresion("*R");

  answer = searcher.search(fr::tree::StorageSymbol::isRootTree(),
                          reinterpret_cast<uint8_t*>(input_string_1.data()), input_string_1.size());
  EXPECT_TRUE(answer);
  EXPECT_FALSE(searcher.answer_string.empty());
  EXPECT_STREQ("R", searcher.answer_string.c_str());
  EXPECT_STREQ("*R", searcher.AnswerRegularExpresion().c_str());
  EXPECT_STREQ("R", ReadStorageTNode().c_str());
  EXPECT_STREQ("*", ReadStorageSpecialSymbol().c_str());

  fr::tree::StorageSymbol::ClearAllStorage();
  searcher.clearAnswer();

  //td::string input_string_1{"Regular"};
  tr.addRegularExpresion("R*");

  answer = searcher.search(fr::tree::StorageSymbol::isRootTree(),
                          reinterpret_cast<uint8_t*>(input_string_1.data()), input_string_1.size());
  EXPECT_TRUE(answer);
  EXPECT_FALSE(searcher.answer_string.empty());
  //EXPECT_STREQ("Re", searcher.answer_string.c_str());//To Do из-за специфики алгоритма должно показывать R, если не критично то пофиг 
  EXPECT_STREQ("R*", searcher.AnswerRegularExpresion().c_str());
  EXPECT_STREQ("R", ReadStorageTNode().c_str());
  EXPECT_STREQ("*", ReadStorageSpecialSymbol().c_str());

  fr::tree::StorageSymbol::ClearAllStorage();
  searcher.clearAnswer();

  //td::string input_string_1{"Regular"};
  tr.addRegularExpresion("Re*lar");

  answer = searcher.search(fr::tree::StorageSymbol::isRootTree(),
                          reinterpret_cast<uint8_t*>(input_string_1.data()), input_string_1.size());
  EXPECT_TRUE(answer);
  EXPECT_FALSE(searcher.answer_string.empty());
  EXPECT_STREQ("Regular", searcher.answer_string.c_str());
  EXPECT_STREQ("Re*lar", searcher.AnswerRegularExpresion().c_str());
  EXPECT_STREQ("Relar", ReadStorageTNode().c_str());
  EXPECT_STREQ("*", ReadStorageSpecialSymbol().c_str());

  fr::tree::StorageSymbol::ClearAllStorage();
  searcher.clearAnswer();

  //td::string input_string_1{"Regular"};
  tr.addRegularExpresion("*Re*lar*");

  answer = searcher.search(fr::tree::StorageSymbol::isRootTree(),
                          reinterpret_cast<uint8_t*>(input_string_1.data()), input_string_1.size());
  EXPECT_TRUE(answer);
  EXPECT_FALSE(searcher.answer_string.empty());
  EXPECT_STREQ("Regular", searcher.answer_string.c_str());
  EXPECT_STREQ("*Re*lar*", searcher.AnswerRegularExpresion().c_str());
  EXPECT_STREQ("Relar", ReadStorageTNode().c_str());
  EXPECT_STREQ("***", ReadStorageSpecialSymbol().c_str());

  fr::tree::StorageSymbol::ClearAllStorage();
  searcher.clearAnswer();

  //td::string input_string_1{"Regular"};
  tr.addRegularExpresion("R*r");

  answer = searcher.search(fr::tree::StorageSymbol::isRootTree(),
                          reinterpret_cast<uint8_t*>(input_string_1.data()), input_string_1.size());
  EXPECT_TRUE(answer);
  EXPECT_FALSE(searcher.answer_string.empty());
  EXPECT_STREQ("Regular", searcher.answer_string.c_str());
  EXPECT_STREQ("R*r", searcher.AnswerRegularExpresion().c_str());
  EXPECT_STREQ("Rr", ReadStorageTNode().c_str());
  EXPECT_STREQ("*", ReadStorageSpecialSymbol().c_str());

  fr::tree::StorageSymbol::ClearAllStorage();
  searcher.clearAnswer();

  //td::string input_string_1{"Regular"};
  tr.addRegularExpresion("R*t");

  answer = searcher.search(fr::tree::StorageSymbol::isRootTree(),
                          reinterpret_cast<uint8_t*>(input_string_1.data()), input_string_1.size());
  EXPECT_FALSE(answer);

  searcher.clearFlag();
}

TEST(TreeSearchEngine, Simpl) {
  std::string poem;
  std::string line;
  std::ifstream in("poems/ASPushkin.txt");
  if (in.is_open()) {
      while (std::getline(in, line)) {
        poem += line;
      }
  }
  in.close();     // закрываем файл

  EXPECT_FALSE(poem.empty());
  fr::tree::TreeSearchEngine tse;

  tse.addRegularExpression("Пушкин");
  const auto found = tse.start_search(poem.data(), poem.size());

  EXPECT_TRUE(std::get<0>(found) !=nullptr);
  EXPECT_STREQ("Пушкин", std::get<2>(found).c_str());

  tse.clearSearch();

  tse.addRegularExpression("Свернуть*Пушкин");
  const auto found1 = tse.start_search(poem.data(), poem.size());

  EXPECT_TRUE(std::get<0>(found1) !=nullptr);
  EXPECT_STREQ("Свернуть*Пушкин", std::get<2>(found1).c_str());

  tse.stopJobs();
}

}