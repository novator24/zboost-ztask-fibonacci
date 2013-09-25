// Copyright 2013 GPL2 (c)
#ifndef ZBOOST_ZTASK_ZHANDLE_H_
#define ZBOOST_ZTASK_ZHANDLE_H_
#include <boost/shared_ptr.hpp>  // boost 1.54
#include <zboost/ztask/zdetail/zfuture.hpp> 
#include <zboost/ztask/zdetail/ztask_zbase.hpp> 
using zboost::ztasks::zdetail::zfuture;
using zboost::ztasks::zdetail::ztask_zbase;
namespace zboost {
  namespace ztasks {
    template<typename ReturnType>
    class ztask;
  }  // namespace ztasks
}  // namespace zboost

namespace zboost {
  namespace ztasks {
    template<typename ReturnType>
    class zhandle {
     public:
      friend class ztask<ReturnType>;

      zhandle()
        : m_shared_ptr(new zfuture<ReturnType>()) {}

      virtual ~zhandle() {}

      zhandle(const zhandle& obj) 
        : m_shared_ptr(obj.m_shared_ptr) {
      }

      zhandle& operator=(const zhandle& obj) {
        if (this != &obj) {
          m_shared_ptr = obj.m_shared_ptr;
        }
        return *this;
      }

      void wait() {
        boost::mutex::scoped_lock lock(m_shared_ptr->get_mutex());
        while (!m_shared_ptr->is_ready()) {
           m_shared_ptr->get_waiter().wait(m_shared_ptr->get_mutex());
        }
      }

      bool is_ready() const {
        return m_shared_ptr->is_ready();
      }

      bool has_value() const { 
        return m_shared_ptr->has_value();
      }

      ReturnType get() const {
        return m_shared_ptr->get();
      }

      bool has_exception() const {
        return m_shared_ptr->has_exception();
      }

      boost::exception_ptr get_exception() const {
        return m_shared_ptr->get_exception();
      }

     private:
      boost::shared_ptr<zfuture<ReturnType> > m_shared_ptr;
    };
  }  // namespace ztasks
}  // namespace zboost
#endif  // ZBOOST_ZTASK_ZHANDLE_H_

