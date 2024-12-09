#include <fstream>
#include <iostream>
#include <string>
#include <regex>

/**
 * @brief Counts and multiplies numbers found in specific patterns within a given string.
 *
 * This function searches for patterns of the form `mul(x,y)`, `do()`, and `don't()` in the input string.
 * It multiplies the numbers found in `mul(x,y)` patterns and sums the products. The `do()` and `don't()`
 * patterns enable or disable the multiplication based on the `check_enable` flag.
 *
 * @param content The input string containing the patterns to be processed.
 * @param check_enable A boolean flag indicating whether to respect the `do()` and `don't()` patterns.
 * @param start_enabled A boolean flag indicating the initial state of the enable flag.
 * @return The sum of the products of the numbers found in the `mul(x,y)` patterns.
 */
int count_multiply(std::string content, bool check_enable = true, bool start_enabled = true) {
    std::regex mul_pattern(R"(mul\(\d{1,3},\d{1,3}\)|do\(\)|don't\(\))");  // match mul(1,2), do(), don't()
    std::regex num_pattern(R"(\d{1,3})");  // match numbers only

    std::sregex_iterator reg_it(content.begin(), content.end(), mul_pattern);
    std::sregex_iterator end;

    std::string curr_mult;
    int curr_product;
    int sum_product = 0;
    bool enabled = start_enabled;

    while (reg_it != end) {
        std::smatch match = *reg_it;
        curr_mult = match.str();

        if (curr_mult == "do()" || curr_mult == "don't()") {
            enabled = curr_mult == "do()";
        } else if ((check_enable && enabled) || !check_enable) {
            std::sregex_iterator mult_it(curr_mult.begin(), curr_mult.end(), num_pattern);
            curr_product = 1;

            while (mult_it != end) {
                std::smatch num_match = *mult_it;
                curr_product *= std::stoi(num_match.str());
                ++mult_it;
            }

            sum_product += curr_product;
        }

        ++reg_it;
    }

    return sum_product;
}

int main() {
    std::ifstream file("data/day3.txt");
    std::string content( (std::istreambuf_iterator<char>(file) ),
                         (std::istreambuf_iterator<char>()     ) );

    int total = count_multiply(content, true);

    std::cout << "Total multiplication: " << total << std::endl;

    return 0;
}
