#pragma once

template<typename Value>
class DoubleLinkedList
{
public:
    struct Node
    {
        Node* prev;
        Node* next;
        Value value;
        Node(const Value& v) : prev(nullptr), next(nullptr), value(v) {}
        ~Node() {}
    };
private:
    Node* head_ = nullptr;
public:
    ~DoubleLinkedList()
    {
        Node* cur = head_;
        while (cur) { Node* next = cur->next; delete cur; cur = next; }
    }
    Node* pushFront(const Value& value)
    {
        Node* n = new Node(value);
        if (empty()) head_ = n;
        else
        {
            head_->prev = n;
            n->next = head_;
            head_ = n;
        }
        return n;
    }
    void remove(Node* n)
    {
        if (n->prev) n->prev->next = n->next;
        else head_ = n->next;
        n->next->prev = n->prev;
        delete n;
    }
    bool  empty() const
    {
        return head_ == nullptr;
    }
    Node* front() const
    {
        return head_;
    }
};