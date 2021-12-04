
#include <iostream>
#include <fstream>

using namespace std;

struct _node {
	string data;
	struct _node* left;
	struct _node* right;
};

typedef struct _node node;

class SplayTree {
private:
	node* root;

	void preorderHelper(node* curr) {
		if (curr == NULL) {
			return;
		}
		cout << curr->data << endl;
		preorderHelper(curr->left);
		preorderHelper(curr->right);
	}

	void inorderHelper(node* curr) {
		if (curr == NULL) {
			return;
		}
		inorderHelper(curr->left);
		cout << curr->data << endl;
		inorderHelper(curr->right);
	}

	void postorderHelper(node* curr) {
		if (curr == NULL) {
			return;
		}
		postorderHelper(curr->left);
		postorderHelper(curr->right);
		cout << curr->data << endl;
	}

	node* createNode(string key) {
		node* newNode = new node;
		newNode->data = key;
		newNode->left = NULL;
		newNode->right = NULL;
		return newNode;
	}

	node* rightRotate(node* curr) {
		node* leftChild = curr->left;
		curr->left = leftChild->right;
		leftChild->right = curr;
		return leftChild;
	}

	node* leftRotate(node* curr) {
		node* rightChild = curr->right;
		curr->right = rightChild->left;
		rightChild->left = curr;
		return rightChild;
	}

	node* splay(node* curr, string key) {
		if (curr == NULL) {
			return NULL;
		}
		if (curr->data == key) {
			return curr;
		}

		if (curr->data > key) {
			if (curr->left == NULL) {
				return curr;
			}
			if (curr->left->data > key) {
				curr->left->left = splay(curr->left->left, key);
				curr = rightRotate(curr);
			}
			else if (curr->left->data < key) {
				curr->left->right = splay(curr->left->right, key);
				if (curr->left->right != NULL) {
					curr->left = leftRotate(curr->left);
				}
			}
			if (curr->left != NULL) {
				return rightRotate(curr);
			} else {
				return curr;
			}
		} else {
			if (curr->right == NULL) {
				return curr;
			}
			if (curr->right->data > key) {
				curr->right->left = splay(curr->right->left, key);
				if (curr->right->left != NULL) {
					curr->right = rightRotate(curr->right);
				}
			}
			else if (curr->right->data < key) {
				curr->right->right = splay(curr->right->right, key);
				curr = leftRotate(curr);
			}
			if (curr->right != NULL) {
				return leftRotate(curr);
			} else {
				return curr;
			}
		}
	}

	node* insertHelper(node* curr, string key) {
		curr = splay(curr, key);
		if (curr->data == key) {
			return curr;
		}

		node* newNode = createNode(key);

		if (curr->data > key) {
			newNode->right = curr;
			newNode->left = curr->left;
			curr->left = NULL;
		} else {
			newNode->left = curr;
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


public:
	SplayTree() {
		root = NULL;
	}

	void insert(string key) {
		if (root == NULL) {
			root = createNode(key);
		}

		root = insertHelper(root, key);
	}

	bool search(string key) {
		root = splay(root, key);
		if (root->data == key) {
			return true;
		}
		return false;
	}

	void preorder() {
		preorderHelper(root);
	}

	void postorder() {
		postorderHelper(root);
	}

	void inorder() {
		inorderHelper(root);
	}

	void remove(string key) {
		deleteNode(root, key);
	}

};

int main(int argc, char const *argv[]){
	SplayTree sTree;
	ifstream inp(argv[1]);
	if (inp.is_open()) {
		string data;
		while (inp >> data) {
			sTree.insert(data);
		}

		cout << "Printing the tree in inorder: " << endl;
		sTree.inorder();

		cout << "\nPrinting the tree in preorder: " << endl;
		sTree.preorder();

		cout << "\nPrinting the tree in postorder: " << endl;
		sTree.postorder();

		cout << "\nEnter a node to search: ";
		cin >> data;
		if (sTree.search(data)) {
			cout << data << " is found in the tree.\n\n";
		} else {
			cout << data << " is not found in the tree.\n\n";
		}

		cout << "\nEnter a node to delete: ";
		cin >> data;
		if (!sTree.search(data)) {
			cout << data << " is not found in the tree.\n\n";
		} else {
			sTree.remove(data);
			cout << "Node is deleted from the tree.\n";
		}
	}
	return 0;
}