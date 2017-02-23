#include "utility.h"
// pac, partition and combine, each could be trivial. 1. dac 2. dp
// Divide and Conquer   https://leetcode.com/tag/divide-and-conquer/
// Dynamic Programming   https://leetcode.com/tag/dynamic-programming/
// Array    https://leetcode.com/tag/array/
using std::max;
using std::min;
using std::swap;

// 152. Maximum Product Subarray https://leetcode.com/problems/maximum-product-subarray/?tab=Description
// Find the contiguous subarray within an array (containing at least one number) which has the largest product. 
// proof of correctness, given a collection of non-zero number, the res is product of all number except either first or last. aBcD aBcDe BcD BcDe
// TODO a one pass implementation, with swap trick.  https://discuss.leetcode.com/topic/4417/possibly-simplest-solution-with-o-n-time-complexity/
// yet another one pass implementation, though less clean. https://discuss.leetcode.com/topic/3607/sharing-my-solution-o-1-space-o-n-running-time
    template <typename ForwardIt, typename T = typename std::iterator_traits<ForwardIt>::value_type>
    int maxProduct_one_side(ForwardIt first, ForwardIt last) {
        if (first == last) return 0;
        int res = *first, product = *first;
        for (; ++first != last;) {
            if (product == 0) {
                product = *first;
            } else {
                product *= *first;
            }
            res = std::max(res, product);
        }
        return res;
    }
    int maxProduct(vector<int>& nums) {
        // -3 -2 -1
        // -1 -2 -3
        return std::max(maxProduct_one_side(nums.begin(), nums.end()), maxProduct_one_side(nums.rbegin(), nums.rend()));
    }

int maxProduct(int A[], int n) { // https://discuss.leetcode.com/topic/4417/possibly-simplest-solution-with-o-n-time-complexity/2
    // store the result that is the max we have found so far
    int r = A[0];

    // imax/imin stores the max/min product of
    // subarray that ends with the current number A[i]
    for (int i = 1, imax = r, imin = r; i < n; i++) {
        // multiplied by a negative makes big number smaller, small number bigger
        // so we redefine the extremums by swapping them
        if (A[i] < 0)
            swap(imax, imin);

        // max/min product for the current number is either the current number itself
        // or the max/min by the previous number times the current one
        imax = max(A[i], imax * A[i]);
        imin = min(A[i], imin * A[i]);

        // the newly computed max value is a candidate for our global result
        r = max(r, imax);
    }
    return r;
}

// 53. Maximum Subarray https://leetcode.com/problems/maximum-subarray/?tab=Description
// Find the contiguous subarray within an array (containing at least one number) which has the largest sum. 
// sol 1, result is max of candidate sum which ending with each item. -- Kadane's algorithm
// sol 2, equal partition, instead of grow-by-one partition. complexity different from merge sort. O(n), since T(n) = 2T(n / 2) + O(1). https://discuss.leetcode.com/topic/25396/c-an-clear-o-n-divide-and-conquer-solution-with-comments
// -- and https://discuss.leetcode.com/topic/4175/share-my-solutions-both-greedy-and-divide-and-conquer
// -- and https://discuss.leetcode.com/topic/426/how-to-solve-maximum-subarray-by-using-the-divide-and-conquer-approach
// -- and https://discuss.leetcode.com/topic/18163/divide-conquer-easy-to-understand
// -- and https://discuss.leetcode.com/topic/21509/12ms-dp-dc-c-solutions/2
// -- TODO the difficulty is how to update lsum and rsum. hint, at most five part. lsum, neg1, mid, neg2, rsum.
    int maxSubArray_zero(vector<int>& nums) {
        int maxEndingHere = 0, res = maxEndingHere;
        for (auto x: nums) {
            maxEndingHere = std::max(0, maxEndingHere) + x;
            res = std::max(res, maxEndingHere);
        }
        return res;
    }
    int maxSubArray_min(vector<int>& nums) {
        if (nums.empty()) return 0;
        int maxEndingHere = nums[0], res = maxEndingHere;
        for (size_t i = 1; i < nums.size(); ++i) {
            maxEndingHere = std::max(0, maxEndingHere) + nums[i];
            res = std::max(res, maxEndingHere);
        }
        return res;
    }
    template <typename RandomIt> // TODO the difficulty is how to update lsum and rsum.
    int maxSubArray_dac(RandomIt first, RandomIt last, int &lsum, int &rsum) {
        if (first == last) return 0;
        auto mid = first + (last - first) / 2;
        if (first == mid) {
            lsum = *first;
            rsum = *first;
            return *first;
        }
        int left, right;
        auto res = maxSubArray_dac(first, mid, lsum, left);
        res = std::max(res, maxSubArray_dac(mid, last, right, rsum));
        res = std::max(res, left + right);
        return res;
    }
    int maxSubArray(vector<int>& nums) {
        int lsum, rsum;
        return maxSubArray_dac(nums.begin(), nums.end(), lsum, rsum);
        return maxSubArray_min(nums);
        return maxSubArray_zero(nums);
    }

