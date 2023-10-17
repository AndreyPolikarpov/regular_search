#ifndef SEARCH_TREE_H
#define SEARCH_TREE_H

#include <cstddef>
#include <cstdint>
#include <string>
#include <thread>
#include <tuple>
#include <vector>
#include <atomic>

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
        //To Do нужно подумать как корректно создавать ветку из регулярного выражения, в частности как хранить повторяющиеся *
  bool quantifierStar(SpecialSymbol *quantifier,
        uint8_t *memory_area, uint8_t *memory_area_end);

  
  std::atomic<bool> search_answer_{}; ///для вывода результата поиска в другой поток 
  std::atomic<bool> search_stop_{}; ///принудительная остановка поиска

public:
  Searcher() : search_answer_(false), search_stop_(true){};
  ~Searcher() = default;
  //Searcher(const Searcher &) = delete;
  //Searcher(Searcher &&) = delete;
  //Searcher &operator=(const Searcher &) = delete;
  //Searcher &operator=(Searcher &&) = delete;

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

  void startSearch(uint8_t *memory, size_t *size);
  bool isResponse() { return search_answer_.load(std::memory_order_release);}
  void stopSearch(bool stop) {search_stop_.store(stop, std::memory_order_release);};
  void clearFlag() {
    search_stop_.store(true, std::memory_order_release); 
    search_answer_.store(false, std::memory_order_release);
  }

  /**
   * @brief Выдает регулярное выражение
   * 
   * @return std::string найденное регулярное выражение
   * если не нашло, то пустая "" строка 
   */
  std::string &AnswerRegularExpresion();

    //To Do завернуть все в tuple и переместить в private
  tnode *answer_tnode{nullptr};
  SpecialSymbol *answer_special{nullptr};
  uint8_t *answer_memory{nullptr};
  uint8_t *answer_memory_end{nullptr};
  size_t answer_size{0};
  size_t answer_repeat{0};
  std::string answer_string{""};

  void clearAnswer() {
    answer_tnode = nullptr;
    answer_special = nullptr;
    answer_memory = nullptr;
    answer_memory_end = nullptr;
    answer_size = 0;
    answer_repeat = 0;
    answer_string = "";
  };
} ;
//To Do возможно переназвать по другому
class ThreadPool {
private:
  size_t count_thread_{4};
  std::vector<std::thread> thread_pool_;
  std::vector<Searcher*> searcher_pool_;
  uint8_t *memory_area_{nullptr};
  size_t *memory_size_{nullptr};

  bool search_works_ {false};
  TreeRegular tree_;

  //bool initialization();
public:
  ThreadPool();
  ~ThreadPool();
  bool addRegularExpression(const std::string &regular);

  std::tuple<void*, size_t, std::string> start_search(void *memory, size_t size);
};

}

#endif // SEARCH_TREE_H