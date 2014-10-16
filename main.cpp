#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <chrono>
#include <getopt.h>
#include "Database.h"
#include "Entry.h"
const std::string VERSION = "0.0.0a";

std::string g_databaseFilename = "pwsafe.yaps";
std::string g_entryTitleToAdd = "";

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
    if (argc == 1)
    {
        std::ifstream fileCheck(g_databaseFilename);
        if (fileCheck.is_open())
        {
            // file already exists, use it
        }
        else
        {
            std::ofstream newFile(g_databaseFilename);
            if (newFile.is_open())
            {
                database = Database();
                database.setName("Password Safe");
                database.setPath(g_databaseFilename);
                database.saveToFile(database.getPath());
                std::cout << "New database created: " << g_databaseFilename << std::endl;
            }
            else std::cout << "Error making new database\n";
        }
        fileCheck.close();
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
                    g_entryTitleToAdd = optarg;
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

    database.readFromFile(g_databaseFilename);

    std::cout << database.print();

    return 0;
}

