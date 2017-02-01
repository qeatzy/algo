#include <unordered_map>
#include "utility.h"
// int DEBUG = 0;
// int DEBUG = 1;
int DEBUG = 2;
// for even K, reduce to 2 sum for a larger list (sum of K/2). for odd K reduce to 3 sum of two list (eg, K/2 and original input). http://cs.stackexchange.com/questions/2973/generalised-3sum-k-sum-problem
// https://leetcode.com/problems/3sum/
// https://discuss.leetcode.com/topic/8125/concise-o-n-2-java-solution
// https://discuss.leetcode.com/post/74515  -- not worth to complicate the intent of your code. -- and is there indeed any performance gain?
// 4 sum. multimap? http://stackoverflow.com/questions/14732277/quadratic-algorithm-for-4-sum

template <typename Iterator, typename T = typename std::iterator_traits<Iterator>::value_type>
inline Iterator next_that_differ(Iterator first, Iterator last) {
    if (first != last) {
        for (auto val = *first; ++first != last && *first == val;) {}
    }
    return first;
}
template <typename Iterator, typename T = typename std::iterator_traits<Iterator>::value_type>
inline Iterator unguarded_next_that_differ(Iterator first) {
    for (auto val = *first; *++first == val;) {}
    return first;
}

    template <typename BidirectIt, typename T = typename std::iterator_traits<BidirectIt>::value_type>
    inline T next_floor_half(BidirectIt first, BidirectIt last, T floor_half, T target) {
        // assert(first != last && *first == floor_half && last[-1] >= target - floor_half);
        ++first;
        if (floor_half + floor_half == target) return *first;
        // assert(last[-1] > floor_half);
        else return *std::upper_bound(first, last, floor_half);
    }
    template <typename BidirectIt, typename T = typename std::iterator_traits<BidirectIt>::value_type>
    inline void add_match_for_two_sum(BidirectIt first, BidirectIt last, T floor_half, T target, std::vector<std::vector<T>> &res) {
        for (;;) {
            while (*first + *last < target) ++first;
            if (!(*first < floor_half)) break;  // or use first < high instead?
            if (*first + *last == target) res.push_back({*first, *last});
            for (auto val = *last; *--last == val;) {}
        }
        if (*first == floor_half && first != last && next_floor_half(first, last, floor_half, target) == target - floor_half) res.push_back({floor_half, target - floor_half});
    }
/*
   leetcode two sum sorted. https://leetcode.com/problems/two-sum-ii-input-array-is-sorted/
 */
