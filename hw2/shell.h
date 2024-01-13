#ifndef SHELL_H
#define SHELL_H
#include <iostream>
#include <memory>
#include <string>
#include <ctime>
#include <fstream>
#include <sstream>
#include <iomanip> //for setw right left functions
#include "directory.h"
class Shell
{
public:
    // constructor
    Shell();
    // make directory
    void mkdir(const std::string &dirName);
    // change directory
    void cd(const std::string &dirName);
    // list the sub directories and files
    void ls(const bool &recursive) const;
    // print the content of file with given name
    void cat(const std::string &name) const;
    // copy function
    void cp(const std::string &filePath);
    // remove directory or file
    void rm(const std::string &name);

    // load the saved run
    void loadRun();
    void loadAddDir(const std::string &dirName, const std::string &dirDate,
                    const int &dirID, const std::shared_ptr<Directory> &curr);
    void loadAddFile(RegularFile &newFile, const int &dirID);
    // remove the directories and files from txt file
    void removeDirFromTxt(const int &dirID);
    void removeFileFromTxt(const int &dirID);

    // find the directory with given id
    std::shared_ptr<Directory> findDirectory(const std::shared_ptr<Directory> &root, const int &dirID) const;

    // get and set function for current directory
    std::shared_ptr<Directory> getCurrentDir() const { return currentDir; }
    void setCurrentDir(std::shared_ptr<Directory> dir) { currentDir = dir; }

protected:
    std::shared_ptr<Directory> rootDir;
    std::shared_ptr<Directory> currentDir;
    // these functions can't be called in the main or other classes
    void rmRecursive(std::shared_ptr<Directory> curDir, const std::string &name);
    void lsRecursive(std::shared_ptr<Directory> curDir, int level) const;
};
#endif