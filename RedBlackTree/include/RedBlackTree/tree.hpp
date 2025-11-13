#pragma once

#include <cstddef>
#include <vector>

#include "RLogSU/error_handler.hpp"
#include "RLogSU/graph_appearance.hpp"
#include "RLogSU/logger.hpp"
#include "RedBlackTree/node.hpp"
#include "RedBlackTree/iterator.hpp"
#include "RLogSU/graph.hpp"

namespace Trees::RBT {

// namespace
// {
//     template<typename Iterator, typename ConstIterator, typename It >
//     concept ValidIterator = 
//         std::is_same_v<It, Iterator> || 
//         std::is_same_v<It, ConstIterator>;
// }

template <typename KeyT, typename Comp>
class Tree
{
public:
    Tree();
    Tree(const Tree& other);
    Tree(Tree&& other);
    Tree& operator=(const Tree& other);
    Tree& operator=(Tree&& other);
    ~Tree();
    
    typedef RBTIterator<KeyT, Comp, KeyT>       iterator;
    typedef RBTIterator<KeyT, Comp, const KeyT> const_iterator;
    
    friend iterator;
    friend const_iterator;

    // template<ValidIterator<iterator, const_iterator> Iterator>          // FIXME насколько это нормальное решение?
    // Iterator begin() const;
    
    iterator       begin()       { return CreateIterator     (BeginNode_()); };
    const_iterator begin() const { return CreateConstIterator(BeginNode_()); };

    iterator       end  ()       { return CreateIterator     (nil_); }
    const_iterator end  () const { return CreateConstIterator(nil_); }

    iterator       find(const KeyT& key)       { return CreateIterator     (FindInSubtree_(root_, key)); };
    const_iterator find(const KeyT& key) const { return CreateConstIterator(FindInSubtree_(root_, key)); };

    iterator       insert(const KeyT& new_key);

    void           erase(iterator erase_it)     { DeleteNode_(erase_it.node_ptr_); }
    void           erase(const KeyT& erase_key) { DeleteNode_(FindInSubtree_(root_, erase_key)); }

#ifndef NDEBUG
    void Dump() const;
#endif

private:
    inline static Comp comparator_ = {};
    using Node = RBTNode<KeyT, Comp>;

    Node* const nil_;
    Node* root_;

    Node *BeginNode_() const;

    Node* GetMin_(Node* subtree_root) const;
    Node* GetMax_(Node* subtree_root) const;

    Node* FindInSubtree_(Node* sub_root, const KeyT& key) const;

    void Transplant_   (Node* sub_root_1, Node* sub_root_2);
    void LeftRotate_   (Node* sub_root);
    void RightRotate_  (Node* sub_root);

    void  RemoveSubtree_(Node* sub_root);
    Node* CopySubtree_  (Node* sub_root, Tree* dest_tree, Node* dest_father) const;

    void DeleteNode_   (Node* del_node);

    void FixupInsert_(Node* inserted);
    void FixupDelete_(Node* fixup_node);

