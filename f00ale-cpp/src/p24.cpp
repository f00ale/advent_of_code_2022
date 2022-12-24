#include "aoc.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <tuple>
#include <set>
#include <deque>

std::tuple<std::string, std::string> p24(const std::string &input) {
    int64_t ans1 = 0;
    int64_t ans2 = 0;

    std::vector<std::tuple<int,int,char>> bliz; // row, col, dir
    int maxcol = 0;
    int maxrow = 0;
    {
        int row = 0, col = 0;
        for (const auto c: input) {
            if(c == '\n') {
                maxcol = std::max(maxcol, col);
                if(col > 0) {
                    row++;
                    maxrow = row;
                }
                col = 0;
            } else {
                if(c == '<' || c == '>' || c == '^' || c == 'v') {
                    bliz.emplace_back(row, col, c);
                }
                col++;
            }

        }
    }

    std::vector<std::string> screen(maxrow, std::string(maxcol, '.'));
    {
        for (int i = 0; i < maxcol; i++) {
            screen.front()[i] = '#';
            screen.back()[i] = '#';
        }
        for (int i = 0; i < maxrow; i++) {
            screen[i].front() = '#';
            screen[i].back() = '#';
        }
        screen[0][1] = '.';
        screen[maxrow - 1][maxcol - 2] = '.';
    }

    auto update = [&]() {
        for(int r = 1; r < screen.size()-1; r++) {
            for(int c = 1; c < screen[r].size()-1; c++) {
                screen[r][c] = '.';
            }
        }
        for(auto [r,c,d] : bliz) {
            if(screen[r][c] == '.')
                screen[r][c] = d;
            else {
                if(screen[r][c] == '<' || screen[r][c] == '>' || screen[r][c] == '^' || screen[r][c] == 'v') screen[r][c]='2';
                else screen[r][c]++;
            }
        }
    };

    auto movebliz = [&] {
        for(auto & [r,c,d] : bliz) {
            switch(d) {
                case '<': c--; if(c == 0) c = maxcol-2; break;
                case '>': c++; if(c==maxcol-1) c = 1;   break;
                case '^': r--; if(r==0) r = maxrow-2; break;
                case 'v': r++; if(r==maxrow-1) r = 1; break;
            }
        }
        update();
    };

    std::deque<std::tuple<int,int,int>> q{{0,0,1}}; // step, row, col
    std::set<std::tuple<int,int,int>> visited;
    update();
    int ls = -1;
    int gr = maxrow-1;
    int gc = maxcol-2;
    int goals = 0;
    while(!q.empty()) {
        auto [s,r,c] = q.front();
        bool seen = visited.count(q.front()) != 0;
        if(!seen) visited.insert(q.front());
        q.pop_front();
        if(seen) continue;
        if(r == gr && c == gc) {
            goals++;
            if(goals == 1) {
                ans1=s;
                gr = 0; gc = 1;
                q.clear();
                q.emplace_back(s,r,c);
                visited.clear();
            }
            if(goals == 2) {
                gr = maxrow-1; gc = maxcol-2;
                q.clear();
                q.emplace_back(s,r,c);
                visited.clear();
            }
            if(goals == 3) {
                ans2 = s;
                break;
            }
            continue;
        }
        if(s != ls) {
            movebliz();
            ls = s;
        }
        if(screen[r][c] == '.') q.emplace_back(s+1,r,c);
        if(r > 0 && screen[r-1][c] == '.') q.emplace_back(s+1,r-1,c);
        if(r < screen.size()-1 && screen[r+1][c] == '.') q.emplace_back(s+1,r+1,c);
        if(c > 0 && screen[r][c-1] == '.') q.emplace_back(s+1,r,c-1);
        if(c < screen[r].size()-1 && screen[r][c+1] == '.') q.emplace_back(s+1,r,c+1);
    }

    return {std::to_string(ans1), std::to_string(ans2)};
}