template <int target = 0, typename BidirectIt, typename T = typename std::iterator_traits<BidirectIt>::value_type>
std::vector<std::vector<T>> two_sum_sorted(BidirectIt first, BidirectIt last) {
    if (DEBUG > 1) { cout << "two Sum: "; print(first, last, "", -2); cout << "target = " << target << endl; }
    std::vector<std::vector<T>> res;
    if (first == last || --last == first) return res;
    constexpr auto floor_half = target / 2 - (target < 0 && target % 2 != 0) ;
    if (*last < target - floor_half) return res;
    // loop invariants: *first <= floor_half <= *last. first and last are mutually guarded.
    // exit-loop-condition: first >= floor_half after first inner loop.
    add_match_for_two_sum(first, last, floor_half, target, res);
    return res;
}
template <typename BidirectIt, typename T = typename std::iterator_traits<BidirectIt>::value_type>
std::vector<std::vector<T>> two_sum_sorted(BidirectIt first, BidirectIt last, const T target) {
    if (DEBUG > 1) { cout << "two Sum: "; print(first, last, "", -2); cout << "target = " << target << endl; }
    std::vector<std::vector<T>> res;
    if (first == last || --last == first) return res;
    const auto floor_half = target / 2 - (target < 0 && target % 2 != 0) ;
    if (*last < target - floor_half) return res;
    // loop invariants: *first <= floor_half <= *last. first and last are mutually guarded.
    // exit-loop-condition: first >= floor_half after first inner loop.
    add_match_for_two_sum(first, last, floor_half, target, res);
    return res;
}
    template <int sum_of_three, typename BidirectIt, typename T = typename std::iterator_traits<BidirectIt>::value_type>
    void two_sum_sorted_for_three_sum(BidirectIt first, BidirectIt last, const T third, std::vector<std::vector<T>> &res) {
        if (DEBUG > 1) { cout << "two Sum: "; print(first, last, "", -2); cout << "third = " << third << endl; }
        if (first == last || --last == first) return;
        auto target = sum_of_three - third;
        const auto floor_half = target / 2 - (target < 0 && target % 2 != 0) ;
        if (*last < target - floor_half) return;
        // loop invariants: *first <= floor_half <= *last. first and last are mutually guarded.
        // exit-loop-condition: first >= floor_half after first inner loop.
        for (;;) {
            while (*first + *last < target) ++first;
            if (!(*first < floor_half)) break;
            if (*first + *last == target) res.push_back({third, *first, *last});
            for (auto val = *last; *--last == val;) {}
        }
        if (*first == floor_half && first != last && next_floor_half(first, last, floor_half, target) == target - floor_half) res.push_back({third, floor_half, target - floor_half});
        // if (*first == floor_half && first != last && *++first == target - floor_half) res.push_back({third, floor_half, target - floor_half});
    }
    template <int base_index = 0, typename BidirectIt, typename T = typename std::iterator_traits<BidirectIt>::value_type>
    std::vector<std::vector<int>> two_sum_index_sorted(BidirectIt first, BidirectIt last, const T target) {
        if (DEBUG) { cout << "two Sum: "; print(first, last, "", -2); cout << "target = " << target << endl; }
        std::vector<std::vector<T>> res;
        if (first == last || --last == first) return res;
        int sz = std::distance(first, last);
        int left = base_index, right = base_index + sz - 1;
        const auto floor_half = target / 2 - (target < 0 && target % 2 != 0) ;
        if (*last < target - floor_half) return res;
        // loop invariants: *first <= floor_half <= *last. first and last are mutually guarded.
        // exit-loop-condition: first >= floor_half after first inner loop.
        for (;;) {
            while (*first + *last < target) { ++first; ++left; }
            if (!(*first < floor_half)) break;
            if (*first + *last == target) res.push_back({left, right});
            auto val = *last;
            for (--last, --right; *last == val; --last, --right) {}
        }
        if (*first == floor_half && first != last && *++first + floor_half == target) res.push_back({left, left + 1});
        return res;
    }
    vector<int> twoSum(vector<int>& nums, int target) { // https://leetcode.com/problems/two-sum/
        print(nums); cout << "target = " << target << endl;
        auto copy = nums;
        std::sort(copy.begin(), copy.end());
        auto values = two_sum_sorted(copy.begin(), copy.end(), target)[0];
        int i1 = std::find(nums.begin(), nums.end(), values[0]) - nums.begin();
        auto offset = (values[0] == values[1])? 1 + i1 : 0;
        int i2 = std::find(nums.begin() + offset, nums.end(), values[1]) - nums.begin();
        if (i1 > i2) std::swap(i1, i2);
        return std::vector<int> {i1,i2};
    }
    vector<int> twoSum_hash(vector<int>& nums, int target) { // https://leetcode.com/problems/two-sum/
        std::unordered_map<int, int> hash;
        for (int i = 0; size_t(i) < nums.size(); ++i) {
            auto p = hash.find(nums[i]);
            if (p == hash.end()) { hash.insert({target - nums[i], i}); }
            else return vector<int> {p->second, i};
        }
        return vector<int> {-1,-1};
    }

