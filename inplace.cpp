#include "utility.h"
// achieve inplace -- O(1) space complexity, or lower space complexity than obvious solution, the trick, insight, and implementation.

// 73. Set Matrix Zeroes https://leetcode.com/problems/set-matrix-zeroes/?tab=Description
// Given a m x n matrix, if an element is 0, set its entire row and column to 0. Do it in place. click to show follow up.
// -- how to do it inplace, given at linear space is needed to store information of zeros? The answer is store the information without data destruction. Use any row and column, if any element is zero, set corresponding element in first row and first column to zero, thus store the information, and the element being set will be set to zero any way.
// a very concise solution. https://discuss.leetcode.com/topic/5056/any-shorter-o-1-space-solution
    void setZeroes_inplace(vector<vector<int>>& matrix) {
        int m = matrix.size(), n = (m > 0)? matrix[0].size() : 0;
        bool has_zero_first_row = false, has_zero_first_col = false;
        for (int i = 0; i < m; ++i) { if (matrix[i][0] == 0) has_zero_first_col = true; }
        for (int j = 0; j < n; ++j) { if (matrix[0][j] == 0) has_zero_first_row = true; }
        for (int i = 1; i < m; ++i) {
            for (int j = 1; j < n; ++j) {
                if (matrix[i][j] == 0) {
                    matrix[i][0] = matrix[0][j] = 0;
                }
            }
        }
        for (int i = 1; i < m; ++i) {
            for (int j = 1; j < n; ++j) {
                if (matrix[i][0] == 0 || matrix[0][j] == 0) {
                    matrix[i][j] = 0;
                }
            }
        }
        if (has_zero_first_col) for (int i = 0; i < m; ++i) matrix[i][0] = 0;
        if (has_zero_first_row) for (int j = 0; j < n; ++j) matrix[0][j] = 0;
    }
    void setZeroes_linear(vector<vector<int>>& matrix) {
        int m = matrix.size(), n = (m > 0)? matrix[0].size() : 0;
        vector<bool> flag(m+n, false);
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (matrix[i][j] == 0) {
                    flag[i] = flag[m + j] = true;
                }
            }
        }
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                if (flag[i] || flag[m+j]) {
                    matrix[i][j] = 0;
                }
            }
        }
    }
    void setZeroes(vector<vector<int>>& matrix) {
        setZeroes_inplace(matrix);
    }

int main() {
    test::init();
}
