# CSE701_final_project

## Overview

The `bigint` class is a custom implementation of arbitrary-precision integers, designed to handle integer values that exceed the limits of standard data types like `int` and `long long`. This program tests the `bigint` class extensively using various test cases for arithmetic operations, comparisons, and error handling.

---

## Key Concepts and Algorithms

1. **Arbitrary-Precision Representation**:
   - Numbers are stored as unsigned 8 bit vector internally in the reverse order to handle very large values.
   - Operations are implemented manually (e.g., addition, subtraction, multiplication) using algorithms similar to elementary arithmetic.

2. **Error Handling**:
   - The class throws exceptions for invalid inputs, such as non-numeric strings or empty strings.

3. **Operator Overloading**:
   - Common operators (`+`, `-`, `*`, `+=`, `-=`, `*=`, `==`, `<`, etc.) are overloaded to provide a seamless interface.
     
## Testing Framework

### Basic Constructors
- Default constructor initializes to `0`.
- String and integer constructors handle valid and invalid inputs.

### Arithmetic Operations
- Verifies results for:
  - Addition
  - Subtraction
  - Multiplication
 
### Comparison Operations
- Tests relational and equality operators.

### Edge Cases
- Handles extremely large numbers (e.g., 1000+ digits).
- Catches invalid inputs such as empty or non-numeric strings.

### Combined Operations
- Verifies distributive properties, e.g., 
  (A + B) * C = (A * C) + (B * C)

## Example Outputs

Here’s an example of the program output:

```bash
Default constructor passed.
Constructor with 64-bit integer passed.
Empty string input passed.
Non-numeric string input passed.
Constructor with string passed.
Addition (+) passed.
Addition (+=) passed.
Subtraction (-) passed.
Subtraction (-=) passed.
Multiplication (*) passed.
Multiplication (*=) passed.
Negation (-) passed.
Comparison passed.
Pre-increment (++a) passed.
Post-increment (a++) passed.
Pre-decrement (--a) passed.
Post-decrement (a--) passed.
Test combined addition and subtraction passed.
Test combined multiplication, addition, and subtraction passed.
Test large number addition passed.
Test large number subtraction passed.

Summary:
Tests passed: 20
Tests failed: 0
All tests passed!
