#include <iostream>
#include <fstream>
#include <algorithm>
#define FOR(i, n)   for (int i = 0; i < n; i++)

using namespace std;

//Функция нахождения количества уникальных элементов матрицы по строкам
int* unique_line(double** arr, int n, int k) {
    int* res = new int[n]; //Создаем массив с ответами
    FOR(i, n) { //Проходимся по строкам
        int count = 0;
        sort(arr[i], arr[i] + k); //Сортируем строку
        for (int j = 0; j < k;) {
            //int c = 0; //Подсчет кол-ва повторений элемента
            for (double x = arr[i][j]; x == arr[i][j]; j++); //c++
            //if (c == 1) count++; //Если встречается только один раз
            count++;
        }
        res[i] = count;
    }
    return res;
}

int main()
{
    // n - кол-во строк; k - кол-во столбцов
    ifstream in_file("input.txt");
    ofstream out_file("output.txt");
    int n, k; in_file >> n >> k;
    //Создем и вводим двумерный динамический массив
    double** arr = new double* [n];
    FOR(i, n) {
        arr[i] = new double[k];
        FOR(j, k) in_file >> arr[i][j];
    }
    int* res = unique_line(arr, n, k);
    //Выводим результат и освобождаем память
    FOR(i, n) { 
        out_file << res[i] << ' ';
        delete[] arr[i];
    }
    delete[] arr;
    delete[] res;
    in_file.close();
    out_file.close();
}
