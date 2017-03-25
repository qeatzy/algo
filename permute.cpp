#include "utility.h"

template <typename ForwardIt>
void inplace_merge_new(ForwardIt left, ForwardIt left_last, ForwardIt right, ForwardIt right_last) {
    if (right == right_last) return;
    // loop invariants: left != left_last && right != right_last && [prefix, right) < [left, left_last)
    for (auto prefix = right; left != left_last; ) {
        if (prefix == right) { // [prefix,right) is empty.
        } else {
        }
    }
}//    3 4 5 6 | 1 2 7 8

// in place merge sort? http://stackoverflow.com/a/2571104/3625404   and   http://stackoverflow.com/a/2126289/3625404     http://stackoverflow.com/a/2128992/3625404
// nice graph of merge?  The trick is to swap the largest elements in A for the smallest elements in B. recursively applying rotations.  https://xinok.wordpress.com/2014/08/17/in-place-merge-sort-demystified-2/
// @DonalFellows: jk is exactly right; quicksort CAN be implemented to be stable, without extra space cost. Check Wikipedia.  comment in http://stackoverflow.com/a/2571094/3625404
// After further research, it appears that there is not a single definition of "in-place" (see en.wikipedia.org/wiki/In-place_algorithm). The traditional definition used by algorithm theorists is one that uses O(1) extra storage, whereas the one used by the C++ standard is apparently one that uses the same storage for input and output.
template <typename ForwardIt>
void inplace_merge(ForwardIt left, ForwardIt left_last, ForwardIt right, ForwardIt right_last) {
    if (right == right_last) return;
    // print(left, left_last, "", -2);
    // print(right, right_last);
    // wait();
    // auto first = left, last = right_last;
    for (auto left_prefix = right; left != left_last; ) {
        if (*right < *left) {
            std::iter_swap(left, right);
            ++right;
        }
        ++left;
        if (right == right_last) {  // test for right **must** precede test for left.
            right = left_prefix;
        }
        if (left == left_last) {
            left = left_prefix;
            left_last = right;
            left_prefix = right;
        }
    }
}

template <typename ForwardIt, typename T = typename std::iterator_traits<ForwardIt>::value_type>
void merge(ForwardIt left, ForwardIt left_last, ForwardIt right, ForwardIt right_last) {
    // assert(sorted(left, left_last) && sorted(right, right_last));
    std::vector<T> buf;
    for (auto p = left; p != left_last; ++p) { buf.push_back(std::move(*p)); }
    // for (auto p = left; p != left_last; ++p) { *p = T{}; }
    auto old_right = right;
    auto it = buf.begin(), it_end = buf.end();
    for (; it != it_end && right != right_last;) {
        if (*right < *it) {
            *left = std::move(*right);
            ++right;
        } else {
            *left = std::move(*it);
            ++it;
        }
        // print(left, right_last);
        // wait();
        ++left;
        if (left == left_last) {
            left = old_right;
            left_last = right_last;
        }
    }
    if (right == right_last) {
        for (; left != left_last; ++left) {
            *left = std::move(*it);
            ++it;
        }
        for (; it != it_end; ++it) {
            *old_right = std::move(*it);
            ++old_right;
        }
    }
}

namespace test {
    void merge() {
        auto test_case = []() {
            std::vector<std::vector<int>> res;
            res.push_back({3,4,5,6,  1,2,7,8});
            res.push_back({15,29,32,64,91, 11,19,27,80,89});
            res.push_back({4,10,70,10,30,69,96,100});
            res.push_back({10,15,19,23,26,32,  4,9,16,30,39,47});
            // res.push_back({9,  4,6,8});
            // res.push_back({6,  4,8,9});
            // res.push_back({8,9,  4,6});
            // res.push_back({6,8,9,  4});
            return res;
        };
        for (auto &v: test_case()) {
            auto first = v.begin(), last = v.end();
            auto mid = isSorted_until(first, last);
            assert(is_sorted(mid, last));
            print(first, mid, "",-1,"|"); print(mid, last);
            // ::merge(first, mid, mid, last);
            ::inplace_merge(first, mid, mid, last);
            print(first, mid, "",-1,"|"); print(mid, last);
        }
    }
}

// http://wordaligned.org/articles/next-permutation#fn1link
// https://code.google.com/codejam/contest/dashboard?c=186264#s=a&a=1
// https://code.google.com/codejam/contest/dashboard?c=186264#s=p1

