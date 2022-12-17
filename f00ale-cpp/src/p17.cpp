#include "aoc.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <tuple>
#include <set>

std::tuple<std::string, std::string> p17(const std::string &input) {
    int64_t ans1 = 0;
    int64_t ans2 = 0;

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

    std::vector<std::string> screen;
    auto getnext = [&input,idx=0]() mutable {
        while(input[idx%input.size()] != '<' && input[idx%input.size()] != '>') idx++;
        return input[idx++ % input.size()];
    };

    //auto getnext = [](){return '>';};

    auto printscreen = [&screen] {
        std::cout << "screen:\n";
        for(auto rit = screen.rbegin(); rit != screen.rend(); rit++) {
            std::cout << *rit << '\n';
        }
    };

    for(int i = 0; i < 2022; i++) {
        screen.push_back(".......");
        screen.push_back(".......");
        screen.push_back(".......");
        for(auto & l : blocks[i%blocks.size()]) {
            screen.push_back(l);
        }
        //screen.back()[0] = '#';
        //screen.back()[6] = '#';

        bool moving = true;
        //if(i == 5) printscreen();
        while(moving) {
            auto m = getnext();
            //if(i==5)printscreen();
            //if(i==5)std::cout << "move " << m << '\n';
            if(m == '<') {
                bool canshift = true;
                for(auto & line : screen) {
                    auto x = line.find('@');
                    if(x != std::string::npos) {
                        if (x == 0 || line[x - 1] == '#') canshift = false;
                    }
                }
                if(canshift) {
                    for(auto & line : screen) {
                        auto x = line.find('@');
                        if(x != std::string::npos) {
                            while(x < line.size() && line[x] == '@') {
                                line[x - 1] = '@';
                                line[x] = '.';
                                x++;
                            }
                        }
                    }
                }
            } else { // m == >
                bool canshift = true;
                for(auto & line : screen) {
                    auto x = line.rfind('@');
                    if(x != std::string::npos) {
                        if (x == line.size() - 1 || line[x + 1] == '#') canshift = false;
                    }
                }
                if(canshift) {
                    for(auto & line : screen) {
                        auto x = line.rfind('@');
                        if(x != std::string::npos) {
                            while (x > 0 && line[x] == '@') {
                                line[x + 1] = '@';
                                line[x] = '.';
                                x--;
                            }
                            if(line[0] == '@') {line[1]='@'; line[0] = '.';}
                        }
                    }
                }
            }
            //if(i==5)printscreen();

            for(int ri = 0; ri < screen.size(); ri++) {
                for(int x = 0; x < screen[ri].size(); x++) {
                    if(screen[ri][x] == '@') {
                        if(ri == 0) moving = false;
                        else if(screen[ri-1][x] == '#') moving = false;
                    }
                    if(!moving) break;
                }
                if(!moving) break;
            }

            if(moving) {
                for(int ri = 0; ri < screen.size(); ri++) {
                    for(int x = 0; x < screen[ri].size(); x++) {
                        if(screen[ri][x] == '@') {
                            screen[ri-1][x] = '@';
                            screen[ri][x] = '.';
                        }
                    }
                }

            }
            if(screen.back() == ".......") screen.pop_back();
            //printscreen();

        }
        for(auto & r : screen) for(auto & c : r) if(c=='@') c = '#';


    }
    //printscreen();
    ans1 = screen.size();
    return {std::to_string(ans1), std::to_string(ans2)};
}
