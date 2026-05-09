#ifndef MODULE09_SIMPLIFY_TREE_H
#define MODULE09_SIMPLIFY_TREE_H

#include "formula_tree.h"

// М9: рекурсивное упрощение дерева:
// x+0=x, 0+x=x, x-0=x, x*1=x, 1*x=x, x*0=0, 0*x=0,
// x/1=x, x^1=x, x^0=1, 0^x=0
NodePtr simplifyTree(const NodePtr& node);

#endif
