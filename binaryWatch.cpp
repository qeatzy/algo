#include <bitset>
#include <list>
#include "utility.h"

// https://en.wikipedia.org/wiki/Catalan_number
// http://mathworld.wolfram.com/CatalanNumber.html
// http://mathworld.wolfram.com/Multifactorial.html
// Euler's polygon division problem.  http://mathworld.wolfram.com/EulersPolygonDivisionProblem.html
template < template<typename ... > class Container, typename T>
Container<T> first_catalan(size_t n, Container<T> c) {
    assert(c.size() == 2 && c[0] < c[1]);
    Container<T> res(n, c[0]);
    res.append(n, c[1]);
    return res;
}
string first_catalan(size_t n, string c) {
    assert(c.size() == 2 && c[0] < c[1]);
    string res(n, c[0]);
    res.append(n, c[1]);
    return res;
}
template <typename BidirectIt>
bool is_catalan_candidate(BidirectIt first, BidirectIt last) {
    int cnt = 0;
    auto p = first;
    for (; p != last && *p == *first; ++p) { ++cnt; }
    for (auto second = p; p != last; ++p) {
        if (*p == *first) ++cnt;
        else if (*p == *second) --cnt;
        else return false;
    }
    return cnt == 0;
}
template <typename BidirectIt, typename T = typename std::iterator_traits<BidirectIt>::value_type>
bool is_catalan_candidate(BidirectIt first, BidirectIt last, T zero, T one) {
    assert(zero < one);
    int cnt = 0;
    for (; first != last; ++first) {
        if (*first == zero) ++cnt;
        else if (*first == one) --cnt;
        else return false;
    }
    return cnt == 0;
}
template <typename BidirectIt, typename T = typename std::iterator_traits<BidirectIt>::value_type>
bool is_catalan(BidirectIt first, BidirectIt last, T zero, T one) {
    assert(zero < one);
    int cnt = 0;
    for (; first != last && cnt >= 0; ++first) {
        if (*first == zero) ++cnt;
        else if (*first == one) --cnt;
        else return false;
    }
    return cnt == 0;
}
template <typename BidirectIt>
bool is_catalan(BidirectIt first, BidirectIt last) {
    if (first == last) return true;
    int cnt = 0;
    auto p = first;
    for (; p != last && *p == *first; ++p) { ++cnt; }
    auto second = p;
    for (; p != last && cnt >= 0; ++p) {
        // if (DEBUG) cout << "*first = " << *first << ", *second = " << *second << " *p = " << *p << endl;
        if (*p == *first) ++cnt;
        else if (*p == *second) --cnt;
        else return false;
    }
    return cnt == 0 && *first < *second;
}
template <typename BidirectIt, typename T = typename std::iterator_traits<BidirectIt>::value_type>
inline bool helper_next_catalan(BidirectIt first, BidirectIt mid, BidirectIt last, T zero, T one) { // if is_catalan, return if modified.
    assert(first != last && is_catalan(first, mid) && is_catalan_candidate(first, last, zero, one) && (mid == last || is_catalan(mid, last) == false));
    DEBUG = 1;
    --mid, --first;
    auto prev = mid;
    // test case: "10"
    for (--prev; mid != first && *prev < *mid; mid = --prev, --prev) {} // trailing "01".
    const bool not_largest = (mid != first);
    if (not_largest) { // "11" before trailing "01"s. and there is "00" before that.
        for (mid = --prev, --prev; *mid == one && *prev == one; mid = --prev, --prev) {
            *--last = one;
            *--last = one;
        }
    // try these: "0011", "001101", "0001 1101"
        if (*mid == zero) { // "10" or "00" before "11"s.
            *mid = one;
        } else { // "01" before "11"s.
            *prev = one, *mid = zero;
            *--last = one;
            *--last = one;
        }
    }
    ++mid;
    // cout << "sz = " << std::distance(mid, last) << endl;
    // print(first, first + 8, "segfault",0);
    // wait();
    for (; mid != last; ++mid) {
        *mid = zero;
        *--last = one;
    }
    return not_largest;
}
template <typename BidirectIt, typename T = typename std::iterator_traits<BidirectIt>::value_type>
bool fix_to_next_catalan(BidirectIt first, BidirectIt last) { // if is_catalan, return if modified.
    // for any valid catalan sequence other than largest one, remove leading "01", what follows immediately is "00".
    assert(is_catalan_candidate(first, last));
    if (DEBUG) print(first, last, "fix next",0);
    if (first == last) return false;
    auto p = first;
    int cnt = 0;
    for (; *++p == *first;) { ++cnt; }
    T zero, one;
    if (*first < *p) { // leading "0"s, valid catalan prefix.
        zero = *first, one = *p;
        // for (; ++p != last && cnt >= 0;) {
        for (;;) {
    if (DEBUG) { cout << "cnt = " << cnt << ", last - first = " << last - first << ", p - first = " << p - first << endl; }
            if (*++p == zero) ++cnt;
            else --cnt;
            if (cnt < 0) break;
        }
    } else { // leading "1"s, invalid catalan prefix.
        zero = *p, one = *first;
        p = first;
    }
    if (DEBUG) {
        cout << "fix: " << "last - first = " << last - first << ", p - first = " << p - first << endl;
        cout << "zero = " << zero << ", one = " << one << endl;
        wait();
    }
    if (p == last) return false;
    helper_next_catalan(first, p, last, zero, one);
    return true;
    // return need_to_modified;
}
template <typename BidirectIt>
bool fix_to_catalan(BidirectIt first, BidirectIt last) { // if is_catalan, return if modified.
    DEBUG = 1;
    return fix_to_next_catalan(first, last);
}
template < template<typename ... > class Container, typename T>
inline bool is_catalan(const Container<T> &c) {
    return is_catalan(std::begin(c), std::end(c));
}
template <typename T>
inline bool is_catalan(const std::initializer_list<T> &c) {
    return is_catalan(std::begin(c), std::end(c));
}
template <typename BidirectIt>
bool nextCatalan_base(BidirectIt first, BidirectIt last) {
    if (first == last) return false;
    // if (DEBUG) {
    //     print(first, last);
    //     wait();
    //     // assert(std::distance(first, last) % 2 == 0);
    //     // assert(makeSet(first, last).size() == 2);
    //     // assert(std::count(first, last, *first) * 2 == std::distance(first, last));
    //     // ++stack_level;
    //     // cout << stack_level << ' ';
    //     // wait("permute\n");
    // }
    auto p = --last;
    --p;
    if (first == p) return false;
    if (*p == *last) {
        for (; *p == *last; --p) {}
        auto pnext = p;
        ++pnext;
        std::iter_swap(p, pnext);
        if (DEBUG) print(first, last + 1, "true, 11",0);
        return true;
    } else {
        // if (DEBUG) cout << "else " << endl;
        for (--p; --p != first && *p != *last; --p) {
            // cout << "index = " << std::distance(first, p);
            // wait();
        }
        if (p != first) {
            for (; *p == *last; --p) {}
            auto pnext = p;
            ++pnext;
            std::iter_swap(p, pnext);
            std::sort(++pnext, last);   // already done in _fast_final: optimize sort, via count and write.
            // if (DEBUG) print(first, last + 1, "true, 01",0);
            return true;
        } else {
            std::sort(++p, last);
            // if (DEBUG) print(first, last + 1, "false   ",0);
            return false;
        }
    }
}
template <typename BidirectIt>
bool nextCatalan_fast(BidirectIt first, BidirectIt last) {
    if (first == last) return false;
    auto p = --last;
    --p;
    if (first == p) return false;
    if (*p == *last) {
        for (; *p == *last; --p) {}
        auto prev = p;
        ++p;
        // std::iter_swap(p, pnext);
        *p = *prev;
        *prev = *last;
        if (DEBUG) print(first, last + 1, "true, 11",0);
        return true;
    } else {
        // if (DEBUG) cout << "else " << endl;
        int cnt = 1;
        for (--p; --p != first && *p != *last; --p) {
            ++cnt;
            // cout << "index = " << std::distance(first, p);
            // wait();
        }
        if (p != first) {
            for (; *p == *last; --p) {}
            auto prev = p;
            ++p;
            // std::iter_swap(p, pnext);
            *p = *prev;
            *prev = *last;
            // std::sort(++pnext, last);   // already done in _fast_final: optimize sort, via count and write.
            for (prev = p; cnt > 0; --cnt) { *++p = *prev; }
            for (; ++p != last; ) { *p = *last; }
            if (DEBUG) print(first, last + 1, "true, 01",0);
            return true;
        } else {
            // std::sort(++p, last);
            for (; cnt > 0; --cnt) { *++p = *first; }
            for (; ++p != last; ) { *p = *last; }
            if (DEBUG) print(first, last + 1, "false   ",0);
            return false;
        }
    }
}
template <typename BidirectIt>
bool nextCatalan_fast_reuse_first_iterator(BidirectIt first, BidirectIt last) {
    if (first == last) return false;
    auto p = --last;
    --p;
    if (first == p) return false;
    if (*p == *last) {
        for (; *p == *last; --p) {}
        auto prev = p;
        ++p;
        // std::iter_swap(p, pnext);
        *p = *prev;
        *prev = *last;
        if (DEBUG) print(first, last + 1, "true, 11",0);
        return true;
    } else {
        // if (DEBUG) cout << "else " << endl;
        int cnt = 1;
        for (--p; --p != first && *p != *last; --p) {
            ++cnt;
            // cout << "index = " << std::distance(first, p);
            // wait();
        }
        const bool res = (p != first);
        if (res) {
            for (; *p == *last; --p) {}
            first = p;
            ++p;
            // std::iter_swap(p, pnext);
            *p = *first;
            *first = *last;
            // std::sort(++pnext, last);   // already done in _fast_final: optimize sort, via count and write.
            first = p;
        }
        // std::sort(++p, last);
        for (; cnt > 0; --cnt) { *++p = *first; }
        for (; ++p != last; ) { *p = *last; }
        // if (DEBUG) print(first, last + 1, "false   ",0);
        return res;
        // if (p != first) {
        //     for (; *p == *last; --p) {}
        //     first = p;
        //     ++p;
        //     // std::iter_swap(p, pnext);
        //     *p = *first;
        //     *first = *last;
        //  // std::sort(++pnext, last);   // already done in _fast_final: optimize sort, via count and write.
        //     for (first = p; cnt > 0; --cnt) { *++p = *first; }
        //     for (; ++p != last; ) { *p = *last; }
        //     if (DEBUG) print(first, last + 1, "true, 01",0);
        //     return true;
        // } else {
        //     // std::sort(++p, last);
        //     for (; cnt > 0; --cnt) { *++p = *first; }
        //     for (; ++p != last; ) { *p = *last; }
        //     if (DEBUG) print(first, last + 1, "false   ",0);
        //     return false;
        // }
    }
}
template <typename BidirectIt>
bool nextCatalan_fast_final(BidirectIt first, BidirectIt last) {
    if (first == last) return false;
    auto p = --last;
    int cnt = 0;
    for (; --p != first && *p != *last; --p) {
        ++cnt;
    }
    const bool not_largest = (p != first);
    for (; *--p == *last; ) {}
    if (not_largest) {
        first = p;
        ++p;
        // std::iter_swap(p, pnext);
        *p = *first;
        *first = *last;
        first = p;
    }
    if (cnt > 0) {
        for (; cnt > 0; --cnt) { *++p = *first; }
        for (; ++p != last; ) { *p = *last; }
    }
    return not_largest;
}
template <typename BidirectIt>
bool nextCatalan_final(BidirectIt first, BidirectIt last) {
    if (first == last) return false;
    // print(first, last, "final",0);
    // wait();
    return helper_next_catalan(first, last, last, *first, last[-1]);
    --last;
    auto p = last;
    for (auto next = p; --p != first && *p < *next; next = --p) { }
    // any catalan candidate sequence can be divide to two part, valid subsequence(not just prefix) + invalid subsequence(begin with "1"). each could be empty.
    // first index that invalidate catalan prefix property must at some p such that p = first + 2*k (*p =='1').  and the sub range [first,p) is a valid catalan sequence, not just catalan prefix.
    // next catalan sequence can be achieved (for all catalan candidate both invalid and valid case), via partition to two subsequence, left subsequence is valid and could be incremented, right subsequence is too large (begin with "1" for invalid case, "11"s followed by "01"s.
    // catalan candidate (for both sequence and subsequence) -- cnt("0") == cnt("1"). and is valid catalan subsequence if sorted according catalan property.
    // any valid catalan sequence could be divided to three parts, leading "01"s, trailing "01"s, ("00" + optional others + "11"). each could be empty. middle part is empty only if the catalan sequence is largest.
    // for any valid catalan sequence other than largest one, remove trailing "01", what prepends immediately is "11".
    // for any valid catalan sequence other than largest one, remove leading "01", what follows immediately is "00".
    // try these: "0011", "001101"
    const bool not_largest = (p != first);
    if (not_largest) { // then "11"
        for (; *--p == *last;) {
            auto tmp = *last;
            *--last = std::move(tmp);
            // wait("inf loop?");
        }
        // wait("inf loop? no!! ");
        *p = *last;
        *++p = *first;
    }
    auto q = last;
    for (; ++p != q; ) {  // before loop, std::distance(p,q) % 2 == 1 && *p == *first && *q == *last. we need to modify item in [p+1,q-1].
        *p = *first;
        *--q = *last;
    }
    return not_largest;
}
template <typename BidirectIt>
bool nextCatalan(BidirectIt first, BidirectIt last) {
    assert(is_catalan(first, last));
    return nextCatalan_final(first, last);
    // return nextCatalan_fast_final(first, last);
    // return nextCatalan_fast_reuse_first_iterator(first, last);
    // return nextCatalan_fast(first, last);
    // return nextCatalan_base(first, last);
}
template < template<typename ... > class Container, template<typename ... > class T_Cont, typename T, typename Iterator> // cc -- Container of Container.  use 'Container<T_Cont<T>> c'. 
void generate_catalan(Iterator first, Iterator last, Container<T_Cont<T>> &res) {
    auto buf = T_Cont<T>(first, last);
    do {
        res.push_back(buf);
    } while (nextCatalan(buf.begin(), buf.end()));
}
template < template<typename ... > class Container, typename T>
std::vector<Container<T>> generate_catalan(Container<T> c) {
    std::vector<Container<T>> res;
    generate_catalan(c.begin(), c.end(), res);
    return res;
}
namespace test {
    void nextCatalan() {
        int n = 4;
        auto parens = first_catalan(n, "01");
        for (auto &s: generate_catalan(parens)) {
            print(s,"",0);
        }
    }
    void fix_to_catalan() {
        int n = 4;
        auto str = first_catalan(n, "01");
        DEBUG = 0;
        // std::prev_permutation(str.begin() + 1, str.end() - 1);
        int cnt = 0;
        do {
            cnt += is_catalan(str);
            // print(str,"",0);
            print(str,is_catalan(str)?"catalan":"not catalan",0);
            auto old = str;
            if (::fix_to_catalan(str.begin(), str.end())) {
                print(str,"modified",0);
            }
            cnt += (str != old) && is_catalan(str);
            // cout << "old <= str == " << (old <= str);
            // cout << "old >= str == " << (old >= str);
            // cout << ", cnt = " << cnt;
            // wait();
        } while (std::next_permutation(str.begin() + 1, str.end() - 1));
        // } while (std::prev_permutation(str.begin() + 1, str.end() - 1));
    }
}

