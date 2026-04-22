/**
 * @file SearchBenchmark.cpp
 * @brief Программа для сравнения времени поиска в различных структурах данных.
 * @details Реализованы: линейный поиск, BST, красно-черное дерево,
 *          хэш-таблица (с улучшенной хэш-функцией и простым размером),
 *          std::multimap. Для каждого размера массива выполняется 10 поисков,
 *          время усредняется. Результаты сохраняются в CSV-файл.
 */

#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <random>
#include <algorithm>
#include <map>
#include <cctype>
#include "Applicant.hpp"
#include "BSTree.hpp"
#include "RBTree.hpp"
#include "HashTable.hpp"

using namespace std;

// Вспомогательные функции для работы с простыми числами 

/**
 * @brief Проверка, является ли число простым.
 */
static bool isPrime(size_t n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (size_t i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

/**
 * @brief Возвращает ближайшее простое число, не меньшее n.
 */
static size_t nextPrime(size_t n) {
    if (n <= 2) return 2;
    while (!isPrime(n)) ++n;
    return n;
}

// Генерация тестовых данных 

/**
 * @brief Генерирует случайный вектор абитуриентов заданного размера.
 * @param n Количество элементов.
 * @return Вектор объектов Applicant.
 */
vector<Applicant> generateApplicants(size_t n) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> scoreDist(0, 300);
    uniform_int_distribution<int> nameIdxDist(0, 9);
    uniform_int_distribution<int> facultyIdxDist(0, 4);
    uniform_int_distribution<int> specIdxDist(0, 9);

    vector<string> firstNames = {"Ivan", "Petr", "Sergey", "Alexey", "Dmitry", "Andrey", "Mikhail", "Nikolay", "Vladimir", "Alexander"};
    vector<string> lastNames = {"Ivanov", "Petrov", "Sidorov", "Smirnov", "Kuznetsov", "Popov", "Vasilyev", "Sokolov", "Mikhailov", "Novikov"};
    vector<string> faculties = {"FIT", "FEU", "FMF", "LF", "IF"};
    vector<string> specialties = {"Software Engineering", "Informatics", "Economics", "Management", "Mathematics", "Physics", "Law", "History", "Biology", "Chemistry"};

    vector<Applicant> result;
    result.reserve(n);
    for (size_t i = 0; i < n; ++i) {
        int idxFirst = nameIdxDist(gen);
        int idxLast  = nameIdxDist(gen);
        string name = lastNames[idxLast] + " " + firstNames[idxFirst];
        string faculty = faculties[facultyIdxDist(gen)];
        string specialty = specialties[specIdxDist(gen)];
        int score = scoreDist(gen);
        result.emplace_back(name, faculty, specialty, score);
    }
    return result;
}

// Линейный поиск 

/**
 * @brief Линейный поиск всех вхождений ключа.
 * @param data Вектор данных (не отсортирован).
 * @param key Искомое ФИО.
 * @return Вектор копий найденных объектов.
 */
vector<Applicant> linearSearch(const vector<Applicant>& data, const string& key) {
    vector<Applicant> res;
    for (const auto& a : data) {
        if (a.fullName == key) res.push_back(a);
    }
    return res;
}

// Измерение времени 

/**
 * @brief Измеряет время выполнения переданной функции.
 * @tparam Func Тип вызываемого объекта.
 * @param func Функция/лямбда без аргументов.
 * @return Время в секундах.
 */
template<typename Func>
double measureTime(Func&& func) {
    auto start = chrono::high_resolution_clock::now();
    func();
    auto end = chrono::high_resolution_clock::now();
    return chrono::duration<double>(end - start).count();
}

// Главная функция 

int main() {
    // Размеры массивов для тестирования
    vector<size_t> sizes = {100, 500, 1000, 5000, 10000, 50000, 100000, 500000, 1000000};
    ofstream csv("search_times.csv");
    csv << "size,linear,bst,rbt,hash,multimap,collisions\n";

    for (size_t n : sizes) {
        cout << "Processing n = " << n << endl;
        vector<Applicant> data = generateApplicants(n);

        // Построение структур данных
        BSTree bst;
        RBTree rbt;
        // Хэш-таблица с размером, равным ближайшему простому числу, не меньшему 2*n
        HashTable ht(nextPrime(n * 2));
        multimap<string, Applicant> mmap;

        // Заполнение структур
        for (const auto& a : data) {
            bst.insert(a);
            rbt.insert(a);
            ht.insert(a);
            mmap.insert({a.fullName, a});
        }

        // Выбор 10 ключей для поиска (первые 10 элементов)
        vector<string> keys;
        for (size_t i = 0; i < std::min<size_t>(10, data.size()); ++i) {
            keys.push_back(data[i].fullName);
        }

        // Линейный поиск 
        double linear_time = 0.0;
        for (const auto& key : keys) {
            linear_time += measureTime([&]() { linearSearch(data, key); });
        }
        linear_time /= keys.size();

        // BST поиск 
        double bst_time = 0.0;
        for (const auto& key : keys) {
            bst_time += measureTime([&]() { bst.find(key); });
        }
        bst_time /= keys.size();

        // RBTree поиск 
        double rbt_time = 0.0;
        for (const auto& key : keys) {
            rbt_time += measureTime([&]() { rbt.find(key); });
        }
        rbt_time /= keys.size();

        // Хэш-таблица поиск 
        double hash_time = 0.0;
        for (const auto& key : keys) {
            hash_time += measureTime([&]() { ht.find(key); });
        }
        hash_time /= keys.size();

        // std::multimap поиск 
        double mmap_time = 0.0;
        for (const auto& key : keys) {
            mmap_time += measureTime([&]() {
            volatile auto range = mmap.equal_range(key); // результат помечается как volatile
            (void)range; // подавляет предупреждение о неиспользуемой переменной
            });
        }
        mmap_time /= keys.size();

        size_t collisions = ht.getCollisions();

        // Запись в CSV
        csv << n << ","
            << linear_time << ","
            << bst_time << ","
            << rbt_time << ","
            << hash_time << ","
            << mmap_time << ","
            << collisions << "\n";
    }

    csv.close();
    cout << "Benchmark finished. Results saved to search_times.csv\n";
    return 0;
}