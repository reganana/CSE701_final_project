/**
 * @file bigint.hpp
 * @brief This file contains the definition of a BigInt class that supports arbitrary-precision integers.
 *
 * The BigInt class allows for the representation of large integers that cannot fit within standard integer types.
 * It supports various arithmetic operations such as addition, subtraction, multiplication, and comparisons.
 *
 * The digits are stored in reverse order.
 *
 * @version 1.0
 * @date 2024-12-15
 */

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>

/**
 * @class bigint
 * @brief A class to represent arbitrary-precision integers.
 *
 * This class provides methods for arbitrary-precision integers manipulation.
 */
class bigint
{
private:
    std::vector<uint8_t> digits; // Store digits in reverse order
    bool is_negative;            // Whether the number is negative

    /**
     * @brief Removes leading zeros from the digits.
     *
     * This function is used to preventing case happen like 0001.
     */
    void removeLeadingZeros()
    {
        while (digits.size() > 1 && digits.back() == 0)
        {
            digits.pop_back();
        }
        if (digits.size() == 1 && digits[0] == 0)
        {
            is_negative = false; // Zero is not negative
        }
    }

public:
    /**
     * @brief Default constructor, initializes the integer to 0.
     */
    bigint() : digits{0}, is_negative(false) {}
    /**
     * @brief Constructor that takes a signed 64-bit integer and converts it to an arbitrary-precision integer.
     *
     * @param value The signed 64-bit integer to convert.
     */
    bigint(int64_t value)
    {
        digits.clear();
        if (value < 0)
        {
            is_negative = true;
            value = -value;
        }
        if (value == 0)
        {
            digits.push_back(0);
        }
        while (value > 0)
        {
            digits.push_back(static_cast<uint8_t>(value % 10));
            value /= 10;
        }
    }

    /**
     * @brief Constructor that takes a string of digits and converts it to an arbitrary-precision integer.
     *
     * @param value The string to convert into a bigint.
     * @throws std::invalid_argument If the input string contains invalid characters/empty.
     */
    bigint(const std::string &value)
    {
        digits.clear();
        if (value.empty())
            throw std::invalid_argument("Invalid input string");
        size_t start = 0;
        is_negative = false;
        if (value[0] == '-')
        {
            is_negative = true;
            start = 1;
        }

        for (size_t i = value.size(); i > start; i--)
        {
            if (!std::isdigit(value[i - 1]))
                throw std::invalid_argument("Invalid digit in string");
            digits.push_back(static_cast<uint8_t>(value[i - 1] - '0'));
        }

        removeLeadingZeros();
    }

    /**
     * @brief Helper function for absolute digit addition.
     *
     * Adds two numbers with their digits provided in reverse order.
     *
     * @param largerDigits The digits of the larger number.
     * @param smallerDigits The digits of the smaller number.
     * @return A bigint containing the sum.
     */
    bigint addDigits(const std::vector<uint8_t> &largerDigits, const std::vector<uint8_t> &smallerDigits) const
    {
        bigint result;
        result.digits.clear();
        uint8_t carry = 0;
        size_t i = 0;

        // Add digits from both numbers
        for (; i < smallerDigits.size(); i++)
        {
            uint8_t sum = largerDigits[i] + smallerDigits[i] + carry;
            carry = sum / 10;
            result.digits.push_back(sum % 10);
        }

        // Add remaining digits from the larger number
        for (; i < largerDigits.size(); ++i)
        {
            uint8_t sum = largerDigits[i] + carry;
            carry = sum / 10;
            result.digits.push_back(sum % 10);
        }

        // Handle leftover carry
        if (carry)
        {
            result.digits.push_back(carry);
        }

        result.removeLeadingZeros();
        return result;
    }

    /**
     * @brief Addition operator for two bigints.
     *
     * @param other The bigint to add.
     * @return A new bigint containing the result of the addition.
     */
    bigint operator+(const bigint &other) const
    {
        // both number have same sign
        if (is_negative == other.is_negative)
        {

            std::vector<uint8_t> largerDigits;
            std::vector<uint8_t> smallerDigits;

            if (digits.size() >= other.digits.size())
            {
                largerDigits = digits;
                smallerDigits = other.digits;
            }
            else
            {
                largerDigits = other.digits;
                smallerDigits = digits;
            }
            bigint result = addDigits(largerDigits, smallerDigits);
            result.is_negative = is_negative;
            return result;
        }
        // number has different sign
        return *this - (-other);
    }

