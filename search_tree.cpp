
#include "search_tree.hpp"
#include "storage_tnode.hpp"
#include "tree_node.hpp"
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <string>
#include <tuple>


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

  if(answer_memory && (answer_size > 0))
    answer_string = std::move(std::string(static_cast<char*>(memory), size));
  else if(answer_memory && answer_memory_end)
    answer_string = std::move(std::string(static_cast<char*>(memory), (answer_memory_end - answer_memory + 1)));

  return true;
}


bool Searcher::search(tnode *root,
               uint8_t *memory_area, size_t memory_size) {

  if((root == nullptr) || (memory_area == nullptr) || (memory_size == 0))
    return false;

  for(size_t diving{0}; diving<memory_size; ++diving) {
    //uint8_t symbol = *(memory_area + diving);
    tnode *current_tnode = root->stairs[*(memory_area + diving)];

    if(current_tnode == isEmptyTNode()) {
      ++diving;
      continue;
    }

    if(current_tnode->end) {
      return PreparingAnswer(memory_area + diving, memory_area + diving + 1, 1, current_tnode);
    };

    if(searchInDepth(current_tnode, memory_area+diving + 1, memory_area + memory_size))
      return PreparingAnswer(memory_area+diving);
    
    continue;
  }

  return false;
}

bool Searcher::searchInDepth(tnode *head, uint8_t *memory_area,
                                        uint8_t *memory_area_end) {
  uint8_t symbol = *(memory_area);

  if(head->is_active_special) {
    return searchInQuantifier(head, memory_area, memory_area_end);
  }

  if((memory_area == memory_area_end) || (head->stairs[symbol] == isEmptyTNode()))
    return false;
  
  if(head->stairs[symbol]->end)
    return PreparingAnswer(nullptr, memory_area, 0, head->stairs[symbol]);    


    /*
    std::tuple<uint8_t*, tnode*, SpecialSymbol*> finding_in_quantifier;
    finding_in_quantifier = 
                searchInQuantifier(head, memory_area, memory_area_end);
    if((std::get<1>(finding_in_quantifier) != isEmptyTNode()) || (std::get<2>(finding_in_quantifier) != isEmptySpecialSymbol())) {
      return finding_in_quantifier;
    }
    */
    
  return searchInDepth(head->stairs[symbol], (memory_area + 1),  memory_area_end);
}

bool Searcher::searchInQuantifier(tnode *head, uint8_t *memory_area,
                                        uint8_t *memory_area_end) {
  
  for(size_t spec_diving=0; spec_diving < head->store_special.store.size(); ++spec_diving) {
    SpecialSymbol *quantifier = head->store_special.store[spec_diving];
    if(quantifier->symbol == '.') {
      return quantifierDot(quantifier, memory_area, memory_area_end);
    } 
    /*else if (quantifier->symbol == '?') {
      return QuantifierQuestion(quantifier, memory_area, memory_area_end);  
    }
    else if (quantifier->symbol == '*') {
      return QuantifierStar(quantifier, memory_area, memory_area_end);  
    } 
    */

  }

  return false;
}

bool Searcher::quantifierDot(SpecialSymbol *quantifier, 
        uint8_t *memory_area, uint8_t *memory_area_end) {
  
  for(const auto &special : quantifier->repeat_store) {
    if(special.repeat > memory_area_end - memory_area)//To Do нужно проверить что бы не уходил за приделы
      continue;

    if(special.end)
      return PreparingAnswer(nullptr, memory_area, 0, nullptr, quantifier, special.repeat);

    uint8_t symbol = memory_area[special.repeat];

    if((quantifier->stairs[symbol] != isEmptyTNode()) &&
            searchInDepth(quantifier->stairs[symbol], memory_area + 1, memory_area_end)) {
      return true;
    }

    continue;
  }

  return false;
}

}