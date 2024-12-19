/**
 * @file test.cpp
 * @brief Testing the BigInt class and its functionalities.
 *
 * This file tests various operations on the bigint class such as addition,
 * subtraction, multiplication, comparison, and large number operations.
 */

#include <iostream>
#include <sstream>
#include <cassert>
#include <stdexcept>
#include <random>
#include <string>
#include <limits>
#include "bigint.hpp"

/**
 * @var successCount
 * @brief Count of the tests that passed.
 */
int successCount = 0;

/**
 * @var failureCount
 * @brief Count of the tests that failed.
 */
int failureCount = 0;

/**
 * @brief Tests different BigInt operations and verifies the results.
 *
 * This function tests constructors, arithmetic operators (+, -, *, etc.),
 * comparison operators, and large number operations.
 * Helper functions to verify the correctness of each operation.
 */
void testBigInt()
{
    /**
     * @brief Helper function to track success or failure of individual tests.
     *
     * @param testName The name of the test being performed.
     * @param condition The condition to check if the test passed.
     */
    auto testSuccess = [](const std::string &testName, bool condition)
    {
        if (condition)
        {
            std::cout << testName << " passed." << std::endl;
            successCount++;
        }
        else
        {
            std::cout << testName << " failed." << std::endl;
            failureCount++;
        }
    };

    // Default Constructor
    {
        bigint a;
        std::ostringstream oss;
        oss << a;
        testSuccess("Default constructor", oss.str() == "0");
    }

    // Constructor with 64-bit integer
    {
        bigint a(9223372036854775807LL);
        std::ostringstream oss;
        oss << a;
        testSuccess("Constructor with 64-bit integer", oss.str() == "9223372036854775807");
    }
    // Invalid Input (Empty string)
    try
    {
        bigint b("");
        testSuccess("Empty string input", false);
    }
    catch (const std::invalid_argument &e)
    {
        testSuccess("Empty string input", std::string(e.what()) == "Invalid input string");
    }
    catch (...)
    {
        testSuccess("Empty string input", false);
    }

    // Invalid Input (Non-numeric string)
    try
    {
        bigint c("abc123");
        testSuccess("Non-numeric string input", false);
    }
    catch (const std::invalid_argument &e)
    {
        testSuccess("Non-numeric string input", std::string(e.what()) == "Invalid digit in string");
    }
    catch (...)
    {
        testSuccess("Non-numeric string input", false);
    }

    // Constructor with string
    {
        bigint a("100000000000000000000");
        std::ostringstream oss;
        oss << a;
        testSuccess("Constructor with string", oss.str() == "100000000000000000000");
    }

    // Addition (+ and +=)
    {
        bigint a("999999999999999999999");
        bigint b("1");
        bigint result = a + b;
        std::ostringstream oss;
        oss << result;
        testSuccess("Addition (+)", oss.str() == "1000000000000000000000");

        a += b;
        oss.str("");
        oss.clear();
        oss << a;
        testSuccess("Addition (+=)", oss.str() == "1000000000000000000000");
    }

    // Subtraction (- and -=)
    {
        bigint a("1000000000000000000000");
        bigint b("1");
        bigint result = a - b;
        std::ostringstream oss;
        oss << result;
        testSuccess("Subtraction (-)", oss.str() == "999999999999999999999");

        a -= b;
        oss.str("");
        oss.clear();
        oss << a;
        testSuccess("Subtraction (-=)", oss.str() == "999999999999999999999");
    }

    // Multiplication (* and *=)
    {
        bigint a("123456789");
        bigint b("987654321");
        bigint result = a * b;
        std::ostringstream oss;
        oss << result;
        testSuccess("Multiplication (*)", oss.str() == "121932631112635269");

        a *= b;
        oss.str("");
        oss.clear();
        oss << a;
        testSuccess("Multiplication (*=)", oss.str() == "121932631112635269");
    }

    // Negation (unary -)
    {
        bigint a("123456789");
        bigint b = -a;
        std::ostringstream oss;
        oss << b;
        testSuccess("Negation (-)", oss.str() == "-123456789");
    }

    // Comparison (==, !=, <, >, <=, >=)
    {
        bigint a("123");
        bigint b("456");

        testSuccess("Comparison", a < b && a <= b && b > a && b >= a && a != b && !(a == b));
    }

    // Increment (++, both pre-increment and post-increment)
    {
        bigint a("999");
        ++a;
        std::ostringstream oss;
        oss << a;
        testSuccess("Pre-increment (++a)", oss.str() == "1000");

        a++;
        oss.str("");
        oss.clear();
        oss << a;
        testSuccess("Post-increment (a++)", oss.str() == "1001");
    }

    // Decrement (--, both pre-decrement and post-decrement)
    {
        bigint a("1001");
        --a;
        std::ostringstream oss;
        oss << a;
        testSuccess("Pre-decrement (--a)", oss.str() == "1000");

        a--;
        oss.str("");
        oss.clear();
        oss << a;
        testSuccess("Post-decrement (a--)", oss.str() == "999");
    }

    // Combined Operations: (A + B) - C == (A - C) + B
    try
    {
        bigint a("1000000000000000000000000");
        bigint b("500000000000000000000000");
        bigint c("200000000000000000000000");

        bigint result1 = (a + b) - c;
        bigint result2 = (a - c) + b;

        std::ostringstream oss1, oss2;
        oss1 << result1;
        oss2 << result2;

        testSuccess("Test combined addition and subtraction", oss1.str() == oss2.str());
    }
    catch (const std::exception &e)
    {
        testSuccess("Test combined addition and subtraction", false);
    }

    // Combined Operations: (A + B) * C == (A * C) + (B * C)
    try
    {
        bigint a("1000000000000000000000000");
        bigint b("500000000000000000000000");
        bigint c("200000000000000000000000");

        // (A + B) * C
        bigint result1 = (a + b) * c;
        // (A * C) + (B * C)
        bigint result2 = (a * c) + (b * c);

        std::ostringstream oss1, oss2;
        oss1 << result1;
        oss2 << result2;

        testSuccess("Test combined multiplication, addition, and subtraction", oss1.str() == oss2.str());
    }
    catch (const std::exception &e)
    {
        testSuccess("Test combined multiplication, addition, and subtraction", false);
    }

    // Test large number operations
    try
    {
        // Create large numbers
        std::string largeNumber1(1000, '1');
        std::string largeNumber2(1000, '2');

        // Create bigint objects
        bigint a(largeNumber1);
        bigint b(largeNumber2);

        // Perform operations
        bigint resultAdd = a + b;
        bigint resultSub = a - b;

        // Convert results to string for comparison
        std::ostringstream ossAdd, ossSub, ossMul;
        ossAdd << resultAdd;
        ossSub << resultSub;

        // Expected results
        std::string expectedAdd(1000, '3');
        std::string expectedSub = "-" + std::string(1000, '1');

        // Check if results match expected values
        testSuccess("Test large number addition", ossAdd.str() == expectedAdd);
        testSuccess("Test large number subtraction", ossSub.str() == expectedSub);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error during large number operations: " << e.what() << std::endl;
        testSuccess("Test large number operations", false);
    }
}

/**
 * @brief Main function to execute the bigint tests and display results.
 *
 * This function runs the testBigInt() function and prints the results of the tests.
 * It also handles unexpected exceptions and displays an error message.
 *
 * @return Returns 0 if all tests passed, 1 if an unexpected error occurred, or 2 for an unknown error.
 */

int main()
{
    try
    {
        testBigInt();

        // Print summary
        std::cout << "\nSummary:\n";
        std::cout << "Tests passed: " << successCount << std::endl;
        std::cout << "Tests failed: " << failureCount << std::endl;

        if (failureCount == 0)
        {
            std::cout << "All tests passed!" << std::endl;
        }
        else
        {
            std::cout << "Some tests failed. Check the details above." << std::endl;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
        return 1;
    }
    catch (...)
    {
        std::cerr << "Unknown error occurred!" << std::endl;
        return 2;
    }

    return 0;
}
