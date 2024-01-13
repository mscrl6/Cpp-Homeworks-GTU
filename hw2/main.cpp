#include "shell.h"
int main()
{
    Shell myShell; // create a new shell object
    std::string input;
    myShell.loadRun();

    std::cout << "Welcome to my shell. (write 'exit' to terminate the program)" << std::endl
              << "The commands are: ls, ls -R, mkdir, cd, rm, cat, cp" << std::endl
              << std::endl;
    while (RegularFile::getTotalBytes() <= 1024 * 10)
    {
        if (RegularFile::getTotalBytes() > 1024 * 10)
        {
            std::cerr << "Disk is full" << std::endl;
            exit(-1);
        }
        std::cout << "> ";
        getline(std::cin, input); // get the input
        if (input == "exit")
        {
            /*
            copy the savefile to the savefilecopy.
            the contents of savefile will be deleted when shell run again
            when shell run again load function will read savefilecopy.
            new created files and directories (including created from savefilecopy)
            will be saved to the empty savefile.txt
            */
            std::ofstream save("savefile.txt", std::ios::app);
            save << "ENDOFTHESAVEFILE" << std::endl;
            save << RegularFile::getTotalBytes() << std::endl; // keep the total bytes
            save.close();
            std::ifstream sourceFile("savefile.txt");
            std::ofstream targetFile("savefilecopy.txt");

            if (!sourceFile.is_open() || !targetFile.is_open())
            {
                return 1;
            }
            std::string line;
            while (getline(sourceFile, line))
            {
                targetFile << line << std::endl;
            }
            sourceFile.close();
            targetFile.close();
            std::cout << "Shell terminated." << std::endl;
            break;
        }

        std::stringstream ss(input);
        std::string command, arg;
        ss >> command;
        if (command == "mkdir")
        {
            if (ss >> arg)
                myShell.mkdir(arg);
            else
                std::cerr << "You should write the name of the new directory" << std::endl;
        }
        else if (command == "ls")
        {
            ss >> arg;
            if (arg == "-R")
                myShell.ls(true); // recursive list
            else if (arg == "")
                myShell.ls(false);
            else
                std::cerr << "Invalid input" << std::endl;
        }
        else if (command == "cd")
        {
            if (ss >> arg)
                myShell.cd(arg);
            else
                std::cerr << "You should write the name of the directory" << std::endl;
        }
        else if (command == "cp")
        {
            if (ss >> arg)
                myShell.cp(arg);
            else
                std::cerr << "You should write the name of the source directory" << std::endl;
        }
        else if (command == "cat")
        {
            if (ss >> arg)
                myShell.cat(arg);
            else
                std::cerr << "You should write the name of the file with extension." << std::endl;
        }
        else if (command == "rm")
        {
            if (ss >> arg)
                myShell.rm(arg);
            else
                std::cerr << "You should write the name of the file or directory." << std::endl;
        }
        else
            std::cerr << "Invalid input." << std::endl;
    }
}
