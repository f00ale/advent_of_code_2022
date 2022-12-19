#include "aoc.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <tuple>
#include <set>
#include <deque>

std::tuple<std::string, std::string> p19(const std::string &input) {
    int64_t ans1 = 0;
    int64_t ans2 = 0;
    std::vector<std::tuple<int, int, int, int, int, int>> v;
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
                if (havenum) {
                    tmp.push_back(num);
                    if (tmp.size() == 7) {
                        v.emplace_back(tmp[1], tmp[2], tmp[3], tmp[4], tmp[5], tmp[6]);
                        tmp.clear();
                    }
                }
                havenum = false;
                num = 0;
            }
        }
    }

    ans2 = 1;
    for(auto p : {1,2}) {
        int id = 0;
        std::set<std::tuple<int, int, int, int, int, int, int, int, int>> seen;
        std::deque<std::tuple<int, int, int, int, int, int, int, int, int>> q; // step, {ore, clay, obs, geo}{'', machine}
        for (auto [cost_ore, cost_clay, cost_obs_o, cost_obs_c, cost_geo_o, cost_geo_obs]: v) {
            q.clear();
            q = {{0, 0, 1, 0, 0, 0, 0, 0, 0}};
            seen.clear();
            int qual = 0;


            int maxoreneeded = std::max(cost_ore, std::max(cost_clay, std::max(cost_obs_o, cost_geo_o)));

            const int STEPS = (p == 1 ? 24 : 32);

            while (!q.empty()) {
                if (seen.count(q.front())) {
                    q.pop_front();
                    continue;
                }
                seen.insert(q.front());
                auto [step, ore, orem, clay, claym, obs, obsm, geo, geom] = q.front();
                q.pop_front();
                if (step >= STEPS) {
                    if (geo + geom > qual) qual = geo + geom;
                    continue;
                }
                if (cost_geo_o < ore && cost_geo_obs <= obs) {
                    auto t = std::make_tuple(step + 1, ore - cost_obs_o + orem, orem, clay + claym, claym,
                                             obs - cost_geo_obs + obsm, obsm, geo + geom, geom + 1);
                    if (!seen.count(t)) q.push_back(t);
                } else {
                    bool b = false;
                    if (orem <= maxoreneeded && cost_ore < ore) {
                        auto t = std::make_tuple(step + 1, ore - cost_ore + orem, orem + 1, clay + claym, claym,
                                                 obs + obsm,
                                                 obsm, geo + geom, geom);
                        if (!seen.count(t)) q.push_back(t);
                        b = true;
                    }
                    if (cost_clay < ore) {
                        auto t = std::make_tuple(step + 1, ore - cost_clay + orem, orem, clay + claym, claym + 1,
                                                 obs + obsm,
                                                 obsm, geo + geom, geom);
                        if (!seen.count(t)) q.push_back(t);
                        b = true;
                    }
                    if (cost_obs_o < ore && cost_obs_c <= clay) {
                        auto t = std::make_tuple(step + 1, ore - cost_obs_o + orem, orem, clay - cost_obs_c + claym,
                                                 claym,
                                                 obs + obsm, obsm + 1, geo + geom, geom);
                        if (!seen.count(t)) q.push_back(t);
                        b = true;
                    }
                    if (!b) {
                        q.emplace_back(step + 1, ore + orem, orem, clay + claym, claym, obs + obsm, obsm, geo + geom,
                                       geom);
                    }
                }
            }
            id++;
            //std::cout << id << ' ' << qual << '\n';
            if(p == 1) {
                ans1 += id * qual;
            } else {
                ans2 *= qual;
                if(id >= 3) break;
            }
        }
    }

    return {std::to_string(ans1), std::to_string(ans2)};
}
