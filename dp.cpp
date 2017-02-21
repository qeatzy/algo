#include "utility.h"
// https://leetcode.com/tag/dynamic-programming/

// 525. Contiguous Array. see also 121, 325   https://leetcode.com/contest/leetcode-weekly-contest-20/problems/contiguous-array/
// other interesting point of view. eg,
// Think of 1 as push, 0 as pop, the whole stack goes up and down. Keep tracking the height of the stack, find which height reoccurs after longest interval.  https://discuss.leetcode.com/topic/79936/python-5-lines-o-n-solution
// Java Divide-and-conquer Time = O(n log n), peak space = O(n).  https://discuss.leetcode.com/topic/79935/java-divide-and-conquer
// Visual Explanation.  https://discuss.leetcode.com/topic/80056/python-o-n-solution-with-visual-explanation
// C++ O(N), array instead of unordered_map.  https://discuss.leetcode.com/topic/79980/c-o-n-array-instead-of-unordered_map
    int findMaxLength(vector<int>& nums) {
        std::map<int,int> low{{0,-1}};
        int sum = 0;
        int res = 0;
        for (int i = 0; size_t(i) < nums.size(); ++i) {
            sum += 2 * nums[i] - 1;
            if (low.find(sum) == low.end()) {
                low[sum] = i;
            } else {
                // cout << "i = " << i << ", low[sum] = " << low[sum] << endl;
                res = std::max(res, i - low[sum]);
            }
        }
        return res;
    }
namespace test {
    void findMaxLength() {
        auto test_case = []() {
            std::vector<std::vector<int>> res;
            res.push_back({0,1,0});
            res.push_back({0,1,0,1});
            return res;
        };
        for (auto &v: test_case()) {
            auto res = ::findMaxLength(v);
            cout << "res = " << res << ", ";
            print(v);
        }
    }
}

// 121. Best Time to Buy and Sell Stock    https://leetcode.com/problems/best-time-to-buy-and-sell-stock/?tab=Description
// -- at most one transaction to gain max profit.
    int maxProfit(vector<int>& prices) {
        int smallest = INT_MAX;
        int res = 0;
        for (auto x: prices) {
            res = std::max(res, x - smallest);
            smallest = std::min(smallest, x);
        }
        return res;
    }

namespace test {
    void maxProfit() {
        auto test_case = []() {
            std::vector<std::vector<int>> res;
            res.push_back({7, 1, 5, 3, 6, 4});
            res.push_back({7, 6, 4, 3, 1});
            return res;
        };
        for (auto &v: test_case()) {
            auto res = ::maxProfit(v);
            cout << "res = " << res << ", ";
            print(v);
        }
    }
}

// 325. Maximum Size Subarray Sum Equals k

// TODO 
// easy problem in dt/lc. (or should be categoried under fc, flow control?)
// 520. Detect Capital   https://leetcode.com/problems/detect-capital/?tab=Description
// python builtin.  return word.isupper() or word.islower() or word.istitle()
// a slightly different version similar to below.  https://discuss.leetcode.com/topic/79912/3-lines/2
    bool detectCapitalUse(string word) {
        if (word.size() < 2) return true;
        auto first = word.begin(), last = word.end();
        if (isupper(*first)) { ++first; }
        for (const auto t = isupper(*first); ++first != last && t == isupper(*first);) {}
        return first == last;
    }

int main() {
    test::init();
    // test::findMaxLength();
    test::maxProfit();
}