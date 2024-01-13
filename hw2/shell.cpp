#include "shell.h"

// shell constructor
Shell::Shell()
{
    // create a main directory
    std::shared_ptr<Directory> mainDir = std::make_shared<Directory>("main");
    rootDir = mainDir; // root and current directory is main directory at first
    currentDir = mainDir;
}
// load saved run function
void Shell::loadRun()
{
    // clear the savefile, we will use it in current run.
    std::ofstream erasefile("savefile.txt", std::ios::out | std::ios::trunc);
    if (!erasefile.is_open())
    {
        std::cerr << "Error opening the file!" << std::endl;
        return;
    }
    erasefile.close();
    // read the copied file from savefile.txt
    std::ifstream file("savefilecopy.txt");
    int dirID, parentID;
    std::string name, date;
    std::vector<char> content;
    std::string line;
    std::shared_ptr<Directory> currentSaveDir = rootDir;
    bool endOfFile = false;
    while (!endOfFile)
    {
        if (!getline(file, line))
        {
            endOfFile = true;
            break;
        }
        // if it's a new directory
        if (line == "NEWDIRECTORY")
        {
            file >> parentID >> dirID;
            getline(file, line);
            getline(file, name);
            getline(file, date);
            loadAddDir(name, date, dirID, findDirectory(rootDir, parentID)); // add the directory to given parent
        }
        // if it's a new file
        else if (line == "NEWFILE")
        {
            file >> dirID;
            getline(file, line);
            getline(file, name);
            getline(file, date);
            RegularFile newFile; // create a new regular file object
            newFile.setName(name);
            newFile.setDate(date);
            while (getline(file, line) && line != "ENDFILE")
            {
                for (const auto &ch : line)
                {
                    newFile.setContent(ch); // set the content
                }
            }
            loadAddFile(newFile, dirID); // add the file to given dirID
        }
        else if (line == "ENDOFTHESAVEFILE")
        {
            double byte;
            file >> byte;
            RegularFile::setTotalBytes(byte);
        }
        else if (line == "ENDsOFDIRECTORY" || line == "ENDFILE")
            continue;
    }
}
// add file function for loading the run operation
void Shell::loadAddFile(RegularFile &newFile, const int &dirID)
{
    std::shared_ptr<Directory> dirPtr = findDirectory(rootDir, dirID); // find the directory with given ID
    dirPtr->getFiles().push_back(newFile);                             // push back the file to the directory
    std::ofstream load("savefile.txt", std::ios::app);                 // save the file to the savefile.txt again.
    if (!load.is_open())
        return;
    load << "NEWFILE" << std::endl;
    load << dirID << std::endl
         << newFile.getName() << std::endl
         << newFile.getDate() << std::endl;
    for (const auto &ch : newFile.getContent())
        load << ch;
    load << std::endl
         << "ENDFILE" << std::endl;
    load.close();
}
// find the directory recursively and return a pointer
std::shared_ptr<Directory> Shell::findDirectory(const std::shared_ptr<Directory> &root, const int &dirID) const
{
    if (root == nullptr)
        return nullptr;

    if (root->getId() == dirID)
        return root;

    auto foundInSub = findDirectory(root->getSub(), dirID);
    if (foundInSub != nullptr)
        return foundInSub;

    return findDirectory(root->getNext(), dirID);
}
// add directory function for loading the run operations, it's same as mkdir
void Shell::loadAddDir(const std::string &dirName, const std::string &dirDate,
                       const int &dirID, const std::shared_ptr<Directory> &curr)
{
    std::shared_ptr<Directory> iter = curr->getSub(); // iterator for finding the right directory

    while (iter != nullptr)
    {
        if (iter->getName() == dirName)
        {
            std::cerr << "There is already a directory named " << dirName << std::endl;
            return;
        }
        iter = iter->getNext();
    }
    std::shared_ptr<Directory> newDir = std::make_shared<Directory>(dirName);

    // if current directory is main
    if (curr->getName() == "main")
    {
        if (rootDir->getSub() == nullptr)
        {
            rootDir->setSub(newDir);
            newDir->setPrev(rootDir);
        }
        else
        {
            iter = rootDir->getSub();
            while (iter->getNext() != nullptr)
                iter = iter->getNext();

            iter->setNext(newDir);
            newDir->setPrev(iter);
        }
    }
    // if current directory is not main
    else
    {
        if (curr->getSub() == nullptr)
        {
            curr->setSub(newDir);
            newDir->setPrev(curr);
        }
        else
        {
            iter = curr->getSub();
            while (iter->getNext() != nullptr)
                iter = iter->getNext();

            iter->setNext(newDir);
            newDir->setPrev(iter);
        }
    }
    newDir->setNext(nullptr);
    newDir->setSub(nullptr);
    newDir->setDate(dirDate);
    newDir->setId(dirID);
    std::ofstream file("savefile.txt", std::ios::app); // save the directory
    if (!file.is_open())
        return;
    file << "NEWDIRECTORY" << std::endl;
    file << curr->getId() << std::endl
         << newDir->getId() << std::endl
         << newDir->getName() << std::endl
         << newDir->getDate() << std::endl
         << "ENDOFDIRECTORY" << std::endl;
    file.close();
}
// recreate the file after remove a directory
void Shell::removeDirFromTxt(const int &dirID)
{
    std::ifstream inputFile("savefile.txt");
    std::ofstream tempFile("temp.txt");
    std::string line;
    int parentID, currentDirID;
    std::string name, date;

    if (!inputFile.is_open() || !tempFile.is_open())
    {
        std::cerr << "Error opening files!" << std::endl;
        return;
    }

    // delete both files and directory with given id
    while (getline(inputFile, line))
    {
        if (line == "NEWDIRECTORY")
        {
            inputFile >> parentID >> currentDirID;
            getline(inputFile, line);
            if (currentDirID == dirID)
            {
                getline(inputFile, line);
                getline(inputFile, line);
                getline(inputFile, line); // enddirectory
            }
            else
            {
                getline(inputFile, name);
                getline(inputFile, date);
                getline(inputFile, line); // enddirectory

                tempFile << "NEWDIRECTORY" << std::endl
                         << parentID << std::endl
                         << currentDirID << std::endl
                         << name << std::endl
                         << date << std::endl
                         << "ENDOFDIRECTORY" << std::endl;
            }
        }

        else if (line == "NEWFILE")
        {
            inputFile >> currentDirID;
            getline(inputFile, line);
            getline(inputFile, name);
            getline(inputFile, date);
            if (dirID == currentDirID)
            {
                while (line != "ENDFILE")
                    getline(inputFile, line); // content
                getline(inputFile, line);
            }
            else
            {
                tempFile << "NEWFILE" << std::endl
                         << currentDirID << std::endl
                         << name << std::endl
                         << date << std::endl;
                while (line != "ENDFILE")
                {
                    getline(inputFile, line);
                    tempFile << line;
                }
                tempFile << "ENDFILE" << std::endl;
            }
        }
    }

    inputFile.close();
    tempFile.close();

    remove("savefile.txt");
    rename("temp.txt", "savefile.txt");
}
// recreate the txt file after remove a text

