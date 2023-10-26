#ifndef STORAGE_TNODE_H
#define STORAGE_TNODE_H

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include "tree_node.hpp"

namespace fr::tree {

class StorageSymbol {
public:
  /**
   * @brief Создание ассоциации с последним узлом дерева и регулярным выражением 
   * @param node_end адрес памяти последнего узла(символ) регулярного выражения инициализированного в дереве
   * @param regular регулярное выражение в виде строки
   * @return true если ассоциация создана, false в ином случае
   */
  static bool rememberRegular(void *node_end, const std::string &regular);
  
  /**
   * @brief Создание узла для дерева 
   * @param symbol символ обозначающий узел дерева 
   * @return созданный узел дерева
   * @return nullptr превышено число символов
  */
  static tnode *createTNode(uint8_t symbol);
  
  /**
   * @brief Создание узла для дерева 
   * @param symbol символ обозначающий узел дерева 
   * @return созданный узел дерева
   * @return nullptr превышено число символов
  */
  static SpecialSymbol *createSpecialSymbol(uint8_t symbol);
  
  /**
   * @brief Хранилище все узлов дерева 
   * @return std::vector<tnode>& хранилище узлов
   */
  static std::vector<tnode> &isStorageTNode();

  /**
   * @brief Хранилище все узлов квантификаторов 
   * @return std::vector<SpecialSymbol>& хранилище узлов
   */
  static std::vector<SpecialSymbol> &isStorageQuantifier();

  static tnode *isRootTree();

  /**
   * @brief Очищение хранилища узлов дерева, ассоциаций адресов памяти и регулярный выражений
   */
  static void ClearAllStorage();

  /**
   * @brief Вывод строки регулярного выражения на основании адреса памяти 
   * @param node_end адрес памяти последнего узла(символ) регулярного выражения инициализированного в дереве
   * @return регулярное выражение если на адрес памяти есть ассоциация,
          пустая строка если нет ассоциации на адрес памяти
   */
  static std::string &RegularExpressionMemorized(void *node_end);

  static size_t MaxSizeStorage();
};

}

#endif // STORAGE_TNODE