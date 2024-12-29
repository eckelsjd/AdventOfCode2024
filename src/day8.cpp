#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include <algorithm>

using Antinodes = std::vector<std::vector<bool>>;
using Grid = std::vector<std::string>;


/**
 * @brief Finds the antinodes in a given grid and updates the antinodes vector.
 * 
 * This function identifies the antinodes in a grid based on the given row and column.
 * An antinode is a point that is linearly spaced from two "antennas" of the same frequency (i.e. matching characters).
 * The function updates the antinodes vector to mark the positions of these antinodes.
 * 
 * @param row The row index in the grid to start the search.
 * @param col The column index in the grid to start the search.
 * @param grid The grid representing the locations of antennas.
 * @param antinodes The vector to be updated with the positions of the antinodes.
 */
void find_antinodes(int row, int col, Grid& grid, Antinodes& antinodes) {
    int max_vert = std::floor(grid.size() / 2);
    int max_hor = std::floor(grid[0].length() / 2);
    char frequency = grid[row].at(col);
    int max_row = static_cast<int>(grid.size() - 1);
    int max_col = static_cast<int>(grid[0].length() - 1);

    int start_row = std::max(row - max_vert, 0);
    int end_row = std::min(row + max_vert, max_row);
    int start_col = std::max(col - max_hor, 0);
    int end_col = std::min(col + max_hor, max_col);

    for (int j = start_row; j <= end_row; j++) {
        for (int i = start_col; i <= end_col; i++) {
            char c = grid[j].at(i);

            // If we find another collinear antenna of matching frequency, look for antinodes on either side
            if (!(j == row && i == col) && c == frequency) {
                int delta_x = i - col;
                int delta_y = j - row;

                int new_col = i;
                int new_row = j;

                while (new_col <= max_col && new_col >= 0 && new_row <= max_row && new_row >= 0) {
                    antinodes[new_row][new_col] = true;
                    new_col += delta_x;
                    new_row += delta_y;
                }

                new_col = i;
                new_row = j;
                while (new_col <= max_col && new_col >= 0 && new_row <= max_row && new_row >= 0) {
                    antinodes[new_row][new_col] = true;
                    new_col -= delta_x;
                    new_row -= delta_y;
                }
            }
        }
    }
}

/**
 * @brief Counts the number of unique antinode locations in a grid and prints to console.
 */
int main() {
    std::ifstream file("../data/day8.txt");
    std::string line;
    Grid grid;
    Antinodes antinodes;
    
    while (std::getline(file, line)) {
        grid.push_back(line);
        std::vector<bool> new_vec(line.length(), false);
        antinodes.push_back(new_vec);
    }

    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < line.length(); j++) {
            char c = grid[i].at(j);
            if (c != '.') {
                find_antinodes(i, j, grid, antinodes);
            }
        }
    }

    unsigned int cnt = 0;
    for (auto& row : antinodes) {
        for (bool is_antinode : row) {
            if (is_antinode) {
                cnt++;
            }
        }
    }

    std::cout << "Total number of unique antinodes: " << cnt << std::endl;
}