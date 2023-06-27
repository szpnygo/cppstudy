#include <iostream>
#include <vector>

#define T 3

// B树节点类
class BTreeNode {
    int* keys;     // 存储键值
    int t;         // 节点可以容纳的最小度数
    BTreeNode** C; // 存储子指针
    int n;         // 当前节点的键值数量
    bool leaf;     // 判断是否为叶子节点

  public:
    BTreeNode(int t1, bool leaf1); // 构造函数

    // 遍历所有节点
    void traverse();

    // 返回第一个大于等于k的键值的索引
    BTreeNode* search(int k);

    // 插入非满节点
    void insertNonFull(int k);

    // 分裂子节点
    void splitChild(int i, BTreeNode* y);

    friend class BTree;

    void remove(int k);

    int findKey(int k);

    void removeFromLeaf(int idx);

    void removeFromNonLeaf(int idx);

    int getPred(int idx);

    int getSucc(int idx);

    void merge(int idx);

    void fill(int idx);

    void borrowFromPrev(int idx);

    void borrowFromNext(int idx);

    ~BTreeNode() {
        delete[] keys;
        for (int i = 0; i <= n; i++) {
            delete C[i];
        }
        delete[] C;
    }
};

// 返回第一个大于或等于 k 的键值的索引
int BTreeNode::findKey(int k) {
    int idx = 0;
    while (idx < n && keys[idx] < k)
        ++idx;
    return idx;
}

// 从叶节点删除键
void BTreeNode::removeFromLeaf(int idx) {
    // 将所有大于 keys[idx] 的键向前移动一步
    for (int i = idx + 1; i < n; ++i)
        keys[i - 1] = keys[i];

    // 减少节点的键值数量
    n--;
}

// 从非叶节点删除键
void BTreeNode::removeFromNonLeaf(int idx) {
    int k = keys[idx];

    // 如果前面的子节点至少有 t 个键，找到前驱
    // 用前驱代替 keys[idx]，在子节点中删除前驱
    if (C[idx]->n >= t) {
        int pred = getPred(idx);
        keys[idx] = pred;
        C[idx]->remove(pred);
    }

    // 如果子节点只有 t-1 个键，找到后继
    // 用后继代替 keys[idx]，在子节点中删除后继
    else if (C[idx + 1]->n >= t) {
        int succ = getSucc(idx);
        keys[idx] = succ;
        C[idx + 1]->remove(succ);
    }

    // 如果前驱和后继子节点都只有 t-1 个键，合并 k 和
    // C[idx]，在合并后的节点中删除 k
    else {
        merge(idx);
        C[idx]->remove(k);
    }
}

int BTreeNode::getPred(int idx) {
    // 一直往右走，直到到达叶节点
    BTreeNode* cur = C[idx];
    while (!cur->leaf)
        cur = cur->C[cur->n];

    // 返回叶子节点的最后一个键
    return cur->keys[cur->n - 1];
}

int BTreeNode::getSucc(int idx) {
    // 一直往左走，直到到达叶节点
    BTreeNode* cur = C[idx + 1];
    while (!cur->leaf)
        cur = cur->C[0];

    // 返回叶子节点的第一个键
    return cur->keys[0];
}

void BTreeNode::merge(int idx) {
    BTreeNode* child = C[idx];
    BTreeNode* sibling = C[idx + 1];

    // 将 keys[idx] 下降到 child，将 sibling 的 keys 添加到 child
    child->keys[t - 1] = keys[idx];

    // 复制 sibling 的 keys 到 child
    for (int i = 0; i < sibling->n; ++i)
        child->keys[i + t] = sibling->keys[i];

    // 复制子指针
    if (!child->leaf) {
        for (int i = 0; i <= sibling->n; ++i)
            child->C[i + t] = sibling->C[i];
    }

    // 将所有键在 idx 后的键前移一位
    for (int i = idx + 1; i < n; ++i)
        keys[i - 1] = keys[i];

    // 将子指针前移一位
    for (int i = idx + 2; i <= n; ++i)
        C[i - 1] = C[i];

    // 更新关键字数量
    child->n += sibling->n + 1;
    n--;

    // 释放合并的节点
    delete (sibling);
}

// 如果子节点有少于 t-1 个键，填充子节点
void BTreeNode::fill(int idx) {
    // 如果前一个子节点 C[idx-1] 至少有 t 个键，借一个键
    if (idx != 0 && C[idx - 1]->n >= t)
        borrowFromPrev(idx);

    // 如果后一个子节点 C[idx+1] 至少有 t 个键，借一个键
    else if (idx != n && C[idx + 1]->n >= t)
        borrowFromNext(idx);

    // 如果前后子节点都没有至少 t 个键，合并这个子节点和一个兄弟节点
    else {
        if (idx != n)
            merge(idx);
        else
            merge(idx - 1);
    }
}

