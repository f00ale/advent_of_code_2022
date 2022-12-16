#include "aoc.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <tuple>
#include <set>
#include <map>
#include <queue>
template<typename T> using V = std::vector<T>;

std::tuple<std::string, std::string> p16(const std::string &input) {
    int64_t ans1 = 0;
    int64_t ans2 = 0;

    std::map<std::string, std::tuple<int, bool, std::vector<std::string>, int>> map1;

    {
        int num = 0;
        std::string current;
        char last = 0;
        int idx = 0;
        for (const auto c: input) {
            if (c >= 'A' && c <= 'Z' && last >= 'A' && last <= 'Z') {
                if (!current.empty()) {
                    std::get<2>(map1[current]).push_back({last, c});
                } else {
                    current = {last, c};
                    std::get<3>(map1[current]) = idx;
                }
            } else if (c >= '0' && c <= '9') {
                num *= 10;
                num += c - '0';
                std::get<0>(map1[current]) = num;

            } else {
                num = 0;
                if (c == '\n') {
                    if (!current.empty()) idx++;
                    current.clear();
                }
            }
            last = c;
        }

    }

    std::vector<std::string> idx2name(map1.size());
    std::vector<int> interesting;
    std::vector<int> flows(map1.size());
    std::vector<std::vector<int>> adj(map1.size());
    for(auto &[k,v]:map1) {
        std::ignore = k;
        auto & [f,b,ve,idx] = v;
        std::ignore = f;
        std::ignore = b;
        for(auto & s : ve) {
            adj[idx].push_back(std::get<3>(map1[s]));
        }
    }

    for(auto &[k,v]:map1) {
        idx2name[std::get<3>(v)] = k;
        flows[std::get<3>(v)] = std::get<0>(v);
        if(std::get<0>(v) > 0) interesting.push_back(std::get<3>(v));
    }
    std::vector<std::vector<int>> paths1(map1.size(), std::vector<int>(map1.size(), -1));
    for(int from = 0; from < static_cast<int>(paths1.size()); from++)  {
        for(int to = 0; to < static_cast<int>(paths1[from].size()); to++) {
            if(from == to) {
                paths1[from][to] = 0;
                continue;
            }
            std::deque<std::tuple<int,int>> q{{0,from}}; // steps, current
            while(!q.empty()) {
                auto [s,c] = q.front();
                q.pop_front();
                if(c == to) {
                    paths1[to][from] = s;
                    break;
                }
                for(auto & n : std::get<2>(map1[idx2name[c]])) {
                    q.push_back({s+1,std::get<3>(map1[n])});
                }
            }
            //std::cout << idx2name[from] << " to " << idx2name[to] << '\n';
        }
    }

    auto calc = [&paths1,&flows](int start, std::vector<int> candidates, const int TSTEPS) {
        int ret = 0;

        //for(auto i : candidates) std::cout << i << ' '; std::cout << '\n';

        std::deque<std::tuple<int, int, int, uint64_t>> // current, steps, total, open
        q{{start, 0, 0, 0llu}};

        while (!q.empty()) {
            auto [curr, steps, total, open] = q.front();
            q.pop_front();
            if (steps >= TSTEPS) continue;
            if (total > ret) ret = total;

            for (auto n: candidates) {
                uint64_t mask = 1 << n;
                if (!(mask & open)) {
                    auto totake = paths1[curr][n];
                    q.emplace_back(n, steps + 1 + totake, total + (TSTEPS - steps - totake - 1) * flows[n],
                                   open | mask);
                }
            }
        }
        return ret;
    };

    auto start = std::get<3>(map1["AA"]);

    std::vector<int> i2i(map1.size(), -1);
    for(int i=0; i<static_cast<int>(interesting.size());i++) i2i[interesting[i]] = i;

    ans1 = calc(start, interesting, 30);

    const int MAXSTEPS = 26;
    std::deque<std::tuple<int,int,int,int,int,uint64_t>> // total, s1, p1, s2, p2, visited
    q{{0,0,start,0,start,1ull<<start}};
    std::map<uint64_t, int> best;
    while(!q.empty()) {
        auto [total, s1, p1, s2, p2, visited] = q.front();
        q.pop_front();
        if(s2 < s1) {
            std::swap(s1,s2);
            std::swap(p1,p2);
        }
        if(s1 >= MAXSTEPS) continue;
        for(auto next : interesting) {
            auto steps = paths1[p1][next];
            if(s1+steps > MAXSTEPS) {
                q.emplace_back(total, s2, p2, s1+steps, p1, visited);
            } else if(!(1llu<<next & visited)) {
                auto ns = s1 + steps + 1;
                auto nt = total + (MAXSTEPS-ns)*flows[next];
                auto nv = visited | (1llu<<next);
                if(nt > best[nv]) {
                    best[nv] = nt;
                    q.emplace_back(nt, ns, next, s2, p2, nv);
                }
            }
        }
    }

    for(auto [k,v] : best) {
        std::ignore = k;
        if(v>ans2) ans2 = v;
    }

    if(0) {
        std::vector<int> i1, i2;
        for (uint32_t iter = 0; iter < (1u << interesting.size()); iter++) {
            i1.clear();
            i2.clear();
            for (size_t i = 0; i < interesting.size(); i++) {
                if ((1u << i) & iter) { i1.push_back(interesting[i]); }
                else { i2.push_back(interesting[i]); }
            }

            //std::cout << '\n';
            auto r1 = calc(start, i1, 26);
            auto r2 = calc(start, i2, 26);
            //for(auto i : interesting) std::cout << i << ' ';
            //std::cout << "-> " << r1 << ' ' << r2 << '\n';
            auto r = r1 + r2;
            if (r > ans2) {
                std::cout << r1 << ' ' << r2 << ' ' << r << '\n';
                ans2 = r;
            }
        }
    }
    return {std::to_string(ans1), std::to_string(ans2)};
}
