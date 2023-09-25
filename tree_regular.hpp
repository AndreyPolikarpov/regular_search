#ifndef TREE_REGULAR_H
#define TREE_REGULAR_H

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <array>
#include <string>
#include <memory>
#include <tuple>

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
 */
class TreeRegular {
private:
  tnode root;  
  bool addRegularElement(tnode *head, str_c_iter &it,
                  const std::string &re);
  bool addRegularElement(SpecialSymbol *quantifier, str_c_iter &it,
                  const std::string &re);
  bool addSpecialSymbol(tnode *head, str_c_iter &it, 
                  const std::string &re);
  std::tuple<uint8_t*, tnode*> searchInDepth(tnode *head,
                              uint8_t *memory_area, uint8_t *memory_area_end);
public:
  /**
   * @brief добавляет в "дерево" регулярное выражение
   *    
   * @param regular_expresion регулярное выражение в виде строки
   * @return true //To Do возможно возвращать void 
   * @return false 
   */
  bool addRegularExpresion(const std::string &regular_expresion);
  /**
   * @brief поиск в регулярных выражений в области памяти
   * 
   * @param memory_area область памяти где производится поиск
   * @param memory_size размер области памяти где производится поиск
   * @return tuple<
      uint8_t* место в памяти где нашлось регулярное выражение, точнее где закончилось //To Do область памяти где производится поиск 
      size_t где закачивается регулярное выражение в памяти//To Do отступ от начала где начинается регулярное выражение  
      const std::string& найденное регулярное выражение 
      > 
   * @return false 
   */
  std::tuple<void*, size_t, std::string>
        search( uint8_t *memory_area, size_t memory_size);
};
}

#endif // TREE_REGULAR_H