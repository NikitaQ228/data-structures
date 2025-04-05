#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <cstring>

using namespace std;

//Список
template <class T> class List {
private:

    struct Node {
        //Значение
        T val;
        //Указатель на предыдущий элемент
        Node* prev;
        //Указатель на следующий элемент
        Node* next;
        //Конструкторы
        Node() {}
        Node(T v) : val(v), prev(NULL), next(NULL) {}
    };

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

public:
    //Конструктор
    List(int n = 0, Node* h = NULL, Node* t = NULL) : size(n), head(h), tail(t) {}

    //Деструктор
    ~List() { while (head != NULL) pop_front(); }

    //функция, которая возвращыет число элементов в списке
    int get_size() const { return size; }

    //Ф-ии вставки в список и удаления из списка
    Node* push_front(T x) {
        Node* nd = new Node(x);
        nd->next = head;
        if (head) head->prev = nd;
        if (!tail) tail = nd;
        head = nd;
        ++size;
        return nd;
    }

    Node* push_back(T x) {
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
        for (Node* nd = head; nd != NULL; nd = nd->next)
            out << nd->val << endl;
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
                if (nd->val > nd->next->val) {
                    swap(nd, nd->next);
                    flag = 1;
                }
                else nd = nd->next;
            }
            nd_end = nd;
        }
    }

    //функция, которая проверяет факт упорядоченности
    bool check_sort() const {
        Node* cur = head;
        while (cur->next != NULL) {
            if (cur->val > cur->next->val) 
                return false;
            cur = cur->next;
        }
        return true;
    }

    //Перегрузка оператора []
    double operator [] (int index) {
        return get_at(index)->val;
    }

    template <class V> friend istream& operator >> (istream& in, List <V>& L);
    template <class V> friend ostream& operator << (ostream& out, const List <V>& L);
};

//Перегрузка оператора >>
template <class V> istream& operator >> (istream& in, List <V>& L) {
    for (V x; in >> x; L.push_back(x));
    return in;
}

//Перегрузка оператора <<
template <class V> ostream& operator << (ostream& out, const List<V>& L) {
    L.print(out);
    return out;
};