// TODO: iterative solution. -- what's the time complexity?  https://discuss.leetcode.com/post/100348
    vector<string> generateParenthesis_todo(int n) { // https://discuss.leetcode.com/post/100348
        vector<string> result;
        if (!n) return result;
        
        string s(n, '(');
        s.append(n, ')');
        
        for (;;) {
            auto l = n, r = n;
            result.push_back(s);
            if (DEBUG) print(s,"prev",0);
            for (;;) {
            if (DEBUG) { print(s,"    ",-2); cout << string(2*n-l-r,' ') << "l = " << l << ", r = " << r << endl; }
                if (s.back() == ')') --r;
                else if (l < r + 2) --l;
                else break;
                s.pop_back();
                if (s.empty()) return result;
            }
            
            if (DEBUG) { print(s,"post",-2); cout << string(2*n-l-r,' ') << " s.sz = " << s.size() << ",  res.sz " << result.size() << endl; }
            if (DEBUG) cout << "append " << n - (l - 1) << " '(', then append " << n - (r + 1) << " ')'" << endl;
            s.back()=')';
            s.append(n - (l - 1), '(');
            s.append(n - (r + 1), ')');
        };
    }

// https://leetcode.com/problems/generate-parentheses/
    vector<string> generateParenthesis_(int n) {
        vector<string> res;
        return res;
    }
    void generateParenthesis_dfs_increment(vector<string> &res, string &str, const int n, int left, int right) {
        int pos = left + right;
        // if (DEBUG) cout << "left = " << left << ", right = " << right << "sz = " << res.size() << endl;
        if (left == n) {
            for (; pos < n * 2; ++pos) str[pos] = ')';
            res.push_back(str);
            return;
        }
        str[pos] = '(';
        generateParenthesis_dfs_increment(res, str, n, left + 1, right);
        if (left > right) {
            str[pos] = ')';
            generateParenthesis_dfs_increment(res, str, n, left, right + 1);
        }
    }
    void generateParenthesis_dfs_decrement(vector<string> &res, string &str, int left, int right) {
        size_t pos = str.size() - left - right;
        if (left == 0) {
            for (; pos < str.size(); ++pos) str[pos] = ')';
            res.push_back(str);
            return;
        }
        str[pos] = '(';
        // if (DEBUG) cout << "str = " << str << ", sz = " << res.size() << endl;
        generateParenthesis_dfs_decrement(res, str, left - 1, right);
        if (left < right) {
            str[pos] = ')';
            generateParenthesis_dfs_decrement(res, str, left, right - 1);
        }
    }
    void generateParenthesis_dfs_decrement_require_trailing_half_of_str_full_of_closing_paren(vector<string> &res, string &str, int left, int right) {
        if (left == 0) {
            res.push_back(str);
            return;
        }
        const size_t pos = str.size() - left - right;
        str[pos] = '(';
        // if (DEBUG) cout << "str = " << str << ", sz = " << res.size() << endl;
        generateParenthesis_dfs_decrement_require_trailing_half_of_str_full_of_closing_paren(res, str, left - 1, right);
        str[pos] = ')';
        if (left < right) {
            generateParenthesis_dfs_decrement_require_trailing_half_of_str_full_of_closing_paren(res, str, left, right - 1);
        }
    }
    void generateParenthesis_dfs_decrement_require_trailing_half_of_str_full_of_closing_paren_loop(vector<string> &res, string &str, int left, int right) {
        if (left == 0) {
            res.push_back(str);
            return;
        }
        const size_t pos = str.size() - left - right;
        str[pos] = '(';
        // if (DEBUG) cout << "str = " << str << ", sz = " << res.size() << endl;
        generateParenthesis_dfs_decrement_require_trailing_half_of_str_full_of_closing_paren_loop(res, str, left - 1, right);
        str[pos] = ')';
        if (left < right) {
            generateParenthesis_dfs_decrement_require_trailing_half_of_str_full_of_closing_paren_loop(res, str, left, right - 1);
        }
    }
    vector<string> generateParenthesis_bfs_list(int n) {
        std::list<string> res;
        res.push_back("(");
        std::list<std::pair<int, int>> cnt { {n - 1, n} };
        for (int level = 1; level < 2 * n; ++level) {
            for (int i = 0; res.front().size() == size_t(level); ++i) {
                auto tmp_str = res.front();
                auto tmp_cnt = cnt.front();
                res.pop_front();
                cnt.pop_front();
                if (tmp_cnt.first > 0) {
                    res.push_back(tmp_str + '(');
                    cnt.push_back({tmp_cnt.first - 1, tmp_cnt.second});
                }
                if (tmp_cnt.first < tmp_cnt.second) {
                    res.push_back(tmp_str + ')');
                    cnt.push_back({tmp_cnt.first, tmp_cnt.second - 1});
                }
            }
        }
        return vector<string> (res.begin(), res.end());
    }
    vector<string> generateParenthesis_bfs(int n) {
        vector<string> res;
        if (n <= 0) return res;
        vector<string> res_prev {"("};
        vector<std::pair<int, int>> cnt;
        vector<std::pair<int, int>> cnt_prev { {n - 1, n} };
        for (int level = 1; level < 2 * n; ++level) {
            res.clear();
            cnt.clear();
            for (size_t i = 0; i < res_prev.size(); ++i) {
                if (cnt_prev[i].first > 0) {
                    res.push_back(res_prev[i] + '(');
                    cnt.push_back({cnt_prev[i].first - 1, cnt_prev[i].second});
                }
                if (cnt_prev[i].first < cnt_prev[i].second) {
                    res.push_back(res_prev[i] + ')');
                    cnt.push_back({cnt_prev[i].first, cnt_prev[i].second - 1});
                }
            }
            res.swap(res_prev);
            cnt.swap(cnt_prev);
        }
        return res_prev;
    }
