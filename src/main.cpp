#include "hello.h"

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

// convenient aliases
using IP = std::vector<std::string>;
using IP_pool = std::vector<IP>;

using IP_int = std::vector<int>;
using IP_pool_int = std::vector<IP_int>;

// ("",  '.') -> [""]
// ("11", '.') -> ["11"]
// ("..", '.') -> ["", "", ""]
// ("11.", '.') -> ["11", ""]
// (".11", '.') -> ["", "11"]
// ("11.22", '.') -> ["11", "22"]
IP split(const std::string &str, char d) {
  std::vector<std::string> r;

  std::string::size_type start = 0;
  std::string::size_type stop = str.find_first_of(d);
  while (stop != std::string::npos) {
    r.push_back(str.substr(start, stop - start));

    start = stop + 1;
    stop = str.find_first_of(d, start);
  }

  r.push_back(str.substr(start));

  // NRVO
  return r;
}

IP_pool_int filter(const IP_pool_int &ip_pool, int first_byte, int second_byte = -1) {
  IP_pool_int filtered_ip_pool;

  std::copy_if(ip_pool.cbegin(), ip_pool.cend(),
               std::back_inserter(filtered_ip_pool), [&](const IP_int &ip) {
                 return ip[0] == first_byte &&
                        (second_byte < 0 || ip[1] == second_byte);
               });

  return filtered_ip_pool;
}

IP_pool_int filter_any(const IP_pool_int &ip_pool, int byte) {
  IP_pool_int filtered_ip_pool;

  std::copy_if(ip_pool.cbegin(), ip_pool.cend(),
               std::back_inserter(filtered_ip_pool), [byte](const IP_int &ip) {
                 return std::any_of(ip.cbegin(), ip.cend(),
                                    [byte](int cur_byte) {
                                      return cur_byte == byte;
                                    });
               });

  return filtered_ip_pool;
}

void print(const IP_pool_int &ip_pool) {
  for (auto ip = ip_pool.cbegin(); ip != ip_pool.cend(); ++ip) {
    for (auto ip_part = ip->cbegin(); ip_part != ip->cend(); ++ip_part) {
      if (ip_part != ip->cbegin())
        std::cout << ".";

      std::cout << *ip_part;
    }
    std::cout << std::endl;
  }
}

int main(int argc, char const *argv[]) {
  // std::cout << version::version() << std::endl << "Hello, World!" <<
  // std::endl;

  try {
    IP_pool ip_pool;

    std::string line;
    while (std::getline(std::cin, line)) {
      IP ip = split(line, '\t');
      // perfect forwarding
      ip_pool.emplace_back(split(ip.at(0), '.'));
    }

    IP_pool_int ip_pool_int;
    for (const auto& ip : ip_pool) {
        IP_int ip_int;
        for (int idx = 0; idx <= 3; ++idx) {
            ip_int.push_back(stoi(ip[idx]));
        }

        ip_pool_int.emplace_back(std::move(ip_int));
    }

    // bitwise stable sorting
    for (int idx = 3; idx >= 0; --idx) {
      std::stable_sort(ip_pool_int.begin(), ip_pool_int.end(),
                       [&idx](const IP_int &ip, const IP_int &othIp) {
                         return ip[idx] > othIp[idx];
                       });
    }

    print(ip_pool_int);

    // std::cout  << std::endl;
    IP_pool_int filtered_ip_pool = filter(ip_pool_int, 1);
    print(filtered_ip_pool);

    // std::cout  << std::endl;
    filtered_ip_pool = filter(ip_pool_int, 46, 70);
    print(filtered_ip_pool);

    // std::cout  << std::endl;
    filtered_ip_pool = filter_any(ip_pool_int, 46);
    print(filtered_ip_pool);
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