void Shell::removeFileFromTxt(const int &dirID)
{
    std::ifstream inputFile("savefile.txt");
    std::ofstream tempFile("temp.txt");
    std::string line;
    bool skipSection = false;

    if (!inputFile.is_open() || !tempFile.is_open())
    {
        std::cerr << "Error opening files!" << std::endl;
        return;
    }
    // delete the files with given directory id
    while (getline(inputFile, line))
    {
        if (line == "NEWFILE")
        {
            int currentDirID;
            inputFile >> currentDirID;
            if (currentDirID == dirID)
            {
                skipSection = true;
                getline(inputFile, line);
                getline(inputFile, line);
                while (getline(inputFile, line) && line != "ENDFILE")
                {
                    // skip file content
                }
            }
            else
            {
                tempFile << "NEWFILE" << std::endl;
                tempFile << currentDirID << std::endl;
            }
        }
        else if (line == "ENDFILE" && skipSection)
        {
            skipSection = false;
        }
        else if (!skipSection)
        {
            tempFile << line << std::endl; // write other lines
        }
    }

    inputFile.close();
    tempFile.close();

    remove("savefile.txt");
    rename("temp.txt", "savefile.txt");
}

// mkdir function to create a new directory in current directory
/*
    the logic is a little bit complicated. i choose the hard way...
    i've main directory at first. it has 3 pointer, sub, next and prev.
    when a directory is created in the main directory sub points to new directory and next and prev are nullptr.
    after creating the first sub directory i add new sub directories to the this directory's next pointer.
    it's like double linked list but it has one more pointer named sub.
*/
void Shell::mkdir(const std::string &dirName)
{
    std::shared_ptr<Directory> iter = currentDir->getSub();

    // check if there is already a directory with given name
    while (iter != nullptr)
    {
        if (iter->getName() == dirName)
        {
            std::cerr << "There are already a directory named " << dirName << std::endl;
            return;
        }
        iter = iter->getNext();
    }
    // if there are no directory with given name create a new directory object
    std::shared_ptr<Directory> newDir = std::make_shared<Directory>(dirName);

    // if current directory is main
    if (currentDir->getName() == "main")
    {
        // if it's the first directory
        if (rootDir->getSub() == nullptr)
        {
            rootDir->setSub(newDir);
            newDir->setPrev(rootDir);
        }
        // if it's not the first directory set iter to next while next!=nullptr and add the directory to the next
        else
        {
            iter = rootDir->getSub();
            while (iter->getNext() != nullptr)
                iter = iter->getNext();

            iter->setNext(newDir);
            newDir->setPrev(iter);
        }
    }
    // if current directory is not main
    else
    {
        // if it's the first directory of given parent
        if (currentDir->getSub() == nullptr)
        {
            currentDir->setSub(newDir);
            newDir->setPrev(currentDir);
        }
        // if it's not the first directory of given parent
        else
        {
            iter = currentDir->getSub();
            while (iter->getNext() != nullptr)
                iter = iter->getNext();

            iter->setNext(newDir);
            newDir->setPrev(iter);
        }
    }
    // set next and sub nullptr
    newDir->setNext(nullptr);
    newDir->setSub(nullptr);

    // set the date with current date
    std::time_t currentTime = std::time(nullptr);
    std::tm *localTime = std::localtime(&currentTime);
    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "%b %d %H:%M", localTime);
    newDir->setDate(std::string(buffer));

    // write the new directory to the end of the savefile.txt
    std::ofstream file("savefile.txt", std::ios::app);
    if (!file.is_open())
        return;
    file << "NEWDIRECTORY" << std::endl;
    file << currentDir->getId() << std::endl
         << newDir->getId() << std::endl
         << newDir->getName() << std::endl
         << newDir->getDate() << std::endl
         << "ENDOFDIRECTORY" << std::endl;
    file.close(); // close the file
}

