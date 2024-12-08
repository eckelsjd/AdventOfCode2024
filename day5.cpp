#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <set>
#include <sstream>
#include <algorithm>

using NumberLists = std::vector<std::vector<int>>;
using NumberMap = std::unordered_map<int, std::set<int>>;

struct Updates {
    NumberLists ordered;
    NumberLists unordered;

    /**
     * @brief Constructs an Updates object with ordered and unordered lists.
     * 
     * @param a The ordered lists of numbers.
     * @param b The unordered lists of numbers.
     */
    Updates(NumberLists a, NumberLists b) : ordered(a), unordered(b) {}
};

/**
 * @brief Sorts each list of numbers in the updates and returns the sorted lists.
 * 
 * @param updates The lists of numbers to be sorted.
 * @param after_map A map that defines the order constraints for sorting.
 * @return NumberLists The sorted lists of numbers.
 */
NumberLists sort_updates(NumberLists updates, NumberMap after_map) {
    NumberLists sorted_updates;

    for (auto update : updates) {

        std::set<int> to_sort(update.begin(), update.end());\
        std::vector<int> sorted_list;

        while (to_sort.size() > 0) {
            for (int page : update) {
                if (to_sort.contains(page)) {
                    if (after_map.contains(page)) {
                        std::set<int> intersection;
                        std::set<int> set1 = after_map.at(page);

                        // Only insert the page in sorted list if does not have to be after any unsorted pages
                        std::set_intersection(set1.begin(), set1.end(),
                                            to_sort.begin(), to_sort.end(),
                                            std::inserter(intersection, intersection.begin()));
                        
                        if (intersection.size() == 0) {
                            sorted_list.push_back(page);\
                            to_sort.erase(page);
                        }

                    } else {
                        // This page doesn't have to be after anyone, so immediately add it
                        sorted_list.push_back(page);
                        to_sort.erase(page);
                    }
                }
            }

        }

        sorted_updates.push_back(sorted_list);
    }

    return sorted_updates;
}

/**
 * @brief Splits the updates into ordered and unordered lists based on the before and after maps.
 * 
 * @param updates The lists of numbers to be split.
 * @param before_map A map that defines the order constraints before each number.
 * @param after_map A map that defines the order constraints after each number.
 * @return An Updates object containing the ordered and unordered lists.
 */
Updates split_updates(NumberLists updates, NumberMap before_map, NumberMap after_map) { 
    NumberLists ordered_updates;
    NumberLists unordered_updates;

    // Temporary sets of pages before/after current page
    std::set<int> before_page;

    for (auto update : updates) {
        before_page.clear();
        std::set<int> after_page(update.begin(), update.end());
        bool is_ordered = true;

        for (int page : update) {
            after_page.erase(page);

            // Intersect after_map with after_page -> should be empty
            if (after_map.contains(page)) {
                std::set<int> intersection;
                std::set<int> set1 = after_map.at(page);

                std::set_intersection(set1.begin(), set1.end(),
                                      after_page.begin(), after_page.end(),
                                      std::inserter(intersection, intersection.begin()));

                if (intersection.size() > 0) {
                    is_ordered = false;
                    break;
                }
            }

            // Intersect before_map with before_page -> should be empty
            if (before_map.contains(page)) {
                std::set<int> intersection;
                std::set<int> set1 = before_map.at(page);

                std::set_intersection(set1.begin(), set1.end(),
                                      before_page.begin(), before_page.end(),
                                      std::inserter(intersection, intersection.begin()));

                if (intersection.size() > 0) {
                    is_ordered = false;
                    break;
                }
            }

            before_page.insert(page);
        }

        if (is_ordered) {
            ordered_updates.push_back(update);
        } else {
            unordered_updates.push_back(update);
        }

    }

    return Updates(ordered_updates, unordered_updates);
}

/**
 * @brief Sums the middle numbers of each list in the updates.
 * 
 * @param updates The lists of numbers to be processed.
 * @return int The sum of the middle numbers of each list.
 */
int sum_middle_numbers(NumberLists updates) {
    int cnt = 0;

    for (auto update : updates) {
        int middle_idx = update.size() / 2;
        cnt += update[middle_idx];
    }

    return cnt;
}

/**
 * Detect unsorted lists of integers (i.e. updates) and sort them according to a set of rules.
 */
int main() {
    std::ifstream file("data/day5.txt");
    std::string line;
    NumberMap before_map;
    NumberMap after_map;
    NumberLists updates;

    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        }

        std::string temp;
        std::istringstream iss(line);

        // Add rules
        if (line.find('|') != std::string::npos) {
            std::getline(iss, temp, '|');
            int first_num = std::stoi(temp);

            std::getline(iss, temp, '|');
            int second_num = std::stoi(temp);

            before_map[first_num].insert(second_num);
            after_map[second_num].insert(first_num);
        } 
        
        // Add updates
        else {
            std::vector<int> pages;

            while (std::getline(iss, temp, ',')) {
                pages.push_back(std::stoi(temp));
            }

            updates.push_back(pages);
        }
    }

    Updates result = split_updates(updates, before_map, after_map);  // Split into ordered and unordered updates
    int middle_sum = sum_middle_numbers(result.ordered);
    std::cout << "Total sum of middle numbers: " << middle_sum << std::endl;

    NumberLists sorted = sort_updates(result.unordered, after_map);
    int middle_sorted_sum = sum_middle_numbers(sorted);
    std::cout << "Total sorted sum of middle numbers: " << middle_sorted_sum << std::endl;

    return 0;
}