// http://marknelson.us/2002/03/01/next-permutation/
// Thomas Draper wrote me a while back and offered up some similar code that implements next_combination(). I've posted a copy here. I'll ask him to post a follow-up comment to explain in his own words a little mroe about it.
// cd ~/code/algo && wget http://marknelson.us/attachments/next-permutation/NextCombination.tgz
// wget may fail, download via browser then.
// Hi Paul, Instead of next_permutation(), you need next_combination(), which unfortunately is not part of the standard library. However, this article comes up with a pretty good implementation of it: http://www.codeguru.com/cpp/cpp/algorithms/combinations/article.php/c5117/
// Sean offered up the following alternative algorithm:
//     #define N 20    // number of items
//     #define R 10    // number of choices
//     int next_combination(int c[])
//     {
//         int i = 0;
//         while ( i <R-1 && c[i+1]==c[i]+1) // for each bump
//             c[i] = i++;                 // fall back
//         return N - ++c[i];              // push forward and verify
//     }
template <typename BidirectIt>
bool nextCombination(BidirectIt first, BidirectIt mid, BidirectIt last) {
    // assert(sorted(first, mid) && sorted(mid, last));
    if (first == mid || mid == last) return false;
    auto pre_last = last;
    --pre_last;
    if (!(*first < *pre_last)) {
        std::rotate(first, mid, last);
        return false;
    }
// in left half find trailing item that is smaller than *last. worst case *first.
// then increase the found one. change to first item larger than it in the right half. worst case *last.
    auto left = std::lower_bound(first, mid, *pre_last);
    --left;
    auto right = std::upper_bound(mid, last, *left);
    std::iter_swap(left, right);
    ++left, ++right;
    // merge(left, mid, right, last);
    inplace_merge(left, mid, right, last);
    return true;
}
// Nice article, Mark. But I was just wondering if you have come across any algorithms that list permutations such that the mirror images of strings already listed are not listed again. As in, while permuting 1234, my list must only include: 1234,1243,1324,1342,1423,1432,2134,2143,2413,3124 & 3214.
// Well, lets say you have 120 permutations and you want permutation number 60. Using STL you woule have to walk through all of them step by step - wont you ? So I've written an implementation that gives you any permutation immideately, without recursion:
// Thomas Nygreen said: Nice article! The original problem can be solved by first listing the seven possible combinations of numbers in the range 1-9 such that no numbers are repeated and the sum is 17:
//     i:   {1, 7, 9}
//     ii:  {2, 6, 9}
//     iii: {2, 7, 8}
//     iv:  {3, 5, 9}
//     v:   {3, 6, 8}
//     vi:  {4, 5, 8}
//     vii: {4, 6, 7}
// Exactly five of these must be included in the solution.
// As there are no combinations containing both 5 and 6 or both 5 and 7, the only positions left for 5 are d and h.
// There is only one combination containing 1, so 1 cannot be in any corner, and there are no combinations with both 8 and 9, so h and i cannot be 9.

template <typename BidirectIt>
bool nextPermutation(BidirectIt first, BidirectIt last) {
// bool next_permutation(BidirectIt first, BidirectIt last) {
    if (first == last || --last == first) return false;
    if (DEBUG) {
        ++stack_level;
        cout << stack_level << ' ';
        // cout << endl;
        wait("permute\n");
    }
    // note that last is decremented if reach here.
    auto p = last;
    --p;
    if (*p < *last) {
        std::iter_swap(p, last);
        return true;
    } else {
        auto pnext = last;
        do {
            --p; --pnext;
        } while (pnext != first && !(*p < *pnext));
        ++last; // last is restored to original value after previous decrement
        if (pnext == first) {
            std::reverse(first, last);
            return false;
        }
        // from 07531 27531 47531 67531
        // to   10357 31257 51347 71356
        pnext = last;
        while(!(*p < *--pnext)) { }
        std::iter_swap(p, pnext);
        ++p;
        std::reverse(p, last);
        return true;
    }
}

template <typename RandomIt, typename T = typename std::iterator_traits<RandomIt>::value_type>
void printPermute(RandomIt first, RandomIt mid, RandomIt last) {
    // print(first,mid,"",-2);
    // print(mid,last);
    // wait();
    assert(first < last);
    if (mid + 1 == last) {
        // print(first, last);
        print(first, last, "", 0);
    } else {
            printPermute(first, mid + 1, last);
        for (auto p = mid + 1; p != last; ++p) {
            // std::rotate(mid,p,p+1);
            std::iter_swap(mid, p);
            printPermute(first, mid + 1, last);
            // std::rotate(mid,mid+1,p+1);
        }
        std::rotate(mid, mid + 1, last);
    }
}

