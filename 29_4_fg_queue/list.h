#ifndef LIST_H
#define LIST_H
#include "node.h"
#include <mutex>

class List {
public:
  List();
  void remove(int value);
  void print() const;
  void insertIntoMiddle(int value, size_t pos);

private:
  Node *_head;

  mutable std::mutex _mtx;
};

#endif // LIST_H
