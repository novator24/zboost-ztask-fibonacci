// Copyright 2013 GPL2 (c)
#ifndef ZBOOST_ZTASK_ZUNBOUNDED_FIFO_H_
#define ZBOOST_ZTASK_ZUNBOUNDED_FIFO_H_
#include <queue>
#include <boost/scoped_ptr.hpp>  // boost 1.54
#include <boost/thread/mutex.hpp>  // boost 1.54
#include <boost/thread/condition.hpp>  // boost 1.54
#include <zboost/ztask/zcallable.hpp>
#include <zboost/ztask/zexceptions.hpp>
using zboost::ztasks::zcallable;
using zboost::ztasks::zrejected_ztask;
namespace zboost {
  namespace ztasks {
    // http://stackoverflow.com/questions/9935665/
    class zunbounded_fifo {
     public:
      typedef zcallable* ObjectType;

      zunbounded_fifo()
        : m_mutex(),
          m_waiter(),
          m_queue(),
          m_num_blocked(0),
          m_deactivated(false),
          m_deactivated_now(false) {}

      virtual ~zunbounded_fifo() { shutdown(); }

      bool put(const ObjectType& item) {
        boost::mutex::scoped_lock lock(m_mutex);
        if (m_deactivated) {
          m_waiter.notify_all();
          throw zrejected_ztask();
        }
        try {
          m_queue.push(item);
        } catch (std::exception) {
          return false;
        }
        m_waiter.notify_one();
        return true;
      }

      bool empty() const {
        boost::mutex::scoped_lock lock(m_mutex);
        return m_queue.empty();
      }

      bool try_take(ObjectType& res) {
        boost::mutex::scoped_lock lock(m_mutex);
        if (m_queue.empty()) {
          return false;
        }
        res = m_queue.front();
        m_queue.pop();
        return true;
      }

      bool wait_take(ObjectType& res) {
        boost::mutex::scoped_lock lock(m_mutex);
        ++m_num_blocked;
        while (!m_deactivated && m_queue.empty()) {
          m_waiter.wait(lock);
        }
        m_num_blocked--;
        if (m_deactivated && m_queue.empty()) {
          return false;
        }
        res = m_queue.front();
        m_queue.pop();
        return true;
      }

      void shutdown_now() {
        boost::mutex::scoped_lock lock(m_mutex);
        m_deactivated = true;
        m_deactivated_now = true;
        while (!m_queue.empty()) {
          delete m_queue.front();
          m_queue.pop();
        }
        m_waiter.notify_all();
      }

      void shutdown() {
        boost::mutex::scoped_lock lock(m_mutex);
        m_deactivated = true;
        m_waiter.notify_all();
        while (m_num_blocked) {
          m_waiter.wait(lock);
        }
      }

     private:
      // multi-thread controllers [always first]
      mutable boost::mutex m_mutex;
      boost::condition_variable_any m_waiter;

      std::queue<ObjectType> m_queue;
      unsigned int m_num_blocked;
      bool m_deactivated;
      bool m_deactivated_now;

      // noncopyable
      zunbounded_fifo(const zunbounded_fifo&);
      void operator=(const zunbounded_fifo&);
    };
  }  // namespace ztasks
}  // namespace zboost
#endif  // ZBOOST_ZTASK_ZUNBOUNDED_FIFO_H_

