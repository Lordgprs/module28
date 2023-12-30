#include "InterruptableThread.h"

thread_local thread_interrupt_struct tis;

InterruptableThread::InterruptableThread(ThreadPool *pool, int qIndex)
    : _pStopFlag{nullptr}, _thread{&InterruptableThread::startFunc, this, pool,
                                   qIndex} {}

InterruptableThread::~InterruptableThread() { _thread.join(); }

void InterruptableThread::interruptableWait(std::condition_variable &cv,
                                            std::unique_lock<std::mutex> &ul,
                                            std::function<bool()> predicate) {
  {
    std::lock_guard<std::mutex> lg{tis._defender};
    tis.cv = &cv;
  }
  try {
    cv.wait(ul, [&] { return predicate() || checkInterrupted(); });
  } catch (...) {
  }

  std::lock_guard<std::mutex> lg{tis._defender};
  tis.cv = nullptr;
}

void InterruptableThread::interrupt() {
  std::lock_guard<std::mutex> lg{_mtx};
  if (_pStopFlag) {
    std::lock_guard<std::mutex> lg2{_pStopFlag->_defender};
    _pStopFlag->thread_interrupt_flag = true;
    if (_pStopFlag->cv) {
      _pStopFlag->cv->notify_one();
      _pStopFlag->cv = nullptr;
    }
  }
}

void InterruptableThread::startFunc(ThreadPool *pool, int qIndex) {
  {
    std::lock_guard<std::mutex> lg{_mtx};
    _pStopFlag = &tis;
  }
  pool->threadFunc(qIndex);
  {
    std::lock_guard<std::mutex> lg{_mtx};
    _pStopFlag = nullptr;
  }
}

bool InterruptableThread::checkInterrupted() {
  return tis.thread_interrupt_flag;
}
