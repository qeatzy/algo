#include "utility.h"
// range, range related. see also " binarySearch.cpp " bit.cpp
// 308 Range Sum Query 2D - Mutable

// 327. Count of Range Sum https://leetcode.com/problems/count-of-range-sum/?tab=Description
// Given an integer array nums, return the number of range sums that lie in [lower, upper] inclusive.
// Range sum S(i, j) is defined as the sum of the elements in nums between indices i and j (i ≤ j), inclusive.
// Note: A naive algorithm of O(n2) is trivial. You MUST do better than that.
// Example: Given nums = [-2, 5, -1], lower = -2, upper = 2, Return 3. The three ranges are: [0, 0], [2, 2], [0, 2] and their respective sums are: -2, -1, 2. 
    int countRangeSum(vector<int>& nums, int lower, int upper) {
        return 0;
    }

// 307. Range Sum Query - Mutable https://leetcode.com/problems/range-sum-query-mutable/?tab=Description
// Given an integer array nums, find the sum of the elements between indices i and j (i ≤ j), inclusive.
// The update(i, val) function modifies nums by updating the element at index i to val.
// Example: Given nums = [1, 3, 5] sumRange(0, 2) -> 9 update(1, 2) sumRange(0, 2) -> 8
// Note: The array is only modifiable by the update function.
// You may assume the number of calls to update and sumRange function is distributed evenly.
class NumArray_mutable {
public:
    // NumArray_mutable(vector<int> nums) : psum(nums.size() + 1), values{nums} {
    NumArray_mutable(vector<int> nums) : psum(nums.size() + 1), values(nums.size()) {
        for (size_t i = 0; i < nums.size(); ++i) update(i, nums[i]);
        // std::copy(nums.begin(), nums.end(), psum.begin() + 1);
        // for (size_t i = 2; i < psum.size(); i += 2) psum[i] += psum[i-1];
        // for (size_t step = 4; step < psum.size(); step <<= 1) {
        //     for (auto root = step; root < psum.size(); root += step) {
        //         auto lastOne = root & (root ^ (root - 1));
        //         auto i = (root & ~lastOne) | (lastOne >> 1);
        //         do {
        //             // cout << "i = " << i << ", lastOne = " << lastOne << endl;
        //             // wait();
        //             psum[root] += psum[i];
        //             lastOne = i & (i ^ (i - 1));
        //             i |= lastOne >> 1;
        //         } while ((i & 1) == 0);
        //     }
        // }
        // print(psum,"psum");
        // for (auto i: range(psum.size())) { cout << "getAccum(" << i << ") = " << getAccum(i) << endl; }
        // update(3,2);
        // print(psum,"psum");
    }
    void update(int i, int val) {
        std::swap(val, values[i]);
        val = values[i] - val;
        ++i;
        while (size_t(i) < psum.size()) {
            // wait("update");
            psum[i] += val;
            i += i & -i;
        }
    }
    int sumRange(int i, int j) {
        return getAccum(j+1) - getAccum(i);
    }
private:
    vector<int> psum;
    vector<int> values;
    int getAccum(int i) {
        int res = 0;
        while (i > 0) {
            res += psum[i];
            i &= i - 1;
        }
        return res;
    }
};

namespace test {
    void sumRange_mutable() {
        std::vector<std::vector<int>> vec;
        // std::vector<int> nums{1,3,-2,5,-1,0,2};
        // std::vector<int> nums{1,3,5};
        // vec.push_back({0,2,0});
        // vec.push_back({1,2,1});
        // vec.push_back({0,2,0});
        // std::vector<int> nums{0};
        // vec.push_back({0,0,0});
        // vec.push_back({0,1,1});
        // vec.push_back({0,0,0});
// ["NumArray","sumRange","update","sumRange","sumRange","update","update","sumRange","sumRange","update","update"] [[[-28,-39,53,65,11,-56,-65,-39,-43,97]],[5,6],[9,27],[2,3],[6,7],[1,-82],[3,-72],[3,7],[1,8],[5,13],[4,-67]]
        std::vector<int> nums{-28,-39,53,65,11,-56,-65,-39,-43,97};
        vec.push_back({5,6,0});
        vec.push_back({9,27,1});
        vec.push_back({2,3,0});
        vec.push_back({6,7,0});
        vec.push_back({1,-82,1});
        vec.push_back({3,-72,1});
        vec.push_back({3,7,0});
        vec.push_back({1,8,0});
        vec.push_back({5,13,1});
        vec.push_back({4,-67,1});
        auto obj = NumArray_mutable(nums);
        for (auto &v: vec) {
            assert(v.size() >= size_t(3));
            if (v[2]) { // update
                obj.update(v[0],v[1]);
            } else {
                cout << "sumRange(" << v[0] << ", " << v[1] << ") = " << obj.sumRange(v[0],v[1]) << endl;
            }
        }
    }
}