// recursive generate, left > 0 && left < right. where 'left' represents how many left parentheses remains. https://discuss.leetcode.com/post/73988
// If you make the string a reference, you can actually get a 3x gain in speed. This solution is only 8ms.  https://discuss.leetcode.com/post/62165
// constructive, f(n) = sum {k: 0:n-1}{f(k) f(n-1-k)}. https://discuss.leetcode.com/post/82592
    vector<string> generateParenthesis(int n) {
        // return generateParenthesis_todo(n);
        // return generateParenthesis_bfs_list(n);
        // return generateParenthesis_bfs(n);
        assert(n >= 0);
        // string str(n*2, ')');
        string str(n, '(');
        str.append(n, ')');
        vector<string> res;
        generate_catalan(str.begin(), str.end(), res);
        // generateParenthesis_dfs_increment(res, str, n, 0, 0);
        // generateParenthesis_dfs_decrement(res, str, n, n);
        // string str(n * 2, ')');
        // generateParenthesis_dfs_decrement_require_trailing_half_of_str_full_of_closing_paren_loop(res, str, n, n);
        // generateParenthesis_dfs_decrement_require_trailing_half_of_str_full_of_closing_paren(res, str, n, n);
        return res;
        // return generateParenthesis_next_permutation(n);
    }

namespace test {
    void generateParenthesis() {
        // DEBUG = 2;
        cout << std::boolalpha;
        for (auto n: {4}) {
            auto res = ::generateParenthesis(n);
            cout << (::is_sorted(res) ? "sorted" : "not sorted") << endl;
            print(res);
            // for (auto const &s: res) { cout << is_catalan(s.begin(), s.end()) << endl; }
            // print(res,"",'\n');
        }
    }
}

