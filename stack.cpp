#include "utility.h"
#include <stack>
#include <initializer_list>
// stack. how to effectively use stack.
// https://leetcode.com/tag/stack/
// "ls
// increasing stack
// How to sort a stack using only stack operations? merge sort with two extra stack. other O(n^2) sort, eg, recursive insertion sort.  http://stackoverflow.com/questions/4826311/how-to-sort-a-stack-using-only-stack-operations

// 456. 132 Pattern https://leetcode.com/problems/132-pattern/?tab=Description
// Given a sequence of n integers a1, a2, ..., an, a 132 pattern is a subsequence ai, aj, ak such that i < j < k and ai < ak < aj.
// Design an algorithm that takes a list of n numbers as input and checks whether there is a 132 pattern in the list. Note: n will be less than 15,000.
// [-2,1,-2]    [3, 1, 4, 2]    [3, 5, 1, 4]    [-1, 3, 2, 0]
// this problem is not symmetric, tackle reversely is much simpler. or not?
// the forward solution is indeed more complex, since we need a maintain both min and max with stack. here is possible implementation.   https://discuss.leetcode.com/topic/68193/java-o-n-solution-using-stack-in-detail-explanation
// whereas the reverse solution, we use a stack to maintain min. here is a cleaner solution, with only bottom variable, with top variable encoded in stack.  https://discuss.leetcode.com/topic/67881/single-pass-c-o-n-space-and-time-solution-8-lines-with-detailed-explanation/2
    bool find132pattern_reversely(vector<int>& nums) { // 3 9 6 5 6/8
        int bottom = INT_MIN, top = bottom;
        std::vector<int> stk{INT_MAX};
        for (auto it = nums.rbegin(); it != nums.rend(); ++it) {
            auto x = *it;
            // cout << "bottom = " << bottom << ", top = " << top << ", x = " << x << " ";
            // print(stk,"\t stk");
            if (x < bottom) return true;
            else if (x > top) bottom = top, top = x;
            else if (x < stk.back() && x > bottom) stk.push_back(x);
            else if (x > stk.back()) {
                decltype(x) tmp;
                do {
                    tmp = stk.back();
                    stk.pop_back();
                } while (x > stk.back());
                bottom = tmp;
            }
        }
        return false;
    }
    bool find132pattern_forwardly_not_work_yet(vector<int>& nums) { // 6 8 3 2 5/7
        int bottom = INT_MIN, top = bottom;
        std::vector<int> stk{INT_MAX};
        for (auto x: nums) {
            if (bottom < x && x < top) return true;
            if (x > top) top = x;
            else if (x < bottom) {
                do {
                } while (0);
            }
        }
        return false;
    }
    bool find132pattern(vector<int>& nums) {
        return find132pattern_reversely(nums);
    }

namespace test {
    void find132pattern() {
        std::vector<std::vector<int>> vec;
        vec.push_back({2,4,3,1});
        for (auto &v: vec) {
            auto res = ::find132pattern(v);
            print(v,"v");
            cout << " res = " << res;
        }
    }
}

// 402. Remove K Digits https://leetcode.com/problems/remove-k-digits/?tab=Description
// Given a non-negative integer num represented as a string, remove k digits from the number so that the new number is the smallest possible.
// Note: The length of num is less than 10002 and will be ≥ k. The given num does not contain any leading zero.
// Input: num = "1432219", k = 3 Output: "1219"
// Input: num = "10200", k = 1 Output: "200"
// Input: num = "10", k = 2 Output: "0"
// Input: num = "1300502", k = 4 Output: "0"
// Input: num = "130256", k = 3 Output: "0"
// Input: num = "123456", k = 2 Output: "0"
// Input: num = "560213", k = 3 Output: "0"
// TODO 
    string removeKdigits(string num, int k) {
        if (size_t(k) >= num.size()) return "0";
        auto mid = std::min_element(num.begin(), num.begin() + k);
        if (*mid > '0') return *mid + num.substr(k);
        assert(mid != num.begin());
        string res;
        auto p = std::min_element(num.begin(), mid);
        res += *p;
        k -= mid - num.begin() + 1;
        for (p = mid; k > 0 && p != num.end(); ++p) {
        }
        return res;
    }