void BTreeNode::borrowFromPrev(int idx) {
    BTreeNode* child = C[idx];
    BTreeNode* sibling = C[idx - 1];

    // 后移 child 的所有键和子指针
    for (int i = child->n - 1; i >= 0; --i)
        child->keys[i + 1] = child->keys[i];
    if (!child->leaf) {
        for (int i = child->n; i >= 0; --i)
            child->C[i + 1] = child->C[i];
    }

    // 将 keys[idx-1] 下降到 child，将 sibling 的最后一个键上升到 keys[idx-1]
    child->keys[0] = keys[idx - 1];
    if (!child->leaf)
        child->C[0] = sibling->C[sibling->n];

    keys[idx - 1] = sibling->keys[sibling->n - 1];

    child->n += 1;
    sibling->n -= 1;
}

void BTreeNode::borrowFromNext(int idx) {
    BTreeNode* child = C[idx];
    BTreeNode* sibling = C[idx + 1];

    // 将 keys[idx] 下降到 child 的最后，将 sibling 的第一个键上升到 keys[idx]
    child->keys[(child->n)] = keys[idx];
    if (!(child->leaf))
        child->C[(child->n) + 1] = sibling->C[0];

    keys[idx] = sibling->keys[0];

    // 前移 sibling 的所有键和子指针
    for (int i = 1; i < sibling->n; ++i)
        sibling->keys[i - 1] = sibling->keys[i];
    if (!sibling->leaf) {
        for (int i = 1; i <= sibling->n; ++i)
            sibling->C[i - 1] = sibling->C[i];
    }

    child->n += 1;
    sibling->n -= 1;
}

BTreeNode::BTreeNode(int t1, bool leaf1) {
    // 赋予最小度数值和是否为叶子节点的属性
    t = t1;
    leaf = leaf1;

    // 初始化键和子指针数组
    keys = new int[2 * t - 1];
    C = new BTreeNode*[2 * t];

    // 初始化当前节点的键值数量为0
    n = 0;
};

void BTreeNode::traverse() {
    // 有 n 个键和 n+1 个孩子
    int i;
    for (i = 0; i < n; i++) {
        // 如果当前节点不是叶子节点，递归调用子树的遍历
        if (leaf == false)
            C[i]->traverse();
        std::cout << " " << keys[i];
    }

    // 打印最后一个子节点的键
    if (leaf == false)
        C[i]->traverse();
};

BTreeNode* BTreeNode::search(int k) {
    // 寻找第一个大于等于 k 的键
    int i = 0;
    while (i < n && k > keys[i])
        i++;

    // 如果找到了，直接返回
    if (keys[i] == k)
        return this;

    // 如果没有找到并且这是一个叶子节点
    if (leaf == true)
        return nullptr;

    // 递归进入子节点查找
    return C[i]->search(k);
};

void BTreeNode::remove(int k) {
    int idx = findKey(k);

    // 如果键 k 在节点中
    if (idx < n && keys[idx] == k) {
        // 如果节点是叶节点，直接从节点中删除
        // 如果节点是非叶节点，进行更复杂的删除
        if (leaf)
            removeFromLeaf(idx);
        else
            removeFromNonLeaf(idx);
    } else {
        // 如果这个节点是叶节点，那么键肯定不存在
        // 如果这个节点是非叶节点，递归寻找需要删除的键
        if (leaf) {
            std::cout << "The key " << k << " is does not exist in the tree\n";
            return;
        }

        bool flag = ((idx == n) ? true : false);

        if (C[idx]->n < t)
            fill(idx);

        if (flag && idx > n)
            C[idx - 1]->remove(k);
        else
            C[idx]->remove(k);
    }
    return;
}

void BTreeNode::insertNonFull(int k) {
    // 初始化 i 为当前节点键值的数量
    int i = n - 1;

    // 如果当前节点是叶子节点
    if (leaf == true) {
        // 从后往前查找插入的位置，并把所有大于 k 的键向后移动一位
        while (i >= 0 && keys[i] > k) {
            keys[i + 1] = keys[i];
            i--;
        }

        // 插入新的键
        keys[i + 1] = k;
        n = n + 1;
    } else { // 如果当前节点不是叶子节点
        // 找到需要插入的子节点
        while (i >= 0 && keys[i] > k)
            i--;

        // 看看我们找到的子节点是否满了
        if (C[i + 1]->n == 2 * t - 1) {
            // 如果满了，我们需要分裂
            splitChild(i + 1, C[i + 1]);

            // 如果中间的键上升到了当前节点，而且它小于
            // k，我们需要向下移动到下一个子节点
            if (keys[i + 1] < k)
                i++;
        }
        C[i + 1]->insertNonFull(k);
    }
};