// change directory function
void Shell::cd(const std::string &dirName)
{
    // if user wants to change the directory to the parent directory
    std::shared_ptr<Directory> iter = currentDir;
    if (dirName == "..")
    {
        if (iter->getName() == "main")
            std::cerr << "You are in the main directory" << std::endl;
        else
            currentDir = currentDir->getParent(currentDir);
        return;
    }

    // stay in the same directory
    else if (dirName == ".")
        return;
    // if user enter the directory with using /
    else if (dirName.find('/') != std::string::npos)
    {
        std::istringstream iss(dirName);
        std::string part;
        std::shared_ptr<Directory> tempDir = currentDir;

        while (getline(iss, part, '/'))
        {
            bool found = false;
            for (std::shared_ptr<Directory> iter = tempDir->getSub(); iter != nullptr; iter = iter->getNext())
            {
                if (iter->getName() == part)
                {
                    tempDir = iter;
                    found = true;
                    break;
                }
            }
            if (!found)
            {
                std::cerr << "No such directory: " << part << std::endl;
                return;
            }
        }
        currentDir = tempDir;
        return;
    }
    // if user wants to change the directory to the sub directory
    else
    {
        std::shared_ptr<Directory> subIter = iter->getSub();
        while (subIter != nullptr)
        {
            if (subIter->getName() == dirName)
            {
                currentDir = subIter;
                return;
            }
            subIter = subIter->getNext();
        }
        std::cerr << "There are no such directory." << std::endl;
    }
}

