#include "directory.h"
#include <iostream>
#include <memory>
#include <fstream>
int Directory::nextId = 0; // static variable to make every directory unique

// get the parent of the directory
std::shared_ptr<Directory> Directory::getParent(const std::shared_ptr<Directory> &dir)
{
    std::shared_ptr<Directory> iter = dir;
    while (iter->getPrev() != nullptr)
    {
        if (iter->getPrev()->getSub() == iter)
        {
            return iter->getPrev();
        }
        iter = iter->getPrev();
    }
    return nullptr; // if there are no parent (main)
}
// add a new file to directory
void Directory::addFile(const RegularFile &file, const int &dirID)
{
    // check if there are already a file with given name if not push back the file
    for (const auto &x : files)
    {
        if (x.getName() == file.getName())
        {
            std::cerr << "There is already a file named " << x.getName() << std::endl;
            return;
        }
    }
    files.push_back(file);
    // write the file to the savefile.txt
    std::ofstream save("savefile.txt", std::ios::app);
    if (!save.is_open())
        return;
    save << "NEWFILE" << std::endl;
    save << dirID << std::endl
         << file.getName() << std::endl
         << file.getDate() << std::endl;
    for (const auto &ch : file.getContent()) // write the content
        save << ch;
    save << std::endl
         << "ENDFILE" << std::endl;
    save.close(); // close file
}