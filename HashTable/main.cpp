#include <iostream>
#include <locale>
#include <cassert>
#include <chrono>
#include <functional>
#include <vector>
#include <algorithm>
#include <iterator>
#include "Tree.h"
#include "Variant.h"
#include "HashTable.h"
#include <sstream>
#include <fstream>

void measure_method(std::function<void()> method, std::string name, size_t amount_times)
{
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    for (size_t i = 0; i < amount_times; ++i)
    {
        method();
    }
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Methoud used " << amount_times << " times. Result for method " << std::uppercase << name << ":" << std::endl;
    std::cout << "Time = " << std::chrono::duration_cast<std::chrono::microseconds> (end - begin).count() << "[micro]" << std::endl;
}

std::vector<Address> generate_address(size_t size)
{
    std::vector<Address> result;
    for (size_t i = 0; i < size; ++i)
    {
        result.push_back(Address::randomize());
    }
    return result;
}

std::vector<size_t> generate_nums(size_t size, size_t border)
{
    std::vector<size_t> result;
    for (size_t i = 0; i < size; ++i)
    {
        result.push_back(rand() % border);
    }
    return result;
}

void measure_performance(ICollection<Address>& a,
                         const std::vector<Address>& elements_to_insert,
                         const std::vector<Address>& pre_inserted_elements,
                         const std::vector<size_t>& indexes_to_find)
{
    for (const auto& v : pre_inserted_elements)
    {
        a.add(v);
    }
    measure_method([i = 0, &elements_to_insert, &a]() mutable {a.add(elements_to_insert[i++]); }, "Insertion 100 times", 100);
    measure_method([i = 0, &pre_inserted_elements, &indexes_to_find,  &a]() mutable {assert(a.contains(pre_inserted_elements[indexes_to_find[i++]])); }, "Finding 500 times", 500);
}

void measure(size_t size)
{
    std::vector<Address> elements_to_insert = generate_address(100);
    std::vector<Address> pre_inserted_elements = generate_address(size);
    std::vector<size_t> indexes_to_find = generate_nums(500, size);

    Tree<Address> tree;
    HashTable<Address> table([](const Address& v)->size_t {return hash(v); });

    std::cout << "Testing with " << size << " elements" << std::endl;
    std::cout << "Tree" << std::endl;
    measure_performance(tree, elements_to_insert, pre_inserted_elements, indexes_to_find);
    std::cout << "\nHashTable" << std::endl;
    measure_performance(table, elements_to_insert, pre_inserted_elements, indexes_to_find);
    std::cout << "\n\n";
}

int main()
{
    measure(1000);
    measure(10000);
    measure(100000);
    measure(200000);
}
