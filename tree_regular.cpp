#include "tree_regular.hpp"
#include "storage_tnode.hpp"
#include "tree_node.hpp"

#include <cstddef>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <vector>

namespace fr::tree {

int8_t isExistSpecialSymbol(tnode *node, uint8_t symbol) {
  for(size_t i{0}; i<g_count_special_symbol; ++i) {
    if(symbol == node->store_special.store[i]->symbol)
      return i;
  }
  return -1;
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
  
  if(isSpecialSymbol(*(it))) {
      addSpecialSymbol(head, it, re);
      return true;
  }

  if(head->stairs[static_cast<uint8_t>(*it)] == isEmptyTNode()) {
    head->stairs[static_cast<uint8_t>(*it)] = &StorageSymbol::createTNode(*it);
  } 

  head = head->stairs[static_cast<uint8_t>(*it)];

  if((it+1) == re.end()) {
    head->end = true;
    StorageSymbol::rememberRegular(head, re);
    return true;
  }

  it += 1;
  return addRegularElement(head, it, re);
}

bool TreeRegular::addRegularElement(SpecialSymbol *quantifizier, std::string::const_iterator &it, const std::string &re) {
  if(quantifizier == nullptr)
    throw std::invalid_argument("add SpecialSymbol:: argument head should nod nullptr ");

  if(quantifizier->stairs[static_cast<uint8_t>(*it)] == isEmptyTNode()) {
    quantifizier->stairs[static_cast<uint8_t>(*it)] = &StorageSymbol::createTNode(static_cast<uint8_t>(*it));
  } 

  tnode *head = quantifizier->stairs[static_cast<uint8_t>(*it)];

  if((it+1) == re.end()) {
    head->end = true;
    StorageSymbol::rememberRegular(head, re);
    return true;
  }

  it += 1;
  
  addRegularElement(head, it, re);

  return false;
}

bool TreeRegular::addSpecialSymbol(tnode *head, std::string::const_iterator &it, const std::string &re) {
  SpecialSymbol *quantifizier{};
  int8_t number = isExistSpecialSymbol(head, static_cast<uint8_t>(*it));

  if(number == -1) {
    number = 0;
    head->store_special.store[number] = &StorageSymbol::createSpecialSymbol(static_cast<uint8_t>(*it)); 
  }

  quantifizier = head->store_special.store[number];
  
  if(quantifizier == nullptr)
    throw std::invalid_argument("Таково не может быть, но произошла ересь, сжечь всех!!!");

  head->is_active_special = true;

  quantifizier->symbol = *it;
  while((quantifizier->repeat < re.size()) &&
         (*(it+(quantifizier->repeat + 1)) == quantifizier->symbol) && 
         ((it+(quantifizier->repeat + 1)) != re.end()))
  {
    ++quantifizier->repeat;
  }

  if(((it+(quantifizier->repeat + 1)) == re.end())) {
    quantifizier->end = true;
    StorageSymbol::rememberRegular(quantifizier, re);
    return true;
  }

  it += quantifizier->repeat + 1; 

  addRegularElement(quantifizier, it, re);

  return false;
}

bool TreeRegular::addRegularExpresion(const std::string &regular_expresion) {
  tnode *head = &root;

  std::string::const_iterator it=regular_expresion.begin();
  return addRegularElement(head, it, regular_expresion);
}
}