// 338. Counting Bits  https://leetcode.com/problems/counting-bits/
    vector<int> countBits_(int num) {
        std::vector<int> res(num + 1);
        return res;
    }
// f[i] = f[i / 2] + i % 2.  https://discuss.leetcode.com/topic/40162/three-line-java-solution/2
    vector<int> countBits_optimal_dp_right_reduce(int num) {
        std::vector<int> res(num + 1);
        for (int i = 1; i <= num; ++i) res[i] = res[i / 2] + (i % 2 != 0);
        return res;
    }
// f[i] = f[i & (i - 1)] + 1.  https://discuss.leetcode.com/topic/40262/four-lines-c-time-o-n-space-o-n/2
    vector<int> countBits_optimal_dp_counting_bits_reduce(int num) {
        std::vector<int> res(num + 1);
        for (int i = 1; i <= num; ++i) res[i] = res[i & (i-1)] + 1;
        return res;
    }
    vector<int> countBits_optimal(int num) {
        return countBits_optimal_dp_right_reduce(num);
        return countBits_optimal_dp_counting_bits_reduce(num);
    }
// f[i] = 1 + f[remove leading 1 of i].  https://discuss.leetcode.com/topic/41785/simple-java-o-n-solution-using-two-pointers/
// My logic was to copy the previous values (starting at 0) until a power of 2 was hit (new range), at which point we just reset the t pointer back to 0 to begin the new range.
    vector<int> countBits_dp_left_reduce(int num) {
        std::vector<int> res(num + 1);
        for (int npow = 1, i = 1, t = 0; i <= num; ++i, ++t) {
            if (npow == i) {
                npow *= 2;
                t = 0;
                cout << npow << endl;
            }
            res[i] = res[t] + 1;
            // res[i] = res[i & (npow / 2 - 1)] + 1;
            cout << "i = " << i << ", npow = " << npow << ", i & (npow - 1) = " << (i & (npow - 1))<< endl;
        }
        return res;
    }
