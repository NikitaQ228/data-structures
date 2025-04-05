#include <iostream>
#include <queue>
#include <fstream>

using namespace std;

//Элемент дерева
class Node {
private:
    //Значение
    int val;
    //Сыновья
    Node* left;
    Node* right;
public:
    //Конструктор
    Node(int v = int(), Node* l = NULL, Node* r = NULL) : 
        val(v), left(l), right(r) {}

    friend class Tree;
};

class Tree {
private:
    //Корень дерева
    Node* root;

    //Удаление дерева
    void DelTree(Node* nd) {
        if (!nd) return;
        //Удаляем левое поддерево
        DelTree(nd->left);
        //Удаляем правое поддерево
        DelTree(nd->right);
        //Удаляем корень
        delete nd;
        nd = NULL;
    }
    //Заполнение дерева в ширину
    void FillTree(istream& in) {
        //Заполнение с  помощью очереди
        int data; in >> data;
        queue<Node*> q;
        root = new Node(data);
        q.push(root);
        while (in) {
            Node* curr = q.front(); q.pop();
            if (!curr->left && in >> data) {
                curr->left = new Node(data);
                q.push(curr->left);
            }
            if (!curr->right && in >> data) {
                curr->right = new Node(data);
                q.push(curr->right);
            }
        }
    }
    //Обратный обход дерева и запись
    void PostOrder(ostream& out, Node* nd) const {
        if (!nd) return;
        PostOrder(out, nd->left);
        PostOrder(out, nd->right);
        out << nd->val << ' ';
    }
    //Ф-я удаления нечётных элементов
    Node* DeleteOdd(Node* nd, Node* parent = NULL) {
        //Если элемента нет, то выходим
        if (!nd) return NULL;
        //Ecли элемент нечетный
        if (nd->val % 2) {
            //Если у элемента есть левый сын
            if (nd->left) {
                Node* nd2 = nd->left;
                if (parent) {
                    if (parent->left == nd) parent->left = nd2;
                    else parent->right = nd2;
                }
                /*При наличии правого сына делаем его левым сыном самого левого элемент
                (по 4 пункту пояснения к заданию)*/
                if (nd->right) {
                    Node* cur = nd2;
                    while (cur->left) cur = cur->left;
                    cur->left = nd->right;
                }
                delete nd;
                //Подтягиваем левого сына на место удаленного элемента
                nd = DeleteOdd(nd2, parent);
            }
            //Если есть только правый сын или нет вообще
            else {
                Node* nd2 = nd->right;
                if (parent) {
                    if (parent->left == nd) parent->left = nd2;
                    else parent->right = nd2;
                }
                delete nd;
                nd = DeleteOdd(nd2, parent);
            }
        }
        //В случае, если это чётный элемент
        else {
            //Передвигаемся дальше по дереву
            DeleteOdd(nd->left, nd);
            DeleteOdd(nd->right, nd);
        }
        return nd;
    }

public:
    //Конструктор
    Tree(Node* r = NULL) : root(r) {}

    //Деструктор
    ~Tree() { DelTree(root); }

    //Ф-я удаляет нечёт элементы в нашем дереве
    void delete_odd() {
        //Вызов ф-ии для корня
        root = DeleteOdd(root);
    }

    //Перегрузки операций
    friend ostream& operator << (ostream&, const Tree&);
    friend istream& operator >> (istream&, Tree&);
};

istream& operator >> (istream& in, Tree& T) {
    T.FillTree(in); return in;
}

ostream& operator << (ostream& out, const Tree& T) {
    T.PostOrder(out, T.root); return out;
}

int main() {
    ifstream file_in("input.txt");
    ofstream file_out("output.txt");

    //Считывание дерева
    Tree T; file_in >> T;

    //Функция, выполняющая процедуру
    T.delete_odd();

    //Запись в файл
    file_out << T;

    file_in.close();
    file_out.close();
}
