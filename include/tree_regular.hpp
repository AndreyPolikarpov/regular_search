#ifndef TREE_REGULAR_H
#define TREE_REGULAR_H

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <array>
#include <string>
#include <memory>

#include "tree_node.hpp"

namespace fr::tree {

using str_c_iter = std::string::const_iterator;

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
 * происходит лексический анализ и создание дерева из регулярных выражений  
 */
class TreeRegular {
private:
  tnode *root{nullptr};  
  bool addRegularElement(tnode *head, str_c_iter &it,
                  const std::string &re);
  bool addRegularElementSpec(SpecialSymbol *quantifier, str_c_iter &it,
                  const std::string &re);
  bool addSpecialSymbol(tnode *head, str_c_iter &it, 
                  const std::string &re);
public:
  /**
   * @brief добавляет в "дерево" регулярное выражение
   *    
   * @param regular_expresion регулярное выражение в виде строки
   * @return true //To Do возможно возвращать void 
   * @return false 
   */
  bool addRegularExpresion(const std::string &regular_expresion);
};
}

#endif // TREE_REGULAR_H