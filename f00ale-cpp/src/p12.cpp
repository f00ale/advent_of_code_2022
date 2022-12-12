#include "aoc.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <tuple>
#include <set>
#include <deque>

std::tuple<std::string, std::string> p12(const std::string &input) {
    int64_t ans1 = 0;
    int64_t ans2 = 0;

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

    for(auto p : {1,2}){
        if(p == 2) {
            startx = endx;
            starty = endy;
        }

        std::deque<std::tuple<int, int, int>> q{{startx, starty, 0}};
        std::set<std::tuple<int, int>> visited{{startx, starty}};
        auto go = [p, &q, &visited, &v](int curr, int nx, int ny, int s) {
            if (nx >= 0 && nx < static_cast<int>(v[0].size()) && ny >= 0 && ny < static_cast<int>(v.size())) {
                if (visited.find({nx, ny}) == visited.end()) {
                    if ((p == 1 && v[ny][nx] <= curr + 1) || (p == 2 && v[ny][nx] >= curr - 1)) {
                        visited.insert({nx, ny});
                        q.emplace_back(nx, ny, s);
                    }
                }
            }
        };

        while (!q.empty()) {
            auto [x, y, s] = q.front();
            q.pop_front();
            auto curr = v[y][x];
            if (p == 1 && x == endx && y == endy) {
                ans1 = s;
                break;
            } else if(p == 2 && curr == 0) {
                ans2 = s;
                break;
            }

            go(curr, x + 1, y, s + 1);
            go(curr, x - 1, y, s + 1);
            go(curr, x, y + 1, s + 1);
            go(curr, x, y - 1, s + 1);

        }
    }

    return {std::to_string(ans1), std::to_string(ans2)};
}