// list the directories and files.
// if it's ls -R call the recursive ls function
void Shell::ls(const bool &recursive) const
{
    // check if the directory is empty
    if (currentDir->getSub() == nullptr && currentDir->getFiles().empty())
    {
        std::cout << "There are no directories or files in " << currentDir->getName() << std::endl;
        return;
    }
    // call the recursive ls function if recursive is true
    if (recursive)
        lsRecursive(currentDir, 0); // 0 is the level for writing the sub directories properly
    // list the all files and sub directories of the directory
    else
    {
        std::shared_ptr<Directory> iter = currentDir->getSub();
        while (iter != nullptr)
        {
            std::cout << "D "
                      << std::left << std::setw(10) << iter->getName()
                      << std::right << std::setw(15) << iter->getDate()
                      << std::endl;
            iter = iter->getNext();
        }
        if (!currentDir->getFiles().empty())
        {
            for (const auto &file : currentDir->getFiles())
            {
                std::cout << "F " << std::left << std::setw(10) << file.getName()
                          << std::right << std::setw(15) << file.getDate()
                          << std::string(4, ' ') << file.getBytes() << "Bytes" << std::endl;
            }
        }
    }
}
// recursive ls functions
void Shell::lsRecursive(std::shared_ptr<Directory> curDir, int level) const
{
    std::shared_ptr<Directory> iter = curDir->getSub();
    while (iter != nullptr)
    {
        for (int i = 0; i < level * 2; i++)
        {
            std::cout << " ";
        }
        std::cout << "D " << iter->getName() << std::string(15 - level * 2, ' ') << iter->getDate() << std::endl;

        // if there are sub directories, call the function again
        lsRecursive(iter, level + 1);

        iter = iter->getNext();
    }
    for (const auto &file : curDir->getFiles())
    {
        for (int i = 0; i < level * 2; i++)
        {
            std::cout << " ";
        }
        std::cout << "F " << file.getName() << std::string(8, ' ') << file.getDate()
                  << std::string(8, ' ') << file.getBytes() << "Bytes" << std::endl;
    }
}

// cp function
void Shell::cp(const std::string &path)
{
    std::ifstream file(path);
    if (file.is_open())
    {
        file.close();
        RegularFile newFile;                               // create a new file if the file is open
        newFile.loadContent(path);                         // call load content function
        currentDir->addFile(newFile, currentDir->getId()); // add the copied file to the current directory
    }
    else
        std::cerr << "Couldn't open the file at " << path << std::endl;
}

// cat function
void Shell::cat(const std::string &name) const
{
    bool fileFound = false;

    // check if the file is in the current directory if so print the content of file
    for (const auto &f : currentDir->getFiles())
    {
        if (f.getName() == name)
        {
            fileFound = true;
            for (const auto &ch : f.getContent())
                std::cout << ch;
            std::cout << std::endl;
            break;
        }
    }
    // if file couldn't found
    if (!fileFound)
        std::cerr << "No such file in this directory." << std::endl;
}
// remove directory of file with given name
void Shell::rm(const std::string &name)
{
    // if user try to remove the main
    if (name == "main")
    {
        std::cerr << "You can't remove the main directory." << std::endl;
        return;
    }
    // check if there are any files or directories can be removed
    else if (currentDir->getSub() == nullptr && currentDir->getFiles().empty())
    {
        std::cerr << "There are no directories or files in " << currentDir->getName() << std::endl;
        return;
    }
    // check if there are directory with given name
    else if (currentDir->getSub() != nullptr)
    {
        std::shared_ptr<Directory> iter = currentDir->getSub();
        while (iter->getName() != name)
            iter = iter->getNext();
        if (iter == nullptr)
            std::cerr << "There are no such directory named" << name << std::endl;
    }
    std::shared_ptr<Directory> iter = currentDir->getSub();
    std::shared_ptr<Directory> prev = nullptr;
    // check and remove sub directories recursively
    while (iter != nullptr)
    {
        if (iter->getName() == name)
        {
            removeDirFromTxt(iter->getId());
            // change current directory for temporary
            std::shared_ptr<Directory> tempDir = currentDir;
            currentDir = iter;

            // recursively remove all sub directories and files
            std::shared_ptr<Directory> subIter = currentDir->getSub();
            while (subIter != nullptr)
            {
                // call rm function with changed currentDir
                rm(subIter->getName());
                subIter = subIter->getNext();
            }

            // remove all files (file names are not important) in the current directory
            currentDir->getFiles().clear();

            // restore the original current directory
            currentDir = tempDir;

            // remove the directory from the parent directory's sub list
            if (prev != nullptr)
                prev->setNext(iter->getNext());
            else
                currentDir->setSub(iter->getNext());
            if (iter->getNext() != nullptr)
                iter->getNext()->setPrev(prev);
            return;
        }
        prev = iter;
        iter = iter->getNext();
    }
    // remove file with given name
    int i = 0;
    for (const auto &file : currentDir->getFiles())
    {
        if (file.getName() == name)
        {
            currentDir->getFiles().erase(currentDir->getFiles().begin() + i);
            removeFileFromTxt(currentDir->getId());
            return;
        }
        ++i;
    }
    // if there are no file with given name
    std::cerr << "File named '" << name << "' not found." << std::endl;
}
