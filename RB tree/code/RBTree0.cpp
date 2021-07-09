#include "RBTree.h"

//对于出现同名函数，请参考以下关于函数重载的定义理解
//函数重载是一种特殊情况，C++允许在同一作用域中声明几个类似的同名函数，这些同名函数的形参列表（参数个数，类型，顺序）必须不同，常用来处理实现功能类似数据类型不同的问题。


/*
 * 构造函数
 */

RBTree ::RBTree() :mRoot(NULL)
{
    mRoot = NULL;
}

/*
 * 析构函数
 */

RBTree ::~RBTree()
{
    destroy();
}

/*
 * 前序遍历"红黑树"
 */
void RBTree ::preOrder(RBTNode * tree) const
{
    if (tree != NULL)
    {
        cout << tree->index << " " << tree->mac << endl;
        preOrder(tree->left);
        preOrder(tree->right);
    }
}

void RBTree ::preOrder()
{
    preOrder(mRoot);
}



/*
 * 中序遍历"红黑树"
 */
void RBTree ::inOrder(RBTNode * tree) const
{
    if (tree != NULL)
    {
        inOrder(tree->left);
        cout << tree->index << " " << tree->mac << endl;
        inOrder(tree->right);
    }
}


void RBTree ::inOrder()
{
    inOrder(mRoot);
}

/*
 * 后序遍历"红黑树"
 */

void RBTree ::postOrder(RBTNode * tree) const
{
    if (tree != NULL)
    {
        postOrder(tree->left);
        postOrder(tree->right);
        cout << tree->index << " " << tree->mac;
    }
}


void RBTree ::postOrder()
{
    postOrder(mRoot);
}

/*
 * (递归实现)查找"红黑树x"中键值为index的节点
 */
RBTNode * RBTree ::search(RBTNode * x, int index) const
{
    if(x==NULL) return NULL;
    if(x->index == index){
        //cout<<x->index<<" : "<<x->mac<<endl;
        return x;
    }
    else if(x->index > index){
        return search(x->left, index);
    }
    else if(x->index < index){
        return search(x->right, index);
    }
}


RBTNode * RBTree ::search(int index)
{
    RBTNode* x = search(mRoot, index);
    return x;
}


/*
 * 对红黑树的节点(x)进行左旋转
 *
 * 左旋示意图(对节点x进行左旋)：
 *      px                              px
 *     /                               /
 *    x                               y
 *   /  \      --(左旋)-->           / \                #
 *  lx   y                          x  ry
 *     /   \                       /  \
 *    ly   ry                     lx  ly
 *
 *
 */

void RBTree ::leftRotate(RBTNode *& root, RBTNode * x)
{
    // 设置x的右孩子为y
    RBTNode*y = x->right;

    // 将 “y的左孩子” 设为 “x的右孩子”；
    // 如果y的左孩子非空，将 “x” 设为 “y的左孩子的父亲”
    x->right = y->left;
    if (y->left != NULL)
        y->left->parent = x;

    // 将 “x的父亲” 设为 “y的父亲”
    y->parent = x->parent;

    if (x->parent == NULL)
    {
        root = y;            // 如果 “x的父亲” 是空节点，则将y设为根节点
    }
    else
    {
        if (x->parent->left == x)
            x->parent->left = y;    // 如果 x是它父节点的左孩子，则将y设为“x的父节点的左孩子”
        else
            x->parent->right = y;   
    }
    
    // 将 “x” 设为 “y的左孩子”
    y->left = x;
    // 将 “x的父节点” 设为 “y”
    x->parent = y;
}

/*
 * 对红黑树的节点(y)进行右旋转
 *
 * 右旋示意图(对节点y进行左旋)：
 *            py                               py
 *           /                                /
 *          y                                x
 *         /  \      --(右旋)-->            /  \                     #
 *        x   ry                           lx   y
 *       / \                                   / \                   #
 *      lx  rx                                rx  ry
 *
 */

