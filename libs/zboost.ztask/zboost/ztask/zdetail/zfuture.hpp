// Copyright 2013 GPL2 (c)
#ifndef ZBOOST_ZTASK_ZDETAIL_ZFUTURE_H_
#define ZBOOST_ZTASK_ZDETAIL_ZFURURE_H_
#include <boost/exception_ptr.hpp>  // boost 1.54
#include <boost/thread/mutex.hpp>  // boost 1.54
#include <boost/thread/condition.hpp>  // boost 1.54
namespace zboost {
  namespace ztasks {
    namespace zdetail {
      template<typename ReturnType>
      class zfuture {
       public:
        zfuture()
          : m_mutex(),
            m_waiter(),
            m_ready(false),
            m_has_value(false),
            m_value_ptr(NULL),
            m_has_exception(false),
            m_exception_ptr() {}

        virtual ~zfuture() { delete m_value_ptr; }

        boost::mutex& get_mutex() {
          return m_mutex;
        }

        boost::condition_variable_any& get_waiter() {
          return m_waiter;
        }

        bool is_ready() const {
          return m_ready;
        }

        void set_ready(const bool ready) {
          m_ready = ready;
        }

        bool has_value() const { 
          return m_has_value;
        }

        ReturnType get() const {
          return *m_value_ptr;
        }

        void set(const ReturnType& value) {
          delete m_value_ptr;
          m_value_ptr = new ReturnType(value);
          m_has_value = true;
        }

        bool has_exception() const {
          return m_has_exception;
        }

        boost::exception_ptr get_exception() const {
          return m_exception_ptr;
        }

        void set_exception(const boost::exception_ptr& value) {
          m_exception_ptr = value;
          m_has_exception = true;
        }

       private:
        // multi-thread controllers [always first]
        boost::mutex m_mutex;
        boost::condition_variable_any m_waiter;

        bool m_ready;
        bool m_has_value;
        ReturnType* m_value_ptr;
        bool m_has_exception;
        boost::exception_ptr m_exception_ptr;

        // noncopyable
        zfuture(const zfuture&);
        void operator=(const zfuture&);
      };
    }  // namespace zdetail
  }  // namespace ztasks
}  // namespace zboost
#endif  // ZBOOST_ZTASK_ZDETAIL_ZFUTURE_H_

