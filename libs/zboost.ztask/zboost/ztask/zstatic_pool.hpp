// Copyright 2013 GPL2 (c)
#ifndef ZBOOST_ZTASK_ZSTATIC_POOL_H_
#define ZBOOST_ZTASK_ZSTATIC_POOL_H_
#include <boost/thread.hpp>  // boost 1.54
#include <zboost/ztask/zhandle.hpp>
#include <zboost/ztask/zcallable.hpp>
#include <zboost/ztask/zexceptions.hpp>
namespace zboost {
  namespace ztasks {
    template <class Queue>
    class zstatic_pool {
     public:
      // do not change order
      // http://www.devexp.ru/2011/11/mnogopotochnost-bagi-i-spiski-inicializacii/
      zstatic_pool(size_t pool_size) \
        : m_queue(),
          m_thread_group(),
          m_reject_new(false) {
        for (size_t i = 0; i < pool_size; ++i) {
          m_thread_group.add_thread(new boost::thread(boost::bind(&zstatic_pool::loop_as_consumer,
              this)));
        }
      }

      virtual ~zstatic_pool() { shutdown(); }

      template <typename ReturnType>
      zhandle<ReturnType> submit_as_producer(ztask<ReturnType>& task) {
        if (m_reject_new) {
          throw zrejected_ztask();
        }
        zhandle<ReturnType> handle;
        task.register_handle(&handle);
        zcallable* ptr = new zcallable(task);
        while (!m_queue.put(ptr)) {}
        return handle;
      }

      void loop_as_consumer() {
        zcallable* ptr = NULL;
        while (true) {
          if (!m_queue.wait_take(ptr)) {
            return;
          }
          if (ptr) {
            ptr->call_task_func();
            delete ptr;
          }
        }
      }

      void shutdown() {
        m_reject_new = true;
        m_queue.shutdown();
        m_thread_group.join_all();
      }

      void shutdown_now() {
        m_reject_new = true;
        m_queue.shutdown_now();
        m_thread_group.join_all();
      }

     private:
      Queue m_queue;
      boost::thread_group m_thread_group;
      bool m_reject_new;

      // noncopyable
      zstatic_pool(const zstatic_pool&);
      void operator=(const zstatic_pool&);
    };
  }  // namespace ztasks
}  // namespace zboost

#endif  // ZBOOST_ZTASK_ZSTATIC_POOL_H_