// 84. Largest Rectangle in Histogram https://leetcode.com/problems/largest-rectangle-in-histogram/?tab=Description
// Given n non-negative integers representing the histogram's bar height where the width of each bar is 1, find the area of largest rectangle in the histogram.
// For example, Given heights = [2,1,5,6,2,3], return 10.
// TODO 1. divide and conquer solution with segment tree.  we get the recurrence similar to worst case of Quick Sort. How to find the minimum efficiently? Range Minimum Query using Segment Tree can be used for this.  http://www.geeksforgeeks.org/largest-rectangular-area-in-a-histogram-set-1/
// -- similar to RMQ but more natural here. https://discuss.leetcode.com/topic/7491/simple-divide-and-conquer-ac-solution-without-segment-tree
// -- also divide and conquer.  https://discuss.leetcode.com/topic/2424/my-modified-answer-from-geeksforgeeks-in-java/2#
// if input is sorted, the left and right index cound be found linearly. Now view the input as concatenation of sorted subarray. Here is a solution utilitize this idea, with two extra array record the left and right index. https://discuss.leetcode.com/topic/39151/5ms-o-n-java-solution-explained-beats-96
    int largestRectangleArea_two_extra_array(vector<int>& heights) {
        if (heights.empty()) return 0;
        int res = 0;
        int sz = heights.size();
        std::vector<int> left(sz), right(sz);   // left[i] < i, right[i] > i
        left[0] = -1, right[sz - 1] = sz;
        for (int i = 1; i < sz; ++i) {
            int p = i - 1;
            for (; p >= 0 && heights[p] >= heights[i];) { p = left[p]; }
            left[i] = p;
        }
        for (int i = sz - 2; i >= 0; --i) {
            int p = i + 1;
            for (; p < sz && heights[p] >= heights[i];) { p = right[p]; }
            right[i] = p;
        }
        for (int i = 0; i < sz; ++i) {
            res = std::max(res, heights[i] * (right[i] - left[i] - 1));
        }
        return res;
    }
    int largestRectangleArea_O_n2_slightly_optimized(vector<int>& heights) {
        int res = 0;
        for (int i = 0, sz = heights.size(); i < sz; ++i) {
            auto min = heights[i];
            int j = i + 1;
            for (; j < sz && heights[j] >= min; ++j) {}
            int k = i - 1;
            for (; k > -1 && heights[k] >= min; --k) {}
                // cout << "i = " << i << ", j = " << j << ", min = " << min << endl;
                res = std::max(res, (j-k-1) * min);
        }
        return res;
    }
    int largestRectangleArea_brute_force(vector<int>& heights) {
        int res = 0;
        for (int i = 0, sz = heights.size(); i < sz; ++i) {
            auto min = heights[i];
            for (int j = i; j < sz; ++j) {
                min = std::min(min, heights[j]);
                // cout << "i = " << i << ", j = " << j << ", min = " << min << endl;
                res = std::max(res, (j-i+1) * min);
            }
        }
        return res;
    }
    int largestRectangleArea_sorted_stack_left_inclusive_modify_input(vector<int>& heights) {
        int res = 0;
        std::vector<int> stk;
        for (int i = 0, sz = heights.size(); ; ++i) {
            auto x = (i<sz ? heights[i] : 0);
            int j = i;
            for (; !stk.empty() && heights[stk.back()] > x;) {
                res = std::max(res, (i - stk.back()) * heights[stk.back()]);
                j = stk.back();
                heights[stk.back()] = x;
                stk.pop_back();
            }
            if (i == sz) break;
            stk.push_back(j);
        }
        return res;
    }
    int largestRectangleArea_sorted_stack(vector<int>& heights) { // both left and right exclusive, left get after pop stack.
        int res = 0;
        std::vector<int> stk{-1}; // as left index if stack is empty.
        for (int i = 0, sz = heights.size(); ; ++i) {
            auto x = (i<sz ? heights[i] : 0);
            int h;
            // cout << "i = " << i << ", x = " << x << endl;
            while (stk.size() > 1 && x < (h = heights[stk.back()])) {
                stk.pop_back();
                res = std::max(res, h * (i - 1 - stk.back()));
            }
            if (i == sz) break;
            stk.push_back(i);
        }
        return res;
    }
int largestRectangleArea_others(vector<int> &height) { // https://discuss.leetcode.com/topic/3913/my-concise-c-solution-ac-90-ms
using std::max;
    int n=height.size();
    if(n==0) return 0;
    if(n==1) return height[0];
    //  
    height.push_back(0);
    n++;
    //  
    int ans=0;
    std::stack<int> s;
    //         
    int i=0,j=0,h=0;
    while(i<n){
        // print(makeVec(s),"stk");
        cout << "i = " << i << ", height[i] = " << height[i] << endl;
        if(s.empty() || height[i]>height[s.top()]) s.push(i++);
        else{
            cout << "s.top() = " << s.top() << endl;
            h=height[s.top()];
            s.pop();
            j= s.empty() ? -1:s.top();
            ans=max(ans,h*(i-j-1));
        }
    }
    return ans;
}
    int largestRectangleArea(vector<int>& heights) {
        // return largestRectangleArea_others(heights);
        return largestRectangleArea_two_extra_array(heights);
        return largestRectangleArea_sorted_stack(heights);
        return largestRectangleArea_sorted_stack_left_inclusive_modify_input(heights);
        return largestRectangleArea_O_n2_slightly_optimized(heights);
    }

namespace test {
    void largestRectangleArea() {
        std::vector<std::vector<int>> vec;
        // vec.push_back({2,1,5,6,2,3});
        // vec.push_back({2,1,2});
        vec.push_back({2,7,9,5,4});
        vec.push_back({2,4,7,9,5,4});
        vec.push_back(range(7) * 2);
        // vec.push_back({0,0,0,0,0,0,0,0,2147483647});
        vec.push_back(range(2e4));
        auto t = Timer();
        cout << "vec.size() = " << vec.size() << endl;
        for (auto &v: vec) {
            // print(v,"v");
            t.clear();
            // auto reso = ::largestRectangleArea_others(v);
            // cout << "res = " << reso << endl;
            // t.show("others");
            auto res_dp = ::largestRectangleArea_two_extra_array(v);
            cout << "res_dp = " << res_dp << endl;
            t.show("res_dp array");
            auto res = ::largestRectangleArea_sorted_stack(v);
            cout << "res = " << res << endl;
            t.show("sorted stack");
            // auto res2 = ::largestRectangleArea_O_n2_slightly_optimized(v);
            // cout << "res2 = " << res2 << endl;
            // t.show("brute force O(n^2)");
        }
    }
}

int main() {
    test::init();
    // test::find132pattern();
    // test::algo();
    // test::adjacent_find();
    // test::largestRectangleArea();
    test::reversePairs();
}
