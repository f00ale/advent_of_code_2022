#include "aoc.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <tuple>
#include <set>
#include <deque>

std::tuple<std::string, std::string> p18(const std::string &input) {
    int64_t ans1 = 0;
    int64_t ans2 = 0;

    std::vector<std::tuple<int,int,int>> v;

    {
        int num = 0;
        bool havenum = false;
        std::vector<int> tmp;
        for (const auto c: input) {
            if (c >= '0' && c <= '9') {
                num *= 10;
                num += c - '0';
                havenum = true;
            } else {
                if(havenum) {
                    tmp.push_back(num);
                    if(tmp.size() == 3) {
                        v.emplace_back(tmp[0],tmp[1],tmp[2]);
                        tmp.clear();
                    }
                }
                havenum = false;
                num = 0;
            }
        }

    }

    std::tuple<int,int,int> maxes{0,0,0};
    auto & [mx,my,mz] = maxes;
    for(auto [x,y,z] : v) {
        if(x > mx) mx = x;
        if(y > my) my = y;
        if(z > mz) mz = z;
    }
    int m = std::max(mx,std::max(my,mz));

    std::vector<std::vector<std::string>> cube(mx+3,std::vector<std::string>(my+3,std::string(mz+3,'.')));
    for(auto [x,y,z] : v) {
        cube[x+1][y+1][z+1] = '#';
    }

    std::deque<std::tuple<int,int,int>> q{{0,0,0}};
    while(!q.empty()) {
        auto [x,y,z] = q.front();
        q.pop_front();

        if(x < 0 || y < 0 || z < 0) continue;
        if(x >= cube.size() || y >= cube[x].size() || z >= cube[x][y].size()) continue;

        if(cube[x][y][z] == '.') {
            cube[x][y][z] = '-';
            q.emplace_back(x+1, y, z);
            q.emplace_back(x-1, y, z);
            q.emplace_back(x, y+1, z);
            q.emplace_back(x, y-1, z);
            q.emplace_back(x, y, z+1);
            q.emplace_back(x, y, z-1);
        }
    }

    for(int x = 1; x < cube.size()-1; x++) {
        for(int y = 1; y < cube[x].size()-1; y++) {
            for(int z = 1; z < cube[x][y].size()-1; z++) {
                if(cube[x][y][z] == '#') {
                    if(cube[x][y][z+1] != '#') ans1++;
                    if(cube[x][y][z-1] != '#') ans1++;
                    if(cube[x][y+1][z] != '#') ans1++;
                    if(cube[x][y-1][z] != '#') ans1++;
                    if(cube[x+1][y][z] != '#') ans1++;
                    if(cube[x-1][y][z] != '#') ans1++;

                    if(cube[x][y][z+1] == '-') ans2++;
                    if(cube[x][y][z-1] == '-') ans2++;
                    if(cube[x][y+1][z] == '-') ans2++;
                    if(cube[x][y-1][z] == '-') ans2++;
                    if(cube[x+1][y][z] == '-') ans2++;
                    if(cube[x-1][y][z] == '-') ans2++;
                }
            }
        }
    }


    return {std::to_string(ans1), std::to_string(ans2)};
}
