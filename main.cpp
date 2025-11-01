#include <functional>
#include <vector>

#include "RedBlackTree/tree.hpp"

// #include "RedBlackTree/red-black_tree.hpp"

int main()
{
    std::vector<int> v(5);

    Trees::RBT::Tree<int, std::greater<int>> tree;

    auto it = tree.begin();
    it++;
}