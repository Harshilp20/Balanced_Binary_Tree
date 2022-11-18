//
//Name: Harshil Patel
//Course: Data Structures CS2028C
//Assignment: Extra Credit

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <cstdlib>

using namespace std;

//Node class with type T
template<typename T>
class Node {
public:
    T data;
    Node* parent;
    Node* left;
    Node* right;
    int height;
    
    Node(T value, Node* parent = NULL, Node* right = NULL, Node* left = NULL) {
        data = value;
        this->parent = parent;
        this->right = right;
        this->left = left;
        height = 0;
    }
};


template<class T>
class BinarySearchTree{
private:
    Node<T>* root = NULL;
    int size = 0;
    int height = 0;
    T values[];
    
    
    //Rebalance and Rotate functions for balancing tree
    Node<T>* TreeRebalance(Node<T>* node){
        TreeUpdateHeight(node);
        if (TreeGetBalance(node) == -2){
            if (TreeGetBalance(node->right) == 1){
                TreeRotateRight(node->right);
            }
            return TreeRotateLeft(node);
        }
        else if (TreeGetBalance(node) == 2){
            if (TreeGetBalance(node->left) == -1){
                TreeRotateLeft(node->left);
            }
            return TreeRotateRight(node);
        }
        return node;
    }
    
    void TreeUpdateHeight(Node<T>* node){
        int leftHeight = -1;
        if (node->left != NULL){
            leftHeight = node->left->height;
        }
        int rightHeight = -1;
        if (node->right != NULL){
            rightHeight = node->right->height;
        }
        node->height = max(leftHeight, rightHeight) + 1;
    }
    
    bool TreeSetChild(Node<T>* parent, string whichChild, Node<T>* child){
        if (whichChild != "left" && whichChild != "right"){
            return false;
        }
        
        if (whichChild == "left"){
            parent->left = child;
        }
        else{
            parent->right = child;
        }
        if (child != NULL){
            child->parent = parent;
        }
        
        TreeUpdateHeight(parent);
        return true;
    }
    
    bool TreeReplaceChild(Node<T>* parent, Node<T>* currentChild, Node<T>* newChild){
        if (parent->left == currentChild){
            return TreeSetChild(parent, "left", newChild);
        }
        else if (parent->right == currentChild){
            return TreeSetChild(parent, "right", newChild);
        }
        else{
            return false;
        }
    }
    
    int TreeGetBalance(Node<T>* node){
        int leftHeight = -1;
        if (node->left != NULL){
            leftHeight = node->left->height;
        }
        int rightHeight = -1;
        if (node->right != NULL){
            rightHeight = node->right->height;
        }
        return leftHeight - rightHeight;
    }
    
    Node<T>* TreeRotateRight(Node<T>* node){
        Node<T>* leftRightChild = node->left->right;
        if (node->parent != NULL){
            TreeReplaceChild(node->parent, node, node->left);
        }
        else{
            root = node->left;
            root->parent = NULL;
        }
        TreeSetChild(node->left, "right", node);
        TreeSetChild(node, "left", leftRightChild);
        
        return node->parent;
    }
    
    Node<T>* TreeRotateLeft(Node<T>* node){
        Node<T>* rightLeftChild = node->right->left;
        if (node->parent != NULL){
            TreeReplaceChild(node->parent, node, node->right);
        }
        else{
            root = node->right;
            root->parent = NULL;
        }
        TreeSetChild(node->right, "left", node);
        TreeSetChild(node, "right", rightLeftChild);
        
        return node->parent;
    }
    
    void InsertionSort(int* arr, int arrSize){
        int j;
        int temp;
        //Start from the beginning of the array
        for (int i = 1; i < arrSize; i++){
            j = i;
            //Swapping arr[j] and arr[j-1] until arr[j] is less than all numbers after it
            while ((j > 0) && (arr[j] < arr[j - 1])){
                temp = arr[j];
                arr[j] = arr[j - 1];
                arr[j - 1] = temp;
                j -= 1;
            }
        }
    }

    
public:
    //Constructor
    BinarySearchTree(){}
    
    //Destructor
    ~BinarySearchTree(){
        emptyTree();
    }
    
