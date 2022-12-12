#include "aoc.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <tuple>
#include <set>
#include <deque>

std::tuple<std::string, std::string> p12(const std::string &input) {
    int ans1 = 0;
    int ans2 = 0;

    std::vector<std::vector<int>> v;

    int startx = 0, starty = 0;
    int endx = 0, endy = 0;

    {

        bool nl = true;
        int x = 0, y = 0;
        for (const auto c: input) {
            if ((c >= 'a' && c <= 'z') || c == 'S' || c == 'E') {
                if(nl) v.emplace_back();
                if(c == 'S') {
                    startx = x;
                    starty = y;
                    v.back().push_back(0);
                } else if(c == 'E') {
                    endx = x;
                    endy = y;
                    v.back().push_back('z'-'a');
                } else {
                    v.back().push_back(c-'a');
                }

                nl = false;
                x++;
            } else if (c == '\n'){
                nl = true;
                x=0;
                y++;

            }
        }

    }

    std::deque<std::tuple<int, int, int>> q{{endx, endy, 0}};
    std::vector<std::vector<char>> visited(v.size(), std::vector<char>(v[0].size(), 0));
    visited[endy][endx] = 1;
    auto go = [&q, &visited, &v](int curr, int nx, int ny, int s) {
        if (nx >= 0 && nx < static_cast<int>(v[0].size()) && ny >= 0 && ny < static_cast<int>(v.size())) {
            if (!visited[ny][nx]) {
                if (v[ny][nx] >= curr - 1) {
                    visited[ny][nx] = 1;
                    q.emplace_back(nx, ny, s);
                }
            }
        }
    };

    while (!q.empty()) {
        auto [x, y, s] = q.front();
        q.pop_front();
        auto curr = v[y][x];
        if(curr == 0 && !ans2) {
            ans2 = s;
        }
        if (x == startx && y == starty) {
            ans1 = s;
            break;
        }

        go(curr, x + 1, y, s + 1);
        go(curr, x - 1, y, s + 1);
        go(curr, x, y + 1, s + 1);
        go(curr, x, y - 1, s + 1);
    }

    return {std::to_string(ans1), std::to_string(ans2)};
}
