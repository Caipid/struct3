#include <iostream>
#include <locale>
#include <fstream>

using namespace std;

// Структура узла бинарного дерева
struct Tree{
    int key;
    Tree* left;
    Tree* right;
};

// Функция для вставки нового узла в бинарное дерево
Tree* insertNode(Tree* root, int key) {
    if (root == NULL) {
     Tree* newNode = new Tree;
        newNode->key = key; 
        newNode->left = NULL; 
        newNode->right = NULL; 
        return newNode; 
    }

    // Рекурсивно ищем место для вставки нового узла
    if (key < root->key)
        root->left = insertNode(root->left, key);
    else if (key > root->key)
        root->right = insertNode(root->right, key);

    return root; // Возвращаем корень дерева после вставки нового узла
}

Tree* findINTree(Tree* root, int key) {
    if (root == NULL) {
        return NULL; // Базовый случай: достигнут конец дерева
    }
    if ((root->left != NULL && root->left->key == key) || (root->right != NULL && root->right->key == key)) {
        return root; // Найден предшественник
    }
    // Рекурсивный поиск в левом и правом поддеревьях
    Tree* left = findINTree(root->left, key);
    Tree* right = findINTree(root->right, key);

    //Возвращаем найденный предшественник или NULL, если не найден
    if (left != NULL) {
        return left;
    }
    else return right;
}

int find_razn(Tree* root, int key,int &min_razn) {
    if (root != NULL) {
        int razn1 = 0;
        int razn2 = 0;
        if (key < root->key) {
            razn1 = root->key - key;
            if (razn1 < min_razn) {
                min_razn = razn1;
            }
            return find_razn(root->left, key, min_razn);
        }

        else if (key > root->key) {
            razn2 = key - root->key;
            if (razn2 < min_razn) {
                min_razn = razn2;
            }
            return find_razn(root->right, key, min_razn);
        }
    }
}

// Функция для удаления узла из бинарного дерева
Tree* deleteNode(Tree* root, int key) {
    if (root == NULL) // Если дерево пустое, возвращаем NULL
        return NULL;
    // Ищем узел, который нужно удалить
    if (key < root->key)
        root->left = deleteNode(root->left, key);
    else if (key > root->key)
        root->right = deleteNode(root->right, key);
    else {
        // Случай 1: У удаляемого узла нет детей или только один ребенок
        if (root->left == NULL) {
            Tree* temp = root->right;
            delete root;
            return temp;
        }
        else if (root->right == NULL) {
            Tree* temp = root->left; 
            delete root; 
            return temp; 
        }

        // Случай 2: У удаляемого узла два ребенка
        Tree* min = root->right; 
        while (min->left != NULL)   
            min = min->left;
        root->key = min->key; 
        root->right = deleteNode(root->right, min->key); 
    }
    return root; 
}

void printTree(Tree* root) {
    cout << root->key << " "; //Выводим корень дерева
    if (root->left != NULL) { //Выводим левого потомка
        cout << root->left->key << " ";
    }
    else { //Если левого потомка нет
        cout << "- ";
    }
    if (root->right != NULL) { //Выводим правого потомка
        cout << root->right->key << " ";
    }
    else { //Если правого потомка нет
        cout << "- ";
    }
    cout << endl; //Переход на поддеревья
    if (root->left != NULL){
        printTree(root->left);
    }
    if (root->right != NULL) { //Если правый потомок есть, рекурсивно выводим его
        printTree(root->right);
    }
}

void writeTree(Tree* root, ofstream& infile) {
    infile << root->key << " ";
    if (root->left != NULL) { //Выводим левого потомка
        infile << root->left->key << " ";
    }
    else { //Если левого потомка нет
        infile << "- ";
    }
    if (root->right != NULL) { //Выводим правого потомка
        infile << root->right->key << " ";
    }
    else { //Есляи правого потомка нет
        infile << "- ";
    }
    infile << endl;
    if (root->left != NULL) {
        writeTree(root->left, infile);
    }
    if (root->right != NULL) { //Если правый потомок есть, рекурсивно выводим его
        writeTree(root->right, infile);
    }
}


int main() {
    setlocale(LC_ALL, "ru");
    ofstream outfile("output.dat");
    ofstream infile("input.dat");
    Tree* root = NULL; // Инициализируем пустое бинарное дерево 
     //Вставляем узлы в бинарное дерево
    root = insertNode(root, 48);
    root = insertNode(root, 32);
    root = insertNode(root, 62);
    root = insertNode(root, 12);
    root = insertNode(root, 36);
    root = insertNode(root, 52);
    root = insertNode(root, 67);
    //root = insertNode(root, 20);
    //root = insertNode(root, 10);
    //root = insertNode(root, 30);
    //root = insertNode(root, 5);
    //root = insertNode(root, 15);
    //root = insertNode(root, 25);
    //root = insertNode(root, 35);
    cout << "Исходное бинарное дерево: " << endl;
    infile << "Исходное бинарное дерево:" << endl;
    outfile << "Исходное бинарное дерево:" << endl;
    printTree(root);
    writeTree(root, infile);
    writeTree(root, outfile);
    int key_now;
    cout << "\n\nВведите ключ, предшественник которого нужно удалить из дерева: ";
    infile << "ключ, предшественник которого нужно удалить из дерева :";
    outfile << "ключ, предшественник которого нужно удалить из дерева :";
    cin >> key_now;
    infile << key_now;
    outfile << key_now << endl;
    Tree* pred = findINTree(root, key_now); // Находим предшественника для заданного ключа
    if (pred == NULL) { // Если заданного ключа нет в дереве, ищем для него потенциальное место в дереве исходя из минимальной разницы 
        int min_razn = 1000000;
        find_razn(root, key_now, min_razn);
        Tree* pred = findINTree(root, key_now - min_razn);
        if (pred == NULL) {
            Tree* pred = findINTree(root, key_now + min_razn);
            outfile << "Найденный предшественник для ключа " << key_now << " : " << key_now + min_razn << endl;
            cout << "Найденный предшественник для ключа " << key_now << " : " << key_now + min_razn << endl;
            root = deleteNode(root, key_now + min_razn);
            outfile << "\nБинарное дерево после удаления узла:" << endl;
            cout << "\nБинарное дерево после удаления узла:" << endl;
            printTree(root);
            writeTree(root, outfile);
        }
        else {
            outfile << "Найденный предшественник для ключа " << key_now << " : " << key_now - min_razn << endl;
            cout << "Найденный предшественник для ключа " << key_now << " : " << key_now - min_razn << endl;
            root = deleteNode(root, key_now - min_razn);
            outfile << "\nБинарное дерево после удаления узла:" << endl;
            cout << "\nБинарное дерево после удаления узла:" << endl;
            printTree(root);
            writeTree(root, outfile);
        }
        
    }
    else {
        infile << "Найденный предшественник для ключа " << key_now << " : " << pred->key << endl;
        cout << "Найденный предшественник для ключа " << key_now << " : " << pred->key << endl;
        root = deleteNode(root, pred->key); // Удаляем найденный узел
        outfile << "\nБинарное дерево после удаления узла:" << endl;
        cout << "\nБинарное дерево после удаления узла:" << endl;
        printTree(root); // Печать бинарного дерева после удаления
        writeTree(root, outfile);
    }
}