    //Insert method
    void Insert(T insert){
        //Create new node
        Node<T>* node = new Node(insert);
        
        int heightOfNode = 0;
        bool repeated = false;
        bool notInserted = false;
        Node<T>* currNode = root;
        
        //Iterate through tree
        while (currNode != NULL){
            //if data already exists, repeated = true and break the loop
            if (currNode->data == node->data){
                repeated = true;
                notInserted = true;
                break;
            }
            
            //if value is less than it's parent, move to the left, else move to the right
            if (node->data < currNode->data){
                if (currNode->left == NULL){
                    currNode->left = node;
                    node->parent = currNode;
                    currNode = NULL;
                }
                else{
                    currNode = currNode->left;
                }
            }
            else{
                if (currNode->right == NULL){
                    currNode->right = node;
                    node->parent = currNode;
                    node->height = heightOfNode;
                    currNode = NULL;
                }
                else{
                    currNode = currNode->right;
                }
            }
            heightOfNode += 1;
        }
        
        //create new tree if it is the first node
        if (root == NULL){
            root = node;
            node->parent = NULL;
        }
        
        if (notInserted == false){
            size += 1;
            values[size-1] = insert;
        }
        
        //Throw error if value is being repeated
        try{
            if (repeated == true){
                throw runtime_error("Repeated Value");
            }
        }
        catch(const std::exception& e){
            std::cout << "Caught exception: " << e.what() << '\n';
        }
        
        node = node->parent;
        while(node != NULL){
            TreeRebalance(node);
            node = node->parent;
        }
    }
    
    //find method
    Node<T>* Find(T value){
        //return null if tree is empty
        if (root == NULL){
            return NULL;
        }
        
        Node<T>* currNode = root;
        //iteratate until data in currNode equals value
        while (currNode != NULL && (currNode->data != value || currNode->data == NULL)){
            if (value < currNode->data){
                //go left
                currNode = currNode->left;
            }
            else{
                //go right
                currNode = currNode->right;
            }
        }
        
        if (currNode == NULL){
            return NULL;
        }
        
        return currNode;
    }
    
    Node<T>* Remove(T value){
        Node<T>* node = Find(value);
        Node<T>* result = node;
        bool removed = RemoveNode(node);
        if (removed == false){
            return NULL;
        }
        size -= 1;
        return result;
    }
    
    bool RemoveNode(Node<T>* node) {
        //return false if node is NULL
        if (node == NULL) {
            return false;
        }
        
        //Parent of Node
        Node<T>* parent = node->parent;
        
        //if node has two children
        if ((node->left != NULL) && (node->right != NULL)) {
            Node<T>* sucNode = node->right;
            while (sucNode->left != NULL) {
                sucNode = sucNode->left;
            }
            //copy value of successor to node
            node->data = sucNode->data;
            //delete successor
            RemoveNode(sucNode);
            return true;
        }
        //if node has less than 2 children
        else if (node == root) {
            if (node->left) {
                root = node->left;
            }
            else {
                root = node->right;
            }

            if (root) {
                root->parent = nullptr;
            }

            delete node;
            return true;
        }
        //If node only has left child
        else if (node->left != NULL) {
            TreeReplaceChild(parent, node, node->left);
        }
        //If node only has right child
        else {
            TreeReplaceChild(parent, node, node->right);
        }
        
        //Delete the node
        delete node;
        
        //Rebalance Tree
        Node<T>* nodeToRebalance = parent;
        while (nodeToRebalance) {
            TreeRebalance(nodeToRebalance);
            nodeToRebalance = nodeToRebalance->parent;
        }
        
        return true;
    }
    
    
    T* GetAllAscending(){
        T* result = new T[size];
        for(int i = 0; i < size; i++){
            result[i] = values[i];
        }
        InsertionSort(result, size);
        return result;
    }
    
    T* GetAllDecending(){
        T* result = new T[size];
        T* list = GetAllAscending();
        for(int i = size; i > 0; i--){
            result[size - i] = list[i-1];
        }
        return result;
    }
    
    void printAscending(){
        cout<<"Ascending Order: ";
        T* list = GetAllAscending();
        for(int i = 0; i < size; i++){
            cout<<list[i]<<" ";
        }
        cout<<endl;
    }
    
    void printDecending(){
        cout<<"Decending Order: ";
        T* list = GetAllDecending();
        for (int i = 0; i < size; i++){
            cout<<list[i]<<" ";
        }
        cout<<endl;
    }
    
    
    //return size of tree
    int getSize(){ return size; }

    //Empty the tree
    void emptyTree(){
        delete root;
        root = NULL;
        size = 0;
    }
    
};


int main() {
    
    //Test
    BinarySearchTree<int> myTree;
    //Insert random values into tree
    int num = 10;
    myTree.Insert(num);
    num = 20;
    myTree.Insert(num);
    num = 30;
    myTree.Insert(num);
    num = 12;
    //Test repeated value
    myTree.Insert(num);
    myTree.Insert(num);
    cout<<"Size of Tree "<<myTree.getSize()<<endl;
    myTree.Remove(12);
    cout<<"Size of Tree "<<myTree.getSize()<<endl;
    
    //print ascending and decending
    myTree.printAscending();
    myTree.printDecending();
    
    myTree.emptyTree();
    cout<<"Size of Tree "<<myTree.getSize()<<endl;
    
    
    return 0;
}
