#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <chrono>
#include <getopt.h>
#include "Database.h"
#include "Entry.h"
const std::string VERSION = "0.0.0a";

enum MainOptions
{
    moNothing, // do nothing!
    moPrint, // print database
    moAdd, // add entry to database
};

std::string g_databaseFilename = "pwsafe.yaps";
MainOptions g_whatDo = moNothing; // determine what to do from invocation, store here. do only one thing
std::string g_optionArgument = ""; // argument given after determing what to do, store here.

Database database;

std::string version()
{
    return "yaps v" + VERSION;
}

std::string usage()
{
    std::string str = "";
    str += version() + "\n";
    str += "Options:\n";
    str += "    --help -h        prints this text\n";
    str += "    --file -f        database file to open\n";
    return str;
}

inline bool doesFileExist(std::string filename)
{
    std::ifstream file(filename);
    return file.is_open();
}

Entry& editEntry(Entry &entry)
{
    using std::cout;
    using std::cin;
    using std::endl;
    std::string title;
    std::string username;
    std::string password;
    std::string url;
    std::string notes;
    cout << "Title [" << entry.getTitle() << "]: "; getline(cin, title);
    cout << "Username [" << entry.getUsername() << "]: "; getline(cin, username);
    cout << "Password [" << entry.getPassword() << "]: "; getline(cin, password);
    cout << "URL [" << entry.getUrl() << "]: "; getline(cin, url);
    cout << "Notes [\n" << entry.getNotes() << "\n]: ";
    for (std::string tmp = "\n"; tmp != ""; getline(cin, tmp))
        {if (tmp!= "\n") notes += tmp;}
    if (title != "") entry.setTitle(title);
    if (username != "") entry.setUsername(username);
    if (password != "") entry.setPassword(password);
    if (url != "") entry.setUrl(url);
    if (notes != "") entry.setNotes(notes);
    return entry;
}

// user can pass a "path" through "folders" in database
// to make/edit an entry with a parent other than root
// this takes "EmailAccounts/Work/Sales" and returns "Sales"
// if "EmailAccounts/Work/Sales/" is passed, returns empty string
std::string basename(std::string pathStr)
{
    size_t slashPosition = pathStr.find('/', 0);
    while  (slashPosition != std::string::npos)
    {
        pathStr = pathStr.substr(slashPosition+1);
        slashPosition = pathStr.find('/', 0);
    }
    return pathStr;
}

int parseInput(int argc, char* argv[])
{
    // contains the short version of each option passed
    // so it can be switched
    char c;
    // contains index so if long option details need to be
    // referenced, they can be
    int longOptIndex;
    // actual long options and their short representations
    // http://www.gnu.org/software/libc/manual/html_node/Getopt-Long-Option-Example.html
    struct option long_options[] =
    {
        {"help", no_argument, 0, 'h'},
        {"file", required_argument, 0, 'f'},
        {"add", required_argument, 0, 'a'},
        //{"verbose", no_argument, 0, 'v'},
        //{"list-all", no_argument, 0, 'l'},
        //{"threads", required_argument, 0, 't'}
    };

    // only one arg (program name)
    // assume user wants to print and filename is default
    if (argc == 1)
    {
        g_whatDo = moPrint;
        return 0;
    }
    else
    {
        // get each short arg representation out of argv
        while ((c = getopt_long(argc, argv, "hf:", long_options, &longOptIndex)) != -1)
        {
            // switch the current option
            switch (c)
            {
                case 'h': // help
                    std::cout << usage();
                    return 1;
                    break;
                case 'f': // file
                    g_databaseFilename = optarg;
                    break;
                case 'a': // add entry (arg is title)
                    if (g_whatDo != moNothing)
                    {
                        std::cout << "Error: can only do one thing at a time\n";
                        std::cout << usage();
                        return 1;
                    }
                    g_whatDo = moAdd;
                    g_optionArgument = optarg;
                    break;
                /*case 'v': // verbose flag
                    verbose = true;
                    break;
                case 'l': // list-all flag
                    listAll = true;
                    break;
                case 't': // threads
                    numThreads = atoi(optarg);
                    break;
                case '?': // option not recognized or no argument if required
                    std::cout << usage();
                    return 1;
                    break;*/

            } // switch
        } // while (all the options)
    } // else (argc > 1)

    if (argc - optind != 0)
    {
        std::cout << "Too many things?\n";
        return 1;
    }
    return 0;
}

int main(int argc, char* argv[])
{
    using std::cout;
    using std::cin;
    using std::endl;

    if (parseInput(argc, argv) != 0) return 1;

    database = Database();

    // create new database file if specified (or default) one
    // does not exist
    if (!doesFileExist(g_databaseFilename))
    {
        std::ofstream file(g_databaseFilename);
        if (file.is_open())
        {
            database = Database();
            database.setName("Password Safe");
            database.setPath(g_databaseFilename);
            database.saveToFile(database.getPath());
            std::cout << "New empty database created: " << g_databaseFilename << std::endl;
        }
        else std::cout << "Error making new database\n";
    }

    switch(g_whatDo)
    {
    case moPrint: {
        database.readFromFile(g_databaseFilename);
        cout << database.print();
        break; }
    case moAdd: {
        database.readFromFile(g_databaseFilename);
        Entry entry = Entry();
        entry.setTitle(basename(g_optionArgument)); // set title to passed arg by default
        entry = editEntry(entry);
        cout << "About to generate new entry.\n" << entry.print() << "Look good ([y]es, [n]o, [E]dit)? ";
        char answer;
        cin >> answer;
        if (answer == 'y')
        {
            database.addEntry(entry, database.getEntries()->getRoot()->getId());
        }
        else if (answer == 'n')
        {
            // done
        }
        else
        {
            ///TODO (matt)
            /// do it all again, or basically edit this into a loop
        }
        cout << database.print() << endl;
        break; }
    default: {
        break; }
    }

    return 0;
}

