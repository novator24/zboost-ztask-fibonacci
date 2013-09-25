// Copyright 2013 GPL2 (c)
#ifndef ZBOOST_ZTASK_ZCALLABLE_H_
#define ZBOOST_ZTASK_ZCALLABLE_H_
#include <zboost/ztask/ztask.hpp>
#include <zboost/ztask/zdetail/ztask_zbase.hpp>
using zboost::ztasks::ztask;
using zboost::ztasks::zdetail::ztask_zbase;
namespace zboost {
  namespace ztasks {
    class zcallable {
     public:
      template<typename ReturnType>
      zcallable(const zboost::ztasks::ztask<ReturnType>& task) \
        : m_task_base_ptr(task.clone()) {}

      zcallable(const zcallable& obj)
        : m_task_base_ptr(obj.m_task_base_ptr->clone()) {}

      zcallable& operator=(const zcallable& obj) {
        if (this != &obj) {
          m_task_base_ptr = obj.m_task_base_ptr->clone();
        }
        return *this;
      }

      virtual ~zcallable() { delete m_task_base_ptr; }

      void call_task_func() {
        m_task_base_ptr->get_main_func()();
      }

     private:
      ztask_zbase* m_task_base_ptr;
    };
  }  // namespace ztasks
}  // namespace zboost
#endif  // ZBOOST_ZTASK_ZCALLABLE_H_