    /**
     * @brief Addition assignment operator.
     *
     * @param other The bigint to add.
     * @return The updated bigint.
     */
    bigint &operator+=(const bigint &other)
    {
        *this = *this + other;
        return *this;
    }

    /**
     * @brief Helper function for subtracting digits.
     *
     * Subtracts the digits of two bigints.
     *
     * @param result The bigint to store the result.
     * @param larger The larger bigint.
     * @param smaller The smaller bigint.
     */
    void subtractDigits(bigint &result, const bigint &larger, const bigint &smaller) const
    {
        int borrow = 0;
        size_t i = 0;

        // Subtract digits for smaller number digits
        for (; i < smaller.digits.size(); i++)
        {
            int diff = static_cast<int>(larger.digits[i]) -
                       static_cast<int>(smaller.digits[i]) -
                       static_cast<int>(borrow);
            if (diff < 0)
            {
                diff += 10; // Borrow from the next higher digit
                borrow = 1;
            }
            else
            {
                borrow = 0;
            }
            result.digits.push_back(static_cast<uint8_t>(diff));
        }

        // Subtract remaining digits in the larger number
        for (; i < larger.digits.size(); i++)
        {
            int diff = larger.digits[i] - borrow;
            if (diff < 0)
            {
                diff += 10; // Borrow from the next higher digit
                borrow = 1;
            }
            else
            {
                borrow = 0;
            }
            result.digits.push_back(static_cast<uint8_t>(diff));
        }

        result.removeLeadingZeros();
    }
    /**
     * @brief Subtraction operator for two bigints.
     *
     * @param other The bigint to subtract.
     * @return A new bigint containing the result of the subtraction.
     */
    bigint operator-(const bigint &other) const
    {
        // Same sign (both positive or both negative)
        if (is_negative == other.is_negative)
        {
            // both negative
            if (is_negative)
            {
                //
                if (*this <= other)
                {
                    bigint result;
                    result.digits.clear();
                    result.is_negative = true;
                    subtractDigits(result, *this, other);
                    return result;
                }
                else
                {
                    bigint result;
                    result.digits.clear();
                    result.is_negative = false;
                    subtractDigits(result, other, *this);
                    return result;
                }
            }
            else
            {
                // both positive
                // larger subtract smaller
                if (*this >= other)
                {
                    bigint result;
                    result.digits.clear();
                    result.is_negative = is_negative;
                    subtractDigits(result, *this, other);
                    return result;
                }
                else
                {
                    // smaller number subtract larger number
                    // negative (larger number - smaller number)
                    bigint result;
                    result.digits.clear();
                    result.is_negative = !is_negative;
                    subtractDigits(result, other, *this);
                    return result;
                }
            }
        }
        else
        // different sign
        {
            std::vector<uint8_t> largerDigits;
            std::vector<uint8_t> smallerDigits;

            if (digits.size() >= other.digits.size())
            {
                largerDigits = digits;
                smallerDigits = other.digits;
            }
            else
            {
                largerDigits = other.digits;
                smallerDigits = digits;
            }
            bigint result = addDigits(largerDigits, smallerDigits);

            // negative subtract positive
            if (is_negative)
            {
                result.is_negative = true;
                return result;
            }
            // positive subtract negative
            else
            {
                result.is_negative = false;
                return result;
            }
        }
    }
    /**
     * @brief Subtraction assignment operator.
     *
     * @param other The bigint to subtract.
     * @return The updated bigint.
     */
    bigint &operator-=(const bigint &other)
    {
        *this = *this - other;
        return *this;
    }
    /**
     * @brief Multiplication operator for two bigints.
     *
     * @param other The bigint to multiply.
     * @return A new bigint containing the result of the multiplication.
     */
    bigint operator*(const bigint &other) const
    {
        bigint result;
        result.digits.clear();
        // set sign
        result.is_negative = (is_negative != other.is_negative);
        // set digits
        result.digits.resize(digits.size() + other.digits.size(), 0);
        // multiplication digits by digits
        for (size_t i = 0; i < digits.size(); i++)
        {
            int carry = 0;
            for (size_t j = 0; j < other.digits.size(); j++)
            {
                int multi = digits[i] * other.digits[j] + result.digits[i + j] + carry;
                result.digits[i + j] = static_cast<uint8_t>(multi % 10); // Store the result digit
                carry = multi / 10;
            }
            // add leftover carry to next position
            result.digits[i + other.digits.size()] += carry;
        }

        result.removeLeadingZeros();
        return result;
    }

