#include "Database.h"
#include "FileIO.h"
#include <fstream>

uint64_t Database::addEntry(Entry entry, uint64_t parentId)
{
    auto matchFunc = [parentId](Tree<Entry>* tree) -> Tree<Entry>* {return (parentId == tree->getRoot()->getId() ? tree : nullptr);};
    Tree<Entry>* parent = m_entries.findUsing(matchFunc);
    if (parent == nullptr)
    {
        // what? If this happens, then big trouble.
    }
    else
    {
        entry.setParent(parent->getRoot()->getId());
        parent->addBranch(entry);
        return entry.getId();
    }
}

std::string indentStr(int indent)
{
    if      (indent <  4) return "";
    else if (indent == 4) return "|---";
    else                  return "    " + indentStr(indent - 4);
}

std::string Database::print(Tree<Entry>* subTree)
{

    if (subTree == nullptr) subTree = &m_entries;
    static int indent = 0;
    std::string str = indentStr(indent) + subTree->getRoot()->getTitle() + "\n";
    indent += 4;
    for (unsigned int i = 0; i < subTree->getBranchCount(); i++)
    {
        Tree<Entry>* t = subTree->getBranch(i);
        str += print(t);
    }
    indent -= 4;
    return str;
}

void saveTreeToFile(std::ofstream &os, Tree<Entry>* tree)
{
    using namespace FileIO;
    Entry* root = tree->getRoot();
    os << root->getId();
    os << root->getParent();
    os << LongData(root->getTitle());
    os << LongData(root->getUsername());
    os << LongData(root->getPassword());
    os << LongData(root->getUrl());
    os << LongData(root->getNotes());
    os << root->getCreatedUInt64();
    os << root->getModifiedUInt64();
    os << LongData(root->getPasswordScheme());
    for (unsigned int i = 0; i < tree->getBranchCount(); i++)
    {
        os << u8(0x01);
        saveTreeToFile(os, tree->getBranch(i));
    }
}

void Database::saveToFile(std::string fileName)
{
    std::ofstream file(fileName.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);
    if (file.is_open())
    {
        using namespace FileIO;
        LongData header = LongData("yapsdata");
        u32 fileFormatVersion = 1;
        file << header;
        file << fileFormatVersion;

        saveTreeToFile(file, &m_entries);
        file << u8(0x00);
        file.close();
    }
}

void readTreeFromFile_v1(std::ifstream &is, Tree<Entry>* tree)
{
    using namespace FileIO;
    u8 processByte = 0x01;
    while (processByte != 0x00)
    {
        u64 id;
        u64 parentId;
        LongData title;
        LongData username;
        LongData password;
        LongData url;
        LongData notes;
        u64 created;
        u64 modified;
        LongData passwordScheme;
        is >> id;
        is >> parentId;
        is >> title;
        is >> username;
        is >> password;
        is >> url;
        is >> notes;
        is >> created;
        is >> modified;
        is >> passwordScheme;
        Entry newEntry = Entry();
        newEntry.setId(id);
        newEntry.setParent(parentId);
        newEntry.setTitle(title.toString());
        newEntry.setUsername(username.toString());
        newEntry.setPassword(password.toString());
        newEntry.setUrl(url.toString());
        newEntry.setNotes(notes.toString());
        newEntry.setCreated(DateTime(created));
        newEntry.setModified(DateTime(modified));
        newEntry.setPasswordScheme(passwordScheme.toString());
        auto parentIdFinder = [&parentId](Tree<Entry>* tree) -> Tree<Entry>* {return (parentId == tree->getRoot()->getId() ? tree : nullptr);};
        Tree<Entry>* parent = tree->findUsing(parentIdFinder);
        if (parent == nullptr)
            tree->addBranch(newEntry);
        else
            parent->addBranch(newEntry);

        is >> processByte;
    }
}

void Database::readFromFile(std::string fileName)
{
    std::ifstream file(fileName.c_str(), std::ios::in | std::ios::binary);
    if (file.is_open())
    {
        using namespace FileIO;
        LongData tmpLD;
        file >> tmpLD;
        if (tmpLD.toString() != "yapsdata"); // bad! throw something probably
        else
        {
            setName(tmpLD.toString());
            u32 saveFileVersion;
            file >> saveFileVersion;
            switch (saveFileVersion)
            {
            case 1:
                readTreeFromFile_v1(file, &m_entries);
                break;
            }
        }
        file.close();
    }
}

Database::Database()
{
    m_entries.setRoot(Entry()); // set the root of the entire database to a new entry
                                // global default info and other big stuff stored here
}
