#include "aoc.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <tuple>
#include <set>

std::tuple<std::string, std::string> p14(const std::string &input) {
    int64_t ans1 = 0;
    int64_t ans2 = 0;
    std::vector<std::vector<std::tuple<int,int>>> v;

    {
        int num = 0;
        bool havenum = false;
        bool havex = false;
        int x = 0, y = 0;
        std::vector<std::tuple<int,int>> current;
        for (const auto c: input) {
            if (c >= '0' && c <= '9') {
                num *= 10;
                num += c - '0';
                havenum = true;
            } else {
                if(havenum) {
                    if(c == ',') {
                        x = num;
                        havex = true;
                    } else if(havex) {
                        current.emplace_back(x,num);
                        havex = false;
                    }
                }
                if(c == '\n' && !current.empty()) {
                    v.emplace_back(std::move(current));
                    current.clear();
                }
                havenum = false;
                num = 0;
            }
        }

    }

    int minx = 500, maxx = 500, miny = 0, maxy = 0;
    for(auto && r : v) {
        for(auto [x,y]:  r) {
            if(x < minx) minx = x;
            if(x > maxx) maxx = x;
            if(y < miny) miny = y;
            if(y > maxy) maxy = y;
        }
    }

    std::vector<std::string> map(maxy+1, std::string(maxx-minx+1, '.'));
    for(auto && r : v) {
        for(int idx = 0; idx < r.size()-1; idx++) {
            auto [sx,sy] = r[idx];
            auto [ex,ey] = r[idx+1];
            if(sx == ex) {
                // vertical
                for(auto y = std::min(sy,ey); y <= std::max(sy,ey); y++) {
                    map[y-miny][sx-minx] = '#';
                }
            } else if(sy == ey) {
                //horisontal
                for(auto x = std::min(sx,ex); x <= std::max(sx,ex); x++) {
                    map[sy-miny][x-minx] = '#';
                }

            }
        }
    }

    auto bkup = map;
    for(ans1 = 0; ; ans1++) {
        int x = 500-minx, y = 0 - miny;


        bool rest = false, offscreen = false;
        while(!rest && !offscreen) {
            /*
            auto c = map[y][x];
            map[y][x] = 'o';
            for(auto && r : map) std::cout << r << '\n';
            map[y][x] = c;
*/
          if(y+1 >= map.size()) {
              offscreen = true;
              break;
          }
          if(map[y+1][x] == '.') { y++; continue;}
          if(x-1<0) {
              offscreen = true; break; }
          if(map[y+1][x-1] == '.') {
              y++; x--;continue;}
          if(x+1>=map[y+1].size()) {
              offscreen=true; break;}
          if(map[y+1][x+1] == '.') {y++; x++;
              continue;}
          break;
        }
        if(!offscreen) {
            map[y][x] = 'o';
        }
        //for(auto && r : map) std::cout << r << '\n';
        if(offscreen) break;
    }

    map = bkup;
    bkup.push_back(std::string(map[0].size(),'.'));

    map.clear();
    int add = 250;
    for(auto && r : bkup)
    {
        map.push_back(std::string(add,'.') + r + std::string(add,'.'));
    }
    minx-=add;maxx+=add;maxy+=1;
    for(ans2 = 1; ; ans2++) {
        int x = 500-minx, y = 0 - miny;

        //std::cout << y << ' ' << x << '\n';

        bool rest = false, offscreen = false;
        while(!rest && !offscreen) {
            if(y+1 >= map.size()) {
                map[y][x] = 'o';
                offscreen = true;
                break;
            }
            if(map[y+1][x] == '.') { y++; continue;}
            if(x-1<0) {
                offscreen = true; break; }
            if(map[y+1][x-1] == '.') {
                y++; x--;continue;}
            if(x+1>=map[y+1].size()) {
                offscreen=true; break;}
            if(map[y+1][x+1] == '.') {y++; x++;
                continue;}
            break;
        }
        if(!offscreen) {
            map[y][x] = 'o';
        }
        if(x == 500-minx && y == 0) break;
    }

    return {std::to_string(ans1), std::to_string(ans2)};
}
