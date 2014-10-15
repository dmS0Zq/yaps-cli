#ifndef TREE_H
#define TREE_H
#include <exception>
#include <functional>
#include "LinkedListNode.h"

class IndexOutOfRangeException: public std::exception
{
  virtual const char* what() const throw() {return "Index out of range";}
};

class InvalidOperationException: public std::exception
{
    virtual const char* what() const throw() {return "Invalid operation";}
};

template <class T>
class Tree
{
public:

    void setRoot(T root) {m_root = root;}
    T* getRoot() {return &m_root;}
    Tree<T>* getBranch(unsigned int i)
    {
        if (i >= getBranchCount()) throw IndexOutOfRangeException();
        else
        {
            LinkedListNode<Tree<T>*>* iter = m_branches;
            for ( ; i > 0; iter = iter->next)
            {
                i--;
            }
            return iter->data;
        }
    }

    unsigned int getBranchCount() {return m_branchCount;}

    Tree<T>* findUsing(std::function<Tree<T>* (Tree<T>*)> searchCriteria)
    {
        Tree<T>* result = searchCriteria(this);
        if (result == nullptr && m_branches != nullptr)
        {
            for (LinkedListNode<Tree<T>*>* iter = m_branches; iter != nullptr; iter = iter->next)
            {
                result = iter->data->findUsing(searchCriteria);
                if (result != nullptr) break;
            }
            return result;
        }
        else return result;

    }

    /*bool contains(T needle)
    {
        if (needle == m_root)
            return true;
        else if (m_count > 0)
            for (LinkedListNode<Tree<T>*>* iter; iter != nullptr; iter = iter->next)
                if (iter->find(needle)) return true;
        else
            return false;
    }*/

    void addBranch(T data)
    {
        LinkedListNode<Tree<T>*>* newBranch = new LinkedListNode<Tree<T>*>;
        newBranch->data = new Tree<T>;
        newBranch->data->m_root = data;
        newBranch->next = m_branches;
        m_branches = newBranch;
        m_branchCount++;
    }
    void addBranch(Tree<T>* tree)
    {
        LinkedListNode<Tree<T>*>* newBranch = new LinkedListNode<Tree<T>*>();
        newBranch->data = tree;
        newBranch->next = m_branches;
        m_branches = newBranch;
        m_branchCount++;
    }

    Tree<T>()
    {
        m_branches = nullptr;
        m_branchCount = 0;
    }
    Tree<T>(T root)
    {
        m_root = root;
        m_branches = nullptr;
        m_branchCount = 0;
    }

    virtual ~Tree<T>() {}

private:
    T m_root;
    unsigned int m_branchCount;
    LinkedListNode<Tree<T>*>* m_branches;

};

#endif // TREE_H
