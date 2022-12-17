#include "aoc.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <tuple>
#include <set>
#include <map>

std::tuple<std::string, std::string> p17(const std::string &oinput) {
    int64_t ans1 = 0;
    int64_t ans2 = 0;

    std::vector<std::vector<std::string>> blocks {
            {"..@@@@."},
            {"...@...",
             "..@@@..",
             "...@..."},
            {"..@@@..",
             "....@..",
             "....@.."},
            {"..@....",
             "..@....",
             "..@....",
             "..@...."},
            {"..@@...",
             "..@@..."}
    };

    std::string input;
    for(auto c : oinput) {
        if(c == '<' || c == '>') input.push_back(c);
    }

    std::vector<std::string> screen;
    size_t idx = 0;
    auto getnext = [&input,&idx]() mutable {
        return input[idx++ % input.size()];
    };

    //auto getnext = [](){return '>';};

    auto printscreen = [&screen] {
        std::cout << "screen:\n";
        for(auto rit = screen.rbegin(); rit != screen.rend(); rit++) {
            std::cout << *rit << '\n';
        }
    };

    for(auto p : {1,2}) {
        screen.clear();
        idx = 0;
        std::map<std::string, std::tuple<size_t, int>> memo;
        int64_t delta_ans = 0;
        bool have_delta = false;
        size_t goal = p==1 ? 2022ull : 1000000000000ull;

        for (size_t i = 0; i < goal; i++) {
            if (!have_delta && screen.size() > 30) {
                std::string key = std::to_string(idx % input.size()) + std::to_string(i % blocks.size());
                for (auto rit = screen.rbegin(); rit != screen.rbegin() + 30; rit++) key += *rit;
                if (auto it = memo.find(key); it != memo.end()) {
                    auto [size, iteration] = it->second;

                    auto deltai = i - iteration;
                    auto deltah = screen.size() - size;

                    auto togo = goal - i;
                    auto imult = togo / deltai;
                    delta_ans = imult * deltah;
                    i = goal - (togo % deltai) - 1;
                    have_delta = true;
                    continue;
                } else memo[key] = std::make_tuple(screen.size(), i);
            }

            screen.push_back(".......");
            screen.push_back(".......");
            screen.push_back(".......");
            for (auto &l: blocks[i % blocks.size()]) {
                screen.push_back(l);
            }

            bool moving = true;
            while (moving) {
                auto m = getnext();
                if (m == '<') {
                    bool canshift = true;
                    for (auto &line: screen) {
                        auto x = line.find('@');
                        if (x != std::string::npos) {
                            if (x == 0 || line[x - 1] == '#') canshift = false;
                        }
                    }
                    if (canshift) {
                        for (auto &line: screen) {
                            auto x = line.find('@');
                            if (x != std::string::npos) {
                                while (x < line.size() && line[x] == '@') {
                                    line[x - 1] = '@';
                                    line[x] = '.';
                                    x++;
                                }
                            }
                        }
                    }
                } else { // m == >
                    bool canshift = true;
                    for (auto &line: screen) {
                        auto x = line.rfind('@');
                        if (x != std::string::npos) {
                            if (x == line.size() - 1 || line[x + 1] == '#') canshift = false;
                        }
                    }
                    if (canshift) {
                        for (auto &line: screen) {
                            auto x = line.rfind('@');
                            if (x != std::string::npos) {
                                while (x > 0 && line[x] == '@') {
                                    line[x + 1] = '@';
                                    line[x] = '.';
                                    x--;
                                }
                                if (line[0] == '@') {
                                    line[1] = '@';
                                    line[0] = '.';
                                }
                            }
                        }
                    }
                }

                for (int ri = 0; ri < screen.size(); ri++) {
                    for (int x = 0; x < screen[ri].size(); x++) {
                        if (screen[ri][x] == '@') {
                            if (ri == 0) moving = false;
                            else if (screen[ri - 1][x] == '#') moving = false;
                        }
                        if (!moving) break;
                    }
                    if (!moving) break;
                }

                if (moving) {
                    for (int ri = 0; ri < screen.size(); ri++) {
                        for (int x = 0; x < screen[ri].size(); x++) {
                            if (screen[ri][x] == '@') {
                                screen[ri - 1][x] = '@';
                                screen[ri][x] = '.';
                            }
                        }
                    }

                }
                if (screen.back() == ".......") screen.pop_back();

            }
            for (auto &r: screen) for (auto &c: r) if (c == '@') c = '#';
        }
        (p == 1 ? ans1 : ans2) = screen.size() + delta_ans;
    }
    return {std::to_string(ans1), std::to_string(ans2)};
}
