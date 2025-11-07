#pragma once

#include <iostream>

#include "RedBlackTree/node.hpp"
#include "RLogSU/logger.hpp"

namespace Trees::RBT {

template <typename KeyT, typename Comp>
class Tree;

template <typename TreeKeyT, typename TreeComp, typename IteratorKeyT>
class RBTIterator
{
    friend class Tree<TreeKeyT, TreeComp>;

    template<typename A, typename B, typename C>
    friend class RBTIterator;

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

    Node* get() const { return node_ptr_; }

    operator RBTIterator<TreeKeyT, TreeComp, const IteratorKeyT>() const { return RBTIterator<TreeKeyT, TreeComp, const IteratorKeyT>(node_ptr_); }
    
    RBTIterator& operator++()
    {
        *this = GetNext_();
        return *this;
    };

    RBTIterator operator++(int)
    {
        RBTIterator temp = *this;
        ++(*this);
        return temp;
    }

    bool operator==(const RBTIterator& other) const
    {
        return this->get() == other.get();
    };

    bool operator!=(const RBTIterator& other) const
    {
        return this->get() != other.get();
    };

private:
    Node* node_ptr_;


    [[nodiscard]] RBTIterator GetNext_()
    {
        Node* cur_node = node_ptr_;
        
        if (cur_node == nullptr)
        {
            RLSU_WARNING("attempt to increment iterator on nullptr");
            return RBTIterator(nullptr);
        }

        if (cur_node->right != nullptr)
        {
            cur_node = cur_node->right;

            while (cur_node->left != nullptr)
            {
                cur_node = cur_node->left;
            }

            return RBTIterator(cur_node);
        }

        // else

        Node* father = cur_node->father;

        while (father != nullptr && cur_node == father->right)
        {
            cur_node = father;
            father = cur_node->father;
        }

        return RBTIterator(father);
    }
};

}