#include "aoc.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <tuple>
#include <set>
#include <limits>

std::tuple<std::string, std::string> p15(const std::string &input) {
    int64_t ans1 = 0;
    int64_t ans2 = 0;

    std::set<std::pair<int, int>> beacons;
    std::vector<std::pair<int, int>> sensors;
    std::vector<std::tuple<int, int, int>> sensors2;

    {
        int num = 0;
        bool havenum = false;
        bool neg = false;
        std::vector<int> nums;
        for (const auto c: input) {
            if (c >= '0' && c <= '9') {
                num *= 10;
                num += c - '0';
                havenum = true;
            } else if (c == '-') {
                neg = true;
            } else {
                if (havenum) {
                    if (neg) num *= -1;
                    nums.push_back(num);
                    if (nums.size() == 4) {
                        sensors.emplace_back(nums[0], nums[1]);
                        beacons.emplace(nums[2], nums[3]);
                        sensors2.emplace_back(nums[0], nums[1], abs(nums[0] - nums[2]) + abs(nums[1] - nums[3]));
                        nums.clear();
                    }
                }
                neg = false;
                havenum = false;
                num = 0;
            }
        }

    }

    {
        const int Y = 2000000;

        std::vector<std::tuple<int, int>> diffs;
        for (auto [x, y, d]: sensors2) {
            int dy = abs(Y - y);
            if (dy < d) {
                auto dx = abs(d - dy);
                diffs.emplace_back(x - dx, x + dx);
            }
        }

        int maxx = std::numeric_limits<int>::min();
        int minx = std::numeric_limits<int>::max();
        for (auto [x1, x2]: diffs) {
            if (x1 < minx) minx = x1;
            if (x2 > maxx) maxx = x2;
        }

        std::string s(maxx - minx + 1, '.');
        for (auto [x1, x2]: diffs) {
            for (int x = x1; x < x2; x++) {
                s[x - minx] = '#';
            }
        }
        for (auto c: s) {
            if (c == '#') ans1++;
        }
    }


    const int MAX = 4000000;
    std::vector<int> on, off;

    for (int Y = 0; Y <= MAX && !ans2; Y++) {
        on.clear();
        off.clear();
        for (const auto [x, y, d]: sensors2) {
            auto dy = abs(Y - y);
            if(dy < d) {
                on.push_back(x-(d-dy));
                off.push_back(x+(d-dy));
            }
        }
        std::sort(on.begin(), on.end());
        std::sort(off.begin(), off.end());
        size_t oni = 0, offi = 0;
        int level = 0;
        while(oni < on.size() && offi < off.size()) {
            if(on[oni] == off[offi]) {
                oni++; offi++;
            } else if(on[oni] < off[offi]) {
                oni++; level++;
            } else {
                offi++;
                level--;
                if(level == 0) {
                    ans2 = (off[offi-1]+1)*4000000ll + Y;
                }
            }
        }
    }

    return {std::to_string(ans1), std::to_string(ans2)};
}
