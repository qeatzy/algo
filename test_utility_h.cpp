#include "utility.h"
// 1. test case for utility.h
// 2. some candidate for utility.h

namespace test {
    void algo() {
        std::vector<int> vec {1,1,3,3,3,1,1,1,8,3,3,9,9,9};
        std::sort(vec.begin(), vec.end());
        print(vec,"before unique");
        std::unique(vec.begin(), vec.end());
        print(vec," after unique");
    }
    void adjacent_find() {
        std::vector<int> vec {1,1,3,3,3,1,1,1,8,3,3,9,9,9};
        print(vec,"vec");
        auto mid = std::adjacent_find(vec.begin(), vec.end(), [](decltype(vec[0]) lhs, decltype(vec[0]) rhs) { return lhs > rhs; });
        print(vec.begin(), mid,"adjacent_find");
    }
}

namespace test {
    void makeSet() {
        DEBUG = 2;
        // auto s = makeSet("abdcccef");
        auto s = ::makeSet(string("abdcccef"));
        print(s);
        auto s2 = ::makeSet(string("abef"));
        cout << ((issubset(s2,s))? "includes" : "not includes") << endl;
        auto sint = ::makeSet({2,3,5,8});
        print(sint);
    }
}

namespace test {
    void makeVec() {
        auto v1 = ::makeVec(range(5),[](int x) { return x*x; });
        print(v1,"v1");
        auto v2 = ::makeVec({3,5});
        print(v2,"v2, from initializer_list");
    }
}

namespace test {
    void uCounter() {
        std::vector<std::vector<int>> vec;
        vec.push_back({1,3,9,2,3,5,2,1,2});
        for (auto &v: vec) {
            auto res = ::uCounter(v);
            print(v,"v");
            print(res, "::uCounter(v)");
            // print(res.begin(), res.end(), "::uCounter(v)");
        }
        auto tmp = ::uCounter(string("aabdecef"));
        print(tmp,"tmp");
        auto tmpint = ::uCounter({1,2,2,2,3,4,5,5,5,5,5});
        print(tmpint,"tmpint");
    }
}

namespace test {
    void vec_operator_mul() {
        auto v = range(3);
        auto m = v * 4;
        print(m,"multiple of v");
    }
    void vec_operator_add() {
        auto v = range(3);
        auto res = v + v;
        print(res,"v + v");
    }
}

namespace test {
    void Rank() {
        std::vector<std::vector<int>> vec;
        vec.push_back({2, 4, 1, 3, 5});
        for (auto &v: vec) {
            auto res = ::Rank(v);
            print(res,"the rank is");
        }
    }
}

namespace test {
    void trimAll() {
        std::vector<std::string> vec;
        vec.push_back("  123   abcd  ef  ");
        vec.push_back("the sky is blue");
        vec.push_back(" ");
        vec.push_back("1 ");
        vec.push_back(" 1");
        vec.push_back("   a   b ");
        vec.push_back("   a   ");
        for (auto &s: vec) {
            cout << "before trim, s = [" << s << "]" << endl;
            ::trimAll(s, ' ');
            cout << " after trim, s = [" << s << "]" << endl;
        }
    }
}

int main() {
    test::init();
    // test::print();
    // test::makeSet();
    // test::makeVec();
    // test::uCounter();
    // test::vec_operator_mul();
    test::vec_operator_add();
    // test::Rank();
    // test::trimAll();
}
