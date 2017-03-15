#include "utility.h"
// grid.cpp, 2-dimensional related problem, some math involved.

// 463. Island Perimeter https://leetcode.com/problems/island-perimeter/#/description
// You are given a map in form of a two-dimensional integer grid where 1 represents land and 0 represents water.
// Grid cells are connected horizontally/vertically (not diagonally).
// The grid is completely surrounded by water, and there is exactly one island (i.e., one or more connected land cells).
// The island doesn't have "lakes" (water inside that isn't connected to the water around the island).
// One cell is a square with side length 1. The grid is rectangular, width and height don't exceed 100. Determine the perimeter of the island.
// Example:
// [[0,1,0,0],
//  [1,1,1,0],
//  [0,1,0,0],
//  [1,1,0,0]]
// Answer: 16
    int islandPerimeter(vector<vector<int>>& grid) {
        int res = 0;
        for (int nrow = grid.size(), ncol = (grid.empty()? 0 : grid[0].size()), i = 0; i < nrow; ++i) {
            for (int j = 0; j < ncol; ++j) {
                if (grid[i][j]) {
                    res += (0 < i)? grid[i-1][j] == 0 : 1;
                    res += (i < nrow - 1)? grid[i+1][j] == 0 : 1;
                    res += (0 < j)? grid[i][j-1] == 0 : 1;
                    res += (j < ncol - 1)? grid[i][j+1] == 0 : 1;
                }
            }
        }
        return res;
    }

namespace test {
    void islandPerimeter() {
        std::vector<std::vector<int>> grid {
            {0,1,0,0},
            {1,1,1,0},
            {0,1,0,0},
            {1,1,0,0}
        };
        print(grid,"grid");
        auto res = ::islandPerimeter(grid);
        cout << "perimeter of island in grid is " << res << endl;
    }
}

// 542. 01 Matrix https://leetcode.com/problems/01-matrix/#/description
//  Given a matrix consists of 0 and 1, find the distance of the nearest 0 for each cell.
// The distance between two adjacent cells is 1.
// Example 1:
// Input:
// 0 0 0
// 0 1 0
// 0 0 0
// Output:
// 0 0 0
// 0 1 0
// 0 0 0
// Example 2:
// Input:
// 0 0 0
// 0 1 0
// 1 1 1
// Output:
// 0 0 0
// 0 1 0
// 1 2 1
// Note:
//     The number of elements of the given matrix will not exceed 10,000.
//     There are at least one 0 in the given matrix.
//     The cells are adjacent in only four directions: up, down, left and right.
// **just update from four direction**, since this is just [Manhattan distance] (https://en.wikipedia.org/wiki/Taxicab_geometry)
    vector<vector<int>> updateMatrix(vector<vector<int>>& matrix) {
        const int m = matrix.size(), n = matrix.empty()? 0 : matrix[0].size();
        auto res = matrix;
        for (auto &vec: res) {
            for (auto &x: vec) {
                if (x != 0) { x = m + n; }  // set to upper bound
            }
        }
        for (int i = 0, h = m - 1; i < m; ++i, --h) {
            for (int j = 0, k = n - 1; j < n; ++j, --k) {
                if (i != 0) {   // not row border
                    res[i][j] = std::min(res[i][j], res[i-1][j] + 1);
                    res[h][j] = std::min(res[h][j], res[h+1][j] + 1);
                }
                if (j != 0) {   // not column border
                    res[i][j] = std::min(res[i][j], res[i][j-1] + 1);
                    res[i][k] = std::min(res[i][k], res[i][k+1] + 1);
                    res[h][j] = std::min(res[h][j], res[h][j-1] + 1);
                    res[h][k] = std::min(res[h][k], res[h][k+1] + 1);
                }
            }
        }
        return res;
    }

namespace test {
    void updateMatrix() {
        std::vector<std::vector<int>> v = {
            {0, 0, 0},
            {0, 1, 0},
            {1, 1, 1}
        };
        auto res = ::updateMatrix(v);
        print(res,"updateMatrix(v)");
    }
}

int main() {
    test::init();
    // test::islandPerimeter();
    test::updateMatrix();
}
