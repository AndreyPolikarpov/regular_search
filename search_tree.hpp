#ifndef SEARCH_TREE_H
#define SEARCH_TREE_H

#include <tuple>

#include "tree_regular.hpp"

namespace fr::tree {
/**
 * @brief Поиск по "дереву" регулярных выражений
 * 
 */
class Searcher{
private:
  std::tuple<uint8_t*, tnode*> searchInDepth(tnode *head,
        uint8_t *memory_area, uint8_t *memory_area_end);

  bool searchInQuantifier(tnode *head,
        uint8_t *memory_area, uint8_t *memory_area_end);
  bool quantifierDot(SpecialSymbol *quantifier,
        uint8_t *memory_area, uint8_t *memory_area_end);
public:
  /**
   * @brief поиск в регулярных выражений в области памяти
   *
   * @param root узел от куда будет начинаться поиск 
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
        search(tnode *root, uint8_t *memory_area, size_t memory_size);
} ;

}

#endif // SEARCH_TREE_H