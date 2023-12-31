#ifndef SEARCH_TREE_H
#define SEARCH_TREE_H

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <string>
#include <thread>
#include <tuple>
#include <vector>

#include "tree_node.hpp"
#include "tree_regular.hpp"

int version();

namespace fr::tree {
/**
 * @brief Поиск по "дереву" регулярных выражений
 *
 */
class Searcher {
private:
  bool PreparingAnswer(void *memory = nullptr, void *memory_end = nullptr,
                       size_t size = 0, tnode *node_end = nullptr,
                       SpecialSymbol *spec_end = nullptr, size_t repeat = 0);

  bool searchInDepth(tnode *head, uint8_t *memory_area,
                     uint8_t *memory_area_end);

  bool searchInQuantifier(tnode *head, uint8_t *memory_area,
                          uint8_t *memory_area_end);
  bool quantifierDot(SpecialSymbol *quantifier, uint8_t *memory_area,
                     uint8_t *memory_area_end);
  bool quantifierQuestion(SpecialSymbol *quantifier, uint8_t *memory_area,
                          uint8_t *memory_area_end);
  // To Do нужно подумать как корректно создавать ветку из регулярного
  // выражения, в частности как хранить повторяющиеся *
  bool quantifierStar(SpecialSymbol *quantifier, uint8_t *memory_area,
                      uint8_t *memory_area_end);

  uint8_t *memory_{nullptr};
  size_t size_{0};

  /**
   * @brief флаг сигнализирует о результате поиска
   * @return true поиск успешен
   * @return false поиск не успешен
   */
  std::atomic<bool>
      search_answer_{}; ///для вывода результата поиска в другой поток
  /**
   * @brief флаг управлением запуска старта
   *
   * @return true запустить поиск или поиск запущен
   * @return false остановить поиск или поиск остановлен
   */
  std::atomic<bool> search_start_{}; ///принудительная остановка поиска
  std::atomic<bool> stop_job_{}; ///остановить поток
  bool exit{false};

  tnode *answer_tnode{nullptr};
  SpecialSymbol *answer_special{nullptr};
  uint8_t *answer_memory{nullptr};
  uint8_t *answer_memory_end{nullptr};  
  size_t answer_repeat{0};
  
public:
  Searcher() : search_answer_(false), search_start_(false), stop_job_(false){};
  ~Searcher() = default;
  // Searcher(const Searcher &) = delete;
  // Searcher(Searcher &&) = delete;
  // Searcher &operator=(const Searcher &) = delete;
  // Searcher &operator=(Searcher &&) = delete;

  /**
   * @brief поиск в регулярных выражений в области памяти
   *   To Do спрятать в private, открыты для тестирования
   * @param root узел от куда будет начинаться поиск
   * @param memory_area область памяти где производится поиск
   * @param memory_size размер области памяти где производится поиск
   * @return true нашел
   * @return false не нашел
   */
  bool search(tnode *root, uint8_t *memory_area, size_t memory_size);

  /**
   * @brief используется для запуска в отдельном потоке  
   * поиск в бесконечном цикле, выход благодаря атомикам
   * для передачи места поиска используется readLocation()
   */
  void searchLocation();
  ///результат поиска
  bool isResponse() { return search_answer_.load(std::memory_order_release); }
  ///на данный момент происходит поиск или нет
  bool isSearchContinues() {
    return search_start_.load(std::memory_order_release);
  }
  ///запустить поиск
  void startSearch(bool start) {
    search_start_.store(start, std::memory_order_release);
  };
  ///обнулить атомики
  void clearFlag() {
    //последовательность важна иначе поток выйдет из бесконечного цикла
    search_start_.store(false, std::memory_order_release);
    search_answer_.store(false, std::memory_order_release);
  };
  /**
   * @brief используется для передачи места поиска для другого потока 
   * работает в паре с searchLocation().
   * для передачи поиск не должен осуществляться
   * 
   * @param memory область памяти где производится поиск
   * @param size размер области памяти где производится поиск
   */
  void readLocation(uint8_t *memory, size_t size) {
    if (!isSearchContinues()) {
      memory_ = memory;
      size_ = size;
    }
  };
  ///остановить поиск
  void stopJob();

  /**
   * @brief Выдает регулярное выражение
   *
   * @return std::string найденное регулярное выражение
   * если не нашло, то пустая "" строка
   */
  std::string &AnswerRegularExpresion();
  //To Do переместить в private
  size_t answer_size{0};
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
};

} // namespace fr::tree

#endif // SEARCH_TREE_H