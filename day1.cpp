#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>

/**
 * @brief Computes the distance between two lists by summing the absolute differences of their sorted elements.
 * 
 * This function sorts both input lists and then iterates through their elements, summing the absolute differences
 * between corresponding elements from the two lists. The result is a measure of how different the two lists are.
 * 
 * @param list_one The first list of integers.
 * @param list_two The second list of integers.
 * @return The total distance, which is the sum of the absolute differences between corresponding elements of the sorted lists.
 */
int list_distance(std::vector<int> list_one, std::vector<int> list_two) {
    int distance = 0;

    std::sort(list_one.begin(), list_one.end());
    std::sort(list_two.begin(), list_two.end());
    
    for (size_t i = 0; i < list_one.size() && i < list_two.size(); ++i) {
        distance += abs(list_one[i] - list_two[i]);
    }
    return distance;
}

/**
 * @brief Computes the similarity between two lists by summing the values that are present in both lists.
 * 
 * This function iterates through each element of the first list and checks if it exists in the second list.
 * If an element from the first list is found in the second list, its value is added to the similarity score.
 * 
 * @param list_one The first list of integers.
 * @param list_two The second list of integers.
 * @return The similarity score, which is the sum of the values that are present in both lists.
 */
int list_similarity(std::vector<int> list_one, std::vector<int> list_two) {
    int similarity = 0;

    for (size_t i = 0; i < list_one.size(); i++) {
        for (size_t j = 0; j < list_two.size(); j++) {
            if (list_one[i] == list_two[j]) {
                similarity += list_two[j];
            }
        }
    }

    return similarity;
}

/**
 * @brief Entry point of the program.
 *
 * This function reads pairs of integers from a file named "data/day1.txt",
 * stores them in two separate lists, calculates the distance and similarity
 * between the two lists, and prints the results to the standard output.
 *
 * @return int Exit status of the program.
 */
int main() {

    std::vector<std::vector<int>> lists = {{}, {}};

    std::ifstream file("data/day1.txt");
    std::string line;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        int number;
        for (int i = 0; i < 2 && iss >> number; ++i) {
            lists[i].push_back(number);
        }
    }

    int distance = list_distance(lists[0], lists[1]);
    int similarity = list_similarity(lists[0], lists[1]);

    std::cout << "Total distance: " << distance << std::endl;
    std::cout << "Similarity: " << similarity << std::endl;
    
    return 0;
}