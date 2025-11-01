#pragma once

#include <vector>

#include "RedBlackTree/node.hpp"
#include "RedBlackTree/iterator.hpp"

namespace Trees::RBT {

template <typename KeyT, typename Comp>
class Tree
{
public:
    Tree() = default;
    
    using Node = RBTNode<KeyT, Comp>;
    

    typedef RBTIterator<KeyT, Comp, KeyT>       iterator;
    typedef RBTIterator<KeyT, Comp, const KeyT> const_iterator;

    
    iterator       begin();
    const_iterator begin() const;
    
    iterator       end  ()       { return       iterator(nullptr); }
    const_iterator end  () const { return const_iterator(nullptr); }

    static Node* GetMin(const Node& subtree_root);
    static Node* GetMax(const Node& subtree_root);

private:
    std::vector<Node> nodes_;
    static Comp comparator_;

    Node* root_;
};



template <typename KeyT, typename Comp>
Tree<KeyT, Comp>::iterator Tree<KeyT, Comp>::begin()
{
    Node* min_node = GetMin(root_);

    return iterator(min_node);
}

template <typename KeyT, typename Comp>
Tree<KeyT, Comp>::const_iterator Tree<KeyT, Comp>::begin() const
{
    Node* min_node = GetMin(root_);

    return const_iterator(min_node);
}

template <typename KeyT, typename Comp>
Tree<KeyT, Comp>::Node* Tree<KeyT, Comp>::GetMin(const Node& subtree_root)
{
    Node* cur_node = &subtree_root;

    while (cur_node->left != nullptr)
    {
        cur_node = cur_node->left;
    }

    return cur_node;
}

template <typename KeyT, typename Comp>
Tree<KeyT, Comp>::Node* Tree<KeyT, Comp>::GetMax(const Node& subtree_root)
{
    Node* cur_node = &subtree_root;

    while (cur_node->right != nullptr)
    {
        cur_node = cur_node->right;
    }

    return cur_node;
}

}