void RBTree ::rightRotate(RBTNode *& root, RBTNode * y)
{
    RBTNode*x = y->left;

    // 将 “x的右孩子” 设为 “y的左孩子”；
    // 如果"x的右孩子"不为空的话，将 “y” 设为 “x的右孩子的父亲”
    y->left = x->right;
    if (x->right != NULL)
        x->right->parent = y;

    // 将 “y的父亲” 设为 “x的父亲”
    x->parent = y->parent;

    if (y->parent == NULL) 
    {
        root = x;            // 如果 “y的父亲” 是空节点，则将x设为根节点
    }
    else
    {
        if (y == y->parent->right)
            y->parent->right = x;    // 如果 y是它父节点的右孩子，则将x设为“y的父节点的右孩子”
        else
            y->parent->left = x;    // (y是它父节点的左孩子) 将x设为“x的父节点的左孩子”
    }

    // 将 “y” 设为 “x的右孩子”
    x->right = y;

    // 将 “y的父节点” 设为 “x”
    y->parent = x;
}

/*
 * 红黑树插入修正函数
 *
 * 在向红黑树中插入节点之后(失去*衡)，再调用该函数；
 * 目的是将它重新塑造成一颗红黑树。
 *
 * 参数说明：
 *     root 红黑树的根
 *     node 插入的结点        // 对应《算法导论》中的z
 */

void RBTree ::insertFixUp(RBTNode *& root, RBTNode * node)
{
    RBTNode *parent, *gparent;

    // 若“父节点存在，并且父节点的颜色是红色”
    while ((parent = rb_parent(node)) && rb_is_red(parent))
    {
        gparent = rb_parent(parent);

        //若“父节点”是“祖父节点的左孩子”
        if (parent == gparent->left)
        {
            // Case 1条件：叔叔节点是红色
            {
                RBTNode *uncle = gparent->right;
                if (uncle && rb_is_red(uncle))
                {
                    rb_set_black(uncle);
                    rb_set_black(parent);
                    rb_set_red(gparent);
                    node = gparent;
                    continue;
                }
            }

            // Case 2条件：叔叔是黑色，且当前节点z是父节点的右孩子
            if (parent->right == node)
            {
                RBTNode *tmp;
                leftRotate(root, parent);
                tmp = parent;
                parent = node;
                node = tmp;
            }

            // Case 3条件：叔叔是黑色，且当前节点是左孩子。
            rb_set_black(parent);
            rb_set_red(gparent);
            rightRotate(root, gparent);
        } 
        else//若“z的父节点”是“z的祖父节点的右孩子”
        {
            // Case 1条件：叔叔节点是红色
            {
                RBTNode *uncle = gparent->left;
                if (uncle && rb_is_red(uncle))
                {
                    rb_set_black(uncle);
                    rb_set_black(parent);
                    rb_set_red(gparent);
                    node = gparent;
                    continue;
                }
            }

            // Case 2条件：叔叔是黑色，且当前节点是左孩子
            if (parent->left == node)
            {
                RBTNode *tmp;
                rightRotate(root, parent);
                tmp = parent;
                parent = node;
                node = tmp;
            }

            // Case 3条件：叔叔是黑色，且当前节点是右孩子
            rb_set_black(parent);
            rb_set_red(gparent);
            leftRotate(root, gparent);
        }
    }

    // 将根节点设为黑色
    rb_set_black(root);
}

/*
 * 将结点插入到红黑树中
 *
 * 参数说明：
 *     root 红黑树的根结点
 *     node 插入的结点        // 对应《算法导论》中的node
 */

