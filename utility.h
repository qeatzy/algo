#ifndef UTILITY_H_INCLUDED   // include guard, http://www.cplusplus.com/forum/articles/10627/#msg49679
#define UTILITY_H_INCLUDED 
int DEBUG = 0, stack_level = 0;

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
// #include <deque>
#include <set>
#include <map>
#include <unordered_map>
#include <string>
#include <cstring>  // strlen, string literal of type const char [].
#include <algorithm>
#include <numeric>
#include <cmath>    // pow
// #include <valarray>
#include <array>
#include <iterator> // std::iterator_traits<Iterator>::value_type  difference_type  iterator_category  http://stackoverflow.com/a/1107235/3625404
#include <utility>
#include <stdexcept>
#include <cassert>  // assert
#include <climits>
#include <cstdlib>  // strtoul, strtoull, strtol
#include <cctype>   // isspace
// #include <ctime>
#include <chrono>
// #include <regex>
#include <memory>   // smart pointers

typedef std::vector<int> iVec;
typedef std::vector<long long> llVec;
typedef std::vector<double> dVec;
typedef std::vector<std::string> sVec;
typedef long long llong;

using std::cerr;
using std::cout;
using std::cin;
using std::ios;
using std::ios_base;
char endl = '\n';
using std::string;
using std::vector;
using std::array;

    // auto start = std::chrono::high_resolution_clock::now();
    // auto end = std::chrono::high_resolution_clock::now();
    // std::chrono::duration<double> diff = end-start;
    // std::cout << diff.count() << " sec\n";

class Timer {
public:
    Timer(): m_time{std::chrono::steady_clock::now()} {}
    void reset() { m_time = std::chrono::steady_clock::now(); }
    void clear() { reset(); }
    void show(std::string description="") {
        auto diff = timeit();
        std::cout << "   " << diff.count() << " sec.\t-- " << description << endl;
    }
private:
    std::chrono::duration<double> timeit() {
        auto old_time = m_time;
        reset();
        return m_time - old_time;
    }
    decltype(std::chrono::steady_clock::now()) m_time;
};

void wait() {
    char c; cin.get(c); if (c=='q') exit(1);
}
void wait(string s) {
    cout << s << ' ';
    char c; cin.get(c); if (c=='q') exit(1);
}

inline bool isneg(int x) { return x < 0; }
inline int sgn(int x) { return (x>0) - (x<0); }
// template <typename T> int sgn(T val) {  // http://stackoverflow.com/a/4609795/3625404
//     return (T(0) < val) - (val < T(0));
// }

// TODO
// c++ format function similar to python
// http://stackoverflow.com/questions/10410023/string-format-alternative-in-c
// http://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf
// http://stackoverflow.com/questions/13969117/wxstring-is-there-any-c-c-code-that-implements-string-formatting-using-python
// https://github.com/fmtlib/fmt

template <typename Iterator, typename T = typename std::iterator_traits<Iterator>::value_type>
void print(Iterator first, Iterator last, std::string description="", signed char sep=' ', std::string trailing_desc = "") { // char can be either signed or unsigned.
    // if (sep != 0) {
    //     cout << "sep = " << int(sep);// << endl;
    //     wait();
    // }
    if (description != string()) {
        cout << description << ": ";
        if (sep == '\n') cout << '\n';
    }
    const bool wait_for_inspect = (sep == 'q');
    auto ending_mark = '\n';
    if (sep <= 0) { // negative -- no newline. 0 or negative and even -- no separator.
        if (sep == 0) { sep = 0; ending_mark = '\n'; }
        else if (sep == -1) { sep = ' '; ending_mark = 0; }
        else if (sep == -2) { sep = 0; ending_mark = ' '; }
        else { sep = ' '; ending_mark = ' '; }
    } else if (!(sep == 0 || sep == ' ' || sep == '\n' || sep == '\t')) {
        sep = ' ';
        auto length = std::distance(first, last);
        if (decltype(length)(sep) < length) { last = first; std::advance(last, sep); }
    }
    for(auto it = first; it != last; ++it) {
        std::cout << *it;
        if (wait_for_inspect) {
            char c;
            std::cin.get(c);
            if (c == 'q') {
                break;
            }
        } else if (sep != 0) {
            std::cout << sep;
        }
    }
    // if (ending_mark != '\n' && trailing_desc != "" && ending_mark != 0) {
    if (trailing_desc != "") {
        if (sep == 0) cout << ' ';
        cout << trailing_desc;// << endl;
        cout << ' ';
    }
    if (ending_mark)
        std::cout << ending_mark;
}

