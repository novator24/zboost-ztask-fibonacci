// Copyright 2013 GPL2 (c)
#ifndef ZBOOST_ZTASK_ZTASK_H_
#define ZBOOST_ZTASK_ZTASK_H_
#include <assert.h>
#include <boost/noncopyable.hpp>  // boost 1.54
#include <boost/bind.hpp>  // boost 1.54
#include <boost/function.hpp>  // boost 1.54
#include <boost/mem_fn.hpp>  // boost 1.54
#include <boost/thread/detail/move.hpp>  // boost 1.54
#include <zboost/ztask/zhandle.hpp>
#include <zboost/ztask/zdetail/ztask_zbase.hpp>
using boost::function;
using boost::detail::thread_move_t;
using zboost::ztasks::zhandle;
using zboost::ztasks::zdetail::ztask_zbase;
namespace zboost {
  namespace ztasks {
    template<class ReturnType>
    class ztask
      : public function<ReturnType(void)>,
        public ztask_zbase {
     public:
      // http://stackoverflow.com/questions/4972157
      ztask(const function<ReturnType(void)>& fn)
        : function<ReturnType(void)>(fn),
          ztask_zbase(boost::bind(boost::mem_fn(&ztask<ReturnType>::run), this)),
          m_handle_ptr(NULL) {}

      virtual ~ztask() {}

      virtual ztask_zbase* clone() const {
        const function<ReturnType(void)>* const fp = this;
        ztask<ReturnType>* result = new ztask<ReturnType>(*fp);
        result->register_handle(m_handle_ptr);
        return result;
      }

      void register_handle(zhandle<ReturnType>* ptr) {
        m_handle_ptr = ptr;
      }

      void run() {
        assert(NULL == m_handle_ptr);
        function<ReturnType(void)>* fp = this;
        boost::mutex::scoped_lock lock(m_handle_ptr->m_shared_ptr->get_mutex());
        try {
          m_handle_ptr->m_shared_ptr->set( (*fp)() );
        } catch ( boost::exception const& e) {
          m_handle_ptr->m_shared_ptr->set_exception( boost::copy_exception( e) );
        } catch ( std::ios_base::failure const& e) {
          m_handle_ptr->m_shared_ptr->set_exception( boost::copy_exception( e) );
        } catch ( std::domain_error const& e) {
          m_handle_ptr->m_shared_ptr->set_exception( boost::copy_exception( e) );
        } catch ( std::invalid_argument const& e) {
          m_handle_ptr->m_shared_ptr->set_exception( boost::copy_exception( e) );
        } catch ( std::length_error const& e) {
          m_handle_ptr->m_shared_ptr->set_exception( boost::copy_exception( e) );
        } catch ( std::out_of_range const& e) {
          m_handle_ptr->m_shared_ptr->set_exception( boost::copy_exception( e) );
        } catch ( std::logic_error const& e) {
          m_handle_ptr->m_shared_ptr->set_exception( boost::copy_exception( e) );
        } catch ( std::overflow_error const& e) {
          m_handle_ptr->m_shared_ptr->set_exception( boost::copy_exception( e) );
        } catch ( std::range_error const& e) {
          m_handle_ptr->m_shared_ptr->set_exception( boost::copy_exception( e) ); 
        } catch ( std::underflow_error const& e) {
          m_handle_ptr->m_shared_ptr->set_exception( boost::copy_exception( e) );
        } catch ( std::runtime_error const& e) {
          m_handle_ptr->m_shared_ptr->set_exception( boost::copy_exception( e) );
        } catch ( std::bad_alloc const& e) {
          m_handle_ptr->m_shared_ptr->set_exception( boost::copy_exception( e) );
        } catch ( std::bad_cast const& e) {
          m_handle_ptr->m_shared_ptr->set_exception( boost::copy_exception( e) );
        } catch ( std::bad_typeid const& e) {
          m_handle_ptr->m_shared_ptr->set_exception( boost::copy_exception( e) );
        } catch ( std::bad_exception const& e) {
          m_handle_ptr->m_shared_ptr->set_exception( boost::copy_exception( e) );
        } catch (...) {
          m_handle_ptr->m_shared_ptr->set_exception( boost::current_exception() );
        }
        m_handle_ptr->m_shared_ptr->set_ready(true);
        m_handle_ptr->m_shared_ptr->get_waiter().notify_all();
      }

     private:
      zhandle<ReturnType>* m_handle_ptr;
    };
  }  // namespace ztasks
}  // namespace zboost
#endif  // ZBOOST_ZTASK_ZTASK_H_

