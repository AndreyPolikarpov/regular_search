#ifndef TREE_NODE_H
#define TREE_NODE_H

#include <array>
#include <vector>
#include <memory>

namespace fr::tree {

struct tnode;

tnode &isEmptyTNode();

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
  //std::array<std::shared_ptr<tnode>, 255> stairs = {std::make_shared<tnode>(isEmptyTNode())};
  tnode* stairs[255]{};//To Do в место shared_ptr нужен указатель

  SpecialSymbol() {
    for(int i = 0; i < 255; ++i)
      stairs[i] = &isEmptyTNode();
  }
};

struct StoreSpecial {
  std::array<SpecialSymbol, 3> store;
  //bool isActive{false};
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
  bool is_active_special{false};
  bool end{false};
  StoreSpecial store_special;
  //std::array<std::shared_ptr<tnode>, 255> stairs = {std::make_shared<tnode>(isEmptyTNode())};
  tnode* stairs[255] = {&isEmptyTNode()}; //To Do в место shared_ptr нужен указатель, shared много весить(нужно проверить)

  tnode() {
    for(int i = 0; i < 255; ++i)
      stairs[i] = &isEmptyTNode();
  }
};

}

#endif