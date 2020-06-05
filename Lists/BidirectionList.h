#pragma once

#include "IContainer.h"

template < class T >
class BidirectionList : public IContainer<T>
{
    struct Node
    {
        T data;
        Node* next;
        Node* prev;

        Node(int d) 
            : data(d)
            , next(nullptr)
            , prev(nullptr)
        { }
    };

public:
    ~BidirectionList()
    {
        while (head != nullptr)
        {
            auto tmp = head->next;
            delete head;
            head = tmp;
        }
    }
    BidirectionList() : head(nullptr) {}

    void insert_after(const T& new_data, Node* prev_node)
    {
        if (prev_node == nullptr)
        {
            return;
        }
        Node* newNode = new Node(new_data);

        newNode->next = prev_node->next;
        prev_node->next = newNode;
        newNode->prev = prev_node;

        if (newNode->next != nullptr)
            newNode->next->prev = newNode;

    }

    void insert(const T& data, size_t index) override
    {
        if (!head)
        {
            head = new Node(data);
        }
        Node* run;
        for (run = head; run->next && index > 1; index--, run = run->next);
        insert_after(data, run);
    }

    void erase(Node* list)
    {
        if (!list)
            return;

        if (list->prev)
            list->prev->next = list->next;
        if (list->next)
            list->next->prev = list->prev;
        if (list == head)
        {
            head = list->next;
        }
        delete list;
    }

    void erase(size_t index) override {
        auto run = head;
        for (; run && index > 0; run = run->next, index--);
        erase(run);
    }

    void replace(const T& data, size_t index ) override {
        if (!head) {
            return;
        }
        auto run = head;
        for (; run->next && index > 0; run = run->next, index--);
        if (index)
            return;
        run->data = data;
    }

    T sum() const override {
        if (!head)
        {
            return T();
        }
        T data = head->data;
        for (auto run = head->next; run; run = run->next)
            data = data + run->data;
        return data;
    }

    size_t find(const T& to_find) const override
    {
        size_t i = 0;
        for (auto run = head; run; run = run->next, ++i)
        {
            if (run->data == to_find)
            {
                return i;
            }
        }
        return IContainer<T>::invalid_index;
    }

private:
    Node* head = nullptr;
};