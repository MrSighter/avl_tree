#include "avl_tree_test.h"

int main() {
    //subtractStressTest();
    minusTest();
    test(-1);
    std::cout<<std::endl;
    testMaxInfoSelector();
    std::cout<<std::endl;
    speedTest();
}