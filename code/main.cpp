#include <fstream>
#include <iostream>
#include <queue>

using namespace std;

struct node {
public:
  int value;
  int height;
  node *left;
  node *right;
};

// create blank node
node *newNode(int key) {
  node *node = new struct node;
  node->value = key;
  node->height = 0;
  node->left = nullptr;
  node->right = nullptr;
  cout << "new node created: " << key << endl;
  return node;
}

// find height
int getHeight(node *node) {
  int left;
  int right;
  if (!node) {
    return -1;
  }
  left = getHeight(node->left);
  right = getHeight(node->right);
  return 1 + max(left, right);
}

// find balance
int getBalance(node *node) {
  return (getHeight(node->left) - getHeight(node->right));
}

// rotate left
node *leftRotate(node *x) {
  struct node *y = x->right;
  struct node *temp = y->left;
  y->left = x;
  x->right = temp;
  return y;
}

// rotate right
node *rightRotate(node *x) {
  struct node *y = x->left;
  struct node *temp = y->right;
  y->right = x;
  x->left = temp;
  return y;
}

// rebalance after inserting
node *rebalance(node *node) {
  node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
  int balance = getBalance(node);
  if (balance >= 2) //+
  {
    if (getBalance(node->left) < 0) //+- double rotation
    {
      node->left = leftRotate(node->left);
    }
    return rightRotate(node);
  }
  if (balance <= -2) //-
  {
    if (getBalance(node->right) > 0) //-+ double rotation
    {
      node->right = rightRotate(node->right);
    }
    return leftRotate(node);
  } else {
    return node;
  }
}

// insert new node
node *insert(node *node, int key) {
  if (!node) {
    return (newNode(key));
  }
  if (key < node->value) {
    node->left = insert(node->left, key);
  } else {
    node->right = insert(node->right, key);
  }
  node = rebalance(node);
  return node;
}

// display AVL level by level
void displayAVL(node *root, ofstream &outputFile) {
  queue<node *> current;
  queue<node *> next;
  current.push(root);
  while (!current.empty()) {
    outputFile << current.front()->value << ' ';
    if (current.front()->left)
      next.push(current.front()->left);
    if (current.front()->right)
      next.push(current.front()->right);
    current.pop();
    if (current.empty()) {
      current.swap(next);
      outputFile << endl;
    }
  }
}

int main() {
  ifstream inputFile;
  inputFile.open("input.txt");
  if (!inputFile) {
    cerr << "input file failed to open" << endl;
  } else {
    int val;
    node *root = nullptr;
    while (inputFile >> val) {
      root = insert(root, val);
    }
    inputFile.close();

    ofstream outputFile;
    outputFile.open("output.txt", ios::trunc);
    if (!outputFile) {
      cerr << "output file failed to open" << endl;
    }
    displayAVL(root, outputFile);
    outputFile.close();
  }
  return 0;
}
