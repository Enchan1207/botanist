/// @file
/// @brief 構文ツリーの直列化
///

#ifndef BOTANIST_SERIALIZER_H
#define BOTANIST_SERIALIZER_H

#include "syntaxtree.hpp"

namespace botanist {

class Serializer {
   private:
    /* data */
   public:
    Serializer(/* args */);
    ~Serializer();

    void serialize(SyntaxNode* node);
};

}  // namespace botanist

#endif