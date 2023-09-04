#include "tree_regular.hpp"
#include <cstddef>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <vector>

namespace fr::tree {


namespace {
  tnode g_empty_tnode;
  uint8_t shielding_symbol = (uint8_t)('/');
}

tnode &isEmptyTNode() {
  return g_empty_tnode;
}

SpecialSymbol *isExistSpecialSymbol(tnode *node, uint8_t symbol) {
  for(size_t i{0}; i<node->store_special.store.size(); ++i) {
    if(symbol == node->store_special.store[i].symbol)
      return &node->store_special.store[i];
  }
  return nullptr;
}

bool /*TreeRegular::*/isSpecialSymbol(uint8_t symbol) {
  switch (symbol) {
    case  static_cast<uint8_t>('.') :
      return true;
    case  static_cast<uint8_t>('?') :
      return true;
    case  static_cast<uint8_t>('*') :
      return true;
  }

  return false;
}

bool TreeRegular::addTNode(tnode *head, std::vector<uint8_t>::const_iterator &it, const std::vector<uint8_t> &re) {
  if(head == nullptr)
    throw std::invalid_argument("add tnode:: argument head should nod nullptr ");

  if((*it == static_cast<uint8_t>(shielding_symbol)) && ((it+1) != re.end())) {
    if(isSpecialSymbol(*(it+1))) {
      if(addSpecialSymbol(head, it, re))
         return true;
    }
  }

  if(head->stairs[*it].get() == &g_empty_tnode) {
    head->stairs[*it] = std::make_shared<tnode>(*it);
  } 

  head = head->stairs[*it].get();

  if((it+1) == re.end()) {
    head->end = true;
    return true;
  }

  it += 1;
  return addTNode(head, it, re);
}

bool TreeRegular::addSpecialSymbol(tnode *head, std::vector<uint8_t>::const_iterator &it, const std::vector<uint8_t> &re) {
  
  SpecialSymbol *quantifizier = isExistSpecialSymbol(head, *it); 
  if(quantifizier == nullptr)
    quantifizier =  isExistSpecialSymbol(head, shielding_symbol);

  if(quantifizier == nullptr)
    throw std::invalid_argument("Таково не может быть, но произошла ересь, сжечь всех!!!");
  
  quantifizier->symbol = *it;
  while(quantifizier->repeat < re.size()) {
    if((it+(quantifizier->repeat+1)) == re.end()) {
      quantifizier->end = true;
      return true;//To Do нужно придумать выход что бы закончить добавление регулярного выражения
    }
  }

  it += quantifizier->repeat+1;
  addTNode(head, it, re);

  return false;
}

bool TreeRegular::addRegularExpresion(const std::vector<uint8_t> &regular_expresion) {
  tnode *head = &root;

  std::vector<uint8_t>::const_iterator it=regular_expresion.begin();
  return  addTNode(head, it, regular_expresion);
}

}