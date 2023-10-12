#ifndef TREE_NODE_H
#define TREE_NODE_H

#include <array>
#include <cstdint>
#include <string>
#include <sys/types.h>
#include <vector>
#include <memory>

namespace fr::tree {
namespace {
  constexpr uint8_t g_count_special_symbol{3};
  constexpr uint8_t g_shielding_symbol{static_cast<uint8_t>('/')};
  constexpr uint8_t g_size_repeat_special{20};
}

struct tnode;
struct SpecialSymbol;

tnode *isEmptyTNode();
SpecialSymbol *isEmptySpecialSymbol();
uint8_t *isEmptyMemory();
std::string &isEmptyRegular();

struct RepeatSpecial {
  bool end{false};
  uint32_t repeat{0};

};

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
  //количество повторяющихся квантификаторов //To Do нужно уйти от магического числа 20   
  std::array<RepeatSpecial, g_size_repeat_special> repeat_store;///число повторений
  std::array <tnode*, 255> stairs;

  SpecialSymbol() {
    stairs.fill(isEmptyTNode());
  }

};

struct StoreSpecial {
  //SpecialSymbol *store[g_count_special_symbol];
  std::array<SpecialSymbol *, g_count_special_symbol> store;
  StoreSpecial() {
    store.fill(isEmptySpecialSymbol());
  }
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
  std::array <tnode*, 255> stairs;

  tnode() {
    stairs.fill(isEmptyTNode());
  }
};

}

#endif