// template < template<typename ... > class Container>
// void print(const Container<T> &vec, std::string description="", signed char sep=' ', std::string trailing_desc = "") {
template <typename Container>
void print(const Container &vec, std::string description="", signed char sep=' ', std::string trailing_desc = "") {
    print(std::begin(vec), std::end(vec), description, sep, trailing_desc);
}
template <typename T>
void print(const std::initializer_list<T> &vec, std::string description="", signed char sep=' ', std::string trailing_desc = "") {
    print(std::begin(vec), std::end(vec), description, sep, trailing_desc);
}

template <typename It>
void print(It b, int n, std::string description="", signed char sep = ' ', std::string trailing_desc = "") {
    assert(n >= 0);
    print(b, b + n, description, sep, trailing_desc);
}

template <typename T>
void print(const vector<vector<T>> &mat, std::string description="", signed char sep=' ', std::string trailing_desc = "") {
    if (!description.empty()) cout << description << ": " << endl;
    for (auto &vec: mat) print(vec, "", sep, trailing_desc);
}

    // // example for print (with caveat.)
    // print(v);   // container
    // print(v, '\t');
    // print(v, 6, ""); // ambiguous if leave empty string out.
    // print(v.begin(), 6);
    // print(v.begin(), v.end(), "second", 6);
    // print(v.begin(), v.end(), "separated with newline", '\n');

// void print(const int *p, int n) {
//     for (auto i = 0; i < n; ++i) printf("%9d, ", p[i]); putchar('\n');
// }

/**
template <>     // template specialization, note the <> is needed.
void print<double>(const std::vector<double> & vec) {
    for(auto x: vec) {
        // printf("%8.4f ", x);
        cout << x << ' ';
    }
    // for(auto x: vec) { printf("%6.2f ", x); }
    putchar('\n');
}
*/

std::vector<int> range(int start, int stop, int step) {
    /* generate a sequence.
     * step must **NOT** be zero.
     * TODO: refactor to generic to allow double parameters. Or restrict range to int, use
     * linspace for double instead?
     */
    // static_assert(step != 0, "step must **NOT** be zero.");
    auto cmp = (step>0) ? [](int left, int right) { return left < right; }
                        : [](int left, int right) { return left > right; };
    std::vector<int> res;
    while(cmp(start, stop)) {
        res.push_back(start);
        start += step;
    }
    return res;
}
std::vector<int> range(int start) {
    return range(0, start, 1);
}
std::vector<int> range(int start, int stop) {
    return range(start, stop, 1);
}
std::vector<int> rrange(int start, int stop, int step = 1) { // similar to colon operator in R, inclusive on both side.
    return range(start, stop + step, step);
}
// dVec range(double start, double stop, double step) {
//     dVec vec;
//     if (step == 0) {
//         throw std::runtime_error("step must be non-zero.");
//     } else if (step > 0) {
//         while (start < stop) {
//             vec.push_back(start);
//             start += step;
//         }
//     } else {
//         while (start > stop) {
//             vec.push_back(start);
//             start += step;
//         }
//     }
//     return vec;
// }
// std::vector<double> range(double start, double stop) {
//     return range(start, stop, 1);
// }
// std::vector<double> range(double stop) {
//     return range(0, stop, 1);
// }

template <typename T>
std::vector<T> operator*(const std::vector<T> &v, int k) {
    k = std::max(0, k);
    std::vector<T> res(v.size() * k);
    for (auto p = res.begin(); k > 0; --k) {
        for (auto x: v) *p++ = x;
    }
    return res;
}
template <typename T>
std::vector<T> operator+(const std::vector<T> &lhs, const std::vector<T> &rhs) {
    std::vector<T> res = lhs;
    res.reserve(lhs.size() + rhs.size());
    for (const auto &x: rhs) { res.push_back(x); }
    return res;
}

template <typename T1, typename T2>
std::ostream& operator<< (std::ostream& os, const std::pair<T1, T2> &pair) {
    os << pair.first << " " << pair.second << ", ";
    return os;
}

template <typename Iterator, typename T = typename std::iterator_traits<Iterator>::value_type>
std::set<T> makeSet(Iterator first, Iterator last) {
    std::set<T> res;
    for (; first != last; ++first) res.insert(*first);
    return res;
}
// typename <typename Container, typename T = typename Container::value_type>
template < template<typename ... > class Container, typename T> // http://stackoverflow.com/a/27078093/3625404
inline std::set<T> makeSet(Container<T> c) {
    return makeSet(std::begin(c), std::end(c));
}
template <typename T>
inline std::set<T> makeSet(const std::initializer_list<T> &c) {
    return makeSet(std::begin(c), std::end(c));
}
template < template<typename ... > class Container, typename T> // http://stackoverflow.com/a/27078093/3625404
bool issubset(const Container<T> &lhs, const Container<T> &rhs) {
    if (DEBUG) { cout << "issubset: " << endl; print(lhs,"lhs"); print(rhs,"rhs"); }
    auto lset = std::set<T>(lhs.begin(), lhs.end());
    auto rset = std::set<T>(rhs.begin(), rhs.end());
    return std::includes(rset.begin(), rset.end(), lset.begin(), lset.end());
}

