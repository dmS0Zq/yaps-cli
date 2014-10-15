#ifndef LINKEDLISTNODE_H
#define LINKEDLISTNODE_H

template <class T>
class LinkedListNode
{
public:
    T data;
    LinkedListNode* next;
    LinkedListNode<T>() {next = nullptr;}
    LinkedListNode<T>(T _data) {data = _data; next = nullptr;}
    virtual ~LinkedListNode<T>() {next = nullptr;}
};

#endif // LINKEDLISTNODE_H
