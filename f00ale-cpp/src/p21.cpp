#include "aoc.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <tuple>
#include <set>
#include <map>

static const std::string humn = "humn";

bool has_child(const std::string & c, const std::string & n, const std::map<std::string, std::tuple<std::string,std::string,char>> & funcs)
{
    if(c == n) return true;
    auto it = funcs.find(c);
    if(it == funcs.end()) return false;
    auto [c1,c2,op] = it->second;
    return has_child(c1, n, funcs) || has_child(c2, n, funcs);
};


int64_t getnum(const std::string & name, std::map<std::string, int64_t> & monkey_num,
               const std::map<std::string, std::tuple<std::string,std::string,char>> & monkey_func)
{
    {
        auto it = monkey_num.find(name);
        if (it != monkey_num.end()) return it->second;
    }
    auto it = monkey_func.find(name);
    if(it == monkey_func.end()) {
        std::cout << "Error" << '\n';
        return -1;
    }

    auto & [m1,m2,op] = it->second;
    auto n1 = getnum(m1, monkey_num, monkey_func);
    auto n2 = getnum(m2, monkey_num, monkey_func);
    int64_t ret = [](char o, int64_t n1, int64_t n2) {
        switch(o) {
            case '+': return n1+n2;
            case '-': return n1-n2;
            case '*': return n1*n2;
            case '/': return n1/n2;
            default: std::cout << 'unk ' << o << '\n'; return -1ll;
        }}(op, n1, n2);
    monkey_num[name] = ret;
    return ret;
}

int64_t solve2(int64_t lhs, const std::string & rhs, std::map<std::string, int64_t> & monkey_num,
               const std::map<std::string, std::tuple<std::string,std::string,char>> & monkey_func)
{
    //std::cout << "solve " << lhs << " = " << rhs << '\n';
    if(rhs == humn) return lhs;
    auto it = monkey_func.find(rhs);
    if(it == monkey_func.end()) {
        std::cout << rhs << " not found\n";
        return -1;
    }
    auto & [sleft,sright,op] = it->second;

    if(has_child(sleft, humn, monkey_func)) {
        //lhs = humn op num
        auto known = getnum(sright, monkey_num, monkey_func);
        switch(op) {
            case '+': lhs -= known; break;
            case '-': lhs += known; break;
            case '*': lhs /= known; break;
            case '/': lhs *= known; break;
        }
        return solve2(lhs, sleft, monkey_num, monkey_func);
    } else {
        auto known = getnum(sleft, monkey_num, monkey_func);
        // lhs = known op HUMN
        switch(op) {
            case '+': lhs -= known; break;
            case '-': lhs =  known - lhs ; break;
            case '*': lhs /= known; break;
            case '/': lhs = known / lhs; break;
        }
        return solve2(lhs, sright, monkey_num, monkey_func);
    }
}

std::tuple<std::string, std::string> p21(const std::string &input) {
    int64_t ans1 = 0;
    int64_t ans2 = 0;
    std::map<std::string, int64_t> monkey_num;
    std::map<std::string, std::tuple<std::string,std::string,char>> monkey_func;

    {
        int64_t num = 0;
        bool havenum = false;
        std::vector<std::string> names;
        char op = 0;
        for (const auto c: input) {
            if (c >= '0' && c <= '9') {
                num *= 10;
                num += c - '0';
                havenum = true;
            } else if (c >= 'a' && c <= 'z') {
                if(names.empty() || names.back().size() == 4) {
                    names.emplace_back();
                }
                names.back().push_back(c);
            } else if(c == '+' || c == '-' || c == '*' || c == '/') {
                op = c;
            } else if(c == '\n') {
                if(havenum && names.size() == 1) {
                    monkey_num[names[0]] = num;
                } else if (names.size() == 3 && op != 0) {
                    monkey_func[names[0]] = {names[1], names[2], op};
                }
                havenum = false;
                num = 0;
                names.clear();
                op = 0;
            }
        }

    }

    ans1 = getnum("root", monkey_num, monkey_func);

    {
        auto & [m1,m2,op] = monkey_func["root"];
        auto target = [&](const std::string & m1, const std::string & m2){
            if(has_child(m1,humn,monkey_func)) return getnum(m2,monkey_num, monkey_func);
            else return getnum(m2,monkey_num, monkey_func);
        }(m1,m2);

        if(has_child(m1, humn, monkey_func)) {
            ans2 = solve2(target, m1, monkey_num, monkey_func);
        } else {
            ans2 = solve2(target, m2, monkey_num, monkey_func);
        }
    }

    return {std::to_string(ans1), std::to_string(ans2)};
}