namespace test {
    void maxSubArray() {
        auto test_case = []() {
            std::vector<std::vector<int>> res;
            res.push_back({-2,1,-3,4,-1,2,1,-5,4});
            res.push_back({-2,-1});
            return res;
        };
        for (auto &v: test_case()) {
            auto res = ::maxSubArray(v);
            cout << "res = " << res << ", ";
            print(v);
        }
    }
    void maxProduct() {
        auto test_case = []() {
            std::vector<std::vector<int>> res;
            res.push_back({2,3,-2,4});
            res.push_back({7, 1, 5, 3, 6, 4});
            res.push_back({7, 6, 4, 3, 1});
            res.push_back({-3,0,1,-2});
            res.push_back({0,0,-2,0});
            res.push_back({4,-3,4});
            return res;
        };
        for (auto &v: test_case()) {
            auto res = ::maxProduct(v);
            // auto res = ::maxProduct(&v[0], v.size());
            cout << "res = " << res << ", ";
            print(v);
        }
    }
}

// 300. Longest Increasing Subsequence https://leetcode.com/problems/longest-increasing-subsequence/?tab=Description
    int lengthOfLIS_set(vector<int>& nums) {
        int res = 0;
        std::map<int,int> hash;
        for (auto val: nums) {
            auto cnt = 0;
            for (auto it = hash.begin(); it != hash.end() && it->first < val; ++it){
                cnt = std::max(cnt, it->second);
            }
            res = std::max(res, 1 + cnt);
            hash[val] = 1 + cnt;
        }
        return res;
    }
    int lengthOfLIS_nlogn(vector<int>& nums) {
        std::vector<int> lis; // ith being smallest which of length i+1.
        for (auto x: nums) {
            auto it = std::lower_bound(lis.begin(), lis.end(), x);
            if (it == lis.end()) {
                lis.push_back(x);
            } else {
                *it = x;
            }
        }
        return lis.size();
    }
    int lengthOfLIS(vector<int>& nums) {
        // return lengthOfLIS_set(nums);
        return lengthOfLIS_nlogn(nums);
    }
// 334. Increasing Triplet Subsequence https://leetcode.com/problems/increasing-triplet-subsequence/?tab=Description
// Given an unsorted array return whether an increasing subsequence of length 3 exists or not in the array. 
// Your algorithm should run in O(n) time complexity and O(1) space complexity. 
// TODO 
    bool increasingTriplet(vector<int>& nums) {
        return lengthOfLIS_nlogn(nums) >= 3;
    }
namespace test {
    void lengthOfLIS() {
        auto test_case = []() {
            std::vector<std::vector<int>> res;
            // res.push_back({});
            // res.push_back({});
            res.push_back({10, 9, 2, 5, 3, 7, 101, 18});
            return res;
        };
        for (auto &v: test_case()) {
            print(v,"v",-1);
            auto res = ::lengthOfLIS(v);
            cout << "\tres = " << res << endl;
        }
    }
}

int main() {
    test::init();
    // test::findMaxLength();
    test::maxProduct();
    // test::maxSubArray();
}
