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

namespace fr::tree {

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
  bool addRegularExpression(const std::string &regular);
  void stopSearch(bool stop);

  std::tuple<void*, size_t, std::string> start_search(void *memory, size_t size);

  void clearSearch();
  void stopJobs();
};
}


#endif //TREE_SEARCHER_ENGINE_H