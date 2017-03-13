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

int main() {
    test::init();
    test::islandPerimeter();
}