    iterator       CreateIterator     (Node* ptr) const { return iterator       (this, ptr); }
    const_iterator CreateConstIterator(Node* ptr) const { return const_iterator (this, ptr); }

#ifndef NDEBUG
    void AddNodeEdges_          (RLSU::Graphics::Graph& graph, const Node* node) const;
    void AddConfiduredGraphNode_(RLSU::Graphics::Graph& graph, const Node* node) const;
#endif
};


template <typename KeyT, typename Comp>
Tree<KeyT, Comp>::Tree()
    : nil_(new Node)
    , root_(nil_)
{}

template <typename KeyT, typename Comp>
Tree<KeyT, Comp>::Tree(const Tree& other)
    : nil_(new Node)
    , root_(nil_)
{
    root_ = other.CopySubtree_(other.root_, this, nil_);
}

template <typename KeyT, typename Comp>
Tree<KeyT, Comp>& Tree<KeyT, Comp>::operator=(const Tree& other)
{
    RemoveSubtree_(root_);

    root_ = other.CopySubtree_(other.root_, this, nil_);

    return *this;
}

template <typename KeyT, typename Comp>
Tree<KeyT, Comp>::Tree(Tree&& other)
{
    root_ = other.root_;
    nil_  = other.nil_;
}

template <typename KeyT, typename Comp>
Tree<KeyT, Comp>& Tree<KeyT, Comp>::operator=(Tree&& other)
{
    RemoveSubtree_(root_);

    root_ = other.root_;
    nil_  = other.nil_;

    return *this;
}

template <typename KeyT, typename Comp>
Tree<KeyT, Comp>::~Tree()
{
    RemoveSubtree_(root_);
    
    delete nil_;
}


template <typename KeyT, typename Comp>
void Tree<KeyT, Comp>::Transplant_(Node* replaceable, Node* substitute)
{
    RLSU_ASSERT(replaceable);

    if (replaceable == root_)
        root_ = substitute;

    else if (replaceable == replaceable->father->left)
        replaceable->father->left = substitute;

    else
        replaceable->father->right = substitute;

    substitute->father = replaceable->father;
}


template <typename KeyT, typename Comp>
void Tree<KeyT, Comp>::LeftRotate_(Node* sub_root)
{
    RLSU_ASSERT(sub_root);
    RLSU_ASSERT(sub_root->right != nil_);

    Node* right_son = sub_root->right;
    sub_root->right = right_son->left;

    if (right_son->left != nil_)
        right_son->left->father = sub_root;

    right_son->father = sub_root->father;

    if (sub_root == root_)
        root_ = right_son;

    else if (sub_root == sub_root->father->left)
        sub_root->father->left = right_son;

    else
        sub_root->father->right = right_son;

    right_son->left = sub_root;

    sub_root->father = right_son;
}


template <typename KeyT, typename Comp>
void Tree<KeyT, Comp>::RightRotate_(Node* sub_root)
{
    RLSU_ASSERT(sub_root);
    RLSU_ASSERT(sub_root->left != nil_);

    Node* left_son = sub_root->left;
    sub_root->left = left_son->right;

    if (left_son->right != nil_)
        left_son->right->father = sub_root;

    left_son->father = sub_root->father;

    if (sub_root == root_)
        root_ = left_son;

    else if (sub_root == sub_root->father->right)
        sub_root->father->right = left_son;

    else
        sub_root->father->left = left_son;

    left_son->right = sub_root;

    sub_root->father = left_son;
}


template <typename KeyT, typename Comp>
void Tree<KeyT, Comp>::RemoveSubtree_(Node* sub_root)
{
    if (sub_root == nil_)
        return;

    if (sub_root->left != nil_)
        ERROR_HANDLE(RemoveSubtree_(sub_root->left));

    if (sub_root->right != nil_)
        ERROR_HANDLE(RemoveSubtree_(sub_root->right));

    delete sub_root;
}


template <typename KeyT, typename Comp>
Tree<KeyT, Comp>::Node* Tree<KeyT, Comp>::CopySubtree_(Node* sub_root, Tree* dest_tree, Node* dest_father) const
{
    RLSU_ASSERT(sub_root);
    RLSU_ASSERT(dest_tree);
    RLSU_ASSERT(dest_father);
 
    RLSU_INFO("key = {}, nil = {}, sr = {}, l = {}, r = {}", sub_root->key, (void*)nil_, (void*) sub_root, (void*) sub_root->left, (void*) sub_root->right);

    if (sub_root == nil_)
        return dest_tree->nil_;

    Node* sub_root_cpy = new Node(sub_root->key, sub_root->color, dest_tree->nil_, dest_tree->nil_, dest_father);

    sub_root_cpy->left  = CopySubtree_(sub_root->left, dest_tree, sub_root_cpy);
    sub_root_cpy->right = CopySubtree_(sub_root->right, dest_tree, sub_root_cpy);

    return sub_root_cpy;
}


template <typename KeyT, typename Comp>
Tree<KeyT, Comp>::iterator Tree<KeyT, Comp>::insert(const KeyT& new_key)
{
    Node* future_father = nil_;
    Node* iterator_node = root_;

    while (iterator_node != nil_)
    {
        future_father = iterator_node;

        if (comparator_(iterator_node->key, new_key))
            iterator_node = iterator_node->left;

        else if (comparator_(new_key, iterator_node->key))
            iterator_node = iterator_node->right;

        else
            return CreateIterator(iterator_node);
    }

    Node* new_node = new Node(new_key, NodeColor::RED, nil_, nil_, nil_);
    new_node->father = future_father;

    if (future_father == nil_)
        root_ = new_node;

    else if (comparator_(new_key, future_father->key))
        future_father->right = new_node;

    else
        future_father->left = new_node;

    ERROR_HANDLE(FixupInsert_(new_node));

    return ERROR_HANDLE(CreateIterator(new_node));
}


template <typename KeyT, typename Comp>
void Tree<KeyT, Comp>::FixupInsert_(Node* inserted)
{
    Node *cur_node = inserted;

    while (cur_node->father->color == NodeColor::RED)
    {
        Node* father  = cur_node->father;
        Node* grandpa = father->father;

        if (father == grandpa->left)
        {
            Node *uncle = grandpa->right;

            if (uncle->color == NodeColor::RED)
            {
                father->color  = NodeColor::BLACK;
                uncle->color   = NodeColor::BLACK;
                grandpa->color = NodeColor::RED;

                cur_node = grandpa;
            }

            else if (cur_node == father->right)
            {
                cur_node = father;
                ERROR_HANDLE(LeftRotate_(cur_node));
            }

            else
            {
                father->color  = NodeColor::BLACK;
                grandpa->color = NodeColor::RED;

                ERROR_HANDLE(RightRotate_(grandpa));
            }
        }

        else
        {
            Node *uncle = grandpa->left;

            if (uncle->color == NodeColor::RED)
            {
                father->color  = NodeColor::BLACK;
                uncle->color   = NodeColor::BLACK;
                grandpa->color = NodeColor::RED;

                cur_node = grandpa;
            }

            else if (cur_node == father->left)
            {
                cur_node = father;
                ERROR_HANDLE(RightRotate_(cur_node));
            }

            else
            {
                father->color  = NodeColor::BLACK;
                grandpa->color = NodeColor::RED;

                ERROR_HANDLE(LeftRotate_(grandpa));
            }
        }
    }

    root_->color = NodeColor::BLACK;
}


template <typename KeyT, typename Comp>
void Tree<KeyT, Comp>::DeleteNode_(Node* del_node)
{
    Node* y_node = del_node;
    NodeColor y_start_color = y_node->color;
    
    Node* fixup_node = nil_;

    if (del_node == nil_)
        return;

    if (del_node->left == nil_)
    {
        fixup_node = del_node->right;
        Transplant_(del_node, del_node->right);
    }

    else if (del_node->right == nil_)
    {
        fixup_node = del_node->left;
        Transplant_(del_node, del_node->left);
    }
    
    else
    {
        y_node = GetMin_(del_node->right);
        y_start_color = y_node->color;
        fixup_node = y_node->right;

        if (y_node->father == del_node)
        {
            fixup_node->father = y_node;
        }

        else
        {
            Transplant_(y_node, y_node->right);
            y_node->right = del_node->right;
            y_node->right->father = y_node;
        }

        Transplant_(del_node, y_node);
        y_node->left = del_node->left;
        y_node->left->father = y_node;
        y_node->color = del_node->color;
    }

    delete del_node;

    if (y_start_color == NodeColor::BLACK)
        ERROR_HANDLE(FixupDelete_(fixup_node));
}


template <typename KeyT, typename Comp>
void Tree<KeyT, Comp>::FixupDelete_(Node* fixup_node)
{
    while (fixup_node != root_ && fixup_node->color == NodeColor::BLACK)
    {
        Node* father = fixup_node->father;

        if (fixup_node == father->left)
        {
            Node* brother = father->right;

            if (brother->color == NodeColor::RED)
            {
                brother->color = NodeColor::BLACK;
                father ->color = NodeColor::RED;

                ERROR_HANDLE(LeftRotate_(father));

                brother = father->right;
            }


            if (brother->left->color == NodeColor::BLACK && brother->right->color == NodeColor::BLACK)
            {
                brother->color = NodeColor::RED;
                fixup_node = father;
            }

            else if (brother->right->color == NodeColor::BLACK)
            {
                brother->left->color = NodeColor::BLACK;
                brother->color       = NodeColor::RED;

                ERROR_HANDLE(RightRotate_(brother));

                brother = father->right;
            }

            else
            {
                brother       ->color = father->color;
                father        ->color = NodeColor::BLACK;
                brother->right->color = NodeColor::BLACK;

                ERROR_HANDLE(LeftRotate_(father));

                fixup_node  = root_;
            }
        }

        else
        {
            Node* brother = father->left;

            if (brother->color == NodeColor::RED)
            {
                brother->color = NodeColor::BLACK;
                father ->color = NodeColor::RED;

                ERROR_HANDLE(RightRotate_(father));

                brother = father->left;
            }


            if (brother->right->color == NodeColor::BLACK && brother->left->color == NodeColor::BLACK)
            {
                brother->color = NodeColor::RED;
                fixup_node = father;
            }

            else if (brother->left->color == NodeColor::BLACK)
            {
                brother->right->color = NodeColor::BLACK;
                brother->color        = NodeColor::RED;

                ERROR_HANDLE(LeftRotate_(brother));

                brother = father->left;
            }

            else
            {
                brother      ->color = father->color;
                father       ->color = NodeColor::BLACK;
                brother->left->color = NodeColor::BLACK;

                ERROR_HANDLE(RightRotate_(father));

                fixup_node  = root_;
            }            
        }
    }

    fixup_node->color = NodeColor::BLACK;
}


template <typename KeyT, typename Comp>
Tree<KeyT, Comp>::Node* Tree<KeyT, Comp>::BeginNode_() const
{
    if (root_ == nil_)
        return nil_;

    return GetMin_(root_);
}


template <typename KeyT, typename Comp>
Tree<KeyT, Comp>::Node* Tree<KeyT, Comp>::FindInSubtree_(Node* sub_root, const KeyT& key) const
{
    if (sub_root == nil_)
        return nil_;

    if (key == sub_root->key)
        return sub_root;

    if (comparator_(key, sub_root->key))
        return FindInSubtree_(sub_root->right, key);
    
    else
        return FindInSubtree_(sub_root->left, key);
}

template <typename KeyT, typename Comp>
Tree<KeyT, Comp>::Node* Tree<KeyT, Comp>::GetMin_(Node* subtree_root) const
{
    Node* cur_node = subtree_root;

    while (cur_node->left != nil_)
    {
        cur_node = cur_node->left;
    }

    return cur_node;
}

template <typename KeyT, typename Comp>
Tree<KeyT, Comp>::Node* Tree<KeyT, Comp>::GetMax_(Node* subtree_root) const
{
    Node* cur_node = subtree_root;

    while (cur_node->right != nil_)
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

    graph.AddNode(nil_, "nil", RLSU::Graphics::Colors::BLACK, RLSU::Graphics::Colors::BLACK, RLSU::Graphics::Colors::PINK);

    if (root_ == nil_)
    {
        graph.LogGraph();
        return;
    }

    RLSU::Graphics::Graph::Node root_node(root_);
    root_node.SetLabel("{}", root_->key);
    root_node.SetColor      (RLSU::Graphics::Colors::DARKDARKRED);
    root_node.SetBorderColor(RLSU::Graphics::Colors::PINK);
    root_node.SetFontcolor  (RLSU::Graphics::Colors::PINK);
    graph.AddNode(root_node);
    
    graph.AddEdge(nil_, root_, 1000);

    for (const_iterator it = begin(); it != end(); ++it)
    {
        if (it.get() == root_)
            continue;

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

    if (node->color == NodeColor::RED)
    {
        new_graph_node.SetColor      (RLSU::Graphics::Colors::PINK);
        new_graph_node.SetBorderColor(RLSU::Graphics::Colors::BLACK);
        new_graph_node.SetFontcolor  (RLSU::Graphics::Colors::BLACK);
    }

    else
    {
        new_graph_node.SetColor      (RLSU::Graphics::Colors::BLACK);
        new_graph_node.SetBorderColor(RLSU::Graphics::Colors::PINK);
        new_graph_node.SetFontcolor  (RLSU::Graphics::Colors::PINK);
    }
    
    graph.AddNode(new_graph_node);
}


template <typename KeyT, typename Comp>
void Tree<KeyT, Comp>::AddNodeEdges_(RLSU::Graphics::Graph& graph, const Node* node) const
{
    if (node->left != nil_)
    {
        ERROR_HANDLE(graph.AddEdge(node, node->left, "l"));
    }

    else
    {
        // ERROR_HANDLE(graph.AddEdge(node, node->left, "l"));
        RLSU::Graphics::Graph::Node fictive_nil((void*)((size_t)node >> 2));

        fictive_nil.SetColor(graph.BACKGROWND_COLOR);
        fictive_nil.SetBorderColor(graph.BACKGROWND_COLOR);
        fictive_nil.SetLabel("");
        ERROR_HANDLE(graph.AddNode(fictive_nil));

        ERROR_HANDLE(graph.AddEdge(node, fictive_nil.OwnerPtr, "l"));
    }
    

    if (node->right != nil_)
    {
        ERROR_HANDLE(graph.AddEdge(node, node->right, "r"));
    }

    else
    {
        // ERROR_HANDLE(graph.AddEdge(node, node->right, "r"));

        RLSU::Graphics::Graph::Node fictive_nil((void*)(((size_t)node >> 3)));

        fictive_nil.SetColor(graph.BACKGROWND_COLOR);
        fictive_nil.SetBorderColor(graph.BACKGROWND_COLOR);
        fictive_nil.SetLabel("");
        ERROR_HANDLE(graph.AddNode(fictive_nil));

        ERROR_HANDLE(graph.AddEdge(node, fictive_nil.OwnerPtr, "r"));
    }
}

#endif

}