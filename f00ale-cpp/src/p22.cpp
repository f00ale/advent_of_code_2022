#include "aoc.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <tuple>
#include <set>

std::tuple<std::string, std::string> p22(const std::string &input) {
    // NB! contains hardcodes stuff for my input!
    int64_t ans1 = 0;
    int64_t ans2 = 0;
    std::vector<std::string> map;
    std::vector<int> steps;
    std::vector<char> turns;
    {
        int num = 0;
        bool havenum = false;
        bool mapmode = true;
        char last = '\n';
        for (const auto c: input) {
            if (mapmode) {
                if (c == '\n') {
                    if (last == '\n') mapmode = false;
                } else {
                    if (last == '\n') map.emplace_back();
                    map.back().push_back(c);
                }
                last = c;
            } else {
                if (c >= '0' && c <= '9') {
                    num *= 10;
                    num += c - '0';
                    havenum = true;
                } else {
                    if (havenum) {
                        steps.push_back(num);
                    }
                    if (c == 'L' || c == 'R') turns.push_back(c);
                    havenum = false;
                    num = 0;
                }
            }
        }

    }

    {
        int maxlen = 0;
        for (auto &r: map) if (r.size() > maxlen) maxlen = r.size();
        for (auto &r: map) if (r.size() < maxlen) r.append(maxlen - r.size(), ' ');
    }

    const std::vector<std::tuple<int, int>> dirmap{
            {1,  0},
            {0,  1},
            {-1, 0},
            {0,  -1}
    };

    const int SIZE = map.size() < 20 ? 4 : 50;
    std::vector<std::vector<std::string>> blocks;
    std::vector<std::tuple<int, int>> blockoffsets;

    std::vector<std::vector<int>> blockmap;
    int cnt = 0;
    for (int y = 0; y < map.size(); y += SIZE) {
        blockmap.emplace_back();
        for (int x = 0; x < map[y].size(); x += SIZE) {
            if (map[y][x] == ' ') blockmap.back().push_back(-1);
            else {
                blockmap.back().push_back(cnt);
                blockoffsets.emplace_back(x, y);
                blocks.emplace_back();
                for (int ty = y; ty < y + SIZE; ty++) {
                    blocks.back().push_back(map[ty].substr(x, SIZE));
                }
                cnt++;
            }
        }
    }
/*
    for (auto &r: blockmap) {
        for (auto i: r) {
            if (i >= 0) std::cout << i;
            else std::cout << ' ';
        }
        std::cout << '\n';
    }
*/
    std::vector<std::vector<std::tuple<int,char>>> nmap1(6, std::vector<std::tuple<int,char>>(4, std::make_tuple(-1, 0)));
    for (int y = 0; y < blockmap.size(); y++) {
        for (int x = 0; x < blockmap[y].size(); x++) {
            int b = blockmap[y][x];
            if (b >= 0) {
                for (int d = 0; d < dirmap.size(); d++) {
                    const auto [dx, dy] = dirmap[d];
                    auto tx = x;
                    auto ty = y;
                    bool found = false;
                    while (!found) {
                        tx += dx;
                        ty += dy;
                        if (ty < 0) ty = blockmap.size() - 1;
                        else if (ty >= blockmap.size()) ty = 0;
                        if (tx < 0) tx = blockmap[ty].size() - 1;
                        else if (tx >= blockmap[ty].size()) tx = 0;
                        if (blockmap[ty][tx] != -1) {
                            found = true;
                            nmap1[b][d] = {blockmap[ty][tx], *("lurd"+d)};
                        }
                    }

                }
            }
        }
    }
    std::vector<std::vector<std::tuple<int,char>>> nmap2(6, std::vector<std::tuple<int,char>>(4, std::make_tuple(-1, 0)));

    if(SIZE==4) {
        nmap2 = {
                {{5, 'r'}, {3, 'u'}, {2, 'u'}, {1, 'u'}}, // 0
                {{2, 'l'}, {4, 'd'}, {5, 'd'}, {0, 'u'}}, // 1
                {{3, 'l'}, {4, 'l'}, {1, 'r'}, {0, 'l'}}, // 2
                {{5, 'u'}, {4, 'u'}, {2, 'r'}, {0, 'd'}}, // 3
                {{5, 'l'}, {1, 'd'}, {2, 'd'}, {3, 'd'}}, // 4
                {{0, 'r'}, {1, 'l'}, {4, 'r'}, {3, 'r'}}, // 5
        };
    } else {
        nmap2 = {
                {{1, 'l'}, {2, 'u'}, {3, 'l'}, {5, 'l'}}, // 0
                {{4, 'r'}, {2, 'r'}, {0, 'r'}, {5, 'd'}}, // 1
                {{1, 'd'}, {4, 'u'}, {3, 'u'}, {0, 'd'}}, // 2
                {{4, 'l'}, {5, 'u'}, {0, 'l'}, {2, 'l'}}, // 3
                {{1, 'r'}, {5, 'r'}, {3, 'r'}, {2, 'd'}}, // 4
                {{4, 'd'}, {1, 'u'}, {0, 'u'}, {3, 'd'}}, // 5
        };
    }

    for(auto p : {1,2}) {
        auto draw = map;
        auto dump = [&]() {
            for(size_t r = 0; r < draw.size(); r++) {
                if (r && r % SIZE == 0) {
                    for (size_t c = 0; c < draw[r].size(); c += SIZE) {
                        std::cout << std::string(SIZE, '-');
                        std::cout << '+';
                    }
                    std::cout << '\n';
                }
                for (size_t c = 0; c < draw[r].size(); c += SIZE) {
                    std::cout << draw[r].substr(c, SIZE);
                    std::cout << '|';
                }
                std::cout << '\n';
            }
        };
        bool shouldabort = false;
        auto err = [&](int dir, char c) {
            std::cout << "ERR " << dir << c << '\n';
            shouldabort = true;
            dump();
        };
        int y = 0;
        int x = 0;
        int dir = 0;
        int b = 0;

        auto & nmap = (p == 1 ? nmap1 : nmap2);

        for (int i = 0; i < steps.size(); i++) {
            auto s = steps[i];
            //if(p==2) std::cout << 'S' << s << ' ' << dir << ' ' << b << '\n';

            while (s--) {
                {
                    auto [ox, oy] = blockoffsets[b];
                    draw[y+oy][x+ox] = *(">v<^"+dir);
                }
                int nx = x, ny = y, nb = b, nd = dir;
                if(dir == 0) {// ->
                    if(x+1 >= SIZE) {
                        auto [nnb, k] = nmap[b][dir];
                        nb = nnb;
                        switch(k) {
                            case 'l': nx = 0; break;
                            case 'd': nd = 3; ny = SIZE - 1; nx = y; break;
                            case 'r': nd = 2; ny = SIZE - 1 - y; nx = SIZE - 1; break;
                            case 'u': nd = 1; nx = SIZE - 1 - y; ny = 0; break;
                            default: std::cout << "error: " << k << '(' << +k << ')' << '\n';
                        }
                    } else {
                        nx = x + 1;
                    }
                } else if(dir == 1) { // v
                    if(y+1 >= SIZE) {
                        auto [nnb, k] = nmap[b][dir];
                        nb = nnb;
                        switch (k) {
                            case 'l': err(dir,k); break;
                            case 'd': nd = 3; nx = SIZE-1-x; break;
                            case 'r': nd = 2; nx = SIZE-1; ny = x; break;
                            case 'u': ny = 0; break;
                        }
                    } else {
                        ny = y+1;
                    }
                } else if(dir == 2) { // <
                    if(x - 1 < 0) {
                        auto [nnb, k] = nmap[b][dir];
                        nb = nnb;
                        switch(k) {
                            case 'l': nd = 0; ny = SIZE - 1 - y; break;
                            case 'd': err(dir,k); break;
                            case 'r': nx = SIZE - 1; break;
                            case 'u': nd = 1; ny = 0; nx = y; break;
                            default: std::cout << "error: " << k << '(' << +k << ')' << '\n';
                        }
                    }
                    else {
                        nx = x - 1;
                    }
                } else if(dir == 3) { // ^
                    if(y - 1 < 0) {
                        auto [nnb, k] = nmap[b][dir];
                        nb = nnb;
                        switch(k) {
                            case 'l': nd = 0; nx = 0; ny = x; break;
                            case 'd': ny = SIZE - 1; break;
                            case 'r': err(dir,k); break;
                            case 'u': nx = SIZE - 1 - x; nd = 1; break;
                            default: std::cout << "error: " << k << '(' << +k << ')' << '\n';
                        }
                    }
                    else {
                        ny = y - 1;
                    }
                } else {
                    std::cout << "dir OOB\n";
                }
                if(shouldabort) break;

                if (blocks[nb][ny][nx] == '.') {
                    b = nb;
                    y = ny;
                    x = nx;
                    dir = nd;
                    auto [ox, oy] = blockoffsets[nb];
                    draw[y+oy][x+ox] = *("}U{A"+dir);
                    //std::cout << 'm' << ny+oy << ' ' << nx+ox << '\n';
                } else {
                    auto [ox, oy] = blockoffsets[nb];
                    //std::cout << 'B' << ny+oy << ' ' << nx+ox << '\n';
                    break;
                }
            }

            if(shouldabort) break;
            //dump();
            int dbg = 100;
            if(false && p == 2 && i > dbg-5) {
                std::cout << "DUMP " << i << '\n';
                dump();
                //if(i > dbg) break;
            }

            if (i < turns.size()) {
                //if(p == 2) std::cout << 'r' << turns[i] << '\n';
                if (turns[i] == 'R') dir = (dir + 1) % 4;
                else dir--;
                if (dir < 0) dir = 3;
            }


        }
        if(shouldabort) break;

        auto [ox, oy] = blockoffsets[b];
        (p==1 ? ans1 : ans2) = 1000 * (y + oy + 1) + 4 * (x + ox + 1) + dir;
        //if(p == 2) dump();

    }
    return {std::to_string(ans1), std::to_string(ans2)};
}