// get cnt from only previous cnt. view ++i as next_permutation on 0 and 1 bits.
    vector<int> countBits_next_permutation(int num) {
        std::vector<int> res(num + 1);
        for (int cnt = 0, i = 1; i <= num; ++i) {
            ++cnt;
            res[i] = cnt;
            int m = 0, mask = 1;
            for (; (i & mask) != 0;) { ++m, mask <<= 1;}
            cnt -= m;
        }
        return res;
    }
    vector<int> countBits(int num) {
        return countBits_optimal_dp_right_reduce(num);
        // return countBits_optimal_dp_counting_bits_reduce(num);
        return countBits_dp_left_reduce(num);
        return countBits_next_permutation(num);
    }

namespace test {
    void countBits() {
        for (auto n: {15}) {
            auto res = ::countBits(n);
            print(res);
            // print(res,"",'\n');
        }
    }
}

// This alg puts the responsibility of computing to programmer's brain or another alg.  https://discuss.leetcode.com/post/141155     https://discuss.leetcode.com/post/140401
    vector<string> readBinaryWatch_precompute(int num) {
        std::vector<std::string> res;
        return res;
    }
    vector<string> readBinaryWatch_brute_force(int num) {
        std::vector<std::string> res;
        for (int h = 0; h < 12; ++h) {
            for (int m = 0; m < 60; ++m) {
                if (std::bitset<10>(h << 6 | m).count() == size_t(num))
                // if (bitCount(h) + bitCount(m) == num)
                    res.emplace_back(std::to_string(h) + (m < 10 ? ":0" : ":") + std::to_string(m));
            }
        }
        // if (DEBUG) std::sort(res.begin(), res.end());
        return res;
    }
    vector<string> readBinaryWatch_combination(int num) {
        std::vector<std::string> res;
        std::vector<int> bits(10);
        for (int i = 10 - num; i < 10; ++i) { bits[i] = 1; }
        do {
            int h = 0, m = 0;
            for (int i = 0, base = 1; i < 4; ++i, base *= 2) h += base * bits[i];
            for (int i = 4, base = 1; i < 10; ++i, base *= 2) m += base * bits[i];
            if (h < 12 && m < 60)
                res.emplace_back(std::to_string(h) + (m < 10 ? ":0" : ":") + std::to_string(m));
        } while (std::next_permutation(bits.begin(), bits.end()));
        // if (DEBUG) std::sort(res.begin(), res.end());
        return res;
    }

    vector<string> readBinaryWatch(int num) {
        return readBinaryWatch_combination(num);
        return readBinaryWatch_brute_force(num);
        return readBinaryWatch_precompute(num);
    }

namespace test {
    void readBinaryWatch() {
        for (auto n: {2}) {
            auto res = ::readBinaryWatch(n);
            print(res);
            // print(res,"",'\n');
        }
    }
}

int main() {
    test::init(1);
    try {
        // test::readBinaryWatch();
        // test::countBits();
        // test::generateParenthesis();
        // test::nextCatalan();
        test::fix_to_catalan();
    } catch (const std::exception &e) {
        wait(e.what());
        return -1;
    }
}
