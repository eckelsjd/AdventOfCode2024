#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <iterator>

/**
 * @brief A class to iterate over the Cartesian product of a vector of characters repeated a specified number of times.
 */
class ProductIterator {
public:
    /**
     * @brief Constructs a ProductIterator object.
     * 
     * @param vec The vector of characters to iterate over.
     * @param repeat The number of times to repeat the vector in the Cartesian product.
     */
    ProductIterator(const std::vector<char>& vec, int repeat)
        : vec_(vec), repeat_(repeat), indices_(repeat, 0), end_(false) {
        // Initialize iterator at the end if the input vector is empty
        if (vec.empty()) {
            end_ = true;
        }
    }

    /**
     * @brief A nested class to represent the iterator interface for ProductIterator.
     */
    class Iterator {
    public:
        /**
         * @brief Constructs an Iterator object.
         * 
         * @param parent A pointer to the parent ProductIterator object.
         * @param end A boolean indicating if the iterator is at the end.
         */
        Iterator(ProductIterator* parent, bool end = false)
            : parent_(parent), end_(end) {}

        /**
         * @brief Dereferences the iterator to get the current product combination.
         * 
         * @return std::vector<char> The current product combination.
         */
        std::vector<char> operator*() const {
            std::vector<char> result;
            for (int i : parent_->indices_) {
                result.push_back(parent_->vec_[i]);
            }
            return result;
        }

        /**
         * @brief Pre-increment operator to move the iterator to the next product combination.
         * 
         * @return Iterator& A reference to the updated iterator.
         */
        Iterator& operator++() {
            if (end_) return *this;

            // Find the rightmost index that can be incremented
            int i;
            for (i = parent_->repeat_ - 1; i >= 0; --i) {
                if (parent_->indices_[i] != parent_->vec_.size() - 1) {
                    break;
                }
            }

            // If no such index exists, we're done
            if (i < 0) {
                end_ = true;
                return *this;
            }

            // Increment this index
            parent_->indices_[i]++;

            // Zero out any indices to the right of this index
            for (int j = i + 1; j < parent_->repeat_; ++j) {
                parent_->indices_[j] = 0;
            }

            return *this;
        }

        /**
         * @brief Inequality comparison operator for iterators.
         * 
         * @param other The other iterator to compare with.
         * @return true If the iterators are not equal.
         * @return false If the iterators are equal.
         */
        bool operator!=(const Iterator& other) const {
            return end_ != other.end_;
        }

    private:
        ProductIterator* parent_;
        bool end_;
    };

    /**
     * @brief Returns an iterator to the beginning of the Cartesian product.
     * 
     * @return Iterator An iterator to the beginning.
     */
    Iterator begin() {
        return Iterator(this);
    }

    /**
     * @brief Returns an iterator to the end of the Cartesian product.
     * 
     * @return Iterator An iterator to the end.
     */
    Iterator end() {
        return Iterator(this, true);
    }

private:
    const std::vector<char>& vec_;
    int repeat_;
    std::vector<int> indices_;
    bool end_;
};

/**
 * @brief Returns an iterator to the end of the Cartesian product.
 * 
 * @return Iterator An iterator to the end.
 */
ProductIterator product(const std::vector<char>& vec, int repeat) {
    return ProductIterator(vec, repeat);
}

/**
 * @brief Checks if a given combination of operands and operators results in the specified result.
 * 
 * @param operands The list of operands.
 * @param result The expected result of the equation.
 * @return true If the combination of operands and operators (+, *, ||) results in the specified result.
 * @return false Otherwise.
 */
bool is_valid_equation(std::vector<int> operands, unsigned long int result) {
    unsigned long int total;
    size_t pos;
    std::vector<char> operators = {'+', '*', '|'};
    bool is_valid = false;

    for (const auto& combo : product(operators, operands.size() - 1)) {
        total = operands[0];
        pos = 1;

        for (char c : combo) {
            if (c == '+') {
                total += operands[pos];
            } else if (c == '*') {
                total *= operands[pos];
            } else if (c == '|') {
                total = std::stoul(std::to_string(total) + std::to_string(operands[pos]));
            }

            if (total > result) {
                break;
            }
            pos++;
        }
        
        if (total == result) {
            is_valid = true;
            break;
        }
    }

    return is_valid;
}

/**
 * @brief The main function reads input from a file, checks for valid equations, and prints the sum of valid results
 * 
 * @return int Exit status of the program.
 */
int main() {
    std::ifstream file("../data/day7.txt");
    std::string line;
    std::string temp_str;
    int temp_int;
    unsigned long int result;
    unsigned long int total;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        iss >> temp_str;
        result = std::stoul(temp_str.substr(0, temp_str.length()-1));
        std::vector<int> operands;

        while (iss >> temp_int) {
            operands.push_back(temp_int);
        }

        if (is_valid_equation(operands, result)) {
            total += result;
        }
    }

    std::cout << "Total sum of valid equation results: " << total << std::endl;

    return 0;
}
