#include "utility.h"

auto maxSum_brute_force(const std::vector<int>& seq) {
    int res = INT_MIN;
    for (auto left = seq.begin(); left != seq.end(); ++left) {
        decltype(res) cum_sum = 0;
        for (auto right = left; right != seq.end(); ++right) {
            cum_sum += *right;
            res = std::max(res, cum_sum);
        }
    }
    return res;
}

auto maxSum(const std::vector<int>& seq) {
    // long long res = 0;
    int res = INT_MIN;
    decltype(res) trailing_sum = 0;
    for (auto it = seq.begin(); it != seq.end(); ++it) {
        trailing_sum = std::max(0, trailing_sum) + *it;
        res = std::max(res, trailing_sum);
    }
    return res;
}

auto maxSum3(const std::vector<int>& seq) {
    // auto maxSumRec = [&seq,&maxSumRec](int low, int high) -> int {
    std::function<int(int,int)> maxSumRec = [&seq,&maxSumRec](int low, int high) {
        if (low < high) {
            auto mid = low + (high - low) / 2;
            auto res = std::max(maxSumRec(low, mid), maxSumRec(mid + 1, high));
            decltype(res) mix_sum = seq[mid];
            auto cum_sum = mix_sum;
            for (auto i = mid + 1; i < high; ++i) {
                cum_sum += seq[i];
                mix_sum = std::max(mix_sum, cum_sum);
            }
            cum_sum = mix_sum;
            for (auto i = mid - 1; i >= low; --i) {
                cum_sum += seq[i];
                mix_sum = std::max(mix_sum, cum_sum);
            }
            res = std::max(res, mix_sum);
            return res;
        } else {
            return INT_MIN;
        }
    };
    return maxSumRec(0, seq.size());
}

auto maxSum3_non_recursive(const std::vector<int>& seq) {
    int res = INT_MIN;
    int low = 0, high = seq.size();
    vector<int> stack {low, high};
    auto tmp_cnt = 0;
    while (!stack.empty()) {
        ++tmp_cnt;
        high = stack.back();
        stack.pop_back();
        low = stack.back();
        stack.pop_back();
        if (low < high) {
            auto mid = low + (high - low) / 2;
            stack.push_back(low);
            stack.push_back(mid);
            stack.push_back(mid + 1);
            stack.push_back(high);
            decltype(res) mix_sum = seq[mid];
            auto cum_sum = mix_sum;
            for (auto i = mid + 1; i < high; ++i) {
                cum_sum += seq[i];
                mix_sum = std::max(mix_sum, cum_sum);
            }
            cum_sum = mix_sum;
            for (auto i = mid - 1; i >= low; --i) {
                cum_sum += seq[i];
                mix_sum = std::max(mix_sum, cum_sum);
            }
            // cout << "mix_sum = " << mix_sum << endl;
            res = std::max(res, mix_sum);
        }
    }
    // cout << "tmp_cnt = " << tmp_cnt << ", seq.size() = " <<  seq.size() << endl;
    return res;
}

std::vector<vector<int>> test_case() {
    std::vector<vector<int>> res;
    res.push_back({-2, 11, -4, 13, -5, -2});
    res.push_back({4, -3, 5, -2, -1, 2, 6, -2});
    res.push_back({4, -3, 5, -2, -1, 2, 6, -2, 4, -3, 5, -2, -1, 2, 6, -2,});
    res.push_back({4, -3, 5, -2, -1, 2, 6, -2, 4, -3, 5, -2, -1, 2, 6, -2,4, -3, 5, -2, -1, 2, 6, -2, 4, -3, 5, -2, -1, 2, 6, -2,4, -3, 5, -2, -1, 2, 6, -2, 4, -3, 5, -2, -1, 2, 6, -2,4, -3, 5, -2, -1, 2, 6, -2, 4, -3, 5, -2, -1, 2, 6, -2,4, -3, 5, -2, -1, 2, 6, -2, 4, -3, 5, -2, -1, 2, 6, -2,4, -3, 5, -2, -1, 2, 6, -2, 4, -3, 5, -2, -1, 2, 6, -2,4, -3, 5, -2, -1, 2, 6, -2, 4, -3, 5, -2, -1, 2, 6, -2,4, -3, 5, -2, -1, 2, 6, -2, 4, -3, 5, -2, -1, 2, 6, -2,});
    res.push_back({4, -3, 5, -2, -1, 2, 6, -2, 4, -3, 5, -2, -1, 2, 6, -2,4, -3, 5, -2, -1, 2, 6, -2, 4, -3, 5, -2, -1, 2, 6, -2,4, -3, 5, -2, -1, 2, 6, -2, 4, -3, 5, -2, -1, 2, 6, -2,4, -3, 5, -2, -1, 2, 6, -2, 4, -3, 5, -2, -1, 2, 6, -2,4, -3, 5, -2, -1, 2, 6, -2, 4, -3, 5, -2, -1, 2, 6, -2,4, -3, 5, -2, -1, 2, 6, -2, 4, -3, 5, -2, -1, 2, 6, -2,4, -3, 5, -2, -1, 2, 6, -2, 4, -3, 5, -2, -1, 2, 6, -2,4, -3, 5, -2, -1, 2, 6, -2, 4, -3, 5, -2, -1, 2, 6, -2,4, -3, 5, -2, -1, 2, 6, -2, 4, -3, 5, -2, -1, 2, 6, -2,});
    res.push_back({-2, -3, -4, -5});
    return res;
}

int main() {
    for (auto seq: test_case()) {
        // print(seq);
        cout << maxSum_brute_force(seq) << '\n';
        cout << maxSum(seq) << '\n';
        cout << maxSum3(seq) << '\n';
        cout << maxSum3_non_recursive(seq) << '\n';
    }
}
