#include <iostream>
#include <fstream>

using namespace std;

//Creates node structure giving it the variables data, left pointer, right pointer
struct _node {
    string data;
    struct _node* left;
    struct _node* right;
};

//allows to use just "node" instead of "struct _node" to access the node
typedef struct _node node;

//creates SplayTree class to house functions 
class SplayTree {
private:
    node* root; //declares variable "root" as node pointer type.
    string prev;

//Allows us to recursivly print out the Tree before it is ordered (parent, left, right)
    void preorderHelper(node* curr) {
        if (curr == NULL) {
            return;
        }
        cout << curr->data << endl;
        preorderHelper(curr->left);
        preorderHelper(curr->right);
    }

//Allows us to recursivly print out the Tree before it is ordered (left, parent, right)
    void inorderHelper(node* curr) {
        if (curr == NULL) {
            return;
        }
        inorderHelper(curr->left);
        cout << curr->data << endl;
        inorderHelper(curr->right);
    }

//Allows us to recursivly print out the Tree after it is ordered (left, right, parent)
    void postorderHelper(node* curr) {
        if (curr == NULL) {
            return;
        }
        postorderHelper(curr->left);
        cout << curr -> data << endl;
        postorderHelper(curr->right);
       // cout << curr -> data << endl;
    }

//creates our node and providing it a key value and setting left and right pointer branches to NULL
    node* createNode(string key) {
        node* newNode = new node;
        newNode->data = key;
        newNode->left = NULL;
        newNode->right = NULL;
        return newNode;
    }

//Zig rotation of tree to take the left child node and making it the parent node
    node* rightRotate(node* curr) { //takes in the memory address of the node we give it    
        node* leftChild = curr->left; //creates a node called leftChild giving it the memory address of "curr" left child 
        curr->left = leftChild->right; //now taking the curr left and setting it to leftChilds right child 
        leftChild->right = curr; //leftChilds right child is now the current node at the top
        return leftChild;
    }

//Zag rotation of tree to take the left child node and making it the parent node 
    node* leftRotate(node* curr) { //takes in the memory address of the node we give it
        node* rightChild = curr->right;//creates a node called rightChild giving it the memory address of "curr" right child
        curr->right = rightChild->left;//now taking the curr right and setting it to RightChilds left child 
        rightChild->left = curr;//leftChilds right child is now the current node at the top
        return rightChild;
    }

//Function that decides how to reorganize the tree when a node is input, removed, or searched
    node* splay(node* curr, string key) { //takes in a node pointer and a key of type string
        if (curr == NULL) { //return Null if curr node pointer being NULL as base case
            return NULL;
        }
        if (curr->data == key) { //if the data at curr node is equal to the key in the function parameter return curr node
            return curr;
        }

        if (curr->data > key) { //if data at current node is greater than the key
            if (curr->left == NULL) { //check if curr is leaf node by seeing if curr left is Null
                return curr; //if so return curr
            }
            if (curr->left->data > key) { //if data at curr left is greater than key
                curr->left->left = splay(curr->left->left, key); // splay curr left childs left child and set curr left left equal to it
                curr = rightRotate(curr); //sets curr = output of right rotate function using curr node
            }
            else if (curr->left->data < key) { //if data at curr right is less than key we will splay and rotate left
                curr->left->right = splay(curr->left->right, key);
                if (curr->left->right != NULL) {
                    curr->left = leftRotate(curr->left);
                }
            }
            if (curr->left != NULL) { //if curr left is not null, rotate right using curr
                return rightRotate(curr);
            } else { //if not return null
                return curr;
            }
        } else {//if key is greater than curr data
            if (curr->right == NULL) { // check if curr is leaf node by seeing if curr right is Null
                return curr;
            }
            if (curr->right->data > key) { //if data at right is greater than Key
                curr->right->left = splay(curr->right->left, key); //splay cur rightleft
                if (curr->right->left != NULL) { //if cur rightleft is not Null
                    curr->right = rightRotate(curr->right); //rotate right 
                }
            }
            else if (curr->right->data < key) { //of curr right data is less than Key
                curr->right->right = splay(curr->right->right, key); //splay curr right right
                curr = leftRotate(curr); //rotate left
            }
            if (curr->right != NULL) { //if cur right is null, rotate left using curr node 
                return leftRotate(curr);
            } else {
                return curr; //if curr right is Null, return the current 
            }
        }
    }

//To insert the current node we splay against the rest of the tree to determine where it will be inserted
    node* insertHelper(node* curr, string key) {
        curr = splay(curr, key);
        if (curr->data == key) { //once the data in the curr node == key we return the mem address of curr
            return curr;
        }

        node* newNode = createNode(key); //here we create a new node filled with the key of parameter w/
                                    //left and right pointers being NULL

        if (curr->data > key) { //if currdata is greater than key, insert new node pushes curr node to the rightchild node of newNode
            newNode->right = curr; //and curr left child is now left of newNode
            newNode->left = curr->left;
            curr->left = NULL;
        } else { //curr data is less than key. curr becomes the left child of newNode
            newNode->left = curr; // cur right child is now right of NewNode
            newNode->right = curr->right;
            curr->right = NULL;
        }

        return newNode;
    }

