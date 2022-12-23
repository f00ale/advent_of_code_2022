#include "aoc.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <tuple>
#include <set>

std::tuple<std::string, std::string> p23(const std::string &input) {
    int64_t ans1 = 0;
    int64_t ans2 = 0;

    std::vector<std::string> map;
    {
        bool first = true;
        for (const auto c: input) {
            if (c == '.' || c == '#') {
                if(first) {
                    map.emplace_back();
                    first = false;
                }
                map.back().push_back(c);
            } else if(c == '\n') {
                first = true;
            }
        }

    }
    //for(auto & r : map) std::cout << r << '\n';

    bool nm = false;
    for(int i = 0; i < 10 || !nm; i++) {
        {// expand
            decltype(map) temp;
            temp.emplace_back(std::string(2 + map[0].size(), '.'));
            for (auto &r: map) temp.emplace_back('.' + r + '.');
            temp.emplace_back(std::string(2 + map[0].size(), '.'));
            map.swap(temp);
        }
        std::vector<std::string> props(map.size(), std::string(map[0].size(), '.'));

        for(int y = 1; y < map.size()-1; y++) {
            for(int x = 1; x < map[y].size()-1; x++) {
                if(map[y][x] == '#') {
                    int n = 0, s = 0, w = 0, e = 0;
                    for (int dy = -1; dy <= 1; dy++) {
                        for (int dx = -1; dx <= 1; dx++) {
                            if (dx == 0 && dy == 0) continue;
                            if (map[y + dy][x+dx] != '.') {
                                if(dy == -1) n++;
                                if(dy == 1) s++;
                                if(dx == -1) w++;
                                if(dx == 1) e++;
                            }
                        }
                    }
                    if(n == 0 && s == 0 && w == 0 && e == 0) {
                        // stay put
                    } else {
                        for(int k = 0; k < 4; k++) {
                            int d = (i+k) % 4;

                            if(d == 0 && n == 0) {
                                if(props[y-1][x] == '.') {
                                    props[y-1][x] = '1';
                                } else {
                                    props[y-1][x] = 'X';
                                }
                                map[y][x] = 'N';
                                break;

                            }
                            if (d == 1 && s == 0) {
                                if(props[y+1][x] == '.') {
                                    props[y+1][x] = '1';
                                } else {
                                    props[y+1][x] = 'X';
                                }
                                map[y][x] = 'S';
                                break;

                            }
                            if(d == 2 && w == 0) {
                                if(props[y][x-1] == '.') {
                                    props[y][x-1] = '1';
                                } else {
                                    props[y][x-1] = 'X';
                                }
                                map[y][x] = 'W';
                                break;

                            }
                            if(d == 3 && e == 0) {
                                if(props[y][x+1] == '.') {
                                    props[y][x+1] = '1';
                                } else {
                                    props[y][x+1] = 'X';
                                }
                                map[y][x] = 'E';
                                break;
                            }
                        }

                    }
                }
            }
        }

        nm = true;
        for(int y = 0; y < map.size(); y++) {
            for (int x = 0; x < map[y].size(); x++) {
                if (map[y][x] == 'N') {
                    if(props[y-1][x] == '1') {
                        map[y-1][x] = '#';
                        map[y][x] = '.';
                        nm = false;
                    } else map[y][x] = '#';
                } else if (map[y][x] == 'S') {
                    if(props[y+1][x] == '1') {
                        map[y+1][x] = '#';
                        map[y][x] = '.';
                        nm = false;
                    } else map[y][x] = '#';
                } else if (map[y][x] == 'W') {
                    if(props[y][x-1] == '1') {
                        map[y][x-1] = '#';
                        map[y][x] = '.';
                        nm = false;
                    } else map[y][x] = '#';
                } else if (map[y][x] == 'E') {
                    if(props[y][x+1] == '1') {
                        map[y][x+1] = '#';
                        map[y][x] = '.';
                        nm = false;
                    } else map[y][x] = '#';
                } else {}
            }
        }
        if(nm) {
            ans2 = i+1;
        }

        {// shrink
            while(map.back().find('#') == std::string::npos) map.pop_back();
            while(map.front().find('#') == std::string::npos) map.erase(map.begin());

            auto trimstart = map[0].find('#');
            auto trimend = map[0].rfind('#');
            for(auto & r : map) {
                auto tmp = r.find('#');
                if(tmp != std::string::npos) {
                    trimstart = std::min(trimstart, tmp);
                    trimend = std::max(trimend, r.rfind('#'));
                }
            }
            for(auto & r : map) r = r.substr(trimstart, trimend-trimstart+1);
        }
        if(i+1 == 10) {
            for(auto & r : map) for(auto c : r) if(c == '.') ans1++;
        }
    }

    return {std::to_string(ans1), std::to_string(ans2)};
}
