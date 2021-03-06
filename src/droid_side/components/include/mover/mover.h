#pragma once

#include "messages/move_message.h"
#include "utils/containers/message_queue.h"
#include "utils/threads/synchronization/lock.h"
#include "utils/threads/thread_runnable.h"

namespace mover {
typedef utils::MessageQueue<messages::MoveMessage> MoveMessageQueue;

class Mover : public utils::threads::ThreadRunnable {
 public:
  virtual void OnMoveMessageReceived(const messages::MoveMessage& message) = 0;

 protected:
  MoveMessageQueue move_queue_;
  utils::synchronization::Lock move_queue_lock_;
};
}