namespace test {
    void twoSum() {
        auto test_case = []() {
            std::vector<std::vector<int>> res;
            res.push_back({2, 7, 11, 15,   9});
            res.push_back({0,0,0,2,3,9,9,   5});
            res.push_back({0,0,1,  1});
            res.push_back({0,1,  1});
            res.push_back({1,1,  2});
            res.push_back({-2,0,0,  2});
            res.push_back({-1,0,2,  0});
            res.push_back({3,3,   6});
            // res.push_back({3,2,4,   6}); // not sorted
            return res;
        };
        for (auto v: test_case()) {
            // auto target = v.back(); v.pop_back(); auto res = ::twoSum(v, target);
            // auto target = v.back(); v.pop_back(); auto res = ::twoSum_hash(v, target);
            auto res = ::two_sum_sorted(v.begin(), v.end() - 1, v.back());
            // auto res = ::two_sum_index_sorted(v.begin(), v.end() - 1, v.back());
            // auto res = ::two_sum_index_sorted<1>(v.begin(), v.end() - 1, v.back());
            print(res);
        }
    }
}


std::vector<std::vector<int>> threeSum_old(std::vector<int> &nums) {
        std::vector<std::vector<int>> res;
        if (nums.empty()) return res;
        std::vector<int> positive, negative;
        int nzero = 0;
        for (auto val: nums) {
            if (val == 0) { ++nzero; }
            else if (val < 0) { negative.push_back(-val); }
            else { positive.push_back(val); }
        }
        if (nzero >= 3) { res.push_back(std::vector<int> {0,0,0}); }
        if (negative.empty() || positive.empty()) { return res; }
        auto pseudoUniqify = [](std::vector<int> &vec) {
            std::vector<int> tmp;
            tmp.swap(vec);
            std::sort(tmp.begin(), tmp.end());
            for (auto it = tmp.begin(); it != tmp.end(); ) {
                auto val = *it;
                vec.push_back(val);
                if (++it != tmp.end() && *it == val) {
                    vec.push_back(val);
                    for (; ++it != tmp.end() && *it == val; ) {}
                }
            }
        };
        // print(negative);
        // print(positive);
        pseudoUniqify(negative);
        pseudoUniqify(positive);
        // print(negative);
        // print(positive);
        // wait();
        typedef decltype(negative.begin()) It;
        auto findAndAppend = [](std::vector<std::vector<int>> &dest, It lfirst, It llast, It rfirst, It rlast, int shift, bool twonega) {
            assert(rfirst < rlast);
            // cout << "twonega = " << twonega << ", shift = " << shift << endl;
            // print(lfirst, llast);
            // print(rfirst, rlast);
            // wait();
            decltype(shift) max = rlast[-1];
            if (max < *lfirst + shift) return rlast;
            // cout << "max = " << max << endl;
            for (; *rfirst < *lfirst + shift; ++rfirst) {}
            auto res = rfirst;
            for (; lfirst != llast && *lfirst + shift < max; ) {
                auto sum = *lfirst + shift;
                // cout << "sum = " << sum << endl;
                // wait();
                for (; *rfirst < sum; ++rfirst) {}
                if (*rfirst == sum) {
                    if (twonega) dest.push_back(std::vector<int> {-*lfirst, -shift, *rfirst});
                    else dest.push_back(std::vector<int> {shift, *lfirst, -*rfirst});
                }
                if (++lfirst != llast && lfirst[-1] == lfirst[0]) { ++lfirst; }
            }
            if (lfirst != llast && *lfirst + shift == max) {
                if (twonega) dest.push_back(std::vector<int> {-*lfirst, -shift, max});
                else dest.push_back(std::vector<int> {shift, *lfirst, -max});
            }
            return res;
        };
        if (nzero > 0) { findAndAppend(res, negative.begin(), negative.end(), positive.begin(), positive.end(), 0, true); }
        for (auto it = negative.begin(), it_end = negative.end(), rit = positive.begin(), rit_end = positive.end(); it < it_end && rit < rit_end; ) {
            auto val = *it;
            ++it;
            if (it == it_end) break;
            rit = findAndAppend(res, it, it_end, rit, rit_end, val, true);
            if (*it == val) { ++it; }
        }
        for (auto it = positive.begin(), it_end = positive.end(), rit = negative.begin(), rit_end = negative.end(); it < it_end && rit < rit_end; ) {
            auto val = *it;
            // cout << "*it = " << *it << ", val = " << val << endl;
            // wait();
            ++it;
            if (it == it_end) break;
            rit = findAndAppend(res, it, it_end, rit, rit_end, val, false);
            if (*it == val) { ++it; }
        }
        // std::sort(res.begin(), res.end());
        return res;
}

    // https://discuss.leetcode.com/post/100423
    // https://discuss.leetcode.com/topic/8125/concise-o-n-2-java-solution
    template <int sum_of_three, typename BidirectIt, typename T = typename std::iterator_traits<BidirectIt>::value_type>
    std::vector<std::vector<int>> threeSum_newer_sorted(BidirectIt first, BidirectIt last) {
        std::vector<std::vector<int>> res;
        // DEBUG = 1;
        // if (DEBUG) { cout << "three Sum: "; print(first, last); }
        if (first == last || --last == first || --last == first) return res;
        constexpr auto floor_one_third = sum_of_three / 3 - (sum_of_three < 0 && sum_of_three % 3 != 0);
        auto first_threshold = std::upper_bound(first, last, floor_one_third);
        auto start_val = sum_of_three - *last;
        ++last;
        if (*last < floor_one_third) return res;
        start_val -= *last;
        first = std::lower_bound(first, first_threshold, start_val);
        // if (DEBUG) cout << "start_val = " << *first << ", *first_threshold = " << *first_threshold << endl;
        // ++last;
        for (; first != first_threshold;) {    // what if *last == floor_one_third??
            const auto val = *first;
            const auto target = sum_of_three - *first;
            const auto floor_half = target / 2 - (target < 0 && target % 2 != 0) ;
            // if (DEBUG > 1) cout << "val = " << val << ", floor_one_third = " << floor_one_third << endl;
            if (*last + *last < target) continue;
            ++first;
            for (auto low = first, high = last; low < high; ) {
                auto sum = *low + *high;
                // if (DEBUG) {
                //     if (low == first
                //             // && target == 4
                //             ) {
                //         cout << "target = " << target << endl;
                //         print(first - 1, last + 1, "2 sum");
                //     }
                //     if (DEBUG > 1 || sum == target) cout << " sum = " << sum  << ", *low = " << *low << ", *high = " << *high << endl;
                // }
                if (sum == target) {    // https://discuss.leetcode.com/post/74515
                    res.push_back({val, *low, *high});
                    if (*low == floor_half) break;
                }
                if (sum <= target) for (auto val_low = *low; *++low == val_low; ) {}
                else for (auto val_high = *high; *--high == val_high; ) {}
                // if (sum == target) {
                //     res.push_back({val, *low, *high});
                //     if (*low == floor_half) break;
                //     for (auto val_high = *high; *--high == val_high; ) {}
                //     for (auto val_low = *low; *++low == val_low; ) {}
                // } else if (sum < target) { ++low; }
                // else { --high; }
            }
            for (; first != first_threshold && *first == val; ++first) {}
        }
        return res;
    }

