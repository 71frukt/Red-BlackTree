#pragma once

#include <iostream>

#include "RLogSU/error_handler.hpp"
#include "RedBlackTree/node.hpp"
#include "RLogSU/logger.hpp"

namespace Trees::RBT {

template <typename KeyT, typename Comp>
class Tree;

template <typename TreeKeyT, typename TreeComp, typename IteratorKeyT>
class RBTIterator// : public std::iterator<std::input_iterator_tag, IteratorKeyT>
{
    friend class Tree<TreeKeyT, TreeComp>;
    using Node = RBT::RBTNode<TreeKeyT, TreeComp>;

    using iterator_category = std::input_iterator_tag;
    using value_type        = IteratorKeyT;
    using difference_type   = std::ptrdiff_t;
    using pointer           = IteratorKeyT*;
    using reference         = IteratorKeyT&;

private:
    explicit RBTIterator(Node *node_ptr)
    : node_ptr_(node_ptr)
    {}
    
public:
    const IteratorKeyT& operator*()  const { return node_ptr_->key; }
          IteratorKeyT& operator*()        { return node_ptr_->key; }
    
    const IteratorKeyT* operator->() const { return &node_ptr_->key; }
          IteratorKeyT* operator->()       { return &node_ptr_->key; }

    operator RBTIterator<TreeKeyT, TreeComp, const TreeKeyT>() const { return Iterator<TreeKeyT, TreeComp, const TreeKeyT>(node_ptr_); }
    
    RBTIterator& operator++()
    {
        *this = GetNext_(*this);
        return *this;
    };

    RBTIterator operator++(int)
    {
        RBTIterator temp = *this;
        ++(*this);
        return temp;
    }

private:
    Node* node_ptr_;

    [[nodiscard]] RBTIterator GetNext_(const RBTIterator& cur_it) const
    {
        Node* cur_node = cur_it.node_ptr_;
        
        RLSU_ASSERT(cur_node, "attempt to increment iterator on nullptr");

        if (cur_node->right != nullptr)
        {
            cur_node = cur_node->right;

            while (cur_node->left != nullptr)
            {
                cur_node = cur_node->left;
            }

            return RBTIterator<TreeKeyT, TreeComp, TreeKeyT>(cur_node);
        }

        // else

        Node* father = cur_node->father;

        while (father != nullptr && cur_node == father->right)
        {
            cur_node = father;
            father = cur_node->father;
        }

        return RBTIterator<TreeKeyT, TreeComp, TreeKeyT>(father);
    }
};

}