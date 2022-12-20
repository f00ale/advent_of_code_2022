#include "aoc.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <tuple>
#include <set>
#include <map>

std::tuple<std::string, std::string> p20(const std::string &input) {
    int64_t ans1 = 0;
    int64_t ans2 = 0;

    std::vector<int> v;

    {
        int num = 0;
        bool havenum = false;
        bool neg = false;
        for (const auto c: input) {
            if (c >= '0' && c <= '9') {
                num *= 10;
                num += c - '0';
                havenum = true;
            } else if(c == '-') {
                neg = true;
            } else if(havenum) {
                havenum = false;
                if(neg) num *= -1;
                v.push_back(num);
                num = 0;
                neg = false;
            }
        }

    }

    for(auto p : {1,2}) {
        std::vector<std::tuple<int64_t, int>> v2(v.size());
        int64_t mul = p == 1 ? 1 : 811589153;
        for (size_t i = 0; i < v.size(); i++) {
            v2[i] = {mul*v[i], i};
        }

        for(int varv = 0; varv < (p==1?1:10); varv++) {
            for (int i = 0; i < v2.size(); i++) {
                int idx = -1;
                for (int j = 0; j < v2.size(); j++) {
                    if (i == std::get<1>(v2[j])) idx = j;
                }
                int64_t num = std::get<0>(v2[idx]);

                if (num > 0) {
                    num %= (v2.size() - 1);
                    while (num--) {
                        std::swap(v2[idx % v2.size()], v2[(idx + 1) % v2.size()]);
                        idx++;
                    }

                } else if (num < 0) {
                    num = v2.size() - num - 1;
                    num %= (v2.size() - 1);

                    while (num--) {
                        int i2 = idx - 1;
                        if (i2 < 0) i2 = v2.size() - 1;
                        std::swap(v2[idx], v2[i2]);
                        idx--;
                        if (idx < 0) idx = v2.size() - 1;
                    }
                }
            }
        }
        auto it0 = std::find_if(v2.begin(), v2.end(), [](auto &&t) { return 0 == std::get<0>(t); });
        auto idx0 = std::distance(v2.begin(), it0);
        for (int i = 0; i < 3; i++) {
            auto tmp = std::get<0>(v2[(idx0 + (i + 1) * 1000) % v2.size()]);
            (p == 1 ? ans1 : ans2) += tmp;
        }
    }

    return {std::to_string(ans1), std::to_string(ans2)};
}
