#pragma once

namespace Trees::RBT {

template<typename KeyT, typename Comp>
class RBTNode
{
public:
    RBTNode(const KeyT& key_ref, const RBTNode* const left_ptr, const RBTNode* const right_ptr, const RBTNode* const father_ptr)
        : key    (key_ref   )
        , left   (left_ptr  )
        , right  (right_ptr )
        , father (father_ptr)
    {}

    KeyT key;

    RBTNode *left;
    RBTNode *right;
    RBTNode *father;

private:

};

}