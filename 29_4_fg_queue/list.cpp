#include "list.h"
#include <iomanip>
#include <iostream>

List::List() : _head{nullptr} {}

void List::remove(int value) {
  Node *prev, *cur;

  _mtx.lock();
  if (!_head) {
    return;
  }
  prev = this->_head;
  cur = this->_head->_next;

  prev->_mtx.lock();
  if (prev->_data == value) {
    _head = prev->_next;
    prev->_mtx.unlock();
    delete prev;
    _mtx.unlock();
    return;
  }
  _mtx.unlock();
  if (cur) {
    cur->_mtx.lock();
  }

  while (cur) {
    if (cur->_data == value) {
      prev->_next = cur->_next;
      prev->_mtx.unlock();
      cur->_mtx.unlock();
      delete cur;
      return;
    }

    Node *old_prev = prev;
    prev = cur;
    cur = cur->_next;
    old_prev->_mtx.unlock();
    if (cur) {
      cur->_mtx.lock();
    }
  }
  prev->_mtx.unlock();
}

void List::print() const {
  std::lock_guard<std::mutex> lg{_mtx};
  for (auto cur = _head; cur != nullptr; cur = cur->_next) {
    std::cout << cur->_data << ' ';
  }
  std::cout << std::endl;
}

void List::insertIntoMiddle(int value, size_t pos) {
  if (!_head) {
    std::lock_guard<std::mutex> lg{_mtx};
    _head = new Node{value};
    return;
  }
  if (pos == 0) {
    std::lock_guard<std::mutex> lg{_mtx};
    auto old_head = _head;
    _head = new Node{value};
    _head->_next = old_head;
    return;
  }
  auto cur = _head;
  for (size_t i = 0; i < pos; ++i) {
    if (!cur->_next) {
      std::lock_guard<std::mutex> lg{cur->_mtx};
      cur->_next = new Node{value};
      return;
    }
    cur = cur->_next;
  }
  {
    std::lock_guard<std::mutex> lg{cur->_mtx};
    auto old_next = cur->_next;
    auto next = new Node{value};
    next->_next = old_next;
    cur->_next = next;
  }
}
