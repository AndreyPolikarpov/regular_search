#ifndef TREE_SEARCHER_ENGINE_H
#define TREE_SEARCHER_ENGINE_H

#include <cstddef>
#include <cstdint>
#include <string>
#include <thread>
#include <tuple>
#include <vector>
#include <atomic>

#include "tree_node.hpp"
#include "tree_regular.hpp"
#include "search_tree.hpp"

namespace fr::tree {

/**
 * @brief заголовочный файл для библиотеки
 * 
 */
class TreeSearchEngine {
private:
  size_t count_thread_{4};
  std::vector<std::thread> thread_pool_;
  std::vector<Searcher*> searcher_pool_;
  uint8_t *memory_area_{nullptr};
  size_t *memory_size_{nullptr};

  bool search_works_ {false};
  TreeRegular tree_;

public:
  TreeSearchEngine();
  ~TreeSearchEngine();

  TreeSearchEngine(const TreeSearchEngine &) = delete;
  TreeSearchEngine(TreeSearchEngine &&) = delete;
  TreeSearchEngine &operator=(const TreeSearchEngine &) = delete;
  TreeSearchEngine &operator=(TreeSearchEngine &&) = delete;
  /**
   * @brief Добавить регулярное выражение 
   * если работает поиск, регулярное выражение не будет добавлено
   * @param regular регулярное выражение
   * @return true добавлено
   * @return false не добавлено
   */
  bool addRegularExpression(const std::string &regular);
  ///остановить поиск
  void stopSearch(bool stop);

  /**
   * @brief запустить поиск 
   * 
   * @param memory область памяти где производится поиск
   * @param size размер области памяти где производится поиск
   * @return std::tuple<void*, size_t, std::string> возвращает 
        <место первого вхождения регулярного выражения,
          порядковый номер символа где начинается регулярное выражение,
          регулярное выражение>
      если ничего не найдено то на выходе будет <null, 0, ""> 
   */
  std::tuple<void*, size_t, std::string> start_search(void *memory, size_t size);

  bool isWorkSearch() {return search_works_;};
  ///чистка поля ответов, атомиков. Необходимо осуществлять перед началом или после поиска
  void clearSearch();
  ///остановить работу потоков //To Do перенести в private
  void stopJobs();
};
}


#endif //TREE_SEARCHER_ENGINE_H