void RBTree ::insert(RBTNode *& root, RBTNode * node)
{
    RBTNode *y = NULL;
    RBTNode *x = root;

    // 1. 将红黑树当作一颗二叉查找树，将节点添加到二叉查找树中。
    while (x != NULL)
    {
        y = x;
        if (node->index < x->index)
            x = x->left;
        else
            x = x->right;
    }

    node->parent = y;
    if (y!=NULL)
    {
        if (node->index < y->index)
            y->left = node;
        else
            y->right = node;
    }
    else
        root = node;

    // 2. 设置节点的颜色为红色
    node->color = RED;

    // 3. 将它重新修正为一颗二叉查找树
    insertFixUp(root, node);
}

/*
 * 将结点(index为节点键值)插入到红黑树中
 *
 * 参数说明：
 *     tree 红黑树的根结点
 *     index 插入结点的键值
 */

void RBTree::insert(int index, string mac)
{
    RBTNode * z = NULL;

    // 如果新建结点失败，则返回。【这里提高了一种新建结点的方法】
    if ((z = new RBTNode (index, mac, BLACK, NULL, NULL, NULL)) == NULL)
        return;

    insert(mRoot, z);
}

/*
 * 红黑树删除修正函数
 *
 * 在从红黑树中删除插入节点之后(红黑树失去平衡)，再调用该函数；
 * 目的是将它重新塑造成一颗红黑树。
 *
 * 参数说明：
 *     root 红黑树的根
 *     node 待修正的节点
 */

void RBTree ::removeFixUp(RBTNode *& root, RBTNode * node, RBTNode * parent)
{
    RBTNode *w;

    while ((!node || rb_is_black(node)) && node != root)
    {
        if (parent->left == node)
        {
            w = parent->right;
            if (rb_is_red(w))
            {
                // Case 1: x的兄弟w是红色的  
                rb_set_black(w);
                rb_set_red(parent);
                leftRotate(root, parent);
                w = parent->right;
            }
            if ((!w->left || rb_is_black(w->left)) &&
                (!w->right || rb_is_black(w->right)))
            {
                // Case 2: x的兄弟w是黑色，且w的俩个孩子也都是黑色的  
                rb_set_red(w);
                node = parent;
                parent = rb_parent(node);
            }
            else
            {
                if (!w->right || rb_is_black(w->right))
                {
                    // Case 3: x的兄弟w是黑色的，并且w的左孩子是红色，右孩子为黑色。  
                    rb_set_black(w->left);
                    rb_set_red(w);
                    rightRotate(root, w);
                    w = parent->right;
                }
                // Case 4: x的兄弟w是黑色的；并且w的右孩子是红色的，左孩子任意颜色。
                rb_set_color(w, rb_color(parent));
                rb_set_black(parent);
                rb_set_black(w->right);
                leftRotate(root, parent);
                node = root;
                break;
            }
        }
        else
        {
            w = parent->left;
            if (rb_is_red(w))
            {
                // Case 1: x的兄弟w是红色的  
                rb_set_black(w);
                rb_set_red(parent);
                rightRotate(root, parent);
                w = parent->left;
            }
            if ((!w->left || rb_is_black(w->left)) &&
                (!w->right || rb_is_black(w->right)))
            {
                // Case 2: x的兄弟w是黑色，且w的俩个孩子也都是黑色的  
                rb_set_red(w);
                node = parent;
                parent = rb_parent(node);
            }
            else
            {
                if (!w->left || rb_is_black(w->left))
                {
                    // Case 3: x的兄弟w是黑色的，并且w的左孩子是红色，右孩子为黑色。  
                    rb_set_black(w->right);
                    rb_set_red(w);
                    leftRotate(root, w);
                    w = parent->left;
                }
                // Case 4: x的兄弟w是黑色的；并且w的右孩子是红色的，左孩子任意颜色。
                rb_set_color(w, rb_color(parent));
                rb_set_black(parent);
                rb_set_black(w->left);
                rightRotate(root, parent);
                node = root;
                break;
            }
        }
    }
    if (node)
        rb_set_black(node);
}

/*
 * 删除结点(node)，并返回被删除的结点
 *
 * 参数说明：
 *     root 红黑树的根结点
 *     node 删除的结点
 */

