#ifndef NODE_H
#define NODE_H

#include <mutex>

class Node {
public:
  Node(int data);
  int _data;
  Node *_next;
  std::mutex _mtx;
};

#endif // NODE_H
