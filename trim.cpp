#include "utility.h"
#include <stack>
// trim the tree traversal process.  剪枝回溯法的递归和非递归实现
// the key point is fail fast, make top level of tree small, thus reduce node in the tree been traversed.

// Permutation - 排列 https://github.com/niangaotuantuan/algorithm-excercise/blob/master/backtracking/permutation.md
// Blog | 逍遥郡    回溯法 http://blog.jqian.net/post/backtracking.html
// HDU1010 Tempter of the Bone(回溯 + 剪枝)  这道题剪枝特别重要。  http://www.cnblogs.com/Ash-ly/p/5398734.html
// 回溯算法--背包问题 0-1背包问题 旅行商问题 n皇后问题 http://www.voidcn.com/blog/sinat_24436879/article/p-3380386.html
// 搜索算法的剪枝优化 http://blog.163.com/zhoumhan_0351/blog/static/3995422720084190493758/
// 回溯全集   https://segmentfault.com/a/1190000006121957
// 1. 打印任意合法的n对括号： 2. Vertex Cover (NPC问题) ，图G中找一个顶点的最小子集，覆盖图的所有边。 https://siyangxie.wordpress.com/2009/11/07/学习了backtrack回溯法/


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
    merge(left, mid, right, last);
    // inplace_merge(left, mid, right, last);
    return true;
}

// http://www.lintcode.com/en/problem/subsets/
    vector<vector<int> > subsets(vector<int> &nums) {
        std::sort(nums.begin(),nums.end());
        vector<vector<int> > res{{}};
        auto first = nums.begin(), mid = first + 1, last = nums.end();
        for (; mid <= last; ++mid) {
            do {
                res.emplace_back(first, mid);
            } while (nextCombination(first, mid, last));
        }
        return res;
    }
// http://www.lintcode.com/en/problem/subsets-ii/
    vector<vector<int> > subsetsWithDup(const vector<int> &S) {
        auto nums = S;
        std::sort(nums.begin(),nums.end());
        vector<vector<int> > res{{}};
        auto first = nums.begin(), mid = first + 1, last = nums.end();
        for (; mid <= last; ++mid) {
            do {
                res.emplace_back(first, mid);
            } while (nextCombination(first, mid, last));
        }
        return res;
    }
namespace test {
    void subsets() {
        auto test_case = []() {
            std::vector<std::vector<int>> res;
            res.push_back({1,2,2});
            return res;
        };
        for (auto &v: test_case()) {
            // auto res = ::subsetsWithDup(v);
            auto res = ::subsets(v);
            print(v,"v");
            print(res,"res");
        }
    }
}

// https://siyangxie.wordpress.com/2009/11/07/学习了backtrack回溯法/
void printParenthes(int N, int left, int right, std::vector<char> &stk){
    if(left == N && right == N){
        print(stk);
        return;
    }
    if(left>right){
        stk.push_back(')');
        printParenthes(N, left,right+1, stk);
        stk.pop_back();
    }
    if(left < N){
        stk.push_back('(');
        printParenthes(N, left+1, right, stk);
        stk.pop_back();
    }   
}
namespace test {
    void printParenthes() {
        std::vector<char> stk;
        ::printParenthes(4,0,0,stk);
    }
}

// LeeCode Combination Sum 剪枝回溯法的递归和非递归实现  http://www.2cto.com/kf/201312/262306.html
// 39. Combination Sum     https://leetcode.com/problems/combination-sum/?tab=Description
// a set(unique) of candidates, and target, all positive. Each number could be used many times.
// A general approach to backtracking questions in Java.  https://discuss.leetcode.com/topic/46161/a-general-approach-to-backtracking-questions-in-java-subsets-permutations-combination-sum-palindrome-partitioning
// Iterative Java DP solution https://discuss.leetcode.com/topic/8200/iterative-java-dp-solution
// Dynamic Programming Solution https://discuss.leetcode.com/topic/4454/dynamic-programming-solution
    void combinationSum(std::vector<int> &candidates, int target, std::vector<std::vector<int> > &res, std::vector<int> &combination, int begin) {
        // https://discuss.leetcode.com/topic/14654/accepted-16ms-c-solution-use-backtracking-easy-understand
        // no loop, but instead reusing recusion instead. neater code, might be slower if
        // function call is more costly, eg, in python.
		if  (!target) {
			res.push_back(combination);
			return;
		}
        for (int i = begin; size_t(i) != candidates.size() && target >= candidates[i]; ++i) {
            combination.push_back(candidates[i]);
            combinationSum(candidates, target - candidates[i], res, combination, i);
            combination.pop_back();
        }
    }
    vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
        vector<vector<int>> res;
        auto first = candidates.begin(), last = candidates.end();
        std::sort(first,last);
        last = std::upper_bound(first, last, target) - 1;
        if (last < first) return res;
        auto comb = decltype(res)::value_type{};
        // combinationSum(candidates, target, res, comb, 0);
        // return res;
        std::function<void(decltype(first),int)> helper;
        helper = [&helper,&res,&comb,first](decltype(first) last, int target) {
            auto const val = *last;
            // cout << "helper: " << "val = " << val << ", target = " << target << endl;
            // if (target < val) return;
            auto const sz = comb.size();
            int cnt = target / val;
            if (target == 0) {
                res.push_back(comb);
                return;
            }
            if (first == last) {
                if (target % val == 0) {
                    comb.resize(sz + cnt, val);
                    res.push_back(comb);
                    comb.resize(sz);
                }
                return;
            }
            --last;
            for (;;) {
                helper(last, target);
                if (cnt-- <= 0) break;
                target -= val;
                comb.push_back(val);
            }
            comb.resize(sz);
        };
        helper(last, target);
        return res;
    }

