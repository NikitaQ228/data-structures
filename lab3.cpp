#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

//Элемент списка
class Node {
private:
    //Значение
    double val;
    //Указатель на предыдущий элемент
    Node* prev;
    //Указатель на следующий элемент
    Node* next;
public:
    //Конструкторы
    Node() {}
    Node(double v) : val(v), prev(NULL), next(NULL) {}

    friend class List;
};

//Список
class List {
private:
    //Голова и хвост, которые указывают 
    //на начало и конец соответственно
    Node* head;
    Node* tail;
    //Размер списка
    int size;

    // Ф-я, возвращающая элемент списка по индексу
    Node* get_at(int index) {
        Node* cur = head;
        for (int i = 0; i != index; ++i) {
            if (cur == NULL) return cur;
            cur = cur->next;
        }
        return cur;
    }

    //Ф-я разделения списка
    Node* split(Node* head) {
        Node* f = head, * s = head;
        while (f->next && f->next->next) {
            f = f->next->next;
            s = s->next;
        }
        Node* t = s->next;
        s->next = NULL;
        return t;
    }

    //Слияние двух частей списка
    Node* merge(Node* one, Node* two)
    {
        if (!one) return two;
        if (!two) return one;

        // Выбор меньшего значения
        if (one->val < two->val) {
            one->next = merge(one->next, two);
            one->next->prev = one;
            one->prev = NULL;
            return one;
        }
        else {
            two->next = merge(one, two->next);
            two->next->prev = two;
            two->prev = NULL;
            return two;
        }
    }

    // Функция сортировки слиянием
    Node* merge_sort(Node* head) {
        //Если нет элементов или остался только один
        if (!head || !head->next) return head;
        //Разделяем наш list на две части
        Node* two = split(head);
        // Сортируем обе части
        head = merge_sort(head);
        two = merge_sort(two);
        // Слияние двух частей
        return merge(head, two);
    }

public:
    //Конструктор
    List(int n = 0, Node* h = NULL, Node* t = NULL) : size(n), head(h), tail(t) {}

    //Деструктор
    ~List() { while (head != NULL) pop_front(); }

    //Ф-ии вставки в список и удаления из списка
    Node* push_front(double x) {
        Node* nd = new Node(x);
        nd->next = head;
        if (head) head->prev = nd;
        if (!tail) tail = nd;
        head = nd;
        ++size;
        return nd;
    }

    Node* push_back(double x) {
        Node* nd = new Node(x);
        nd->prev = tail;
        if (tail) tail->next = nd;
        if (!head) head = nd;
        tail = nd;
        ++size;
        return nd;
    }

    void pop_front() {
        if (!head) return;
        Node* nd = head->next;
        if (nd) nd->prev = NULL;
        else tail = NULL;
        delete head;
        head = nd;
        --size;
    }

    void pop_back() {
        if (!tail) return;
        Node* nd = tail->prev;
        if (nd) nd->next = NULL;
        else head = NULL;
        delete tail;
        tail = nd;
        --size;
    }

    Node* insert(int index, double x) {
        Node* right = get_at(index);
        if (!right) return push_back(x);
        Node* left = right->prev;
        if (!left) return push_front(x);
        Node* nd = new Node(x);
        nd->prev = left;
        nd->next = right;
        left->next = nd;
        right->prev = nd;
        ++size;
        return nd;
    }

    void earse(int index) {
        Node* nd = get_at(index);
        if (!nd) return;
        Node* left = nd->prev;
        if (!left) {
            pop_front();
            return;
        }
        Node* right = nd->next;
        if (!right) {
            pop_back();
            return;
        }
        left->next = right;
        right->prev = left;
        delete nd;
        --size;
    }

    //Ф-я вывода списка
    void print(ostream& out) const {
        out << size << ' ';
        for (Node* nd = head; nd != NULL; nd = nd->next)
            out << nd->val << ' ';
    }

    //Ф-я обмена указателями между элементами списка
    void swap(Node* nd1, Node* nd2) {
        if (nd1 == head) head = nd2;
        else if (nd2 == head) head = nd1;

        if (nd1 == tail) tail = nd2;
        else if (nd2 == tail) tail = nd1;

        Node* t = nd1->next;
        nd1->next = nd2->next;
        nd2->next = t;

        if (nd1->next != NULL) nd1->next->prev = nd1;
        if (nd2->next != NULL) nd2->next->prev = nd2;

        t = nd1->prev;
        nd1->prev = nd2->prev;
        nd2->prev = t;

        if (nd1->prev != NULL) nd1->prev->next = nd1;
        if (nd2->prev != NULL) nd2->prev->next = nd2;
    }

    //Сортировка пузырьком
    void BubbleSort() {
        Node* nd;
        Node* nd_end = NULL;
        if (!head) return;
        bool flag = 1;
        while (flag) {
            flag = 0;
            nd = head;
            while (nd->next != nd_end) {
                if (nd->val > nd->next->val){
                    swap(nd, nd->next);
                    flag = 1;
                }
                else nd = nd->next;
            }
            nd_end = nd;
        }
    }

    //Сортировка слиянием
    void MergeSort() {
        //Сортировка
        head = merge_sort(head);
        //Возращаем tail на место
        Node* cur = head;
        while (cur->next) cur = cur->next;
        tail = cur;
    }

    //Перегрузка оператора []
    double operator [] (int index) {
        return get_at(index)->val;
    } 

    friend istream& operator >> (istream& in, List& L);
    friend ostream& operator << (ostream& out, const List& L);
};

//Перегрузка оператора >>
istream& operator >> (istream& in, List& L) {
    for (double x; in >> x; L.push_back(x));
    return in;
}

//Перегрузка оператора <<
ostream& operator << (ostream& out, const List& L) {
    L.print(out);
    return out;
};

int main() {
    ifstream file_in("input.txt");
    ofstream file_out("output.txt");
    bool flag; List l;
    //Чтение из файла
    file_in >> flag >> l;
    //Сортировка списка
    if (flag) l.BubbleSort();
    else l.MergeSort();
    //Вывод в файл
    file_out << l;
    file_in.close();
    file_out.close();
}
