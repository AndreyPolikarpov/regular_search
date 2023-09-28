
#include "search_tree.hpp"
#include "storage_tnode.hpp"


namespace fr::tree {

std::tuple<void*, size_t, std::string> Searcher::search(tnode *root,
               uint8_t *memory_area, size_t memory_size) {
  if((root == nullptr) || (memory_area == nullptr) || (memory_size == 0))
    return std::make_tuple(isEmptyMemory(), 0, isEmptyRegular());

  for(size_t diving{0}; diving<memory_size; ++diving) {
    uint8_t symbol = *(memory_area + diving);

    if(root->stairs[symbol] == isEmptyTNode()) {
      ++diving;
      continue;
    }

    uint8_t *memory_found{};
    tnode *tnode_found{};
    std::tie(memory_found, tnode_found) 
            = searchInDepth(root->stairs[symbol], memory_area+diving + 1, memory_area + memory_size);
    if(tnode_found != isEmptyTNode()) {
      size_t length =  memory_found - (memory_area + diving);
      //To Do нужно найти функцию для экранирования/удаления не печатных симовлов
      //std::string str = std::string((memory_area + diving), (memory_found + 1));
      return std::make_tuple(memory_found, 
                            ((length > 0 )? length : 0), 
                            std::string((memory_area + diving), (memory_found + 1)));
    }
  }

  return std::make_tuple(isEmptyMemory(), 0, isEmptyRegular());
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

}