void printPermute(string str) {
    // cout << str;
    // wait();
    printPermute(&str[0],&str[0],&str[str.size()]);
}

template <typename RandomIt, typename T = typename std::iterator_traits<RandomIt>::value_type>
void printSelection(RandomIt seq, int sz, int k, std::vector<T> &buf) {
    if (k > 0 && sz >= k) {
        // cout << "*seq = " << *seq << ", sz = " << sz << ", k = " << k << ", buf.size() = " << buf.size() << endl;
        // wait();
        for (int i = 0; i <= sz - k; ++i) {
            buf.push_back(seq[i]);
            printSelection(seq + i + 1, sz - i - 1, k - 1, buf);
            buf.pop_back();
        }
    } else {
        print(buf);
    }
}

// template <typename RandomIt, typename T = typename std::iterator_traits<RandomIt>::value_type>
// void printSelection_loop(RandomIt seq, int sz, int k, std::vector<T> &buf) {
//     auto k_original = k, sz_original = sz;
//     int start = 0;
//     for (; ; ) {
//         for (int i = start; i <= sz; ++i) {
//             buf.push_back(
//     }
// }

template <typename RandomIt, typename T = typename std::iterator_traits<RandomIt>::value_type>
void selectMultiPos(RandomIt seq, int sz, int k) {
    assert(sz >= k && k >= 0);
    std::vector<T> buf;
    printSelection(seq, sz, k, buf);
    // printSelection_loop(seq, sz, k, buf);
}

template <typename ForwardIt, typename RandomIt>
void printPermuteWithDuplicate(ForwardIt first, RandomIt buf, RandomIt buf_mid, RandomIt buf_last);

/*
sorted part 1 | sorted part 2 | ....
12345 3
1 2345 2
1 2 345 1
1 2 3 45 0
1 2 4 35 0
1 2 5 34 0
*/

template <typename ForwardIt, typename RandomIt>
void helper(ForwardIt first, RandomIt buf, RandomIt buf_mid, RandomIt buf_last, int k, RandomIt new_buf_mid) {
    if (k > 0) {
        int sz = std::distance(buf_mid, buf_last);
        int i = 0;
        for (; i <= sz - k; ++i) {
            // if (i > 0) { std::iter_swap(new_buf_mid, buf_mid + i); }
            if (new_buf_mid != buf_mid || i > 0) { std::iter_swap(new_buf_mid, buf_mid + i); }
            **new_buf_mid = *first;
            helper(first + 1, buf, buf_mid + i + 1, buf_last, k - 1, new_buf_mid + 1);
            if (new_buf_mid != buf_mid || i > 0) { std::iter_swap(new_buf_mid, buf_mid + i); }
        }
    } else {
        printPermuteWithDuplicate(first, buf, new_buf_mid, buf_last);
    }
}

template <typename ForwardIt, typename RandomIt>
void selectMultiPosWithBuffer(ForwardIt first, RandomIt buf, RandomIt buf_mid, RandomIt buf_last, int k) {
    // cout << " k = " << k << endl;
    helper(first, buf, buf_mid, buf_last, k, buf_mid);
}

template <typename ForwardIt>
int unguardedCountEqual(ForwardIt first) {
    int k = 1;
    for (auto val = *first; *++first == val; ) { ++k; }
    return k;
}

template <typename ForwardIt>
int countEqual(ForwardIt first, ForwardIt last) {
    assert(first != last);
    if (first == last) return 0;
    int k = 1;
    auto val = *first;
    for (auto p = first; ++p != last && *p == val; ) { ++k; }
    return k;
}

template <typename ForwardIt, typename RandomIt>
void printPermuteWithDuplicate(ForwardIt first, RandomIt buf, RandomIt buf_mid, RandomIt buf_last) {
    if (buf_mid == buf_last) {
        ++stack_level;
        auto it = *std::min_element(buf, buf_last);
        int n = std::distance(buf, buf_last);
        // print(it, n);
        print(it, n, "", 0);
        // for (; buf != buf_last; ++buf) cout << *buf - it << " "; cout << endl;
        // print(buf, buf_last);
    } else {
        int sz = std::distance(buf_mid, buf_last);
        int k = countEqual(first, first + sz);
        // cout << " k = " << k << ", sz = " << sz << endl;
        selectMultiPosWithBuffer(first, buf, buf_mid, buf_last, k);
    }
}

