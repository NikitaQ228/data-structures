#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

int size(ifstream& file) {
    int size = 0;
    for (double x; file >> x; size++);
    file.clear();
    file.seekg(0);
    return size - 1;
}

int function(double* & a, int size, int d) {
    int count, rez = 0;
    sort(a, a + size);
    for (int i = 0; i < size;) {
        count = 0;
        for (double x = a[i]; x == a[i]; i++, count++);
        if (count <= d) rez += count;
    }
    return rez;
}

int main() {
    ifstream in_file("input.txt");
    ofstream out_file("output.txt");
    int n = size(in_file);
    double* arr = new double[n];
    int d; in_file >> d;
    for (int i = 0; i < n; i++) in_file >> arr[i];
    out_file << function(arr, n, d);
    delete[] arr;
    in_file.close();
    out_file.close();
}
