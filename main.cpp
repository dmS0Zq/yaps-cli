#include <iostream>
#include <string>
#include <random>
#include <chrono>
#include <getopt.h>
#include "Database.h"
#include "Entry.h"
const std::string VERSION = "0.0.0a";
std::string databaseFilename;

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
        {"file", required_argument, 0, 'f'}
        //{"verbose", no_argument, 0, 'v'},
        //{"list-all", no_argument, 0, 'l'},
        //{"threads", required_argument, 0, 't'}
    };

    // only one arg (program name)
    if (argc == 1)
    {
        std::cout << usage();
        return 1;
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
                    databaseFilename = optarg;
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

    Database db;
    db.setName("pwsafe");
    Entry newEntry = Entry();
    newEntry.setTitle("Reddit");
    db.addEntry(newEntry, db.getEntries()->getRoot()->getId());
    newEntry = Entry();
    newEntry.setTitle("Facebook");
    db.addEntry(newEntry, db.getEntries()->getRoot()->getId());
    newEntry = Entry();
    newEntry.setTitle("Email");
    db.addEntry(newEntry, db.getEntries()->getRoot()->getId());

    std::string title = "Email";
    auto byTitle = [&title](Tree<Entry>* tree) -> Tree<Entry>* {return (title == tree->getRoot()->getTitle() ? tree : nullptr);};
    newEntry = Entry();
    newEntry.setTitle("gmail");
    db.addEntry(newEntry, db.getEntries()->findUsing(byTitle)->getRoot()->getId());

    title = "gmail";
    newEntry = Entry();
    newEntry.setTitle("mt.traudt@gmail.com");
    db.addEntry(newEntry, db.getEntries()->findUsing(byTitle)->getRoot()->getId());

    cout << db.print() << endl;

    if (parseInput(argc, argv) != 0) return 1;

    db.saveToFile(databaseFilename);

    db = Database();

    db.readFromFile(databaseFilename);

    std::cout << db.print() << std::endl;

    return 0;
}

