#include "Database.h"
#include "FileIO.h"
#include <fstream>
#include <string.h> // strncmp()

uint64_t Database::addEntry(Entry entry, uint64_t parentId)
{
    auto matchFunc = [parentId](Tree<Entry> *tree) -> Tree<Entry>* {return (parentId == tree->getRoot().getId() ? tree : nullptr);};
    Tree<Entry> *parent = m_entries.findUsing(matchFunc);
    if (parent == nullptr)
    {
        // what? If this happens, then big trouble.
        // consider better solution.
        // idea: throw exception
        // idea: add to an "orphan" group
        return 0; // if issue, then make child of root
    }
    else
    {
        entry.setParent(parent->getRoot().getId());
        parent->addBranch(entry);
        return entry.getId();
    }
}

// little helper function that builds the
// leading indentation string for printing
// each line of database display
std::string indentStr(int indent)
{
    if      (indent <  4) return "";
    else if (indent == 4) return "|---";
    else                  return "    " + indentStr(indent - 4);
}

// returns a std::string with the contents of the database
// all sorted in a nice tree.
///TODO (matt): allow more printing options such as
/// "username@title" instead of just "title"
std::string Database::print(Tree<Entry>* subTree)
{

    if (subTree == nullptr) subTree = &m_entries;
    static int indent = 0;
    std::string str = indentStr(indent) + subTree->getRoot().getTitle() + "\n";
    indent += 4;
    for (unsigned int i = 0; i < subTree->getBranchCount(); i++)
    {
        Tree<Entry> t = subTree->getBranch(i);
        str += print(&t);
    }
    indent -= 4;
    return str;
}

// little recursive helper function that recursively
// writes an entire tree to file
void saveTreeToFile(std::ofstream &os, Tree<Entry> &tree)
{
    using namespace FileIO;
    Entry root = tree.getRoot();
    os << root.getId();
    os << root.getParent();
    os << LongData(root.getTitle());
    os << LongData(root.getUsername());
    os << LongData(root.getPassword());
    os << LongData(root.getUrl());
    os << LongData(root.getNotes());
    os << root.getCreatedUInt64();
    os << root.getModifiedUInt64();
    os << LongData(root.getPasswordScheme());
    for (unsigned int i = 0; i < tree.getBranchCount(); i++)
    {
        os << u8(0x01); // "processBytes" indicates more to come
        saveTreeToFile(os, tree.getBranch(i));
    }
}

// basically a " << " function to write to the given file.
// writes out a little header information, then uses the above helper.
void Database::saveToFile(std::string fileName)
{
    std::ofstream file(fileName.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);
    if (file.is_open())
    {
        using namespace FileIO;
        byte header[] = "yapsdata";
        u32 fileFormatVersion = 1;
        file << header;
        file << fileFormatVersion;

        saveTreeToFile(file, m_entries);
        file << u8(0x00); // "processByte" indicates done
        file.close();
    }
}

// little helper function to read in from a file
void readTreeFromFile_v1(std::ifstream &is, Tree<Entry>* tree)
{
    using namespace FileIO;
    u8 processByte = 0x01;
    // loop while processByte indicates
    // another entree is in file
    while (processByte != 0x00)
    {
        // all the variables stored
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
        // read them in
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
        // make a new entry
        Entry newEntry = Entry();
        // and set all its values
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
        // lambda to find a parent tree node based on id (other examples would be one to find based on title)
        auto parentIdFinder = [&parentId](Tree<Entry>* tree) -> Tree<Entry>* {return (parentId == tree->getRoot().getId() ? tree : nullptr);};
        // find and get the parent
        Tree<Entry>* parent = tree->findUsing(parentIdFinder);
        if (parent == nullptr)
            // parent not found.
            // will happen if parentId = 0
            // hopefully won't happen otherwise ...
            tree->setRoot(newEntry);
        else
            // parent found, add an entry to it
            parent->addBranch(newEntry);
        // get next process byte for while loop
        is >> processByte;
    }
}

// basically a " >> " function to read from the give file.
// gets a little header information, then calls the appropriate
// helper to read in the data
void Database::readFromFile(std::string fileName)
{
    m_path = fileName;
    std::ifstream file(m_path.c_str(), std::ios::in | std::ios::binary);
    if (file.is_open())
    {
        using namespace FileIO;
        byte header[8];
        file.read(header, 8);
        if (strncmp(header, "yapsdata", 8)!=0); // bad! throw something probably
        else
        {
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
                                // global default info and other big stuff can be stored in here
}