template <int sum_of_three = 0>
std::vector<std::vector<int>> threeSum_newer(std::vector<int> &nums) {
        std::vector<std::vector<int>> res;
        std::sort(nums.begin(), nums.end());
        if (nums.empty() || nums[0] >= 0 || nums.back() <= 0) {
            auto sz = nums.size();
            if (sz >= 3) {
                if ((nums[0] == 0 && nums[2] == 0) || (nums[sz-1] == 0 && nums[sz-3] == 0)) { res.push_back({0,0,0}); }
            }
        } else for (size_t i = 0; nums[i] <= 0; ) {
            auto val = nums[i];
            for (size_t low = i + 1, high = nums.size() - 1; low < high; ) {
                auto sum = val + nums[low] + nums[high];
                if (sum == 0) {
                    res.push_back({nums[i], nums[low], nums[high]});
                    for (auto val_low = nums[low]; ++low < high && nums[low] == val_low; ) {}
                    for (auto val_high = nums[high]; --high > low && nums[high] == val_high; ) {}
                } else if (sum < 0) { ++low; }
                else { --high; }
            }
            for (; ++i < nums.size() && nums[i] == val; ) {}
        }
        return res;
}

    template <int sum_of_three, typename BidirectIt, typename T = typename std::iterator_traits<BidirectIt>::value_type>
    std::vector<std::vector<T>> three_sum_sorted(BidirectIt first, BidirectIt last) {
        std::vector<std::vector<T>> res;
        if (DEBUG) { cout << "three Sum: "; print(first, last); }
        if (first == last || --last == first || --last == first) return res;
        constexpr auto floor_one_third = sum_of_three / 3 - (sum_of_three < 0 && sum_of_three % 3 != 0);
        ++last;
        if (*last < floor_one_third) return res;
        ++last;
        // similar to two sum, first is guarded by last item in [first, last).
        for (;;) {
            auto val = *first;
            ++first;
            two_sum_sorted_for_three_sum<sum_of_three>(first, last, val, res);
            if (DEBUG > 1) cout << "val = " << val << ", floor_one_third = " << floor_one_third << endl;
            if (!(val < floor_one_third)) break;
            for (; *first == val; ++first) {}
        }
        return res;
    }

    template <int sum_of_three = 0>
    std::vector<std::vector<int>> threeSum(std::vector<int> &nums) {
        std::vector<std::vector<int>> res;
        std::sort(nums.begin(), nums.end());
        // return three_sum_sorted<sum_of_three>(nums.begin(), nums.end());
        return threeSum_newer_sorted<sum_of_three>(nums.begin(), nums.end());
    }