    /**
     * @brief Multiplication assignment operator.
     *
     * @param other The bigint to multiply.
     * @return The updated bigint.
     */
    bigint &operator*=(const bigint &other)
    {
        *this = *this * other;
        return *this;
    }

    /**
     * @brief Negation operator for bigints.
     *
     * @return A new bigint with the negated value.
     */
    bigint operator-() const
    {
        bigint result = *this;
        result.is_negative = !is_negative;
        return result;
    }
    /**
     * @brief Equality comparison operator for two bigints.
     *
     * @param other The bigint to compare.
     * @return True if the bigints are equal, false otherwise.
     */
    bool operator==(const bigint &other) const
    {
        return is_negative == other.is_negative && digits == other.digits;
    }
    /**
     * @brief Inequality comparison operator for two bigints.
     *
     * @param other The bigint to compare.
     * @return True if the bigints are not equal, false otherwise.
     */
    bool operator!=(const bigint &other) const
    {
        return !(*this == other);
    }

    /**
     * @brief Less-than comparison operator for two bigints.
     *
     * @param other The bigint to compare.
     * @return True if the current bigint is less than the other bigint, false otherwise.
     */
    bool operator<(const bigint &other) const
    {
        // different sign
        if (is_negative && !other.is_negative)
        {
            return true;
        }
        if (!is_negative && other.is_negative)
        {
            return false;
        }
        // same sign
        // different number digits
        if (digits.size() < other.digits.size())
        {
            return true;
        }
        if (digits.size() > other.digits.size())
        {
            return false;
        }

        // same digits
        // positive
        if (!is_negative)
        {
            for (size_t i = digits.size(); i > 0; i--)
            {
                // digit number smaller
                if (digits[i - 1] < other.digits[i - 1])
                {
                    return true;
                }
                // digit number larger
                if (digits[i - 1] > other.digits[i - 1])
                {
                    return false;
                }
            }
        }
        else
        {
            // negative, reserve the positive
            for (size_t i = digits.size(); i > 0; i--)
            {
                if (digits[i - 1] > other.digits[i - 1])
                {
                    return true;
                }
                if (digits[i - 1] < other.digits[i - 1])
                {
                    return false;
                }
            }
        }
        // equal
        return false;
    }

    /**
     * @brief Less-than or equal comparison operator for two bigints.
     *
     * @param other The bigint to compare.
     * @return True if the current bigint is less than or equal to the other bigint, false otherwise.
     */
    bool operator<=(const bigint &other) const
    {
        return *this < other || *this == other;
    }

    /**
     * @brief Greater-than comparison operator for two bigints.
     *
     * @param other The bigint to compare.
     * @return True if the current bigint is greater than the other bigint, false otherwise.
     */
    bool operator>(const bigint &other) const
    {
        return !(*this <= other);
    }
    /**
     * @brief Greater-than or equal comparison operator for two bigints.
     *
     * @param other The bigint to compare.
     * @return True if the current bigint is greater than or equal to the other bigint, false otherwise.
     */
    bool operator>=(const bigint &other) const
    {
        return !(*this < other);
    }

    /**
     * @brief Pre-increment operator.
     *
     * Increments the current bigint by 1.
     *
     * @return The updated bigint.
     */
    bigint &operator++()
    {
        *this += bigint(1);
        return *this;
    }

    /**
     * @brief Post-increment operator.
     *
     * Increments the current bigint by 1, returning the old value.
     *
     * @return A copy of the original bigint.
     */
    bigint operator++(int)
    {
        bigint temp = *this;
        ++(*this);
        return temp;
    }

    /**
     * @brief Pre-decrement operator.
     *
     * Decrements the current bigint by 1.
     *
     * @return The updated bigint.
     */
    bigint &operator--()
    {
        *this -= bigint(1);
        return *this;
    }

    /**
     * @brief Post-decrement operator.
     *
     * Decrements the current bigint by 1, returning the old value.
     *
     * @return A copy of the original bigint.
     */
    bigint operator--(int)
    {
        bigint temp = *this;
        --(*this);
        return temp;
    }

    /**
     * @brief Insertion operator for printing a bigint to an output stream.
     *
     * @param os The output stream.
     * @param value The bigint to print.
     * @return The output stream with the bigint inserted.
     */
    friend std::ostream &operator<<(std::ostream &os, const bigint &value)
    {
        if (value.is_negative)
        {
            os << '-';
        }
        for (size_t i = value.digits.size(); i > 0; i--)
        {
            os << static_cast<int>(value.digits[i - 1]);
        }
        return os;
    }
};
