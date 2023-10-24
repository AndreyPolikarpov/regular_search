#include "search_tree.hpp"
#include "storage_tnode.hpp"
#include "tree_node.hpp"
#include "tree_search_engine.hpp"

#include "version.h"

#include <algorithm>
#include <atomic>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <netdb.h>
#include <string>
#include <thread>
#include <tuple>

int version() {
  return PROJECT_VERSION_PATCH;
}


namespace fr::tree {

std::tuple<void*, size_t, std::string> EmptyTupleTNode() { return std::make_tuple(isEmptyMemory(), 0, isEmptyRegular());}

bool Searcher::PreparingAnswer(void* memory, void* memory_end, size_t size,
              tnode *node_end, SpecialSymbol *spec_end, size_t repeat) {
  if((answer_tnode == nullptr) && (node_end != nullptr)) answer_tnode = node_end;
  if((answer_special == nullptr) && (spec_end != nullptr)) answer_special = spec_end;
  if((answer_memory == nullptr) && (memory != nullptr)) answer_memory = static_cast<uint8_t*>(memory);
  if((answer_memory_end == nullptr) && (memory_end != nullptr)) answer_memory_end = static_cast<uint8_t*>(memory_end);
  if((answer_size == 0) && (size != 0)) answer_size = size;
  if((answer_repeat == 0) && (repeat != 0)) answer_repeat = repeat;

  if(answer_memory && (answer_size > 0)  && memory)
    answer_string = (std::string(static_cast<char*>(memory), size));
  else if(answer_memory && answer_memory_end && memory)
    answer_string = (std::string(static_cast<char*>(memory), (answer_memory_end - answer_memory + 1)));

  return true;
}

bool Searcher::search(tnode *root,
               uint8_t *memory_area, size_t memory_size) {

  if(root == nullptr) 
    return false;

  for(size_t diving{0}; diving<memory_size; ++diving) {
    if(root->is_active_special) {
      if(searchInQuantifier(root, memory_area + diving, memory_area + memory_size))
        return PreparingAnswer(memory_area + diving);
    }
    
    tnode *current_tnode = root->stairs[*(memory_area + diving)];

    if(current_tnode == isEmptyTNode()) {
      continue;
    }

    if(current_tnode->end) {
      return PreparingAnswer(memory_area + diving, memory_area + diving + 1, 1, current_tnode);
    };

    if(current_tnode->is_active_special &&
      searchInQuantifier(current_tnode, memory_area + diving + 1, memory_area + memory_size)) {
      return PreparingAnswer(memory_area+diving);
    }

    if(searchInDepth(current_tnode, memory_area+diving + 1, memory_area + memory_size))
      return PreparingAnswer(memory_area+diving);
    
    continue;
  }

  return false;
}

bool Searcher::searchInDepth(tnode *head, uint8_t *memory_area,
                                        uint8_t *memory_area_end) {
  uint8_t symbol = *(memory_area); 

  if((memory_area == memory_area_end) || (head->stairs[symbol] == isEmptyTNode()) 
          || !search_start_.load(std::memory_order_release))
    return false;

  if(head->stairs[symbol]->end)
    return PreparingAnswer(nullptr, memory_area, 0, head->stairs[symbol]);    
  
  if(head->stairs[symbol]->is_active_special) {
    return searchInQuantifier(head->stairs[symbol], memory_area, memory_area_end);
  }

  return searchInDepth(head->stairs[symbol], (memory_area + 1),  memory_area_end);
}

bool Searcher::searchInQuantifier(tnode *head, uint8_t *memory_area,
                                        uint8_t *memory_area_end) {
  
  for(size_t spec_diving=0; spec_diving < head->store_special.store.size(); ++spec_diving) {
    SpecialSymbol *quantifier = head->store_special.store[spec_diving];
    if(quantifier->symbol == '.') {
      return quantifierDot(quantifier, memory_area, memory_area_end);
    } 
    else if (quantifier->symbol == '?') {      
      return quantifierQuestion(quantifier, memory_area, memory_area_end);  
    }
    else if (quantifier->symbol == '*') {
      return quantifierStar(quantifier, memory_area, memory_area_end);  
    }
  }

  return false;
}

bool Searcher::quantifierDot(SpecialSymbol *quantifier, 
        uint8_t *memory_area, uint8_t *memory_area_end) {
  
  for(const auto &special : quantifier->repeat_store) {
    if(special.repeat == 0)
      break;
    if(special.repeat > (memory_area_end - memory_area))//To Do нужно проверить что бы не уходил за приделы
      continue;

    if(special.end)
      return PreparingAnswer(nullptr, (memory_area + special.repeat), 
                                0, nullptr, quantifier, special.repeat);

    uint8_t symbol = memory_area[special.repeat];

    if(quantifier->stairs[symbol]->is_active_special) {
      return searchInQuantifier(quantifier->stairs[symbol], memory_area + special.repeat,
                                                   memory_area_end);
    }

    if((quantifier->stairs[symbol] != isEmptyTNode()) &&
            searchInDepth(quantifier->stairs[symbol],
                  memory_area + special.repeat + 1, memory_area_end)) {
      return true;
    }
  }

  return false;
}

bool Searcher::quantifierQuestion(SpecialSymbol *quantifier, 
             uint8_t *memory_area, uint8_t *memory_area_end) {
  for(const auto &special : quantifier->repeat_store) {
    if(special.repeat == 0)
      break;
    if(special.end) {
      return PreparingAnswer(nullptr, memory_area,
                                0, nullptr, quantifier, special.repeat);
    }

    uint8_t symbol{0};
    size_t size_memory = memory_area_end - memory_area;
    //для ? нужно проверять как отсутствие ? так и его присутствие
    for(uint32_t i{0}; i<special.repeat || i < size_memory; ++i) { 
      symbol = memory_area[i];
      if((quantifier->stairs[symbol] != isEmptyTNode())) {
        if(quantifier->stairs[symbol]->end) {
          return PreparingAnswer(nullptr, (memory_area + i), 
                                0, quantifier->stairs[symbol], nullptr, i);
        }
        if(searchInDepth(quantifier->stairs[symbol], memory_area + i + 1, memory_area_end)) {
          return true;
        }
      }
    }
  }

  return false;
} 

bool Searcher::quantifierStar(SpecialSymbol *quantifier,
              uint8_t *memory_area, uint8_t *memory_area_end) {
  for(const auto &special : quantifier->repeat_store) {
    if(special.repeat == 0)
      break;
    
    if(special.end)
      return PreparingAnswer(nullptr, memory_area, 0, nullptr, quantifier);

    uint8_t symbol{0};
    size_t size_memory = memory_area_end - memory_area;
    for(uint32_t i{0}; i < size_memory; ++i) {
      symbol = memory_area[i];
      if((quantifier->stairs[symbol] != isEmptyTNode())) {
        if(quantifier->stairs[symbol]->end) {
          return PreparingAnswer(nullptr, (memory_area + i), 
                                0, quantifier->stairs[symbol], nullptr, i);
        }
        if(searchInDepth(quantifier->stairs[symbol], memory_area + i + 1, memory_area_end)) {
          return true;
        }
      }
    }
  }

  return false;
}

std::string &Searcher::AnswerRegularExpresion() {
  if(answer_tnode) {
    return fr::tree::StorageSymbol::RegularExpressionMemorized(answer_tnode);
  }

  return fr::tree::StorageSymbol::RegularExpressionMemorized(answer_special);
}

void Searcher::searchLocation() {
  for(;;) {
    if(search_start_.load(std::memory_order_acquire)) {
      if(search(fr::tree::StorageSymbol::isRootTree(), memory_, size_)) {
        search_answer_.store(true, std::memory_order_release);  
        search_start_.store(true, std::memory_order_release);
      } else {
        search_start_.store(false, std::memory_order_release);
      }
    }
    if(stop_job_.load(std::memory_order_acquire))
          break;
      
  }

  return;
}

void Searcher::stopJob() {
    search_start_.store(false, std::memory_order_release); 
    search_answer_.store(false, std::memory_order_release);
    stop_job_.store(true, std::memory_order_release);
}

TreeSearchEngine::TreeSearchEngine(){
  if(std::thread::hardware_concurrency() != 0) {
    count_thread_ = std::thread::hardware_concurrency();
  }

  for(size_t i{0}; i<count_thread_; ++i) {
    searcher_pool_.push_back(new Searcher);
    Searcher *searcher = searcher_pool_.back();
    
    thread_pool_.emplace_back(&Searcher::searchLocation, &(*searcher));
  }
}

TreeSearchEngine::~TreeSearchEngine() {
  for(auto searcher : searcher_pool_) {
    searcher->stopJob();
  }
  for(auto &thread : thread_pool_) {
    thread.join();
  }
}

std::tuple<void*, size_t, std::string> TreeSearchEngine::start_search(void *memory, size_t size) {
  if(memory == nullptr || size == 0) 
    return std::make_tuple(nullptr, 0, isEmptyRegular());
  
  memory_area_ = static_cast<uint8_t*>(memory); memory_size_ = &size; 
  size_t retreat = size / (searcher_pool_.size() + 1);
  size_t i{0};
  for(const auto &searcher : searcher_pool_) {
    size_t i_retreat = i*retreat;
    searcher->readLocation((uint8_t*)memory + i_retreat, size - i_retreat);
    searcher->clearAnswer();
    searcher->startSearch(true);
    ++i;
  }

  search_works_ = true;
  size_t index{0};
  size_t not_found{0};
  bool exit{false};
  while(!exit) {
    for(size_t i{0}; i<count_thread_; ++i) {
      if(searcher_pool_[index]->isResponse() || !search_works_)
        exit = true;
      if(!searcher_pool_[index]->isSearchContinues()) 
        ++not_found;
    }

    if(not_found == count_thread_) exit = true;    
  }

  if(search_works_) {
    for(const auto &searcher : searcher_pool_) {
      searcher->clearFlag();
    }
  }

  search_works_ = false;
  if(not_found == count_thread_)
    return std::make_tuple(nullptr, 0, isEmptyRegular());

  return std::make_tuple(memory,
    searcher_pool_[index]->answer_size,   searcher_pool_[index]->AnswerRegularExpresion());
}

bool TreeSearchEngine::addRegularExpression(const std::string &regular) {
  if(!search_works_)
    return tree_.addRegularExpresion(regular);

  return false;
}

void TreeSearchEngine::stopSearch(bool stop) {
  search_works_ = !stop;
}

void TreeSearchEngine::clearSearch() {
  for(auto searcher : searcher_pool_) {
    searcher->clearFlag();
    searcher->clearAnswer();
  }
  search_works_ = false;
}

void TreeSearchEngine::stopJobs() {
  for(auto searcher : searcher_pool_) {
    searcher->stopJob();
  }
  search_works_ = false;
}
}