vector<vector<int>> fourSum(vector<int>& nums, int target) {
    std::sort(nums.begin(), nums.end());
    std::vector<std::vector<int>> res;
    for (int a = 0, a_end = nums.size() - 3; a < a_end; ) {
        if (a == 0 || nums[a] != nums[a-1]) {
        }
    }
    return res;
}

namespace test {
    void threeSum() {
        auto test_case = []() {
            std::vector<std::vector<int>> res;
            res.push_back({-4,-2,-2,-2,0,1,2,2,2,3,3,4,4,6,6});
            // res.push_back({2,0,-2,-5,-5,-3,2,-4});
            // res.push_back({-1,0,1,2,-1,-4});
            // res.push_back({1,1,-2});
            // res.push_back({-2,-3,0,0,-2});
            // res.push_back({-1,0,1});
            // res.push_back({-1,0,1,0});
            // res.push_back({1,2,-2,-1});
            // res.push_back({-1,0,1,2,-1,-4});
            // res.push_back({3,-2,1,0});
            // res.push_back({1,1,-2});
            return res;
        };
        for (auto v: test_case()) {
            // print(v);
            auto res = ::threeSum(v);
            // auto res = ::threeSum_old(v);
            print(res);
        }
    }
}

std::vector<int> divmod(std::vector<int> v) { return std::vector<int> {v[0]/v[1], v[0]%v[1]}; }
std::vector<int> divmod_floor(std::vector<int> v) {
    int x = v[0], y = v[1];
    int c = ((x < 0) - (y < 0)) != 0;   // (1 for "+ -", -1 for "- +", else 0) != 0
    int div = x / y, mod = x % y;
    div -= c;
    mod += c * y;
    return std::vector<int> {div, mod};
}
namespace test {
    // x = [[11,3],[-11,3],[11,-3],[-11,-3]]
    // print([divmod(i[0],i[1]) for i in x])
    void divmod() {
        auto test_case = []() {
            std::vector<std::vector<int>> res;
            res.push_back({11,3});
            res.push_back({-11,3});
            res.push_back({11,-3});
            res.push_back({-11,-3});
            return res;
//  11 divmod_floor  3 ==  3  2
// -11 divmod_floor  3 == -4  1
//  11 divmod_floor -3 == -4 -1
// -11 divmod_floor -3 ==  3 -2
//  11 divmod  3 ==   3  2
// -11 divmod  3 == --3 -2
//  11 divmod -3 == --3  2
// -11 divmod -3 ==   3 -2
        };
        for (auto v: test_case()) {
            cout << v[0] << " divmod " << v[1] << " == ";
            auto res = ::divmod(v);
            // auto res = ::divmod_floor(v);
            print(res);
        }
    }
}

int main() {
    test::threeSum();
    // test::twoSum();
    // test::divmod();
}
