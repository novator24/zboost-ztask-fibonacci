// Copyright 2013 GPL2 (c)
#ifndef ZBOOST_ZTASK_ZASYNC_H_
#define ZBOOST_ZTASK_ZASYNC_H_
#include <zboost/ztask/zhandle.hpp>
#include <zboost/ztask/ztask.hpp>
#include <zboost/ztask/zstatic_pool.hpp>
namespace zboost {
  namespace ztasks {
    template<typename ReturnType, typename Queue>
    zhandle<ReturnType> zasync(ztask<ReturnType>& t, zstatic_pool<Queue> &p) {
      return p.submit_as_producer(t);
    }
  }  // namespace ztasks
}  // namespace zboost
#endif  // ZBOOST_ZTASK_ZASYNC_H_

