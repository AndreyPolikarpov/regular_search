
#include "search_tree.hpp"
#include "storage_tnode.hpp"
#include "tree_node.hpp"
#include <cstddef>
#include <cstdint>


namespace fr::tree {

std::tuple<void*, size_t, std::string> EmptyTupleTNode() { return std::make_tuple(isEmptyMemory(), 0, isEmptyRegular());}
std::tuple<void*, size_t, std::string> FindingTupleTNode(void* memory, size_t size, std::string regular) {
  return std::make_tuple(memory, 
                  ((size > 0 )? size : 0), //To Do возможно лишняя проверка
                  regular);
}


std::tuple<void*, size_t, std::string> Searcher::search(tnode *root,
               uint8_t *memory_area, size_t memory_size) {

  if((root == nullptr) || (memory_area == nullptr) || (memory_size == 0))
    return EmptyTupleTNode();

  for(size_t diving{0}; diving<memory_size; ++diving) {
    //uint8_t symbol = *(memory_area + diving);
    tnode *current_tnode = root->stairs[*(memory_area + diving)];

    if(current_tnode == isEmptyTNode()) {
      ++diving;
      continue;
    }

    if(diving == (memory_size-1)) {//Встали на последний символ
      if(current_tnode->end) {
        return FindingTupleTNode(memory_area + diving, 
                               1,
                               std::string((memory_area + diving), (memory_area + diving + 1)));
      } else {
        return EmptyTupleTNode();
      }
    }

    uint8_t *memory_found{};
    tnode *tnode_found{};
    std::tie(memory_found, tnode_found) 
            = searchInDepth(current_tnode, memory_area+diving + 1, memory_area + memory_size);
    if(tnode_found != isEmptyTNode()) {
      //size_t length =  memory_found - (memory_area + diving);
      //To Do нужно найти функцию для экранирования/удаления не печатных симовлов
      //std::string str = std::string((memory_area + diving), (memory_found + 1));
      return FindingTupleTNode(memory_found, 
                               memory_found - (memory_area + diving),
                               std::string((memory_area + diving), (memory_found + 1)));
      
      }
  }

  return EmptyTupleTNode();
}

std::tuple<uint8_t*, tnode*> Searcher::searchInDepth(tnode *head, uint8_t *memory_area,
                                        uint8_t *memory_area_end) {
  uint8_t symbol = *(memory_area);

  if((memory_area == memory_area_end) || (head->stairs[symbol] == isEmptyTNode()))
    return std::make_tuple(memory_area, isEmptyTNode());
  
  if(head->stairs[symbol]->end)
    return std::make_tuple(memory_area, head->stairs[symbol]);

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

  return true;
}

bool Searcher::quantifierDot(SpecialSymbol *quantifier, 
        uint8_t *memory_area, uint8_t *memory_area_end) {
  
  //To Do доделать
  if(quantifier->repeat > (memory_area_end - memory_area))
    return false;
  
  uint8_t symbol = memory_area[quantifier->repeat];

  if(quantifier->stairs[symbol] == isEmptyTNode())
    return false;
  
  return true;

}

}