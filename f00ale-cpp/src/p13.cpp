#include "aoc.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <tuple>
#include <set>
#include <stack>

struct node {
    bool islist = true;
    std::vector<std::unique_ptr<node>> nodes;
    int num = 0;
};

void print(node * n) {
    if(n->islist) {
        std::cout << '[';
        bool first = true;
        for(auto & c : n->nodes) {
            if(!first) std::cout << ',';
            print(c.get());
            first = false;
        }
        std::cout << ']';
    } else {
        std::cout << n->num;
    }
}

int compare(node *l, node *r, std::string in = "") {
    int ret = 0;
    /*
    std::cout << in << "cmp ";
    print(l);
    std::cout << " vs ";
    print(r);
    std::cout << '\n';
*/
    if(!l->islist && !r->islist) {
        ret = (l->num == r->num ? 0 : (l->num < r->num ? -1 : 1));
    } else if(l->islist && r->islist) {
        for(size_t idx = 0; idx < l->nodes.size() && idx < r->nodes.size(); idx++) {
            auto tmp = compare(l->nodes[idx].get(), r->nodes[idx].get(), in+"  ");
            if(tmp == 0) continue;
            else {
                ret = tmp;
                break;
            }
        }
        if(!ret) {
            if (l->nodes.size() < r->nodes.size()) ret = -1;
            if (l->nodes.size() > r->nodes.size()) ret = 1;
        }
    } else if(l->islist && !r->islist) {
        node tmp;
        tmp.nodes.push_back(std::make_unique<node>());
        tmp.nodes.back()->islist = false;
        tmp.nodes.back()->num = r->num;
        ret = compare(l, &tmp, in+"  ");
    } else if(!l->islist && r->islist) {
        node tmp;
        tmp.nodes.push_back(std::make_unique<node>());
        tmp.nodes.back()->islist = false;
        tmp.nodes.back()->num = l->num;
        ret = compare(&tmp, r, in + "  ");
    }

    return ret;
}

std::tuple<std::string, std::string> p13(const std::string &input) {
    int64_t ans1 = 0;
    int64_t ans2 = 0;

    std::vector<std::unique_ptr<node>> nodes;

    {
        int num = 0;
        bool havenum = false;

        std::stack<node*> st;
        node* current = nullptr;
        for (const auto c: input) {
            if (c >= '0' && c <= '9') {
                num *= 10;
                num += c - '0';
                havenum = true;
            } else {
                if(havenum) {
                    current->nodes.push_back(std::make_unique<node>());
                    current->nodes.back()->islist = false;
                    current->nodes.back()->num = num;
                }
                if(c == '[') {
                    if(current == nullptr) {
                        nodes.push_back(std::make_unique<node>());
                        current = nodes.back().get();
                    } else {
                        current->nodes.push_back(std::make_unique<node>());
                        st.push(current);
                        current = current->nodes.back().get();
                    }
                } else if(c == ']') {
                    if(st.empty()) current = nullptr;
                    else {
                        current = st.top();
                        st.pop();
                    }
                }
                havenum = false;
                num = 0;

            }
        }

    }

    //std::cout << nodes.size() << '\n';


    for(size_t i = 0; i < nodes.size(); i+=2) {
        auto tmp = compare(nodes[i].get(), nodes[i+1].get());
        //std::cout << (i/2)+1 << ' ' << tmp << '\n';
        if(tmp < 0) ans1 += (i/2)+1;
    }

    {
        auto n1 = std::make_unique<node>();
        n1->nodes.push_back(std::make_unique<node>());
        n1->nodes.back()->nodes.push_back(std::make_unique<node>());
        n1->nodes.back()->nodes.back()->islist = false;
        n1->nodes.back()->nodes.back()->num = 2;
        auto n2 = std::make_unique<node>();
        n2->nodes.push_back(std::make_unique<node>());
        n2->nodes.back()->nodes.push_back(std::make_unique<node>());
        n2->nodes.back()->nodes.back()->islist = false;
        n2->nodes.back()->nodes.back()->num = 6;

        nodes.push_back(std::move(n1));
        nodes.push_back(std::move(n2));
    }
    std::sort(nodes.begin(), nodes.end(), [](auto & n1, auto & n2) { return compare(n1.get(), n2.get()) < 0;});
/*
    for(auto & n: nodes) {
        print(n.get());
        std::cout << '\n';
    }
*/
    auto n1 = std::make_unique<node>();
    n1->nodes.push_back(std::make_unique<node>());
    n1->nodes.back()->nodes.push_back(std::make_unique<node>());
    n1->nodes.back()->nodes.back()->islist = false;
    n1->nodes.back()->nodes.back()->num = 2;
    auto n2 = std::make_unique<node>();
    n2->nodes.push_back(std::make_unique<node>());
    n2->nodes.back()->nodes.push_back(std::make_unique<node>());
    n2->nodes.back()->nodes.back()->islist = false;
    n2->nodes.back()->nodes.back()->num = 6;

    for(size_t idx = 0; idx < nodes.size(); idx++) {
        if(!ans2 && compare(nodes[idx].get(), n1.get()) == 0) ans2 = idx+1;
        if(ans2 && compare(nodes[idx].get(), n2.get()) == 0) {
            ans2 *= idx+1;
            break;
        }
    }


    return {std::to_string(ans1), std::to_string(ans2)};
}
