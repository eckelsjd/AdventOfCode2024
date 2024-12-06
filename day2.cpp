#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

/**
 * @brief Checks if a given report is safe based on the specified criteria.
 * 
 * A report is considered safe if the difference between consecutive levels is not zero 
 * and does not exceed 3, and the sequence of levels is either strictly increasing or 
 * strictly decreasing. The function allows for a specified number of problems (default is 1).
 * 
 * @param report The report string containing space-separated levels.
 * @param max_problems The maximum number of problems allowed for the report to be considered safe (default is 1).
 * @return true If the report is safe.
 * @return false If the report is not safe.
 */
bool is_safe_report(std::string report, int max_problems = 1) {
    std::istringstream iss(report);
    int curr_level;
    int prev_level;
    int diff;
    int problem_count = 0;
    bool found_problem;

    iss >> prev_level;
    iss >> curr_level;
    diff = abs(curr_level - prev_level);

    if (diff == 0 || diff > 3) {
        problem_count++;
    }

    if (problem_count > max_problems) {
        return false;
    }

    bool is_increasing = (curr_level > prev_level);
    prev_level = curr_level;

    while (iss >> curr_level) {
        diff = abs(curr_level - prev_level);

        found_problem = ((diff == 0 || diff > 3) || 
                         (is_increasing && (curr_level < prev_level)) || 
                         (!is_increasing && (curr_level > prev_level)));

        if (found_problem) {
            problem_count++;
        }

        if (problem_count > max_problems) {
            return false;
        }
        
        prev_level = curr_level;
    }

    return true;
}

// Count and return the number of safe reports in the data file
int main() {
    std::ifstream file("data/day2.txt");
    std::string line;

    int num_safe_reports = 0;

    while (std::getline(file, line)) {
        if (is_safe_report(line)) {
            num_safe_reports++;
        }
    }

    std::cout << "Number of safe reports: " << num_safe_reports << std::endl;

    return 0;
}