#pragma once

namespace Trees::RBT {

enum class NodeColor { RED, BLACK };

template<typename KeyT, typename Comp>
class RBTNode
{
public:
    RBTNode(const KeyT& key_ref, NodeColor node_color, RBTNode* left_ptr   = nullptr,
                                                       RBTNode* right_ptr  = nullptr,
                                                       RBTNode* father_ptr = nullptr)
        : key    (key_ref   )
        , color  (node_color)
        , left   (left_ptr  )
        , right  (right_ptr )
        , father (father_ptr)
    {}

    RBTNode() : RBTNode({}, NodeColor::BLACK) {}

    KeyT      key;
    NodeColor color;

    RBTNode*  left;
    RBTNode*  right;
    RBTNode*  father;

private:
};

}