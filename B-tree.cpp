#include <iostream>
#include <vector>
using namespace std;

const int ORDEN = 3;
const int MAX_KEYS = ORDEN - 1;

class BTreeNode {
public:
    vector<int> keys;
    vector<BTreeNode*> children;
    bool isLeaf;
    
    BTreeNode(bool leaf = true) : isLeaf(leaf) {}
    
    bool isFull() { return keys.size() >= MAX_KEYS; }
};

class BTree {
private:
    BTreeNode* root;
    int insertionStep;
    
    void splitChild(BTreeNode* parent, int index) {
        BTreeNode* fullChild = parent->children[index];
        BTreeNode* newChild = new BTreeNode(fullChild->isLeaf);
        
        int midIndex = MAX_KEYS / 2;
        int midKey = fullChild->keys[midIndex];
        
        cout << "\n  [DIVISION] Nodo lleno con keys: ";
        for(size_t k = 0; k < fullChild->keys.size(); k++) cout << fullChild->keys[k] << " ";
        cout << "\n  Subiendo clave " << midKey << " al padre\n";
        
        for(size_t i = midIndex + 1; i < fullChild->keys.size(); i++) {
            newChild->keys.push_back(fullChild->keys[i]);
        }
        
        if(!fullChild->isLeaf) {
            for(size_t i = midIndex + 1; i < fullChild->children.size(); i++) {
                newChild->children.push_back(fullChild->children[i]);
            }
            fullChild->children.resize(midIndex + 1);
        }
        
        fullChild->keys.resize(midIndex);
        
        parent->keys.insert(parent->keys.begin() + index, midKey);
        parent->children.insert(parent->children.begin() + index + 1, newChild);
    }
    
    void insertNonFull(BTreeNode* node, int key) {
        int i = node->keys.size() - 1;
        
        if(node->isLeaf) {
            node->keys.push_back(0);
            while(i >= 0 && key < node->keys[i]) {
                node->keys[i + 1] = node->keys[i];
                i--;
            }
            node->keys[i + 1] = key;
        } else {
            while(i >= 0 && key < node->keys[i]) i--;
            i++;
            
            if(node->children[i]->isFull()) {
                splitChild(node, i);
                if(key > node->keys[i]) i++;
            }
            insertNonFull(node->children[i], key);
        }
    }
    
    void printTree(BTreeNode* node, int level = 0) {
        if(!node) return;
        
        cout << "  Nivel " << level << ": [";
        for(size_t i = 0; i < node->keys.size(); i++) {
            if(i > 0) cout << ", ";
            cout << node->keys[i];
        }
        cout << "]";
        if(node->isLeaf) cout << " (hoja)";
        cout << "\n";
        
        if(!node->isLeaf) {
            for(size_t i = 0; i < node->children.size(); i++) {
                printTree(node->children[i], level + 1);
            }
        }
    }
    
public:
    BTree() : root(new BTreeNode()), insertionStep(0) {}
    
    void insert(int key) {
        insertionStep++;
        cout << "\n--- Paso " << insertionStep << ": Insertar " << key << " ---\n";
        
        if(root->isFull()) {
            BTreeNode* newRoot = new BTreeNode(false);
            newRoot->children.push_back(root);
            splitChild(newRoot, 0);
            root = newRoot;
            insertNonFull(root, key);
        } else {
            insertNonFull(root, key);
        }
        
        cout << "\nArbol actual:\n";
        printTree(root);
    }
    
    void displayFinal() {
        cout << "\n\n========== ARBOL FINAL ==========\n";
        printTree(root);
        cout << "=================================\n";
    }
};

int main() {
    BTree btree;
    
    cout << "B-TREE DE ORDEN 3\n";
    cout << "Insertando: 10, 20, 5, 6, 12, 30, 7, 17\n";
    cout << "=========================================\n";
    
    int values[] = {10, 20, 5, 6, 12, 30, 7, 17};
    for(size_t i = 0; i < 8; i++) {
        btree.insert(values[i]);
    }
    
    btree.displayFinal();
    
    return 0;
}
