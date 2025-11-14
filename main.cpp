#include <functional>
#include <iterator>

#include "RLogSU/logger.hpp"
#include "RedBlackTree/tree.hpp"

int main()
{
    Trees::RBT::Tree<int, std::greater<int>> tree;

    tree.insert(1 );
    tree.insert(2 );
    tree.insert(3 );
    tree.insert(4 );
    tree.insert(5 );
    tree.insert(6 );
    tree.insert(7 );
    tree.insert(8 );
    tree.insert(9 );
    tree.insert(10);

    Trees::RBT::Tree<int, std::greater<int>>::iterator start = tree.LowerBound(3);
    Trees::RBT::Tree<int, std::greater<int>>::iterator end   = tree.UpperBound(9);

    auto dist = std::distance(start, end);

    RLSU_DUMP(tree.Dump());

}