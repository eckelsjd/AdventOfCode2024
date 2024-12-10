#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>


/**
 * @brief Counts the occurrences of a given word in a word search grid.
 * 
 * This function searches for the specified word in the provided word search grid.
 * It checks all possible directions (horizontally, vertically, and diagonally) 
 * to find the word and counts its occurrences.
 * 
 * @param word_search A vector of strings representing the word search grid.
 * @param word_to_find The word to search for in the grid.
 * @return int The number of times the word is found in the grid.
 */
int count_word_search(std::vector<std::string> word_search, std::string word_to_find) {
    int row = 0;
    int col = 0;
    int cnt = 0;

    // Main search loop
    for (const std::string& line : word_search) {
        col = 0;
        for (char c : line) {
            if (c == word_to_find[0]) {
                // Check all adjacent characters
                for (int i = std::max(row-1, 0); i <= std::min(row+1, int(word_search.size()) - 1); i++) {
                    for (int j = std::max(col-1, 0); j <= std::min(col+1, int(line.length()) - 1); j++) {
                        if (!(i == row && j == col)) {
                            if (word_search[i].at(j) == word_to_find[1]) {
                                int h_direction = j - col;  // 1=right, -1=left
                                int v_direction = i - row;  // 1=down, -1=up

                                // Follow the direction of the word and check each letter
                                bool found_word = true;
                                int curr_row = row + v_direction;
                                int curr_col = col + h_direction;
                                for (size_t k = 0; k < word_to_find.length() - 2; k++) {
                                    curr_row += v_direction;
                                    curr_col += h_direction;
                                    if (curr_row < 0 || curr_row > word_search.size() - 1 || 
                                        curr_col < 0 || curr_col > line.size() - 1) {
                                            found_word = false;
                                            break;
                                    }
                                    if (word_search[curr_row].at(curr_col) != word_to_find[k + 2]) {
                                        found_word = false;
                                        break;
                                    }
                                }

                                if (found_word) {
                                    cnt++;
                                }
                            }
                        }
                    }
                }
                
            }
            col++;
        }
        row++;
    }

    return cnt;
}

/**
 * @brief Counts the occurrences of the pattern "XMAS" in a word search grid.
 * 
 * This function searches for the pattern "XMAS" in the provided word search grid.
 * The pattern is defined as follows:
 * - The letter 'A' must be surrounded diagonally by 'M' and 'S' in any order.
 * - For example, 'A' should have 'M' and 'S' diagonally adjacent in any of the four corners.
 * 
 * @param word_search A vector of strings representing the word search grid.
 * @return int The number of times the pattern "XMAS" is found in the grid.
 */
int count_xmas(std::vector<std::string> word_search) {
    int row = 0;
    int col = 0;
    int cnt = 0;

    // Main search loop
    for (const std::string& line : word_search) {
        col = 0;
        for (char c : line) {
            if (c == 'A') {
                // Check diagonal characters for M and S
                if (row > 0 && (row < word_search.size() - 1) && col > 0 && (col < line.length() - 1)) {
                    bool found_xmas = true;
                    char top_left = word_search[row-1].at(col-1);
                    char top_right = word_search[row-1].at(col+1);
                    char bottom_left = word_search[row+1].at(col-1);
                    char bottom_right = word_search[row+1].at(col+1);
                    if (top_left == 'M') {
                        if (bottom_right != 'S') {
                            found_xmas = false;
                        }
                    } else if (top_left == 'S') {
                        if (bottom_right != 'M') {
                            found_xmas = false;
                        }
                    } else {
                        found_xmas = false;
                    }

                    if (top_right == 'M') {
                        if (bottom_left != 'S') {
                            found_xmas = false;
                        }
                    } else if (top_right == 'S') {
                        if (bottom_left != 'M') {
                            found_xmas = false;
                        }
                    } else {
                        found_xmas = false;
                    }

                    if (found_xmas) {
                        cnt++;
                    }
                }
            }
            col++;
        }
        row++;
    }

    return cnt;
}

int main() {
    std::ifstream file("../data/day4.txt");
    std::string line;
    std::vector<std::string> lines;

    while (std::getline(file, line)) {
        lines.push_back(line);
    }

    std::string word_to_find = "XMAS";

    int total_cnt = count_word_search(lines, word_to_find);
    int xmas_cnt = count_xmas(lines);

    std::cout << "Total count for " << word_to_find << " is: " << total_cnt << std::endl;
    std::cout << "Total X-MAS count for part two is: " << xmas_cnt << std::endl;

    return 0;
}
