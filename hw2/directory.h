#ifndef DIRECTORY_H
#define DIRECTORY_H
#include "file.h"
class Directory
{
public:
    // directory constructor. every directory has unique id
    Directory(const std::string &dn) : directoryName(dn), next(nullptr), prev(nullptr), sub(nullptr)
    {
        id = ++nextId;
    }
    // setters and getters
    void setNext(const std::shared_ptr<Directory> &nextDirectory) { next = nextDirectory; }
    void setPrev(const std::shared_ptr<Directory> &prevDirectory) { prev = prevDirectory; }
    void setSub(const std::shared_ptr<Directory> &subDirectory) { sub = subDirectory; }
    void setDate(const std::string &date) { directoryDate = date; }
    std::string getName() const { return directoryName; }
    std::string getDate() const { return directoryDate; }
    std::vector<RegularFile> &getFiles() { return files; }
    std::shared_ptr<Directory> getNext() const { return next; }
    std::shared_ptr<Directory> getPrev() const { return prev; }
    std::shared_ptr<Directory> getSub() const { return sub; }
    int getId() const { return id; }
    void setId(const int &i) { id = i; }
    // find the parent of directory
    std::shared_ptr<Directory> getParent(const std::shared_ptr<Directory> &dir);
    // add file to the directory with given id
    void addFile(const RegularFile &file, const int &dirID);

private:
    std::string directoryName;
    std::string directoryDate;
    int id;
    static int nextId;

    std::shared_ptr<Directory> next;
    std::shared_ptr<Directory> prev;
    std::shared_ptr<Directory> sub;
    
    std::vector<RegularFile> files;
};

#endif