template <typename ForwardIt, typename T = typename std::iterator_traits<ForwardIt>::value_type>
void printPermuteWithDuplicate(ForwardIt first, ForwardIt last) {
    std::vector<T> buf;
    for (auto p = first; p != last; ++p) { buf.push_back(*p); }
    std::vector<T*> address;
    for (auto it = &buf[0], it_end = it + buf.size(); it != it_end; ++it) { address.push_back(it); }
    printPermuteWithDuplicate(first, &address[0], &address[0], &address[0] + address.size());
}

template <typename ForwardIt, typename T = typename std::iterator_traits<ForwardIt>::value_type>
void printPermuteWithDuplicate_sorted_order(ForwardIt first, ForwardIt last) {
}

// two way to solve print permute with duplicates.
// 1. reduce from trailing vs grow from trailing
// 2. insertion sort approach vs selection sort approach.
// some other considerations
// 1. inplace vs buffer(eg, extra indirection)
// 2. inplace, increamental grow, eg, next_permutation, which handle duplicates well.
void printStringPermuteWithDuplicate(string str) {
    // printPermuteWithDuplicate_sorted_order(str.begin(), str.end());
    printPermuteWithDuplicate(str.begin(), str.end());
    // auto buf = str;
    // auto address = std::vector<char*>();
    // for (auto p = &buf[0], p_end = p + buf.size(); p != p_end; ++p) { address.push_back(p); }
    // printPermuteWithDuplicate(&str[0], &address[0], &address[0], &address[0] + address.size());
}

// http://marknelson.us/attachments/next-permutation/NextCombination.tgz
// next_combination Template
// Written by Thomas Draper
// Designed after next_permutation in STL
// Inspired by Mark Nelson's article http://www.dogma.net/markn/articles/Permutations/
// Start with a sorted container with thee iterators -- first, k, last
// After each iteration, the first k elements of the container will be 
// a combination. When there are no more combinations, the container
// will return to sorted order.
template <class BidirectionalIterator>
bool next_combination(BidirectionalIterator first, BidirectionalIterator k,
                      BidirectionalIterator last) {
   if ((first == last) || (k == first) || (k == last)) return false;
   BidirectionalIterator i = first;
   BidirectionalIterator ii = last;
   
   ++i;
   if (i == last) return false;
   i = last;
   --i;
   
   i = k;
   --ii;
   while (i != first){ 
      if (*--i < *ii){ // Search down to find first comb entry less than final entry
         BidirectionalIterator j = k;
         while(!(*i < *j)) j++; // Find swap position [good-1-high|good-2-low]
         iter_swap(i,j); // Swap [good-2-high|good-1-low]
         i++;j++;ii=k; // Move to rotation positions
         rotate(i,j,last); // Rotate [good-2-low-high-good-1]
         while(j!=last){ ++j; ++ii;} // Find high good position
         rotate(k,ii,last); // Rotate [good-2-(low/high>|good-1-<low/high)]
         return true;
      }
   }
   rotate (first,k,last);
   return false;
}

namespace test {
    void next_combination() {
        // int sep = ' ';
        // std::vector<int> v = {1,2,2,3,5};
        auto v = range(1,8);
        int k = 3;
        // auto v = range(1,10);
        // int k = 3;
        // std::vector<int> v = {1,2,2,4,4,6,8,9};
        // int k = 4;
        // std::string v = "ABCDEF";
        // int k = 2; // sep = 0;
        do {
            auto first = v.begin(), mid = first + k, last = first + v.size();
            print(first, mid, "",-1,"|");
            print(mid, last);
        } while (::nextCombination(v.begin(), v.begin() + k, v.end()));
        // } while (::next_combination(v.begin(), v.begin() + k, v.end()));
    }
    void next_permutation() {
        // std::vector<int> v = range(5);
        // std::vector<int> v = {1,1,1,2,2};
        std::vector<int> v = {1,2,2,3,5};
        // std::vector<int> v = {1,1,2};
        // std::vector<int> v = {2,2,1,1,1};
        // std::vector<int> v = {0,4,3,2,1};
        // std::vector<int> v = {2,3,1};
        // string v = "ABBC";
        print(v);
        while(true) {
            bool res = ::nextPermutation(v.begin(), v.end());
            // bool res = std::next_permutation(v.begin(), v.end());
            print(v);
            if (!res) break;
            // wait("test");
        }
        print(v);
    }
    void printStringPermuteWithDuplicate() {
        auto test_case = []() {
            std::vector<std::string> res;
            // res.push_back("AABBBCDEEE");
            // res.push_back("AABCCC");
            // res.push_back("AAABC");
            // res.push_back("ABCD");
            res.push_back("ABBC");
            // res.push_back("ABC");
            // res.push_back("AAABB");
            return res;
        };
        for (auto v: test_case()) {
            auto old_stack_level = stack_level;
            ::printStringPermuteWithDuplicate(v);
            // ::printPermute(v);
            cout << stack_level - old_stack_level << " permutations." << endl;
        }
    }
    void selectMultiPos() {
        std::vector<int> seq = range(1,6);
        ::selectMultiPos(seq.begin(), seq.size(), 3);
    }
}