void RBTree ::remove(RBTNode *& root, RBTNode * node)
{
    RBTNode *child, *parent;
    RBTColor color;

    // 被删除节点的"左右孩子都不为空"的情况。
    if ( (node->left!=NULL) && (node->right!=NULL) ) 
    {
        // 被删节点的后继节点。(称为"取代节点")
        // 用它来取代"被删节点"的位置，然后再将"被删节点"去掉。
        RBTNode *successor = node;

        // 获取后继节点
        successor = successor->right;
        while (successor->left != NULL)
            successor = successor->left;

        // "node节点"不是根节点(只有根节点不存在父节点)
        if (rb_parent(node))
        {
            if (rb_parent(node)->left == node)
                rb_parent(node)->left = successor;
            else
                rb_parent(node)->right = successor;
        } 
        else 
            // "node节点"是根节点，更新根节点。
            root = successor;

        // child是"后继结点"的右孩子，也是需要"调整的节点"。
        // "后继结点"肯定不存在左孩子！因为它是一个后继节点。
        child = successor->right;
        parent = rb_parent(successor);
        // 保存"后继结点"的颜色
        color = rb_color(successor);

        // "被删除节点"是"它的后继节点的父节点"
        if (parent == node)
        {
            parent = successor;
        } 
        else
        {
            // child不为空
            if (child)
                rb_set_parent(child, parent);
            parent->left = child;

            successor->right = node->right;
            rb_set_parent(node->right, successor);
        }

        successor->parent = node->parent;
        successor->color = node->color;
        successor->left = node->left;
        node->left->parent = successor;

        if (color == BLACK)
            removeFixUp(root, child, parent);

        delete node;
        return ;
    }

    if (node->left !=NULL)//左孩子不为空
        child = node->left;
    else 
        child = node->right;

    parent = node->parent;
    // 保存"删除节点"的颜色
    color = node->color;

    if (child)
        child->parent = parent;

    // "node节点"不是根节点 即Parent不为空 和line473一样
    if (parent)
    {
        if (parent->left == node)
            parent->left = child;
        else
            parent->right = child;
    }
    else
        root = child;

    if (color == BLACK)
        removeFixUp(root, child, parent);
    delete node;
}

/*
 * 删除红黑树中键值为index的节点
 *
 * 参数说明：
 *     tree 红黑树的根结点
 */

void RBTree ::remove(int index)
{

    RBTNode *node; 

    // 查找key对应的节点(node)，找到的话就删除该节点
    if ((node = search(mRoot, index)) != NULL)
        cout<<node->index<<" : "<<node->mac<<endl;
        remove(mRoot, node);
}

/*
 * 销毁红黑树
 */

void RBTree ::destroy(RBTNode *& tree)
{
    if (tree == NULL)
        return;

    if (tree->left != NULL)
        return destroy(tree->left);
    if (tree->right != NULL)
        return destroy(tree->right);

    delete tree;
    tree = NULL;
}


void RBTree ::destroy()
{
    destroy(mRoot);
}

/*
 * 打印"二叉查找树"
 *
 * index       -- 节点的键值
 * direction  --  0，表示该节点是根节点;
 *               -1，表示该节点是它的父结点的左孩子;
 *                1，表示该节点是它的父结点的右孩子。
 */

void RBTree ::print(RBTNode * tree, int index, int direction)
{
    if (tree != NULL)
    {
        if (direction == 0)    // tree是根节点
            cout << setw(2) << tree->index << "(B) is root" << endl;
        else                // tree是分支节点
            cout << setw(2) << tree->index << (rb_is_red(tree) ? "(R)" : "(B)") << " is " << setw(2) << index << "'s " << setw(12) << (direction == 1 ? "right child" : "left child") << endl;

        print(tree->left, tree->index, -1);
        print(tree->right, tree->index, 1);
    }
}


void RBTree ::print()
{
    if (mRoot != NULL)
        print(mRoot, mRoot->index, 0);
}


