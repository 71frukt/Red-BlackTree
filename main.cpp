#include <functional>
#include <vector>

#include "RLogSU/logger.hpp"
#include "RedBlackTree/tree.hpp"

// #include "RedBlackTree/red-black_tree.hpp"

int main()
{
    std::vector<int> v(5);

    Trees::RBT::Tree<int, std::greater<int>> tree;

    RLSU_DUMP(tree.Dump());

    tree.insert(1);
    tree.insert(2);
    tree.insert(3);
    tree.insert(4);
    tree.insert(5);

    RLSU_DUMP(tree.Dump());

    Trees::RBT::Tree<int, std::greater<int>> tree1(tree);
    tree1.Dump();

    tree1 = tree;
    tree1.Dump();
}