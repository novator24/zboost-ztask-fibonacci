#include <iostream>
#include <zboost/ztask.hpp>

long fibonacci( long n, int thread) {
  if ( n == 0) return 0;
  if ( n == 1) return 1;
  long k1( 1), k2( 0);
  for ( int i( 2); i <= n; ++i) {
    long tmp( k1);
    std::cout << "#" << thread << " TEMP: "
              << k1 << "+" << k2 << "=" << (k1+k2) << std::endl;
    usleep(100000);
    k1 = k1 + k2;
    k2 = tmp;
  }
  std::cout << "#" << thread << " FINISH: " << k1 << std::endl;
  return k1;
}

using zboost::ztasks::zasync;
using zboost::ztasks::zcallable;
using zboost::ztasks::ztask;
using zboost::ztasks::zhandle;
using zboost::ztasks::zunbounded_fifo;
using zboost::ztasks::zstatic_pool;

int main() {
  // create pool
  zstatic_pool<zunbounded_fifo> pool(1);

  // create task
  ztask<long> t1(boost::bind(fibonacci, 20, 1));
  ztask<long> t2(boost::bind(fibonacci, 5, 2));

  // move task ownership to executor
  zhandle<long> h1(zasync(t1, pool));
  zhandle<long> h2(zasync(t2, pool));

  std::cout << "h1 is ready == " << std::boolalpha << h1.is_ready() << "\n";
  std::cout << "h2 is ready == " << std::boolalpha << h2.is_ready() << "\n";

  // wait for task completion
  h1.wait();
  h2.wait();

  std::cout << "h1 has value == " << std::boolalpha << h1.has_value() << "\n";
  std::cout << "h2 has value == " << std::boolalpha << h2.has_value() << "\n";
  std::cout << "h1 has exception == " << std::boolalpha << h1.has_exception() << "\n";
  std::cout << "h2 has exception == " << std::boolalpha << h2.has_exception() << "\n";

  // return result
  std::cout << "fibonacci(20) == " << h1.get() << std::endl;
  std::cout << "fibonacci(5) == " << h2.get() << std::endl;

  pool.shutdown_now();
  return 0;
}

