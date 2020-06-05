#include <iostream>
#include <locale>
#include <cassert>
#include <chrono>
#include <functional>
#include <vector>
#include <algorithm>
#include <iterator>
#include "Tree.h"

using UnbalancedIntTree = Tree<int>;
using BalancedIntTree = Tree<int, true>;

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

void measure_performance()
{
    auto measure_1000_times = [](std::function<void()> func, std::string name) {
        measure_method(func, name, 10000);
    };
    {   std::cout << "Loading" << std::endl;
        BalancedIntTree balancing;
        UnbalancedIntTree without_balancing;
        measure_1000_times([i = 0, &without_balancing]() mutable { without_balancing.add(i++); }, "Sequential addition elements");
        without_balancing.clear();
        measure_1000_times([&]() { without_balancing.add(rand() % 200'000); }, "Random addition elements");
        
        measure_1000_times([i = 0, &balancing]() mutable { balancing.add(i++); }, "Sequential with balancing");
        balancing.clear();
        measure_1000_times([&]() { balancing.add(rand() % 200'000); }, "Random with balancing");
        std::cerr << std::endl;
    }
    {
        std::cerr << "Searching 10000 elements in 10000 Sequential" << std::endl;
        UnbalancedIntTree tree_without_balance;
        BalancedIntTree balanced_tree;
        std::vector< int > elements;
        elements.reserve(10000);
        for (size_t i = 0; i < 10000; i++)
        {
            auto value = i;
            tree_without_balance.add(value);
            balanced_tree.add(value);
            elements.push_back(value);
        }
        std::vector< size_t > indexes;
        for (size_t i = 0; i < 10000; ++i)
        {
            indexes.push_back(rand() % elements.size());
        }
        measure_1000_times([i = 0, &indexes, &tree_without_balance, &elements]() mutable { assert(tree_without_balance.contains(elements[i++])); }, "Searching 1000 elements in not balanced");
        measure_1000_times([i = 0, &indexes, &balanced_tree, &elements]() mutable { assert(balanced_tree.contains(elements[i++])); }, "Searching 1000 elements in the balanced");
        std::cerr << std::endl;
    }

    {
        std::cerr << "Searching 10000 elements in 10000 random" << std::endl;
        UnbalancedIntTree tree_without_balance;
        BalancedIntTree balanced_tree;
        std::vector< int > elements;
        elements.reserve(10000);
        for (size_t i = 0; i < 10000; i++)
        {
            auto value = rand() % 1'000'000;
            tree_without_balance.add(value);
            balanced_tree.add(value);
            elements.push_back(value);
        }
        std::cout << "Receiving elements to search" << std::endl;
        std::vector< size_t > indexes;
        for (size_t i = 0; i < 10000; ++i)
        {
            indexes.push_back(rand() % elements.size());
        }
        measure_1000_times([i = 0, &indexes, &tree_without_balance, &elements]() mutable { assert(tree_without_balance.contains(elements[i++])); }, "Searching 1000 elements in not balanced");
        measure_1000_times([i = 0, &indexes, &balanced_tree, &elements]() mutable { assert(balanced_tree.contains(elements[i++])); }, "Searching 1000 elements in the balanced");
        std::cerr << std::endl;
    }

    {
        std::cerr << "Deleting elements" << std::endl;
        UnbalancedIntTree tree1;
        UnbalancedIntTree tree2;
        std::vector< int > elements;
        elements.reserve(10000);
        for (size_t i = 0; i < 10000; i++)
        {
            auto value = rand() % 1'000'000;
            tree1.add(value);
            tree2.add(value);
            elements.push_back(value);
        }
        tree1.balance();
        tree2.balance();
        std::vector< size_t > indexes;
        for (size_t i = 0; i < 1000; ++i)
        {
            indexes.push_back(rand() % elements.size());
        }
        measure_1000_times([i = 0, &indexes, &tree1, &elements]() mutable { tree1.remove(elements[i++]); }, "Removing elements without rebalancing");
        measure_1000_times([i = 0, &indexes, &tree2, &elements]() mutable { tree2.remove(elements[i++]); tree2.balance(); }, "Removing elements with rebalancing");
        std::cerr << std::endl;
    }
}


int main()
{
    measure_performance();   
}