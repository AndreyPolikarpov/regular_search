#ifndef TREE_REGULAR_H
#define TREE_REGULAR_H

#include <cstdint>
#include <iostream>
#include <array>
#include <vector>
#include <memory>

namespace fr::tree {

struct tnode;
tnode &isEmptyTNode();

//to do для * нужно добавить хранилище в котором будет добавляться значения стоящие после * и нужно будет искать
// и добавить это хранилище в общий поиск для проверки 

/*
 так же ограничения по типу регулярных выражении:
  1. Одинаковый вид квалификаторов может повторятся сколь угодно раз,
      но разные типы квалификаторов не могут повторятся друг за другом
      например: .? не правильно
                .. или ?? правильно
*/

/**
 * @brief Квантификатор, например .?*
 * @param symbol определенный Квантификатор
 * @param end на этой квантификаторе заканчивается регулярное выражение
 * @param repeat количество повторений подряд
 * //@param is_activated если true использовать в поиске, иначе нет
 * @param stairs массив указателей на потомков
 */
struct SpecialSymbol {
  char symbol{'/'};
  bool end{false};
  //bool is_activated{false};  
  uint32_t repeat{0};///число повторений
  std::array<std::shared_ptr<tnode>, 255> stairs = {std::make_shared<tnode>(isEmptyTNode())};
  //tnode* stairs[255] = {isEmptyTNode()};//To Do в место shared_ptr нужен указатель
};

struct SpecialSymbolTest {
  char symbol{'/'};
  bool end{false};
  //bool is_activated{false};  
  uint32_t repeat{0};///число повторений
  //std::array<std::shared_ptr<tnode>, 255> stairs = {std::make_shared<tnode>(isEmptyTNode())};
  tnode* stairs[255] = {&isEmptyTNode()};//To Do в место shared_ptr нужен указатель
};

struct StoreSpecial {
  std::array<SpecialSymbol, 3> store;
  bool isActive{false};
};

struct StoreSpecialTest {
  std::array<SpecialSymbolTest, 3> store;
  bool isActive{false};
};

/**
 * @struct узел дерева
 * @param symbol значение для поиска 
 * @param stairs массив указателей на потомков
 * @param end на этой ноде заканчивается регулярное выражение
 * @param store_special хранилище Квантификатор
*/


struct tnode {
  uint8_t symbol{0};
  bool special{false};
  bool end{false};
  StoreSpecial store_special;
  std::array<std::shared_ptr<tnode>, 255> stairs = {std::make_shared<tnode>(isEmptyTNode())};
  //tnode* stairs[255] = {&isEmptyTNode()}; //To Do в место shared_ptr нужен указатель, shared много весить(нужно проверить)
};

struct tnodeTest {
  uint8_t symbol{0};
  bool special{false};
  bool end{false};
  StoreSpecialTest store_special;
  //std::array<std::shared_ptr<tnode>, 255> stairs = {std::make_shared<tnode>(isEmptyTNode())};
  tnode* stairs[255] = {&isEmptyTNode()}; //To Do в место shared_ptr нужен указатель, shared много весить(нужно проверить)
};

/**
 * @brief "дерево" для регулярных выражений
 * 
 */
class TreeRegular {
private:
  tnode root;  
  bool addTNode(tnode *head, std::vector<uint8_t>::const_iterator &it,
                  const std::vector<uint8_t> &re);
  bool addSpecialSymbol(tnode *head, std::vector<uint8_t>::const_iterator &it, 
                  const std::vector<uint8_t> &re);
  //bool isSpecialSymbol(uint8_t symbol);
  //bool isExistSpecialSymbol(tnode &node, uint8_t symbol)
public:
 bool addRegularExpresion(const std::vector<uint8_t> &regular_expresion);
};
}

#endif // TREE_REGULAR_H