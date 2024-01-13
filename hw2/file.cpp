#include "file.h"
#include <ctime>
double RegularFile::totalByte = 0;
// load the contents from source
void RegularFile::loadContent(const std::string &filePath)
{
    std::ifstream file(filePath);

    if (file.is_open())
    {
        // load the content of source file to created file
        char ch;
        while (file.get(ch))
        {
            content.push_back(ch);
        }
        file.close();

        // setting the file name with extension
        int i = filePath.length() - 1;
        while (i >= 0 && filePath[i] != '/')
        {
            filename.push_back(filePath[i]);
            --i;
        }
        reverse(filename.begin(), filename.end());

        // setting the date
        std::time_t currentTime = std::time(nullptr);
        std::tm *localTime = std::localtime(&currentTime);
        char buffer[80];
        std::strftime(buffer, sizeof(buffer), "%b %d %H:%M", localTime);
        this->setDate(std::string(buffer));
        totalByte += getBytes();
    }
    else
    {
        std::cerr << "Couldn't open the file at " << filePath << std::endl;
        return;
    }
}