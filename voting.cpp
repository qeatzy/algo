#include "utility.h"
// the basic concept is 'Let's cancel each others’ vote!'
// http://www.cs.utexas.edu/users/misra/scannedPdf.dir/FindRepeatedElements.pdf
// Fewer comparisons (but more memory)  && Distributed Boyer-Moore   https://gregable.com/2013/10/majority-vote-algorithm-find-majority.html or http://goo.gl/64Nams
// Finding the Majority Element in Parallel.  http://www.crm.umontreal.ca/pub/Rapports/3300-3399/3302.pdf
// https://discuss.leetcode.com/topic/17564/boyer-moore-majority-vote-algorithm-and-my-elaboration
// https://leetcode.com/problems/majority-element-ii/
// https://leetcode.com/problems/majority-element/
// https://en.wikipedia.org/wiki/Boyer-Moore_majority_vote_algorithm

template <typename T, size_t K>
vector<T> majorityElement(vector<T>& nums) {
    std::array<T, K-1> candidate{};
    std::array<size_t, K-1> count{};
    for (auto value: nums) {
        // print(candidate,"",-2); print(count);
        // wait();
        bool equal = false;
        // first test for equality, then can test nullity, otherwise duplicate might appear.
        for (size_t i = 0; i < K-1; ++i) {
            // if (count[i] != 0 && candidate[i] == value) {
            if (candidate[i] == value) {    // since candidate/approximate, off-by-one error is acceptable and be removed by second pass.
                ++count[i];
                equal = true;
                break;
            }
        }
        if (equal) continue;
        bool empty = false;
        for (size_t i = 0; i < K-1; ++i) {
            if (count[i] == 0) {
                candidate[i] = value;
                ++count[i];
                empty = true;
                break;
            }
        }
        if (empty) continue;
        for (size_t i = 0; i < K-1; ++i) {
            --count[i];
        }
    }
    std::array<size_t, K-1> final_count{};
    for (auto value: nums) {
        for (size_t i = 0; i < K-1; ++i) {
            if (count[i] > 0 && candidate[i] == value) ++final_count[i];
        }
    }
    // print(candidate); print(final_count);
    vector<T> res;
    for (size_t i = 0; i < K-1; ++i) {
        const auto threshold = nums.size() / K;
        if (final_count[i] > threshold) {
            res.push_back(candidate[i]);
        }
    }
    return res;
}

std::vector<int> majorityElementMoreThanOneThird(std::vector<int> &nums) {
    return majorityElement<int, 3>(nums);
    // return majorityElement_refactored<int, 3>(nums);
}

namespace test {
    void majorityElementMoreThanOneThird() {
        auto test_case = []() {
            std::vector<std::vector<int>> res;
            res.push_back({2,2,3,3,4,5,6,7,2,2});
            res.push_back({1,1,1,3,3,2,2,2});
            res.push_back({0,-1,2,-1});
            res.push_back({0,0,1,2,1,1});
            res.push_back({1,2,2,3,2,1,1,3});
            res.push_back({-1,1,1,1,2,1});
            return res;
        };
        for (auto v: test_case()) {
            print(v);
            auto res = ::majorityElementMoreThanOneThird(v);
            print(res);
        }
    }
}

int main() {
    test::majorityElementMoreThanOneThird();
}

