#ifndef TREE_H
#define TREE_H
#include <exception>
#include <functional>
#include <vector>

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
    T &getRoot() {return m_root;}
    Tree<T> &getBranch(unsigned int i)
    {
        if (i >= getBranchCount()) throw IndexOutOfRangeException();
        else return m_branches[i];
    }

    unsigned int getBranchCount() {return m_branchCount;}

    Tree<T>* findUsing(std::function<Tree<T>*( Tree<T>*)> searchCriteria)
    {
        // example searchCriteria functions
        // mainly useful in this (yaps) program
        //
        // search by title:
        // auto byTitle    = [&title](Tree<Entry>* tree) -> Tree<Entry>* {return (title == tree->getRoot()->getTitle() ? tree : nullptr);};
        // search by parentId
        // auto byParentId = [&parentId](Tree<Entry>* tree) -> Tree<Entry>* {return (parentId == tree->getRoot()->getId() ? tree : nullptr);};
        Tree<T> *result = searchCriteria(this);
        if (result == nullptr && m_branches.size() > 0)
        {
            for (typename std::vector<Tree<T>>::iterator iter = m_branches.begin(); iter != m_branches.end(); ++iter)
            {
                result = (*iter).findUsing(searchCriteria);
                if (result != nullptr) break;
            }
            return result;
        }
        else return result;
    }

    void addBranch(T data)
    {
        m_branches.push_back(Tree<T>(data));
        m_branchCount++;
    }
    void addBranch(Tree<T> tree)
    {
        m_branches.push_back(tree);
        m_branchCount++;
    }

    void prune()
    {
        m_branches.resize(0);
        m_branchCount = 0;
    }
    void prune(unsigned int i)
    {
        if (i >= getBranchCount()) throw IndexOutOfRangeException();
        else
        {
            m_branches.erase(i);
            m_branchCount--;
        }
    }
    void prune(T val)
    {
        for (typename std::vector<Tree<T>>::iterator iter = m_branches.begin(); iter != m_branches.end(); ++iter)
        {
            if (val == (*iter).getRoot())
            {
                m_branches.erase(iter);
                m_branchCount--;
                break;
            }
        }
    }

    Tree<T>()
    {
        m_branchCount = 0;
    }
    Tree<T>(T root)
    {
        m_root = root;
        m_branchCount = 0;
    }

    virtual ~Tree<T>()
    {

    }

private:
    T m_root;
    unsigned int m_branchCount;
    std::vector<Tree<T>> m_branches;

};

#endif // TREE_H
