#include "aoc.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <tuple>
#include <set>

std::tuple<std::string, std::string> p25(const std::string &input) {
    int64_t sum = 0;
    {
        int64_t num = 0;
        for (const auto c: input) {
            if (c >= '0' && c <= '2') {
                num *= 5;
                num += (c - '0');
            } else if(c == '-') {
                num *= 5;
                num += -1;
            } else if(c == '=') {
                num *= 5;
                num += -2;
            } else {
                sum += num;
                num = 0;
            }
        }
    }
    
    std::string tmp;
    while(sum) {
        auto d = sum % 5;
        if(d < 3) tmp.push_back('0'+d);
        sum /= 5;
        if(d >= 3) {
            tmp.push_back(d == 3 ? '=' : '-');
            sum++;
        }
    }

    return {{tmp.rbegin(), tmp.rend()}, "-"};
}
// 100-0=011=20-1 fel
