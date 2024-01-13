#ifndef FILE_H
#define FILE_H
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm> // for reverse function
#include <ctime>

class RegularFile
{
public:
    // regular file constructor
    RegularFile() {}
    // load the content of given file to the created file
    void loadContent(const std::string &filePath);
    // setters and getters
    std::vector<char> getContent() const { return content; }
    std::string getName() const { return filename; }
    void setName(const std::string &n) { filename = n; }
    void setDate(const std::string &d) { date = d; }
    std::string getDate() const { return date; }
    int getBytes() const { return content.size(); }
    void setContent(const char &ch) { content.push_back(ch); }
    static double getTotalBytes() { return totalByte; }
    static void setTotalBytes(const double &byte) { totalByte = byte; }

private:
    std::string filename;
    std::string date;
    std::vector<char> content;
    static double totalByte;
};

#endif