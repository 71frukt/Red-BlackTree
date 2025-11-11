#pragma once

#include <cstddef>
#include <vector>

#include "RLogSU/graph_appearance.hpp"
#include "RLogSU/logger.hpp"
#include "RedBlackTree/node.hpp"
#include "RedBlackTree/iterator.hpp"
#include "RLogSU/graph.hpp"

namespace Trees::RBT {

namespace
{
    template<typename Iterator, typename ConstIterator, typename It >
    concept ValidIterator = 
        std::is_same_v<It, Iterator> || 
        std::is_same_v<It, ConstIterator>;
}

template <typename KeyT, typename Comp>
class Tree
{
public:
    Tree() = default;

    ~Tree();
    
    typedef RBTIterator<KeyT, Comp, KeyT>       iterator;
    typedef RBTIterator<KeyT, Comp, const KeyT> const_iterator;


    template<ValidIterator<iterator, const_iterator> Iterator>          // FIXME насколько это нормальное решение?
    Iterator begin() const;
    iterator       begin()       { return begin<iterator>();       };
    const_iterator begin() const { return begin<const_iterator>(); };

    iterator       end  ()       { return       iterator(nullptr); }
    const_iterator end  () const { return const_iterator(nullptr); }

    iterator       find(const KeyT& key)       { return iterator      (FindInSubtree_(root_, key)); };
    const_iterator find(const KeyT& key) const { return const_iterator(FindInSubtree_(root_, key)); };

    iterator       insert(const KeyT& new_key);

    void           erase(iterator erase_it)     { DeleteNode_(erase_it.node_ptr_); }
    void           erase(const KeyT& erase_key) { DeleteNode_(FindInSubtree_(root_, erase_key)); }

#ifndef NDEBUG
    void Dump() const;
#endif

private:
    inline static Comp comparator_ = {};
    using Node = RBTNode<KeyT, Comp>;

    Node* root_;

    static Node* GetMin_(Node* subtree_root);
    static Node* GetMax_(Node* subtree_root);

    static Node* FindInSubtree_(Node* sub_root, const KeyT& key);

    void Transplant_   (Node* sub_root_1, Node* sub_root_2);
    void DeleteSubtree_(Node* sub_root);
    void DeleteNode_   (Node* del_node);

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
void Tree<KeyT, Comp>::Transplant_(Node* replaceable, Node* substitute)
{
    RLSU_ASSERT(replaceable);

    if (replaceable == root_)
    {
        root_ = substitute;
    }

    else if (replaceable == replaceable->father->left)
    {
        replaceable->father->left = substitute;
    }

    else
    {
        replaceable->father->right = substitute;
    }

    if (substitute != nullptr)
    {
        substitute->father = replaceable->father;
    }
}

template <typename KeyT, typename Comp>
void Tree<KeyT, Comp>::DeleteSubtree_(Node* sub_root)
{
    RLSU_ASSERT(sub_root);

    if (sub_root->left != nullptr)
        DeleteSubtree_(sub_root->left);

    if (sub_root->right != nullptr)
        DeleteSubtree_(sub_root->right);

    delete sub_root;
}


template <typename KeyT, typename Comp>
Tree<KeyT, Comp>::iterator Tree<KeyT, Comp>::insert(const KeyT& new_key)
{
    Node* future_father = nullptr;
    Node* iterator_node = root_;

    while (iterator_node != nullptr)
    {
        future_father = iterator_node;

        if (comparator_(iterator_node->key, new_key))
            iterator_node = iterator_node->left;

        else if (comparator_(new_key, iterator_node->key))
            iterator_node = iterator_node->right;

        else
            return iterator(iterator_node);
    }

    Node* new_node = new Node(new_key);
    new_node->father = future_father;

    if (future_father == nullptr)
        root_ = new_node;

    else if (comparator_(new_key, future_father->key))
        future_father->right = new_node;

    else
        future_father->left = new_node;

    return iterator(new_node);
}


template <typename KeyT, typename Comp>
void Tree<KeyT, Comp>::DeleteNode_(Node* del_node)
{
    if (del_node == nullptr)
        return;

    if (del_node->left == nullptr)
    {
        Transplant_(del_node, del_node->right);
    }

    else if (del_node->right == nullptr)
    {
        Transplant_(del_node, del_node->left);
    }
    
    else
    {
        Node* min = GetMin_(del_node->right);

        if (min->father != del_node)
        {
            Transplant_(min, min->right);
            min->right = del_node->right;
            min->right->father = min;
        }

        Transplant_(del_node, min);
        min->left = del_node->left;
        min->left->father = min;
    }

    delete del_node;
}


template <typename KeyT, typename Comp>
template <ValidIterator<typename Tree<KeyT, Comp>::iterator, typename Tree<KeyT, Comp>::const_iterator> It>
It Tree<KeyT, Comp>::begin() const
{
    if (root_ == nullptr)
        return It(nullptr);

    Node* min_node = GetMin_(root_);

    return It(min_node);
}


template <typename KeyT, typename Comp>
Tree<KeyT, Comp>::Node* Tree<KeyT, Comp>::FindInSubtree_(Node* sub_root, const KeyT& key)
{
    if (sub_root == nullptr)
        return  nullptr;

    if (key == sub_root->key)
        return sub_root;

    if (comparator_(key, sub_root->key))
        return FindInSubtree_(sub_root->right, key);
    
    else
        return FindInSubtree_(sub_root->left, key);
}

template <typename KeyT, typename Comp>
Tree<KeyT, Comp>::Node* Tree<KeyT, Comp>::GetMin_(Node* subtree_root)
{
    Node* cur_node = subtree_root;

    while (cur_node->left != nullptr)
    {
        cur_node = cur_node->left;
    }

    return cur_node;
}

template <typename KeyT, typename Comp>
Tree<KeyT, Comp>::Node* Tree<KeyT, Comp>::GetMax_(Node* subtree_root)
{
    Node* cur_node = subtree_root;

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
    RLSU::Graphics::Graph graph(
        [](size_t graph_size) -> size_t {
            return (size_t)std::pow(2, std::log2(graph_size) / 2);
        }
    );

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
        graph.AddEdge(node, node->left, "l");
    }

    else
    {
        RLSU::Graphics::Graph::Node null_node((void*)((size_t)node >> 2));

        null_node.SetColor(graph.BACKGROWND_COLOR);
        null_node.SetBorderColor(graph.BACKGROWND_COLOR);
        null_node.SetLabel("");
        graph.AddNode(null_node);

        graph.AddEdge(node, null_node.OwnerPtr, "l");
    }
    

    if (node->right != nullptr)
    {
        graph.AddEdge(node, node->right, "r");
    }

    else
    {
        RLSU::Graphics::Graph::Node null_node((void*)(((size_t)node >> 3)));

        null_node.SetColor(graph.BACKGROWND_COLOR);
        null_node.SetBorderColor(graph.BACKGROWND_COLOR);
        null_node.SetLabel("");
        graph.AddNode(null_node);

        graph.AddEdge(node, null_node.OwnerPtr, "r");
    }
}

#endif

}