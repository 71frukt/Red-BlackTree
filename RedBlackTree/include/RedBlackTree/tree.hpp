#pragma once

#include <functional>

#include "RLogSU/logger.hpp"
#include "RedBlackTree/node.hpp"
#include "RedBlackTree/iterator.hpp"
#include "RLogSU/graph.hpp"

namespace Trees::RBT {

template <typename KeyT, typename Comp>
class Tree
{
public:
    Tree() = default;

    ~Tree();
    
    using Node = RBTNode<KeyT, Comp>;
    
    typedef RBTIterator<KeyT, Comp, KeyT>       iterator;
    typedef RBTIterator<KeyT, Comp, const KeyT> const_iterator;

    iterator       begin();
    const_iterator begin() const;
    
    iterator       end  ()       { return       iterator(nullptr); }
    const_iterator end  () const { return const_iterator(nullptr); }

    static Node* GetMin(Node& subtree_root);
    static Node* GetMax(Node& subtree_root);

    Node* Insert(const KeyT& key);

#ifndef NDEBUG
    void Dump() const;
#endif

private:
    static Comp comparator_;

    Node* root_;

    void DeleteSubtree_(Node* sub_root);

#ifndef NDEBUG
    void AddNodeEdges_          (RLSU::Graphics::Graph& graph, const Node* node) const;
    void AddConfiduredGraphNode_(RLSU::Graphics::Graph& graph, const Node* node) const;
#endif
};


template <typename KeyT, typename Comp>
Tree<KeyT, Comp>::~Tree()
{
    DeleteSubtree_(root_);
}


template <typename KeyT, typename Comp>
void Tree<KeyT, Comp>::DeleteSubtree_(Node* sub_root)
{
    if (sub_root->left != nullptr)
        DeleteSubtree_(sub_root->left);

    if (sub_root->right != nullptr)
        DeleteSubtree_(sub_root->right);

    delete sub_root;
}


template <typename KeyT, typename Comp>
Tree<KeyT, Comp>::Node* Tree<KeyT, Comp>::Insert(const KeyT& new_key)
{
    Node* new_node = new Node(new_key);

    Node* future_father = nullptr;
    Node* iterator_node = root_;

    while (iterator_node != nullptr)
    {
        future_father = iterator_node;

        if (new_key < iterator_node->key)
            iterator_node = iterator_node->left;

        else
            iterator_node = iterator_node->right;
    }

    new_node->father = future_father;

    if (future_father == nullptr)
        root_ = new_node;

    else if (new_key < future_father->key)
        future_father->left = new_node;

    else
        future_father->right = new_node;

    return new_node;
}


template <typename KeyT, typename Comp>
Tree<KeyT, Comp>::iterator Tree<KeyT, Comp>::begin()
{
    if (root_ == nullptr)
        return iterator(nullptr);

    Node* min_node = GetMin(*root_);

    return iterator(min_node);
}

template <typename KeyT, typename Comp>
Tree<KeyT, Comp>::const_iterator Tree<KeyT, Comp>::begin() const
{
    if (root_ == nullptr)
        return iterator(nullptr);

    Node* min_node = GetMin(*root_);

    return const_iterator(min_node);
}

template <typename KeyT, typename Comp>
Tree<KeyT, Comp>::Node* Tree<KeyT, Comp>::GetMin(Node& subtree_root)
{
    Node* cur_node = &subtree_root;

    while (cur_node->left != nullptr)
    {
        cur_node = cur_node->left;
    }

    return cur_node;
}

template <typename KeyT, typename Comp>
Tree<KeyT, Comp>::Node* Tree<KeyT, Comp>::GetMax(Node& subtree_root)
{
    Node* cur_node = &subtree_root;

    while (cur_node->right != nullptr)
    {
        cur_node = cur_node->right;
    }

    return cur_node;
}


#ifndef NDEBUG

template <typename KeyT, typename Comp>
void Tree<KeyT, Comp>::Dump() const
{
    RLSU::Graphics::Graph graph;

    for (const_iterator it = begin(); it != end(); ++it)
    {
        AddConfiduredGraphNode_(graph, it.get());
    }

    for (const_iterator it = begin(); it != end(); ++it)
    {
        AddNodeEdges_(graph, it.get());
    }

    graph.LogGraph();
}


template <typename KeyT, typename Comp>
void Tree<KeyT, Comp>::AddConfiduredGraphNode_(RLSU::Graphics::Graph& graph, const Node* node) const
{
    RLSU::Graphics::Graph::Node new_graph_node(node);

    new_graph_node.SetLabel("{}", node->key);

    if (node == root_)
    {
        new_graph_node.SetColor(RLSU::Graphics::Colors::AQUAMARINE);
    }

    else 
    {
        new_graph_node.SetColor(RLSU::Graphics::Colors::SKYBLUE);
    }

    graph.AddNode(new_graph_node);
}


template <typename KeyT, typename Comp>
void Tree<KeyT, Comp>::AddNodeEdges_(RLSU::Graphics::Graph& graph, const Node* node) const
{
    if (node->left != nullptr)
    {
        graph.AddEdge(node, node->left );
    }

    else
    {
        RLSU::Graphics::Graph::Node null_node((void*)((size_t)node >> 2));

        null_node.SetColor(graph.BACKGROWND_COLOR);
        null_node.SetBorderColor(graph.BACKGROWND_COLOR);
        null_node.SetLabel("");
        graph.AddNode(null_node);

        graph.AddEdge(node, null_node.OwnerPtr);
    }
    

    if (node->right != nullptr)
    {
        graph.AddEdge(node, node->right);
    }

    else
    {
        RLSU::Graphics::Graph::Node null_node((void*)(((size_t)node >> 3)));

        null_node.SetColor(graph.BACKGROWND_COLOR);
        null_node.SetBorderColor(graph.BACKGROWND_COLOR);
        null_node.SetLabel("");
        graph.AddNode(null_node);

        graph.AddEdge(node, null_node.OwnerPtr);
    }
}

#endif

}