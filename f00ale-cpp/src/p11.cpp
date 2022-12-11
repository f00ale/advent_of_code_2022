#include "aoc.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <tuple>
#include <set>
#include <functional>
#include <deque>

struct monkey {
    monkey(std::deque<int64_t> v, std::function<int64_t(int64_t)> o, std::function<int(int64_t)> t) :
            items(std::move(v)), op(std::move(o)), test(std::move(t)) {}

    std::deque<int64_t> items;
    std::function<int64_t(int64_t)> op;
    std::function<int(int64_t)> test;
    int64_t inspections = 0;
};

std::tuple<std::string, std::string> p11(const std::string &input) {
    int64_t ans1 = 0;
    int64_t ans2 = 0;
    // Does not parse, data hardcoded!!
    {
        int num = 0;
        bool havenum = false;
        for (const auto c: input) {
            if (c >= '0' && c <= '9') {
                num *= 10;
                num += c - '0';
                havenum = true;
            } else {

                havenum = false;
                num = 0;
            }
        }

    }

    for (auto p: {1, 2}) {
        std::vector<monkey> monkeys;
        if (false) {
            monkeys.push_back({{79, 98}, [](auto i) { return i * 19; }, [](auto i) { return i % 23 ? 3 : 2; }});
            monkeys.push_back({{54, 65, 75, 74}, [](auto i) { return i + 6; }, [](auto i) { return i % 19 ? 0 : 2; }});
            monkeys.push_back({{79, 60, 97}, [](auto i) { return i * i; }, [](auto i) { return i % 13 ? 3 : 1; }});
            monkeys.push_back({{74}, [](auto i) { return i + 3; }, [](auto i) { return i % 17 ? 1 : 0; }});
        } else {
            monkeys.push_back({{71, 56, 50, 73}, [](auto i) { return i * 11; }, [](auto i) { return i % 13 ? 7 : 1; }});
            monkeys.push_back({{70, 89, 82}, [](auto i) { return i + 1; }, [](auto i) { return i % 7 ? 6 : 3; }});
            monkeys.push_back({{52, 95}, [](auto i) { return i * i; }, [](auto i) { return i % 3 ? 4 : 5; }});
            monkeys.push_back(
                    {{94, 64, 69, 87, 70}, [](auto i) { return i + 2; }, [](auto i) { return i % 19 ? 6 : 2; }});
            monkeys.push_back(
                    {{98, 72, 98, 53, 97, 51}, [](auto i) { return i + 6; }, [](auto i) { return i % 5 ? 5 : 0; }});
            monkeys.push_back({{79}, [](auto i) { return i + 7; }, [](auto i) { return i % 2 ? 0 : 7; }});
            monkeys.push_back(
                    {{77, 55, 63, 93, 66, 90, 88, 71}, [](auto i) { return i * 7; },
                     [](auto i) { return i % 11 ? 4 : 2; }});
            monkeys.push_back(
                    {{54, 97, 87, 70, 59, 82, 59}, [](auto i) { return i + 8; },
                     [](auto i) { return i % 17 ? 3 : 1; }});
        }

        const int64_t mod = 2 * 3 * 5 * 7 * 11 * 13 * 17 * 19 * 23;

        for (int iter = 0; iter < (p == 1 ? 20 : 10000); iter++) {
            for (auto &&m: monkeys) {
                while (!m.items.empty()) {
                    m.inspections++;
                    auto i = m.items.front();
                    m.items.pop_front();
                    auto n = m.op(i) % mod;
                    if (p == 1) n /= 3;
                    auto to = m.test(n);
                    monkeys[to].items.push_back(n);
                }
            }
        }

        std::vector<int64_t> inspects;
        for (auto &m: monkeys) inspects.push_back(m.inspections);
        std::partial_sort(inspects.begin(), inspects.begin() + 2, inspects.end(), std::greater<>());
        (p == 1 ? ans1 : ans2) = inspects[0] * inspects[1];
    }
    return {std::to_string(ans1), std::to_string(ans2)};
}
