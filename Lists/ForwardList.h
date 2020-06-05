#pragma once

#include "IContainer.h"
#include <limits>

template< class T >
class ForwardList : public IContainer<T>
{
    struct Node {
        Node* next;
        T data;

        Node(const T& data) : next(nullptr), data(data) {}
    }; 

public:
    static const uint32_t invalid_index = std::numeric_limits<uint32_t>::max();

    ~ForwardList() {
        while (m_head != nullptr)
        {
            auto tmp = m_head->next;
            delete m_head;
            m_head = tmp;
        }
    }

    void insert(const T& data, size_t index) override
    {
        Node* node = new Node(data);
        if (index == 0 || !m_head)
        {
            node->next = m_head;
            m_head = node;
            return;
        }
        Node* run = m_head;
        while (run->next && index > 1)
        {
            index--;
            run = run->next;
        }
        node->next = run->next;
        run->next = node;
    }

    void erase(size_t index) override // Bigger index handled like pop_back
    {
        if (!m_head)
        {
            return;
        }

        if (index == 0 || !m_head->next)
        {
            auto tmp = m_head->next;
            delete m_head;
            m_head = tmp;
            return;
        }

        auto run = m_head;
        while (run->next && run->next->next && index > 1)
        {
            run = run->next;
            index--;
        }
        auto tmp = run->next;
        run->next = run->next->next;
        delete tmp;
    }

    void replace(const T& data, size_t index) override
    {
        Node* run;
        for (run = m_head; run && run->next && index; run = run->next, index--); // iterate to last member or index member
        if (index || !run)
        {
            throw std::invalid_argument("");
        }
        run->data = data;
    }

    T sum() const override
    {
        if (!m_head)
        {
            return T();
        }
        T node = m_head->data;
        for (auto run = m_head->next; run; run = run->next)
        {
            node = node + run->data;
        }
        return node;
    }
    size_t find(const T& to_find) const override
    {
        size_t i = 0;
        for (auto run = m_head; run; run = run->next, ++i)
        {
            if (run->data == to_find)
            {
                return i;
            }
        }
        return IContainer<T>::invalid_index;
    }
private:
    Node* m_head = nullptr;
};