    node* deleteNode(node* curr, string key) {
        if (curr == NULL) {
            return curr;
        }
        if (curr->data > key) {
            curr->left = deleteNode(curr->left, key);
        }
        else if (curr->data < key) {
            curr->right = deleteNode(curr->right, key);
        }
        else {
            if (curr->left == NULL && curr->right == NULL) {
                return NULL;
            }
            else if (curr->left == NULL) {
                return curr->right;
            }
            else if (curr->right == NULL) {
                return curr->left;
            }
            node* minNode = curr;
            while(minNode && minNode->left != NULL) {
                minNode = minNode->left;
            }

            curr->data = minNode->data;
            curr->right = deleteNode(curr->right, minNode->data);
        }
        return curr;
    }

//public accessable information from "main"
public:
//always start off splay tree with root being NULL
    SplayTree() {
        root = NULL;
    }

//inserts node with value of key into appropriate location in tree using insertHelper
    void insert(string key) {
        if (root == NULL) { //unless root is NULL then the newly created node is the first node
            root = createNode(key);
        }

        root = insertHelper(root, key);
    }

//searchs through tree using the splay function using the key of the node until the value is found or false is returned 
    bool search(string key) {
        root = splay(root, key);
        if (root->data == key) {
            return true;
        }
        return false;
    }

//prints out in preorder starting at the root
    void preorder() {
        preorderHelper(root);
    }

//prints out in postorder starting at the root
    void postorder() {
        postorderHelper(root);
    }

//prints out inorder starting at the root
    void inorder() {
        inorderHelper(root);
    }

//deletes node according to the key of the node by parsing through the tree
    void remove(string key) {
        deleteNode(root, key);
    }

};

//main function accepts file names as arguments.
int main(int argc, char const *argv[]){
    
    SplayTree sTree; //creating an new splay tree called sTree
    
    int count = 0;  //set counter for amount of nodes in file
    ifstream inp(argv[1]); //uses argv[1] as file and naming it inp
    
    if (inp.is_open()) { //while the file is open, insert all the values of the file into the sTree and counting them
        string data;
        while (getline(inp,data)) {
            sTree.insert(data);
            count ++;
        }

        //prints out nodes after thet have been ordered to begin 
        cout << "\nPrinting the tree in postorder: " << endl; 
        sTree.postorder();
        cout << "\n" << count <<" Students enrolled\n\n";

        //allows user to manipulate the tree basically as much as they want and to escape the program just ytpe delete
        for (int i = 0; i < 1000; i++){
            
            string func; 
            cout<< "Input Function: Insert, Delete, Search, Print, Complete\n";
            cin >> func; //takes in user input corresponding to ope of 5 prompts
            
            if (func == "Insert"){ //if user types insert, console will prompt for name, insert into the tree
                cout<<"Enter Name to Insert: ";
                cin >> data;
                sTree.insert(data);
                cout << data << " has been added.\n";
                cout << "\n" << ++count<<" Students enrolled\n\n"; //and add 1 to the count
            }
            if (func == "Delete"){//if user type delete, console will prompt for name, name is nopw deleted from the tree
                cout<<"Enter Name to Delete: ";
                cin >> data;
                sTree.remove(data);
                cout << data << " has been deleted.\n";
                cout << "\n" << --count <<" Students enrolled\n\n"; //and subrtact 1 from count 
            }
            if (func == "Print"){ //if user types print, console will print out the updated list of students
                cout << "\nPrinting the tree in postorder: " << endl; 
                sTree.postorder();
                cout << "\n" << count <<" Students enrolled\n\n"; //and give count
            }
            if (func == "Search"){ //if user types search, console will promt for name and return true or false
                cout<<"Enter Name to Search: ";
                cin >> data;
                if (sTree.search(data)) {
                    cout << data << " is found in the tree.\n\n";
                    cout << "\n" << count <<" Students enrolled\n\n"; //and give count
                } 
                else {
                    cout << data << " is not found in the tree.\n\n";
                }
            }
            if (func == "Complete"){ //if user types complete program ends
                break;
            }
        }
    return 0;
}
}