//
// Created by aleks on 25.12.2023.
//

#ifndef AVL_TREE_AVL_TREE_H
#define AVL_TREE_AVL_TREE_H

#include <vector>
#include <functional>
#include "algorithm"
#include "iostream"

template <typename Key, typename Info>
class avl_tree
{
private:
    //data of the tree
    struct node{
        Key key;
        Info info;
        int balance;
        int height;
        node* left;
        node* right;

        // Constructor
        node(const Key& newKey, const Info& newInfo,int balance=0, node* newLeft = nullptr, int height=1, node* newRight = nullptr)
                : key(newKey), info(newInfo), left(newLeft), right(newRight), balance(balance), height(height) {}

        // Copy constructor
        node(const node& other)
                : key(other.key), info(other.info), left(nullptr), right(nullptr), balance(other.balance), height(other.height){}
        // Assignment operator
        node& operator=(const node* other) {
            if (this == other) return *this;
            key = other->key;
            info = other->info;
            left = nullptr;
            right = nullptr;
            balance = other->balance;
            height = other->height;
            return *this;
        }
        ~node() = default;
    };
    node* pHead;
    Info defaultInfo;

    //returns node with a given key
    node*& find(const Key& findKey, node*& current){
        return !current || findKey == current->key ? current : findKey < current->key ? find(findKey, current->left) : find(findKey, current->right);

    }
    node* find(const Key& findKey, node* current)const  {
        return !current || findKey == current->key ? current : findKey < current->key ? find(findKey,
                                                                                             current->left)
                                                                                      : find(findKey,
                                                                                             current->right);

    }

    //returns number of nodes deleted (effectively size) from the tree
    int clear(node*& current){
        if(!current) return 0;
        int removed=0;
        removed+=clear(current->left);
        removed+=clear(current->right);
        delete current;
        current= nullptr;
        return ++removed;
    }

    //copy constructor
    void copyNode(node*& dest, const node* src){
        if(!src) return;
        dest = new node(*src);
        copyNode(dest->left, src->left);
        copyNode(dest->right, src->right);
    }

    //performs an operation on every node of the tree
    void forEachNode(node* current, Info& newInfo, void operation(Info& oldInfo, Info& newInfo)){
        if(!current) return;
        operation(current->info, newInfo);
        forEachNode(current->left, newInfo, operation);
        forEachNode(current->right, newInfo, operation);
    }

    //prints the tree sideways
    //uses indent to maintain the "look" of the tree
    void print(const node* current, int indent) const {
        if (current != nullptr) {
            print(current->right, indent + 4);

            std::cout << std::string(indent, ' ') << current->key << ", " << current->info << "\n";

            print(current->left, indent + 4);
        }
    }

    //pivots used for balancing of the tree
    void llShift(node*& pivot) {
        auto newRoot = pivot->right;
        pivot->right = newRoot->left;
        newRoot->left = pivot;
        pivot = newRoot;
        updateNode(pivot->left);
        updateNode(pivot);
    }
    void rrShift(node*& pivot) {
        auto newRoot = pivot->left;
        pivot->left = newRoot->right;
        newRoot->right = pivot;
        pivot = newRoot;
        updateNode(pivot->right);
        updateNode(pivot);
    }
    void rlShift(node*& pivot) {
        llShift(pivot->left);
        rrShift(pivot);
    }
    void lrShift(node*& pivot) {
        rrShift(pivot->right);
        llShift(pivot);
    }

    //balancing function
    bool balance(node*& pivot) {
        if (pivot->balance > 1) {
            if (pivot->right->balance < 0) {
                lrShift(pivot);
                return true;
            }
            llShift(pivot);
            return true;
        }
        if (pivot->balance < -1) {
            if (pivot->left->balance > 0) {
                rlShift(pivot);
                return true;
            }
            rrShift(pivot);
            return true;
        }
        return false;
    }

    //updating node information+usage of balance() function
    void updateNode(node* n) {
        if(!n)return;
        int left= n->left ? n->left->height : 0;
        int right = n->right ? n->right->height : 0;
        n->height = std::max(left,right)+1;
        n->balance=right-left;
        balance(n);
    }

    //updates node information and performs balance() function
    //key is given, so that the function updates only the changed branch
    int updateNodes(node*& current, const Key& key) {
        if (!current) return 0;
        int heightR=1;
        int heightL=1;
        if (key > current->key) {
            heightR = updateNodes(current->right, key) + 1;
            if(current->left) heightL += current->left->height;
            current->balance = heightR-heightL;
        } else if (key < current->key) {
            heightL = updateNodes(current->left, key) + 1;
            if(current->right) heightR += current->right->height;
            current->balance = heightR-heightL;
        }
        current->height = std::max(heightR, heightL);
        current->balance=heightR-heightL;
        balance(current);
        return current->height;
    }

    //function returning a node with the smallest value among the given fragment
    //used in pop()
    node*& getSuccessor(node*& root){
        while(root->left){
            root =root->left;
        }
        return root;
    }

    //returns a tree of elements present in this tree, but not present in another
    void subtract(avl_tree<Key, Info>& result, const avl_tree<Key, Info>& src, node* subtrahend) {
        if (!subtrahend)
            return;

        // Check if the subtrahend node's key exists in src
        node* found = src.find(subtrahend->key, src.pHead);
        if (!found) {
            // If the node's key is not found in src, insert it into result
            result.insert(subtrahend->key, subtrahend->info,
                          [](const Info& a, const Info& b) { return a + b; });
        }

        // Recursively process left and right subtrees
        subtract(result, src, subtrahend->left);
        return subtract(result, src, subtrahend->right);
    }

