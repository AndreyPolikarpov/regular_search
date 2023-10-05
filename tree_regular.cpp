#include "tree_regular.hpp"
#include "storage_tnode.hpp"
#include "tree_node.hpp"

//#include <cstddef>
//#include <cstdint>
//#include <cstdint>
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <string>

#include <vector>

namespace fr::tree {

int8_t isExistSpecialSymbol(tnode *node, uint8_t symbol) {
  for(size_t i{0}; i<g_count_special_symbol; ++i) {
    if(symbol == node->store_special.store[i]->symbol)
      return i;
  }
  return -1;
}

bool isSpecialSymbol(uint8_t symbol) {
    switch (symbol) {
    case ('.') : {
       return true;
    }
    case ('?') : {
       return true;
    }
    case ('*') :  {
       return true;
    }
    }

   return false;
 }

/**
 * @brief Функция для определения экранированных квантификаторов
 * если после символа / следует квантификатор то переводит shielding на квантификатор
 * если нужно использовать квантификатор как простой символ перед ним нужно поставить символ / 
 * 
 * @param shielding ссылка проверяемый символ
 * @param re регулярное в виде строки 
 * @return true за / следует квантификатором, false в противном случае
 */
 bool isShieldingSymbol(str_c_iter &shielding, const std::string &re) {
  if((*shielding == g_shielding_symbol) && ((shielding + 1) != re.end()) && (isSpecialSymbol(*(shielding + 1)))) {
    shielding += 1;
    return true;
  }

  return false;
 }
 

bool TreeRegular::addRegularElement(tnode *head, str_c_iter &it, const std::string &re) {
  if(head == nullptr)
    throw std::invalid_argument("add tnode:: argument head should nod nullptr ");
  
  if(!isShieldingSymbol(it, re)) {
    if(isSpecialSymbol(*(it))) {
        addSpecialSymbol(head, it, re);
        return true;
    }
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

bool TreeRegular::addRegularElement(SpecialSymbol *quantifier, str_c_iter &it, const std::string &re) {
  if(quantifier == nullptr)
    throw std::invalid_argument("add SpecialSymbol:: argument head should nod nullptr ");

  isShieldingSymbol(it, re); 

  if(quantifier->stairs[static_cast<uint8_t>(*it)] == isEmptyTNode()) {
    quantifier->stairs[static_cast<uint8_t>(*it)] = &StorageSymbol::createTNode(static_cast<uint8_t>(*it));
  } 
    
  tnode *head = quantifier->stairs[static_cast<uint8_t>(*it)];

  if((it+1) == re.end()) {
    head->end = true;
    StorageSymbol::rememberRegular(head, re);
    return true;
  }

  it += 1;
  
  addRegularElement(head, it, re);

  return false;
}

bool TreeRegular::addSpecialSymbol(tnode *head, str_c_iter &it, const std::string &re) {
  SpecialSymbol *quantifier{};
  int8_t number = isExistSpecialSymbol(head, static_cast<uint8_t>(*it));

  if(number == -1) {
    number = 0;
    head->store_special.store[number] = &StorageSymbol::createSpecialSymbol(static_cast<uint8_t>(*it)); 
  }

  quantifier = head->store_special.store[number];
  
  if(quantifier == nullptr)
    throw std::invalid_argument("Таково не может быть, но произошла ересь, сжечь всех!!!");

  head->is_active_special = true;

  quantifier->symbol = *it;
  RepeatSpecial repeat_spec;
  
  while((repeat_spec.repeat < re.size()) && 
         (*(it+(repeat_spec.repeat + 1)) == quantifier->symbol) && 
         ((it+(repeat_spec.repeat + 1)) != re.end()))
  {
    ++repeat_spec.repeat;
  }  
  // + 1, то есть сам квантификатор //возможно это костыль 
  ++repeat_spec.repeat;

  if(((it+(repeat_spec.repeat)) == re.end())) { //для случая когда Точка последний символ регулярного выражения
    repeat_spec.end = true;

    for(size_t i{0}; i<g_size_repeat_special; ++i) {
      if(quantifier->repeat_store[i].repeat == 0) {
        quantifier->repeat_store[i] = repeat_spec;
        break;
      }
    }
    
    StorageSymbol::rememberRegular(quantifier, re);
    return true;
  }

  for(size_t i{0}; i<g_size_repeat_special; ++i) {
      if(quantifier->repeat_store[i].repeat == 0) {
        quantifier->repeat_store[i] = repeat_spec;
        break;
      }
    }

  it += repeat_spec.repeat; 

  addRegularElement(quantifier, it, re);

  return false;
}

bool TreeRegular::addRegularExpresion(const std::string &regular_expresion) {
  if(root == nullptr) {
    root = StorageSymbol::isRootTree();
  }
  tnode *head = root;

  std::string::const_iterator it=regular_expresion.begin();
  return addRegularElement(head, it, regular_expresion);
}
}