#ifndef DATABASE_H
#define DATABASE_H
#include "Tree.h"
#include "Entry.h"
#include "FileIO.h"
#include <string>

class Database
{
private:
    //std::string m_name;
    std::string m_path;
    std::string m_password;
    Tree<Entry> m_entries;
public:
    void setName(std::string name) {m_entries.getRoot()->setTitle(name);}
    void setPath(std::string path) {m_path = path;}
    void setPassword(std::string password) {m_password = password;}
    std::string getName() {return m_entries.getRoot()->getTitle();}
    std::string getPath() {return m_path;}
    Tree<Entry>* getEntries() {return &m_entries;}
    //std::string getPassword() {return m_password;}
    uint64_t addEntry(Entry entry, uint64_t parent);
    std::string print(Tree<Entry>* subTree = nullptr);
    void saveToFile(std::string fileName);
    void readFromFile(std::string fileName);
    Database();
};

#endif // DATABASE_H
