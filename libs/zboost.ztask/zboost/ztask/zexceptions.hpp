// Copyright 2013 GPL2 (c)
#ifndef ZBOOST_ZTASK_ZEXCEPTIONS_H_
#define ZBOOST_ZTASK_ZEXCEPTIONS_H_
#include <boost/exception/all.hpp> // boost 1.54
namespace zboost {
  namespace ztasks {
    class zrejected_ztask : public std::logic_error {
     public:
      zrejected_ztask() : std::logic_error("rejected task") {}
    };
  }  // namespace ztasks
}  // namespace zboost
#endif  // ZBOOST_ZTASK_ZEXCEPTIONS_H_

