#include <stdio.h>
#include "../include/create.h"

// Define a function to compare expected and actual results
void assert_equal(int expected, int actual) {
  if (expected == actual) {
    printf("PASS\n");
  } else {
    printf("FAIL: Expected %d, but got %d\n", expected, actual);
  }
}

// Define and run your test cases
int main() {
  // Test case 1: Test the add function
  int result = add(2, 3);
  assert_equal(5, result);

  // Test case 2: Test some other functionality
  // ...

  return 0;
}