namespace test {
    void printPermute() {
        auto test_case = []() {
            std::vector<std::string> res;
            res.push_back("ABCD");
            return res;
        };
        for (auto v: test_case()) {
            cout << v << endl;
            ::printPermute(v);
            cout << v << endl;
        }
    }
}

// 484 Find Permutation    http://www.cnblogs.com/grandyang/p/6366738.html    http://bookshadow.com/weblog/2017/01/22/leetcode-find-permutation/
// By now, you are given a secret signature consisting of character 'D' and 'I'. 'D' represents a decreasing relationship between two numbers, 'I' represents an increasing relationship between two numbers. And our secret signature was constructed by a special integer array, which contains uniquely all the different number from 1 to n (n is the length of the secret signature plus 1). For example, the secret signature "DI" can be constructed by array [2,1,3] or [3,1,2], but won't be constructed by array [3,2,4] or [2,1,3,4], which are both illegal constructing special string that can't represent the "DI" secret signature.
// On the other hand, now your job is to find the lexicographically smallest permutation of [1, 2, ... n] could refer to the given secret signature in the input.
// Example 1:
// Input: "I"
// Output: [1,2]
// Explanation: [1,2] is the only legal initial spectial string can construct secret signature "I", where the number 1 and 2 construct an increasing relationship.
// Example 2:
// Input: "DI"
// Output: [2,1,3]
// Explanation: Both [2,1,3] and [3,1,2] can construct the secret signature "DI", 
// but since we want to find the one with the smallest lexicographical permutation, you need to output [2,1,3]
// Note:
// The input string will only contain the character 'D' and 'I'.
// The length of input string is a positive integer and will not exceed 10,000
// discuss, https://discuss.leetcode.com/category/630/find-permutation
// TODO 
    vector<int> findPermutation(string s) {
        vector<int> res;
        return res;
    }

// 60. Permutation Sequence https://leetcode.com/problems/permutation-sequence/?tab=Description
// The set [1,2,3,…,n] contains a total of n! unique permutations.  
// By listing and labeling all of the permutations in order, Given n and k, return the kth permutation sequence.
// Note: Given n will be between 1 and 9 inclusive.
// TODO 
    string getPermutation(int n, int k) {
        if (n < 1 || n > 9) n = 0;
        string res;
        int fact = 1;
        for (int i = 1; i <= n; ++i) {
            res.push_back('0' + i);
            fact *= i;
        }
        auto first = res.begin();
        for (; n > 0; --n, ++first) {
            fact /= n;
            --k;
            auto d = first + k / fact;
        // print(res,"\t gres");
        // cout << "dist = " << d - first << ", k = " << k << ", fact = " << fact << endl;
            std::rotate(first, d, d+1);
            k %= fact;
            ++k;
        }
        return res;
    }
namespace test {
    void getPermutation() {
        std::vector<std::pair<int,int>> vec;
        // vec.push_back({4,8});
        // vec.push_back({4,7});
        // vec.push_back({3,2});
        vec.push_back({3,4});
        for (auto x: vec) {
            auto res = ::getPermutation(x.first, x.second);
            cout << "pair = " << x << ", res = " << res << endl;
        }
    }
}

int main() {
    // test::printPermute();
    // test::selectMultiPos();
    // test::next_permutation();
    // test::next_combination();
    // test::merge();
    // test::printStringPermuteWithDuplicate();
    test::getPermutation();
}