    //returns vector of a given size, with nodes complying with given operation
    //sorter is used to sort vector for ie. getting n nodes with the smallest key
    //  endEarly is used to end the function as soon as the vector is full
    //  it is used for ie. getting any n nodes with key values <50
    void getNodeRecursive(node *root, int size,
                          bool (*operation)(const std::pair<Key, Info> &, const std::pair<Key, Info> &),
                          bool (*sorter)(const std::pair<Key, Info> &, const std::pair<Key, Info> &),
                          std::vector<std::pair<Key, Info>> &filtered, bool endEarly=false) const {

        if (!root) return;

        getNodeRecursive(root->left, size, operation, sorter, filtered, endEarly);

        if (filtered.empty() ? operation(std::pair<Key, Info>{}, std::pair<Key, Info>{root->key, root->info}): operation(filtered.front(), std::pair<Key, Info>{root->key, root->info})) {
            if (filtered.size() == size && endEarly) {
                return;
            }
            filtered.push_back(std::pair<Key, Info>(root->key, root->info));
            std::sort(filtered.begin(), filtered.end(), sorter);
            if (filtered.size() > size) {
                filtered.erase(filtered.begin());
            }
        }

        getNodeRecursive(root->right, size, operation, sorter, filtered, endEarly);
    }
public:
    //ctor cctor and dtor
    explicit avl_tree(const Info& defaultInfo) : pHead(nullptr), defaultInfo(defaultInfo){}
    avl_tree (const avl_tree& src){
        copyNode(this->pHead, src.pHead);
        defaultInfo=src.defaultInfo;
    }
    ~avl_tree(){
        clear();
    }

    //assignment operation
    avl_tree& operator=(const avl_tree& src){
        if(this==&src) return *this;
        defaultInfo=src.defaultInfo;
        copyNode(pHead, src.pHead);
        return *this;
    }

    //returns true if there are no nodes
    [[nodiscard]] bool empty() const{
        return pHead== nullptr;
    }

    //prints the tree sideways
    void print() const {
        print(pHead, 0);
    }

    //returns default info
    Info& getDefaultInfo(){
        return defaultInfo;
    }
    const Info& getDefaultInfo() const{
        return defaultInfo;
    }

    //returns <Key, Info> with the data of the node
    std::pair<Key, Info> find(const Key& searchKey){
        node* found= find(searchKey,pHead);
        return std::make_pair(found->key,found->info);
    }

    //inserts new element with a given key
    //if given element exists, performs operation() on its info
    //returns true if an alement is created
    bool insert (const Key& key,const Info& info,Info operation(const Info& foundInfo,const Info& newInfo)){
        node*& found = find(key, pHead);
        if(found){
            found->info = operation(found->info,info);
            return false;
        }
        found = new node(key, info);
        updateNodes(pHead, key);
        return true;
    }

    //deletes element with a given key
    //returns true if an element is "popped"
    bool pop(const Key& key) {
        node*& found = find(key, pHead);
        if (!found) return false;
        if(found->height==1) {
            delete found;
            found=nullptr;
            updateNodes(pHead, key);
            return true;
        }
        if(!found->left){
            node* toDel = found;
            found=found->right;
            delete toDel;
            updateNodes(pHead, key);
            return true;
        }
        if(!found->right){
            node* toDel = found;
            found=found->left;
            delete toDel;
            updateNodes(pHead,key);
            return true;
        }
        node*& replacement = getSuccessor(found->right);
        found->key = replacement->key;
        found->info = replacement->info;
        node *toDel = replacement;
        replacement = replacement->right;
        delete toDel;
        updateNodes(pHead, found->key);
        return true;
    }

    //deletes the tree
    //returns number of nodes deleted
    int clear(){
       return clear(pHead);
    }

    //performs operation on each element of the tree
    void forEach(Info& newInfo, void operation(Info& oldInfo, Info& newInfo)){
        forEachNode(pHead, newInfo, operation);
    }

    //returns tree with elements from this tree that do not exist in subtrahend
    avl_tree<Key, Info> subtract(const avl_tree<Key, Info>& subtrahend) {
        avl_tree<Key, Info> result(defaultInfo);
        subtract(result, subtrahend, pHead);
        return std::move(result);
    }

    //returns vector of a given size with size nodes complying with operation
    //end early used for finishing the function as soon as the vector is full
    //sorter used to sort the vector, if a user would want ie. 5 nodes with biggest keys that are <50 (45,46,47,48,49)
    std::vector<std::pair<Key, Info>> getNodes( int size, bool operation(const std::pair<Key, Info>& oldPair, const std::pair<Key, Info>& chosenPair),bool predicate(const std::pair<Key, Info>& oldPair, const std::pair<Key, Info>& chosenPair),bool endEarly=false) const {
        std::vector<std::pair<Key, Info>> filtered;
        getNodeRecursive(pHead, size, operation, predicate, filtered, endEarly);
        return filtered;
    }

    //performs insert(), returns info of the node used by operator[]
    void operator[](const Key& key){
        insert(key, defaultInfo, [](const int& a, const int& b){return a+b;});
    }
};

#endif //AVL_TREE_AVL_TREE_H
