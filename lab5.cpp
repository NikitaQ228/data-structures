#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <cstring>

using namespace std;

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
    bool operator > (const My_set <T>& S) {
        int cnt = intersect(S);
        if (S.count == cnt && cnt < count) return true;
        return false;
    }
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

int main()
{
    ifstream file_in("input.txt");
    ofstream file_out("output.txt");
    int key; file_in >> key;
    switch (key)
    {
        // 1) Конструктор копирования
    case 1: {
        My_set <int> s;
        file_in >> s;
        My_set <int> t(s);
        file_out << s << endl << t;
        break;
    }
          // 2) Конструктор инциализации
    case 2: {
        int n; file_in >> n;
        int* arr = new int[n];
        for (int i = 0; i < n; ++i)
            file_in >> arr[i];
        My_set <int> s(arr, n);
        file_out << s;
        break;
    }
          // 3) Операция присваивания
    case 3: {
        My_set <int> s, t;
        file_in >> s >> t;
        s = t;
        file_out << s << endl << t;
        break;
    }
          // 4) Добавление элемента
    case 4: {
        int n; My_set <int> s;
        file_in >> n >> s;
        file_out << s.new_set_with(n);
        break;
    }
          // 5) Удаление элемента
    case 5: {
        int n; My_set <int> s;
        file_in >> n >> s;
        file_out << s.new_set_without(n);
        break;
    }
          // 6) Операции над множествами
    case 6: {
        My_set <int> s, t;
        file_in >> s >> t;
        file_out << (s + t) << endl; // Объединение
        file_out << (s - t) << endl; // Пересечение
        file_out << (s / t); // Симметрическая разность
        break;
    }
          // 7) Проверка отношений между множествами
    case 7: {
        My_set <int> s, t;
        file_in >> s >> t;
        file_out << s.check(t);
        break;
    }
          // 8) Операции сравнения
    case 8: {
        My_set <int> s, t;
        file_in >> s >> t;
        file_out << (s == t) << ' ';
        file_out << (s < t) << ' ';
        file_out << (s > t) << ' ';
        file_out << (s <= t) << ' ';
        file_out << (s >= t);
        break;
    }
          //Шаблоный класс (множество символов)
          // 9) Операции над множествами
    case 9: {
        My_set <char> s, t;
        file_in >> s >> t;
        file_out << (s + t) << endl; // Объединение
        file_out << (s - t) << endl; // Пересечение
        file_out << (s / t); // Симметрическая разность
        break;
    }
          // 10) Операции сравнения
    case 10: {
        My_set <char> s, t;
        file_in >> s >> t;
        file_out << (s == t) << ' ';
        file_out << (s < t) << ' ';
        file_out << (s > t) << ' ';
        file_out << (s <= t) << ' ';
        file_out << (s >= t);
        break;
    }
    }
    file_in.close();
    file_out.close();
    return 0;
}