template <typename T>
std::vector<T> makeVec(const std::initializer_list<T> &c) {
    return std::vector<T>(c);
}
template <typename Iterator,
          typename UnaryFunc>
auto makeVec(Iterator first, Iterator last, UnaryFunc f) {
    std::vector<std::decay_t<decltype(f(*first))>> res;
    for (; first != last; ++first) res.push_back(f(*first));
    return res;
}
template <typename Iterator,
          typename UnaryFunc>
auto imakeVec(Iterator first, Iterator last, UnaryFunc f) { // i for iterator, -- f's parameter is iterator
    std::vector<std::decay_t<decltype(f(first))>> res;
    for (; first != last; ++first) res.push_back(f(first));
    return res;
}
template <typename Container, typename T = typename Container::value_type,
          typename UnaryFunc>
inline auto makeVec(const Container &vec, UnaryFunc f) {
    return makeVec(std::begin(vec), std::end(vec), f);
}
template <typename T,
          typename UnaryFunc>
inline auto makeVec(const std::initializer_list<T> &c, UnaryFunc f) {
    return makeVec(std::begin(c), std::end(c), f);
}

template <typename Iterator, typename T = typename std::iterator_traits<Iterator>::value_type>
std::unordered_map<T,int> uCounter(Iterator first, Iterator last) {
    std::unordered_map<T,int> res;
    for (; first != last; ++first) {
        ++res[*first];  // value-initialization. http://stackoverflow.com/a/12965621/3625404  http://stackoverflow.com/a/2667376/3625404
        // auto p = res.find(*first);
        // if (p == res.end()) res[*first] = 1;
        // else ++p->second;
    }
    return res;
}
// template < template<typename ... > class Container, typename T> // http://stackoverflow.com/a/27078093/3625404
// inline std::unordered_map<T,int> uCounter(Container<T> c) {
template <typename Container, typename T = typename Container::value_type>
inline std::unordered_map<T,int> uCounter(Container c) {
    return uCounter(std::begin(c), std::end(c));
}
template <typename T>
inline std::unordered_map<T,int> uCounter(const std::initializer_list<T> &c) {
    return uCounter(std::begin(c), std::end(c));
}

template <typename Iterator, typename T = typename std::iterator_traits<Iterator>::value_type>
std::map<T,int> Rank(Iterator first, Iterator last) {
    std::map<T,int> res;
    for (; first != last; ++first) res[*first];
    int rank = 0;
    for(auto &x: res) x.second = rank++;
    return res;
}
template <typename Container>
auto Rank(const Container &c) {
    return Rank(std::begin(c), std::end(c));
}
template <typename T>
auto Rank(const std::initializer_list<T> &c) {
    return Rank(std::begin(c), std::end(c));
}

template <typename ForwardIt, typename T = typename std::iterator_traits<ForwardIt>::value_type>
ForwardIt isSorted_until(ForwardIt first, ForwardIt last) {
// ForwardIt is_sorted_until(ForwardIt first, ForwardIt last) {
    if (first == last) return last;
    auto next = first;
    ++next;
    for (; next != last && !(*next < *first); ++first, ++next) { }
    return next;
}
// template <typename ForwardIt, typename T = typename std::iterator_traits<ForwardIt>::value_type>
// inline bool isSorted(ForwardIt first, ForwardIt last) {
// // inline bool is_sorted(ForwardIt first, ForwardIt last) {
//     return is_sorted_until(first, last) == last;
// }
template < template<typename ... > class Container, typename T>
inline bool is_sorted(const Container<T> &c) {
    return is_sorted(std::begin(c), std::end(c));
}
template <typename T>
inline bool is_sorted(const std::initializer_list<T> &c) {
    // return is_sorted(std::begin(c), std::end(c));    // deduction failed for STL. const's fault??  eg, const int*
    return isSorted_until(std::begin(c), std::end(c)) == std::end(c);
}

vector<string> split(const string &s, char delim) {
    vector<string> res;
    auto it = s.begin(), last = s.end();
    for (;;) {
        for (; it != last && *it == delim; ++it) {}
        if (it == last) break;
        auto it_end = it;
        for (; ++it_end != last && *it_end != delim;) {}
        res.emplace_back(it, it_end);
        it = it_end;
    }
    return res;
}
string join(const vector<string> &vec, const string &sep) {
    string res;
    if (!vec.empty()) {
        res += vec[0];
        for (size_t i = 1, sz = vec.size(); i < sz; ++i) {
            res += sep;
            res += vec[i];
        }
    }
    return res;
}

