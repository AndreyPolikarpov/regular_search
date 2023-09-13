#ifndef TREE_REGULAR_H
#define TREE_REGULAR_H

#include <cstdint>
#include <iostream>
#include <array>
#include <string>
#include <memory>

#include "tree_node.hpp"

namespace fr::tree {

//struct tnode;


//to do для * нужно добавить хранилище в котором будет добавляться значения стоящие после * и нужно будет искать
// и добавить это хранилище в общий поиск для проверки 

/*
 так же ограничения по типу регулярных выражении:
  1. Одинаковый вид квалификаторов может повторятся сколь угодно раз,
      но разные типы квалификаторов не могут повторятся друг за другом
      например: .? не правильно
                .. или ?? правильно
*/

/**
 * @brief "дерево" для регулярных выражений
 * 
 */
class TreeRegular {
private:
  tnode root;  
  bool addRegularElement(tnode *head, std::string::const_iterator &it,
                  const std::string &re);
  bool addSpecialSymbol(tnode *head, std::string::const_iterator &it, 
                  const std::string &re);
public:
 bool addRegularExpresion(const std::string &regular_expresion);
};
}

#endif // TREE_REGULAR_H