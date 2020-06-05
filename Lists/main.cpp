#include <iostream>
#include <locale>
#include <cassert>
#include <chrono>
#include <functional>

#include "ForwardList.h"
#include "BidirectionList.h"
#include "CycleList.h"

void measure_method(std::function<void()> method, std::string name, size_t amount_times)
{
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    for (size_t i = 0; i < amount_times; ++i)
    {
        method();
    }
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Methoud used " << amount_times << " times. Result for method " << std::uppercase << name <<  ":" << std::endl;
    std::cout << "Time = " << std::chrono::duration_cast<std::chrono::microseconds> (end - begin).count() << "[micro]" << std::endl;
}

void measure_performance(IContainer<int>& container, std::string container_name)
{
    for (auto i = 0; i < 100'000; ++i)
    {
        container.insert(rand() % 200'000, 0);
    }
    if (container.find(250'000) != IContainer<int>::invalid_index) // it shouldn't find it
    {
        std::cerr << "Error: container shouldn't found the value" << std::endl;
    }

    auto measure_1000_times = [](std::function<void()> func, std::string name) {
        measure_method(func, name, 1000);
    };

    std::cout << "Started measuring of container: " << container_name << std::endl;
    measure_1000_times([&]() { assert(container.find(250'000) == IContainer<int>::invalid_index); }, "find");
    measure_1000_times([&]() { container.insert(50000, 50000); }, "insert");
    measure_1000_times([&]() { container.erase(50000); }, "erase");
    measure_1000_times([&]() { container.replace(12312, 50000); }, "replace");
    measure_1000_times([&]() { container.sum(); }, "size");
    std::cout << "\n\n";
}

int main()
{
    ForwardList<int> list;
    measure_performance(list, "ForwardList");
    BidirectionList<int> bi_dir;
    measure_performance(bi_dir, "BidirectionalList");
    CycleList<int> array;
    measure_performance(array, "Cycle List");
}