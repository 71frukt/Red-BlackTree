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

    tree.insert(1);
    tree.insert(6);
    tree.insert(15);
    tree.insert(7);
    tree.insert(17);
    tree.insert(2);
    tree.insert(5);
    tree.insert(16);
    tree.insert(3);

    RLSU_DUMP(tree.Dump());

    tree.erase(15);
    RLSU_DUMP(tree.Dump());

    tree.erase(1);
    RLSU_DUMP(tree.Dump());

    tree.erase(2);
    RLSU_DUMP(tree.Dump());
}