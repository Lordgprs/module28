#include "list.h"
#include <iostream>
#include <limits>

int main() {
  // Заполняем список нечётными числами
  List list;
  list.insertIntoMiddle(1, std::numeric_limits<size_t>::max());
  list.insertIntoMiddle(3, std::numeric_limits<size_t>::max());
  list.insertIntoMiddle(5, std::numeric_limits<size_t>::max());
  list.insertIntoMiddle(7, std::numeric_limits<size_t>::max());
  list.insertIntoMiddle(9, std::numeric_limits<size_t>::max());
  list.print();

  // Добавляем ноль в начало
  list.insertIntoMiddle(0, 0);
  list.print();

  // Добиваем список чётными числами
  list.insertIntoMiddle(2, 1);
  list.insertIntoMiddle(4, 3);
  list.insertIntoMiddle(6, 5);
  list.insertIntoMiddle(8, 7);
  list.print();

  return EXIT_SUCCESS;
}