// 40. Combination Sum II    https://leetcode.com/problems/combination-sum-ii/?tab=Description
// a collection(non-unique) of candidates, and target, all positive. Each number could be used at most once.
namespace test {
    void combinationSum() {
        auto test_case = []() {
            std::vector<std::vector<int>> res;
            res.push_back({2, 3, 6, 7,  7});
            return res;
        };
        for (auto &v: test_case()) {
            auto target = v.back();
            v.pop_back();
            print(v,"v");
            auto res = ::combinationSum(v, target);
            print(res,"res");
        }
    }
}

// 322. Coin Change https://leetcode.com/problems/coin-change/?tab=Description
// TODO  Fast Python BFS Solution   https://discuss.leetcode.com/topic/32589/fast-python-bfs-solution
// Python, 11-line, 280ms, DFS with early termination, 99% up.  https://discuss.leetcode.com/topic/36306/python-11-line-280ms-dfs-with-early-termination-99-up
// 8 ms,15 lines concise DFS solution[c++] https://discuss.leetcode.com/topic/47774/8-ms-15-lines-concise-dfs-solution-c
// Java recursive solution 3ms -- early termination and also using loop -- https://discuss.leetcode.com/topic/32527/java-recursive-solution-3ms
    void coinChange(vector<int>& coins, int r, int amount, int cnt, int &res) { // early termination, accumulate with &res, optimal.
        if (amount < 0) return;
        if (amount == 0) {
            // res = std::min(res, cnt);
            res = cnt;
            return;
        }
        for (; size_t(r) < coins.size(); ++r) {
            if (res <= cnt || amount / coins[r] >= (res - cnt)) continue; // decreasing coins needed, since for >=, lhs - rhs increase.
            coinChange(coins, r, amount - coins[r], cnt + 1, res);
        }
    }
    int coinChange_dp(vector<int>& coins, int amount) {
        std::vector<int> cnt(1 + amount, -1);
        cnt[0] = 0;
        for (auto x: coins) { cnt[x] = 1; }
        auto smallest = *std::min_element(coins.begin(), coins.end());
        for (int i = smallest * 2; i <= amount; ++i) {
            for (auto x: coins) {
                if (i > x && cnt[i-x] > 0) {
                    if (cnt[i] > 0) {
                        cnt[i] = std::min(cnt[i], 1 + cnt[i-x]);
                    } else {
                        cnt[i] = 1 + cnt[i-x];
                    }
                }
            }
        }
        return cnt[amount];
    }
    int coinChange_dp_v2(vector<int>& coins, int amount) {
        std::sort(coins.begin(), coins.end());
        coins.resize(std::upper_bound(coins.begin(), coins.end(), amount) - coins.begin());
        std::vector<int> cnt(1 + amount, -1);
        cnt[0] = 0;
        for (auto x: coins) { cnt[x] = 1; }
        auto smallest = *std::min_element(coins.begin(), coins.end());
        for (auto x: coins) {
            for (int i = smallest + x; i <= amount; ++i) {
                if (cnt[i-x] > 0) {
                    if (cnt[i] > 0) {
                        cnt[i] = std::min(cnt[i], 1 + cnt[i-x]);
                    } else {
                        cnt[i] = 1 + cnt[i-x];
                    }
                }
            }
        }
        return cnt[amount];
    }
    int coinChange_dp_v3(vector<int>& coins, int amount) {
        std::sort(coins.begin(), coins.end());
        coins.resize(std::upper_bound(coins.begin(), coins.end(), amount) - coins.begin());
        std::vector<int> cnt(1 + amount, -1);
        cnt[0] = 0;
        for (auto x: coins) { cnt[x] = 1; }
        auto smallest = *std::min_element(coins.begin(), coins.end());
        for (int i = smallest; i <= amount; ++i) {
            if (cnt[i] <= 0) continue;
            int next;
            for (auto it = coins.begin(); it != coins.end() && (next = *it + i) <= amount; ++it) {
                if (cnt[next] > 0) {
                    cnt[next] = std::min(cnt[next], 1 + cnt[i]);
                } else {
                    cnt[next] = 1 + cnt[i];
                }
            }
        }
        return cnt[amount];
    }
    int coinChange(vector<int>& coins, int amount) {
        // return coinChange_dp_v3(coins, amount);
        return coinChange_dp_v2(coins, amount);
        // return coinChange_dp(coins, amount);
        int res = INT_MAX;
        std::sort(coins.begin(), coins.end(), std::greater<int>()); // TODO why decreasing sort is needed?
        // std::sort(coins.begin(), coins.end());
        coinChange(coins, 0, amount, 0, res);
        return (res == INT_MAX)? -1 : res;
        // return (res == INT_MAX && INT_MAX != amount * coins[0])? -1 : res;
    }

