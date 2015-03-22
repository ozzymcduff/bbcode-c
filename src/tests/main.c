#include <stdio.h>
#include "test_bbcode.h"
int main (int argc, const char * argv[]) {
    // insert code here...
  
  if (!bbcode_testfixture("test_bbcode.txt")){
    return 1;
  }
  return 0;
}