template <typename Container, typename T = typename Container::value_type>
void trimAll(Container &c, T delim) {
    if (!c.empty()) {
        typename Container::iterator d = c.begin(), last = c.end(), p;
        // cout << "sz = " << c.size() << endl;
        for (; last != d && *--last == delim;) {}
        if (!(last == d && *last == delim)) ++last; // trim trailing delim(s).
        // cout << "sz = " << last - c.begin() << " -- after trim trailing" << endl;
        for (; d != last && *d != delim;) { // skip leading words that are already in place.
            for (; ++d != last && *d != delim;) {}
            if (d == last) break;
            ++d;
        }
        // assert(d <= last);
        for (p = d;;) {
            // cout << "index = " << p - c.begin() << endl;
            for (; p != last && *p == delim; ++p) {}
            for (; p != last && *p != delim; ++p, ++d) *d = std::move(*p);
            if (p == last) break;
            *d++ = std::move(*p++);
        }
        c.resize(d - c.begin());
    }
}

namespace test {
    using ::print;
    void init(int DEBUG = 0) {
        ::DEBUG = DEBUG;
        cout << std::boolalpha;
    }
}

namespace test {
    using::range;
    void is_sorted() {
        print(range(4),"",-2);
        cout << "sorted? " << ::is_sorted(range(4)) << endl;
        print({1,3,2,5},"",-2);
        cout << "sorted? " << ::is_sorted({1,3,2,5}) << endl;
    }
}

namespace test {
    void deduction() {
        print({2,3,5,8});
        // print({2,3.1,5,8}); // error, type deduction failed.
        print(std::initializer_list<double>{2,3.1,5,8});    // ok, explicit type.
    }
}

bool isPrime(int n) { 
    if (n <= 1) return false;
    if (n <= 3) return true;
    if ((n % 2 == 0) || (n % 3 == 0)) return false;
    for (int i = 5; i * i < n; i += 6) {
        if ((n % i == 0) || (n % (i + 2) == 0)) return false;
    }
    return true;
}

class Prime {
    public:
        Prime()
            : primes {2,3,5,7,11},
            threshold {11}
        { }
        Prime(int threshold) : Prime() {
            if (threshold > this->threshold)
                this->update(threshold);
        }
        iVec operator()(int threshold) {
            if (threshold > this->threshold) {
                this->update(threshold);
            }
            // TODO(efficiency): is this copy approach good enough, could a non-writable view be returned instead?
            // NOT. since this->primes, could be invalidated, returning pointer/reference
            // may result dangling reference. Which do not change for const reference.
            return iVec(primes.begin(), std::upper_bound(primes.begin(), primes.end(), threshold));
        }
        int nth_prime(int n) {
            auto threshold = n;
            while ((size_t)n > primes.size()) {
                if (threshold > this->threshold) {
                    this->update(threshold);
                }
                else {
                    threshold *= 2;
                    // cout << "will update. current size is " << primes.size() << ", threshold is " << threshold << '\n';
                }
            }
            // printf("primes.size() = %zu\n", primes.size());
            return primes[n-1];
        }
        const iVec getall() { return this->primes; }
        iVec::const_iterator get_pos(int threshold) {
            cout << "this->threshold = " << this->threshold << ", threshold = " << threshold << ", primes.size() = " << primes.size() << '\n';
            if (threshold > this->threshold) {
                this->update(threshold);
            }
            return std::upper_bound(primes.cbegin(), primes.cend(), threshold);
        }
    private:
        iVec primes;
        int threshold;
        void update(int threshold) {
            if (primes.capacity() < (size_t) threshold) {
                this->primes.reserve(1.25506 * threshold / log(threshold) );
                printf("reserve size %f\n", 1.25506 * threshold / log(threshold) );
            }
            // cin.get();
            // printf("debug_num = %d\n", ++debug_num);
            auto pos = this->get_pos(sqrt(threshold));
            // cout << "threshold = " << threshold << ", pos is nth: " << pos - primes.cbegin() << '\n';
            auto is_prime = [this, pos](int val) {
                for (auto it = primes.cbegin(); it != pos; ++it) {
                    if (val % (*it) == 0)
                        return false;
                }
                return true;
            };
            auto candidate = 1 + this->threshold;
            candidate += (candidate + 1) % 2;
            for(; candidate <= threshold; candidate += 2) {
                if (is_prime(candidate))
                    this->primes.push_back(candidate);
            }
            this->threshold = threshold;
            cout << "this->threshold = " << this->threshold << ", threshold = " << threshold << ", primes.size() = " << primes.size() << '\n';
        }
};

#endif
// below are vocabulary that aid vim completion.
// extensibility
/**
    runtime_error overflow_error
    bad_alloc out_of_range invalid_argument
    throw std::invalid_argument( "received negative value" );
    catch(const std::invalid_argument& e) {   // And you should always catch exceptions as const? comment in http://stackoverflow.com/a/8480675/3625404
**/