void BTreeNode::splitChild(int i, BTreeNode* y) {
    // 创建一个新的节点，它将存储 t-1 个键值
    BTreeNode* z = new BTreeNode(y->t, y->leaf);
    z->n = t - 1;

    // 把 y 的后 t-1 个键值复制到 z
    for (int j = 0; j < t - 1; j++)
        z->keys[j] = y->keys[j + t];

    // 如果 y 不是叶子节点，复制后 t 个子节点到 z
    if (y->leaf == false) {
        for (int j = 0; j < t; j++)
            z->C[j] = y->C[j + t];
    }

    // 减少 y 的键值数量
    y->n = t - 1;

    // 把新创建的节点插入到当前节点的子节点
    for (int j = n; j >= i + 1; j--)
        C[j + 1] = C[j];

    C[i + 1] = z;

    // 在当前节点插入 y 的中间键
    for (int j = n - 1; j >= i; j--)
        keys[j + 1] = keys[j];

    keys[i] = y->keys[t - 1];

    // 更新当前节点的键值数量
    n = n + 1;
};

class BTree {
    BTreeNode* root; // 指向根节点
    int t;           // 树的最小度数

  public:
    // 构造函数
    BTree(int _t) {
        root = nullptr;
        t = _t;
    }

    ~BTree() { delete root; }

    // 遍历树
    void traverse() {
        if (root != nullptr)
            root->traverse();
    }

    // 搜索键在树中的位置
    BTreeNode* search(int k) {
        return (root == nullptr) ? nullptr : root->search(k);
    }

    // 插入新的键到树中
    void insert(int k);

    void remove(int k);
};

void BTree::remove(int k) {
    if (!root) {
        std::cout << "The tree is empty\n";
        return;
    }

    // Call the remove function for root
    root->remove(k);

    // If the root node has 0 keys, make its first child as the new root
    // if it has a child, otherwise set root as NULL
    if (root->n == 0) {
        BTreeNode* tmp = root;
        if (root->leaf)
            root = NULL;
        else
            root = root->C[0];

        // Free the old root
        delete tmp;
    }
    return;
}

void BTree::insert(int k) {
    // 如果树为空
    if (root == nullptr) {
        // 创建新节点
        root = new BTreeNode(t, true);
        root->keys[0] = k; // 插入键
        root->n = 1;       // 更新键值数量
    } else {               // 如果树非空
        // 如果根节点满了
        if (root->n == 2 * t - 1) {
            // 创建新的根节点
            BTreeNode* s = new BTreeNode(t, false);

            // 旧的根节点变成新根节点的子节点
            s->C[0] = root;

            // 分裂旧的根节点，并将一个键移动到新的根节点
            s->splitChild(0, root);

            // 新的根节点有两个子节点，决定应该将新的键插入哪个子节点
            int i = 0;
            if (s->keys[0] < k)
                i++;
            s->C[i]->insertNonFull(k);

            // 改变根节点
            root = s;
        } else { // 如果根节点未满
            root->insertNonFull(k);
        }
    }
};

int main() {
    BTree t(3); // 假设 B 树的最小度数为 3

    t.insert(10);
    t.insert(20);
    t.insert(5);
    t.insert(6);
    t.insert(12);
    t.insert(30);
    t.insert(7);
    t.insert(17);

    std::cout << "Traversal of the constucted B-Tree is: ";
    t.traverse();
    std::cout << std::endl;

    int k = 6;
    (t.search(k) != nullptr) ? std::cout << "\nPresent"
                             : std::cout << "\nNot Present";

    k = 15;
    (t.search(k) != nullptr) ? std::cout << "\nPresent"
                             : std::cout << "\nNot Present";

    std::cout << "\nDeleting 6";
    t.remove(6);
    std::cout << "\nTraversal of the modified B-Tree is: ";
    t.traverse();
    std::cout << std::endl;

    std::cout << "\nDeleting 12";
    t.remove(12);
    std::cout << "\nTraversal of the modified B-Tree is: ";
    t.traverse();
    std::cout << std::endl;

    std::cout << "\nDeleting 30";
    t.remove(30);
    std::cout << "\nTraversal of the modified B-Tree is: ";
    t.traverse();
    std::cout << std::endl;

    return 0;
}
