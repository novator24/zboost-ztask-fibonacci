// Copyright 2013 GPL2 (c)
#ifndef ZBOOST_ZTASK_ZDETAIL_ZTASK_ZBASE_H_
#define ZBOOST_ZTASK_ZDETAIL_ZTASK_ZBASE_H_
#include <boost/function.hpp>  // boost 1.54
#include <boost/exception_ptr.hpp>  // boost 1.54
namespace zboost {
  namespace ztasks {
    namespace zdetail {
      class ztask_zbase {
       public:
        typedef boost::function<void (void)> void_function_t;

        ztask_zbase(void_function_t main_func_) : m_main_func(main_func_) {}

        virtual ~ztask_zbase() { return; }

        virtual ztask_zbase* clone() const = 0;

        void_function_t get_main_func() const { return m_main_func; }

       private:
        void_function_t m_main_func;

        // noncopyable
        ztask_zbase(const ztask_zbase&);
        void operator=(const ztask_zbase&);
      };
    }  // namespace zdetail
  }  // namespace ztasks
}  // namespace zboost
#endif  // ZBOOST_ZTASK_ZDETAIL_ZTASK_ZBASE_H_

