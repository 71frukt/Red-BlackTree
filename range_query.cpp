#include <iostream>
#include <iterator>
#include "RLogSU/logger.hpp"
#include "RedBlackTree/tree.hpp"

// #include "RedBlackTree/red-black_tree.hpp"

int main()
{
    Trees::RBT::Tree<int, std::greater<int>> tree;

    std::string command;

    while (std::cin >> command)
    {
        if (command == "k")
        {
            int key;
            std::cin >> key;
            tree.insert(key);
                RLSU_DUMP(tree.Dump());

        }
        
        else if (command == "q")
        {
            int a, b;
            std::cin >> a >> b;
            
            auto start = tree.end();
            auto end   = tree.end();

            if (a <= b)
            {
                start = tree.LowerBound(a);
                end   = tree.UpperBound(b);
            }

            else
            {
                start = tree.LowerBound(b);
                end   = tree.UpperBound(a);
            }

            // RLSU_INFO("a = {}, b = {}; start = {}, end = {}", a, b, *start, *end);
            
            std::ptrdiff_t dist = std::distance(start, end);
            std::cout << dist << " ";
        }

        else
        {
            RLSU_WARNING("invalid request: '{}'", command);
        }
    }

    RLSU_DUMP(tree.Dump());
}