// 303. Range Sum Query - Immutable https://leetcode.com/problems/range-sum-query-immutable/?tab=Description
// Given an integer array nums, find the sum of the elements between indices i and j (i ≤ j), inclusive.
// Example: Given nums = [-2, 0, 3, -5, 2, -1] sumRange(0, 2) -> 1 sumRange(2, 5) -> -1 sumRange(0, 5) -> -3
// Note: You may assume that the array does not change. There are many calls to sumRange function.
class NumArray{
public:
    NumArray(vector<int> nums) : prefix_sum(nums.size() + 1) {
        for (size_t i = 0; i < nums.size(); ++i) {
            prefix_sum[i+1] = prefix_sum[i] + nums[i];
        }
    }
    int sumRange(int i, int j) {
        return prefix_sum[j+1] - prefix_sum[i];
    }
private:
    vector<int> prefix_sum;
};
namespace test {
    void sumRange() {
        vector<int> nums {-2, 0, 3, -5, 2, -1};
        auto obj = ::NumArray(nums);
        std::vector<std::pair<int,int>> vec;
        vec.push_back({0,2});
        vec.push_back({2,5});
        vec.push_back({0,5});
        for (auto x: vec) {
            auto res = obj.sumRange(x.first, x.second);
            cout << "sumRange(" << x.first << ", " << x.second << ") = " << res << endl;
        }
    }
}

// 304. Range Sum Query 2D - Immutable https://leetcode.com/problems/range-sum-query-2d-immutable/?tab=Description
// Given a 2D matrix matrix, find the sum of the elements inside the rectangle defined by its upper left corner (row1, col1) and lower right corner (row2, col2).
// Given matrix = [
//   [3, 0, 1, 4, 2],
//   [5, 6, 3, 2, 1],
//   [1, 2, 0, 1, 5],
//   [4, 1, 0, 1, 7],
//   [1, 0, 3, 0, 5]
// ]
// sumRegion(2, 1, 4, 3) -> 8
// sumRegion(1, 1, 2, 2) -> 11
// sumRegion(1, 2, 2, 4) -> 12
// Note: You may assume that the matrix does not change.
// There are many calls to sumRegion function.
// You may assume that row1 ≤ row2 and col1 ≤ col2.
class NumMatrix {
public:
    NumMatrix(vector<vector<int>> matrix) : prefix_sum(matrix.size() + 1, vector<int>(matrix.empty()? 1 : matrix[0].size() + 1)) {
        for (size_t i = 0; i < matrix.size(); ++i) {
                auto sum = prefix_sum[i+1][0];
            for (size_t j = 0; j < matrix[0].size(); ++j) {
                // prefix_sum[i+1][j+1] = prefix_sum[i+1][j] + prefix_sum[i][j+1] + matrix[i][j];
                sum += matrix[i][j];
                prefix_sum[i+1][j+1] = prefix_sum[i][j+1] + sum;
            }
        }
        // print(matrix);
        // print(prefix_sum);
    }
    int sumRegion(int row1, int col1, int row2, int col2) {
        return prefix_sum[row2+1][col2+1] - prefix_sum[row2+1][col1] + prefix_sum[row1][col1] - prefix_sum[row1][col2+1];
    }
private:
    vector<vector<int>> prefix_sum;
};
namespace test {
    void sumRegion() {
        std::vector<std::vector<int>> mat {
                {3, 0, 1, 4, 2},
                {5, 6, 3, 2, 1},
                {1, 2, 0, 1, 5},
                {4, 1, 0, 1, 7},
                {1, 0, 3, 0, 5}
            };
        auto obj = ::NumMatrix(mat);
        std::vector<std::vector<int>> vec;
        vec.push_back({2,1,4,3});
        vec.push_back({1,1,2,2});
        vec.push_back({1,2,2,4});
        for (auto v: vec) {
            assert(v.size() >= 4);
            auto res = obj.sumRegion(v[0],v[1],v[2],v[3]);
            cout << "sumRegion(";
            print(v,"",',');
           cout << ") = " << res << endl;
        }
    }
}

// 228. Summary Ranges 
// Given a sorted integer array without duplicates, return the summary of its ranges.
// For example, given [0,1,2,4,5,7], return ["0->2","4->5","7"]. 
// -- another way to do it is groupby, base on diff of index and value, eg, python itertools.groupby.
    vector<string> summaryRanges(vector<int>& nums) {
        vector<string> res;
        auto first = nums.begin(), last = nums.end();
        for (; first != last;) {
            res.push_back(std::to_string(*first));
            auto prev = first, old = first;
            for (; ++first != last && *first - *prev == 1; prev = first) {}
            if (prev != old)
                res.back() += "->" + std::to_string(*prev);
        }
        return res;
    }
namespace test {
    void summaryRanges() {
        std::vector<std::vector<int>> vec;
        vec.push_back({0,1,2,4,5,7});
        for (auto &v: vec) {
            auto res = ::summaryRanges(v);
            print(res);
        }
    }
}

