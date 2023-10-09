#ifndef SEARCH_TREE_H
#define SEARCH_TREE_H

#include <cstddef>
#include <cstdint>
#include <tuple>

#include "tree_node.hpp"
#include "tree_regular.hpp"

namespace fr::tree {
/**
 * @brief Поиск по "дереву" регулярных выражений
 * 
 */
class Searcher{
private:
  bool PreparingAnswer(void* memory=nullptr, void* memory_end=nullptr, size_t size = 0, 
        tnode *node_end = nullptr, SpecialSymbol *spec_end = nullptr, size_t repeat = 0);

  bool searchInDepth(tnode *head,
        uint8_t *memory_area, uint8_t *memory_area_end);

  bool searchInQuantifier(tnode *head,
        uint8_t *memory_area, uint8_t *memory_area_end);
  bool quantifierDot(SpecialSymbol *quantifier,
        uint8_t *memory_area, uint8_t *memory_area_end);
  bool quantifierQuestion(SpecialSymbol *quantifier,
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
  bool search(tnode *root, uint8_t *memory_area, size_t memory_size);

  /**
   * @brief Выдает регулярное выражение
   * 
   * @return std::string найденное регулярное выражение
   * если не нашло, то пустая "" строка 
   */
  std::string AnswerRegularExpresion();

    //To Do завернуть все в tuple
  tnode *answer_tnode{nullptr};
  SpecialSymbol *answer_special{nullptr};
  uint8_t *answer_memory{nullptr};
  uint8_t *answer_memory_end{nullptr};
  size_t answer_size{0};
  size_t answer_repeat{0};
  std::string answer_string{""};

  void ClearAnswer() {
    answer_tnode = nullptr;
    answer_special = nullptr;
    answer_memory = nullptr;
    answer_memory_end = nullptr;
    answer_size = 0;
    answer_repeat = 0;
    answer_string = "";
  };
} ;

}

#endif // SEARCH_TREE_H