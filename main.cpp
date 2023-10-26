#include <cstddef>
#include <fstream>
#include <ostream>
#include <stdexcept>
#include <string>
#include <tuple>
#include <cstddef>
#include <cstdint>

#include "include/tree_search_engine.hpp"

using namespace fr::tree;

int main(int argc, char **argv) {
  if(argc < 3) {
    std::cout << "Не верно количество аргументов" << std::endl;
    return 1;
  }    

  std::string poem;
  std::string line;
  std::string name_file = argv[1];
  std::ifstream in(name_file.c_str());
  if (in.is_open()) {
      while (std::getline(in, line)) {
        poem += line;
      }
  } else {
    std::cout << "Не получилось считать файл" << std::endl;
    return 1;
  }
  in.close();     // закрываем файл

  fr::tree::TreeSearchEngine tse;
  try{
    for(int i{2}; i < argc; ++i) {
      std::string temp = argv[i];
      tse.addRegularExpression(temp);
    }
  } catch(const std::invalid_argument &ia) {
    std::cout << "Превышено число символов в 'дереве'" << std::endl;
    std::cout << ia.what() << std::endl;
    return 1;
  }
  tse.clearSearch();

  const auto found = tse.start_search(static_cast<void*>(poem.data()), poem.size());

  if(!std::get<2>(found).empty()) {
    std::cout << "Регулярное выражение " << std::get<2>(found) << std::endl;    
  } else {
    std::cout << "Не нашел" << std::endl;
  }

  tse.clearSearch();//обязателен
  
  return 0;
}