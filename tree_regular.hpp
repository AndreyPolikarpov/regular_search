#ifndef TREE_REGULAR_H
#define TREE_REGULAR_H

#include <cstdint>
#include <iostream>
#include <array>
#include <string>
#include <memory>

#include "tree_node.hpp"

namespace fr::tree {

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
  bool addRegularElement(SpecialSymbol *quantifizier, std::string::const_iterator &it,
                  const std::string &re);
  bool addSpecialSymbol(tnode *head, std::string::const_iterator &it, 
                  const std::string &re);
public:
  /**
   * @brief добавляет в "дерево" регулярное выражение
   * To Do нужно добавить экранирующие символ для экранирования квантификатор
      
   * @param regular_expresion регулярное выражение в виде строки
   * @return true //To Do возможно возвращать void 
   * @return false 
   */
  bool addRegularExpresion(const std::string &regular_expresion);
  /**
   * @brief поиск в регулярных выражений в области памяти
   * 
   * @return true To Do нужно возвращать std::touple{void *memory область памяти где производится поиск,
                                                    size_t offset где начинается выражение в memory,
                                                    std::string regular найденное регулярное выражение}
   * @return false 
   */
  bool search();
};
}

#endif // TREE_REGULAR_H