// 518. Coin Change 2 https://leetcode.com/problems/coin-change-2/?tab=Description
// You are given coins of different denominations and a total amount of money. Write a function to compute the number of combinations that make up that amount. You may assume that you have infinite number of each kind of coin. 
    void change(int amount, vector<int>& coins, int i, int &res) {
        if (amount == 0) {
            ++res;
            return;
        }
        if (amount < 0 || size_t(i) == coins.size()) return;
        if (size_t(i + 1) == coins.size()) {
            res += amount % coins[i] == 0;
            return;
        }
        for (int val = coins[i], x = 0; x <= amount; x += val) {
                // size_t(i) < coins.size(); ++i) {
            change(amount - x, coins, i + 1, res);
        }
    }
    int change_mul(int amount, vector<int>& coins, int i) {
        if (amount == 0) {
            return 1;
        }
        if (amount < 0 || size_t(i) == coins.size()) return 0;
        if (size_t(i + 1) == coins.size()) {
            return amount % coins[i] == 0;
        }
        int res = 0;
        for (int val = coins[i], x = 0; x <= amount; x += val) {
                // size_t(i) < coins.size(); ++i) {
            res += change_mul(amount - x, coins, i + 1);
        }
        return res;
    }
    int change_dp(int amount, vector<int>& coins) {
        std::vector<int> dp(1 + amount, 0);
        dp[0] = 1;
        for (auto x: coins) {
            dp[x] = 1;
        }
        for (int i = 1; i <= amount; ++i) {
            for (auto x: coins) {
                if (i > x) {
                    dp[i] += dp[i-x];
                }
            }
        }
        return dp[amount];
    }
    int change_dp_init(int amount, vector<int>& coins) {
        auto first = coins.begin(), last = coins.end(), mid = first + (last - first) / 2;
        ++mid;
        return -1;
    }
    // TODO divide and conquer, two half or three half, then two sum.
    int change(int amount, vector<int>& coins) {
        std::sort(coins.begin(), coins.end(), std::greater<int>());
        // std::sort(coins.begin(), coins.end());
        return change_dp(amount, coins);
        return change_mul(amount, coins, 0);
        int res = 0;
        change(amount, coins, 0, res);
        return res;
    }

namespace test {
    void change() {
        auto test_case = []() {
            std::vector<std::vector<int>> res;
            // res.push_back({1,2,5,  5});
            res.push_back({2,3  ,100});
            // res.push_back({2,3  ,1000});
            // res.push_back({2,3  ,2000});
            // res.push_back({2,3  ,5000});
            // res.push_back({3,5,7,8,9,10,11,  500});
            // res.push_back({3,5,7,8,9,10,11,  1000});
            // res.push_back({1,2,5,  11});
            // // res.push_back({346,29,395,188,155,109,   9401});
            // // res.push_back({1,2,5,10,  18});
            // res.push_back({3,7,405,436, 8839});
            // res.push_back({284,260,393,494, 7066});
            // res.push_back({216,94,15,86, 5372});
            // res.push_back({37,233,253,483, 7163});
            // res.push_back({357,239,73,52, 9832});
            // res.push_back({431,62,88,428, 9084});
            // res.push_back({1, 0});
            // res.push_back({1, 2147483647});
            return res;
        };
        for (auto &v: test_case()) {
            auto target = v.back();
            v.pop_back();
            print(v,"v",-1); cout << "\ttarget = " << target << endl;
            auto res = ::change(target, v);
            cout << "res = " << res << endl;
        }
    }
    void coinChange() {
        auto test_case = []() {
            std::vector<std::vector<int>> res;
            // res.push_back({1,2,5,  11});
            // res.push_back({346,29,395,188,155,109,   9401});
            // res.push_back({1,2,5,10,  18});
            res.push_back({3,7,405,436, 8839});
            res.push_back({284,260,393,494, 7066});
            res.push_back({216,94,15,86, 5372});
            res.push_back({37,233,253,483, 7163});
            res.push_back({357,239,73,52, 9832});
            res.push_back({431,62,88,428, 9084});
            res.push_back({1, 0});
            // res.push_back({1, 2147483647});
            return res;
        };
        for (auto &v: test_case()) {
            auto target = v.back();
            v.pop_back();
            print(v,"v",-1); cout << "\ttarget = " << target << endl;
            auto res = ::coinChange(v, target);
            cout << "res = " << res << endl;
        }
    }
}

int main() {
    test::init();
    auto t = Timer();
    // test::combinationSum();
    // test::change();
    test::printParenthes();
    // test::subsets();
    t.show();
}
