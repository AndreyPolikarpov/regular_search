#include "tree_regular.hpp"
#include "storage_tnode.hpp"
#include "tree_node.hpp"

#include <cstddef>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <vector>

namespace fr::tree {

namespace {
  uint8_t shielding_symbol = (uint8_t)('/');
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

bool TreeRegular::addRegularElement(tnode *head, std::string::const_iterator &it, const std::string &re) {
  if(head == nullptr)
    throw std::invalid_argument("add tnode:: argument head should nod nullptr ");

  if((*it == static_cast<uint8_t>(shielding_symbol)) && ((it+1) != re.end())) {
    if(isSpecialSymbol(*(it+1))) {
      if(addSpecialSymbol(head, it, re))
         return true;
    }
  }

  if(head->stairs[static_cast<uint8_t>(*it)] == &isEmptyTNode()) {
    head->stairs[static_cast<uint8_t>(*it)] = &StorageTNode::createTNode(*it);
  } 

  head = head->stairs[static_cast<uint8_t>(*it)];

  if((it+1) == re.end()) {
    head->end = true;
    StorageTNode::rememberRegular(head, re);
    return true;
  }

  it += 1;
  return addRegularElement(head, it, re);
}

bool TreeRegular::addSpecialSymbol(tnode *head, std::string::const_iterator &it, const std::string &re) {
  
  SpecialSymbol *quantifizier = isExistSpecialSymbol(head, static_cast<uint8_t>(*it)); 
  if(quantifizier == nullptr)
    quantifizier =  isExistSpecialSymbol(head, shielding_symbol);

  if(quantifizier == nullptr)
    throw std::invalid_argument("Таково не может быть, но произошла ересь, сжечь всех!!!");

  head->store_special.isActive = true;

  quantifizier->symbol = *it;
  while(quantifizier->repeat < re.size()) {
    if((it+(quantifizier->repeat+1)) == re.end()) {
      quantifizier->end = true;
      StorageTNode::rememberRegular(quantifizier, re);
      return true;//To Do нужно придумать выход что бы закончить добавление регулярного выражения
    }
  }

  it += quantifizier->repeat+1;
  addRegularElement(head, it, re);

  return false;
}

bool TreeRegular::addRegularExpresion(const std::string &regular_expresion) {
  tnode *head = &root;

  std::string::const_iterator it=regular_expresion.begin();
  return addRegularElement(head, it, regular_expresion);
}
}