#pragma once

#include "IContainer.h"

template <class T>
class CycleList : public IContainer<T>
{
    struct Node
    {
        Node(T data)
        : data(data)
        , next(nullptr)
        {
        }
        T data;
        Node* next;
    };
public:
    ~CycleList()
    {
        while(m_head)
        {
            erase(1);
        }
    }

    void insert(const T& data, size_t index) override
    {
        Node* node = new Node(data);
        m_nodes++;
        if (index == 0 || !m_head)
        {
            node->next = m_head ? m_head : node;
            m_head = node;
            return;
        }
        auto run = m_head;
        while (index > 1)
        {
            index--;
            run = run->next;
        }
        node->next = run->next;
        run->next = node;
    }
    void erase(size_t index) override
    {
        if (!m_head)
        {
            return;
        }
        else if (index == 0)
        {
            index = m_nodes;
        }

        if (m_nodes == 1)
        {
            delete m_head;
            m_head = nullptr;
            m_nodes = 0;
            return;
        }
        auto run = m_head;
        while (index > 1)
        {
            index--;
            run = run->next;
        }
        auto tmp = run->next;
        run->next = tmp->next;
        delete tmp;
        m_nodes--;
    }
    void replace(const T& data, size_t index) override
    {
        if (!m_head)
        {
            return;
        }
        auto run = m_head;
        while (index > 0)
        {
            index--;
            run = run->next;
        }
        run->data = data;
    }

    T sum() const override
    {
        if (!m_head)
        {
            return T();
        }
        auto run = m_head->next;
        T data = m_head->data;
        for (int i = 1; i < m_nodes; ++i)
        {
            data = data + run->data;
            ++i;
            run = run->next;
        }
        return data;
    }
    size_t find(const T& data) const override
    {
        if (!m_head)
        {
            return IContainer<T>::invalid_index;
        }
        auto run = m_head;
        for (int i = 0; i < m_nodes; ++i)
        {
            if (run->data == data)
            {
                return i;
            }
            run = run->next;
        }
        return IContainer<T>::invalid_index;
    }
private:
    Node* m_head = nullptr;
    size_t m_nodes = 0;
};