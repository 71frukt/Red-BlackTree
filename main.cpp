#include <functional>
#include <vector>

#include "RLogSU/logger.hpp"
#include "RedBlackTree/tree.hpp"

// #include "RedBlackTree/red-black_tree.hpp"

int main()
{
    std::vector<int> v(5);

    Trees::RBT::Tree<int, std::greater<int>> tree;

    // auto it = tree.begin();
    // it++;

    tree.Insert(17);
    tree.Insert(15);
    tree.Insert(16);
    tree.Insert(1);
    tree.Insert(6);
    tree.Insert(7);
    tree.Insert(2);
    tree.Insert(3);
    tree.Insert(5);
    
    RLSU_DUMP(tree.Dump());
}