// 163 Missing Ranges  http://www.cnblogs.com/grandyang/p/5184890.html
// Given a sorted integer array where the range of elements are [0, 99] inclusive, return its missing ranges.
// For example, given [0, 1, 3, 50, 75], return ['2', '4->49', '51->74', '76->99']
// -- 99 log(n) vs O(n).
// -- corner cases. https://discuss.leetcode.com/topic/5906/shouldn-t-we-also-consider-corner-cases-where-int_min-and-int_max-are-involved/5
// discuss. https://discuss.leetcode.com/category/171/missing-ranges
// "of(overflow)
    vector<string> findMissingRanges(vector<int>& nums, int lower, int upper) {
        vector<string> res;
        if (lower > upper) return res;
        auto p = nums.begin(), last = nums.end();
        for (;;) {
            p = std::lower_bound(p, last, lower);
            if (p == last || *p != lower) {
                res.push_back(std::to_string(lower));
                if (p == last) {
                    if (lower != upper) res.back() += "->" + std::to_string(upper);
                    lower = upper;
                } else {
                    if (lower != *p - 1) res.back() += "->" + std::to_string(*p-1);
                    lower = *p;
                }
            }
            if (!(lower < upper)) break;
            ++lower;
        }
        return res;
    }
namespace test {
    void findMissingRanges() {
        std::vector<std::vector<int>> vec;
        // vec.push_back({0, 1, 3, 50, 75, 0, 99});
        vec.push_back({INT_MAX, 0, INT_MAX});
        vec.push_back({0, INT_MAX - 1, INT_MAX, 0, INT_MAX});
        vec.push_back({INT_MAX, INT_MAX, INT_MAX});
        vec.push_back({INT_MIN, INT_MIN});
        vec.push_back({INT_MIN, INT_MIN, INT_MAX});
        vec.push_back({INT_MIN, INT_MIN, 0, INT_MAX, INT_MAX, INT_MIN, INT_MAX});
        vec.push_back({INT_MIN, INT_MIN, INT_MIN + 1});
        vec.push_back({0, INT_MAX});
        vec.push_back({0, 0, INT_MAX});
        vec.push_back({50, 75, 0, 10}); // invalid input, all elements in [lower,upper]
        for (auto &v: vec) {
            assert(int(v.size()) >= 2);
            auto upper = v.back(); v.pop_back();
            auto lower = v.back(); v.pop_back();
            print(v,"v",-1); cout << "\t lower = " << lower << ", upper = " << upper << endl;
            auto res = ::findMissingRanges(v, lower, upper);
            print(res,"",'\n');
        }
    }
}

// 370 Range Addition http://www.cnblogs.com/grandyang/p/5628786.html
// Assume you have an array of length n initialized with all 0's and are given k update operations.
// Each operation is represented as a triplet: [startIndex, endIndex, inc] which increments each element of subarray A[startIndex ... endIndex] (startIndex and endIndex inclusive) with inc. Return the modified array after all k operations were executed.
// Example: Given: length = 5, updates = [[1, 3, 2], [2, 4, 3], [0, 2, -2]] Output: [-2, 0, 3, 5, 3]
// Hint: Thinking of using advanced data structures? You are thinking it too complicated. For each update operation, do you really need to update all elements between i and j? Update only the first and end element is sufficient. The optimal time complexity is O(k + n) and uses O(1) extra space. Credits:
// -- why BIT(binary indexed array) not needed? 1. O(1) update O(n) query 2. O(n) update O(1) query  3. O(log(n)) update O(log(n)) query -- binary indexed array.
// -- why it works? binary operator is associative, each element has unique inverse.
// here query not needed, thus choose O(1) update solution is good enough.
// https://discuss.leetcode.com/category/454/range-addition
// use prefix sum, suffix could be used instead.
    vector<int> getModifiedArray(int length, vector<vector<int>>& updates) {
        vector<int> res (length);
        for (auto x: updates) {
            auto val = x[2];
            res[x[1]] += val;
            if (x[0] > 0) res[x[0]-1] -= val;
        }
        print(res);
        for (int i = res.size() - 2; i >= 0; --i) {
            res[i] += res[i+1];
        }
        return res;
    }
namespace test {
    void getModifiedArray() {
        std::vector<std::vector<int>> v {{1,3,2},{2,4,3},{0,2,-2}};
        int k = 5;
        auto res = ::getModifiedArray(k, v);
        print(res);
    }
}

int main() {
    test::init();
    // test::canPermutePalindrome();
    // test::getPermutation();
    // test::getModifiedArray();
    // test::summaryRanges();
    // test::findMissingRanges();
    // test::sumRange_mutable();
    // test::sumRange_mutable();
    // test::sumRegion();
    // test::findComplement();
    test::hammingDistance();
}