template <class T> class My_set {
private:
    T* elem; //Массив с элементами множества
    int count; //Кол-во элементов
    int capacity; //Емкость массива

    bool contains(T value) {
        for (int i = 0; i < count; ++i)
            if (elem[i] == value)
                return true;
        return false;
    }

    int find_pos(T value) {
        for (int i = 0; i < count; ++i)
            if (elem[i] == value)
                return i;
        return count;
    }

    int intersect(const My_set <T>& S) {
        int cnt = 0;
        for (int i = 0; i < S.count; ++i)
            if (contains(S.elem[i])) ++cnt;
        return cnt;
    }

public:
    //Конструктор по умолчанию
    My_set() : elem(NULL), count(0), capacity(0) {}

    //Конструктор копирования
    My_set(const My_set <T>& S) : count(S.count), capacity(S.capacity) {
        elem = new T[capacity];
        memcpy(elem, S.elem, sizeof(int) * count);
    }

    //Конструктор инциализации
    My_set(T* m, int s) {
        if (s && m) {
            count = capacity = s;
            elem = new T[count];
            memcpy(elem, m, sizeof(int) * count);
        }
        else {
            count = capacity = 0;
            elem = NULL;
        }
    }

    //Деструктор
    ~My_set() {
        if (elem) delete[] elem;
    }

    //Операция присваивания
    My_set <T>& operator = (const My_set <T>& S) {
        if (&S == this) return *this;
        My_set <T> temp(S);
        swap(elem, temp.elem);
        swap(count, temp.count);
        swap(capacity, temp.capacity);
        return *this;
    }

    //Добавление элемента
    void insert(T c) {
        if (contains(c)) return;
        if (count >= capacity) {
            capacity += 20;
            T* temp = new T[capacity];
            memcpy(temp, elem, sizeof(int) * count);
            temp[count++] = c;
            delete[] elem;
            elem = temp;
        }
        else elem[count++] = c;
    }
    My_set <T> new_set_with(T data) {
        My_set <T> temp(*this);
        temp.insert(data);
        return temp;
    }

    //Удаление элемента
    void erase(T c) {
        int pos = find_pos(c);
        if (pos == count) return;
        memcpy(elem + pos, elem + pos + 1, sizeof(int) * (count-- - pos - 1));
    }
    My_set <T> new_set_without(T data) {
        My_set <T> temp(*this);
        temp.erase(data);
        return temp;
    }

    //Операции над множествами

    //Объединение
    My_set <T>& operator += (const My_set <T>& S) {
        for (int i = 0; i < S.count; ++i) insert(S.elem[i]);
        return *this;
    }
    My_set <T> operator + (const My_set <T>& S) {
        My_set <T> temp(*this);
        temp += S; return temp;
    }

    //Пересечение
    My_set <T>& operator -= (const My_set <T>& S) {
        for (int i = 0; i < S.count; ++i) erase(S.elem[i]);
        return *this;
    }
    My_set <T> operator - (const My_set <T>& S) {
        My_set <T> temp(*this);
        temp -= S; return temp;
    }

    //Симметрическая разность
    My_set <T>& operator /= (const My_set <T>& S) {
        for (int i = 0; i < S.count; ++i) {
            if (contains(S.elem[i])) erase(S.elem[i]);
            else insert(S.elem[i]);
        }
        return *this;
    }
    My_set <T> operator / (const My_set <T>& S) {
        My_set <T> temp(*this);
        temp /= S; return temp;
    }

    //Проверка отношений между множествами
    string check(const My_set <T>& S) {
        int cnt = intersect(S);
        if (cnt == 0) return "The sets do not intersect.";
        if (count == cnt && cnt == S.count) return "The sets are equal.";
        if (S.count == cnt && cnt < count) return "Set 2 is included in set 1.";
        if (count == cnt && cnt < S.count) return "Set 1 is included in set 2.";
        return "The sets intersect.";
    }

    //Операции сравнения
    bool operator == (const My_set <T>& S) {
        if (&S == this) return true;
        if (count != S.count) return false;
        for (int i = 0; i < S.count; ++i)
            if (!contains(S.elem[i])) return false;
        return true;
    }
    bool operator != (const My_set <T>& S) {
        return !(*this == S);
    }
    bool operator < (const My_set <T>& S) {
        int cnt = intersect(S);
        if (count == cnt && cnt < S.count) return true;
        return false;
    }
    /*bool operator > (const My_set <T>& S) {
        int cnt = intersect(S);
        if (S.count == cnt && cnt < count) return true;
        return false;
    }*/
    bool operator <= (const My_set <T>& S) {
        if (&S == this) return true;
        int cnt = intersect(S);
        if (count == cnt && cnt <= S.count) return true;
        return false;
    }
    bool operator >= (const My_set <T>& S) {
        if (&S == this) return true;
        int cnt = intersect(S);
        if (S.count == cnt && cnt <= count) return true;
        return false;
    }


    //Перегрузка > для сортировки по сумме элементов
    bool operator > (const My_set <T>& S) {
        if (&S == this || count < S.count) return false;
        if (count > S.count) return true;
        sort(elem, elem + count);
        sort(S.elem, S.elem + count);
        for (int i = 0; i < count; ++i) {
            if (elem[i] > S.elem[i]) return true;
            if (elem[i] < S.elem[i]) return false;
        }
        return false;
    }

    //Перегрузка операции вывода в поток
    template <class V> friend ostream& operator<< (ostream& out, const My_set <V>& S);

    //Перегрузка операции чтения из потока
    template <class V> friend istream& operator>> (istream& in, My_set <V>& S);
};

template <class T> ostream& operator<< (ostream& out, const My_set <T>& S) {
    for (int i = 0; i < S.count; ++i)
        out << S.elem[i] << ' ';
    return out;
}

template <class T> istream& operator>> (istream& in, My_set <T>& S) {
    My_set <T> temp; T c;
    while (in >> c && in.peek() != '\n') temp.insert(c);
    if (in.peek() == '\n') temp.insert(c);
    if (S.elem && S.count && S.capacity) {
        swap(S.elem, temp.elem);
        swap(S.count, temp.count);
        swap(S.capacity, temp.capacity);
    }
    else {
        S.elem = temp.elem; temp.elem = NULL;
        S.count = temp.count; temp.count = 0;
        S.capacity = temp.capacity; temp.capacity = 0;
    }
    return in;
}

//функция, которая создает объект со случайными параметрами
void random_creat(List <My_set <int>>& list, int size) {
    srand(time(NULL));
    int count;
    while (size--) {
        My_set <int> temp;
        count = rand() % 20;
        while (count--) 
            temp.insert(rand() % 50);
        list.push_back(temp);
    }
}


int main()
{
    ifstream file_in("input.txt");
    ofstream file_out("output.txt");
    int size; file_in >> size;
    List <My_set <int>> list;
    random_creat(list, size);
    list.BubbleSort();
    cout << list.check_sort();
    file_out << list.get_size();
    file_in.close();
    file_out.close();
    return 0;
}
