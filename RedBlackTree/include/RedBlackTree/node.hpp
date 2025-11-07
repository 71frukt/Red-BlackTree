#pragma once

namespace Trees::RBT {

template<typename KeyT, typename Comp>
class RBTNode
{
public:
    RBTNode(const KeyT& key_ref, RBTNode* left_ptr   = nullptr,
                                 RBTNode* right_ptr  = nullptr,
                                 RBTNode* father_ptr = nullptr)
        : key    (key_ref   )
        , left   (left_ptr  )
        , right  (right_ptr )
        , father (father_ptr)
    {}

    RBTNode() : RBTNode(0) {}

    KeyT key;

    RBTNode* left;
    RBTNode* right;
    RBTNode* father;

private:

};

}