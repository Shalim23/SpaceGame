#pragma once
#include <fstream>
#include <vector>

class FileHandler
{

public:
    FileHandler(std::string_view path);
    ~FileHandler();

    void setPosition(const std::streampos position);
    std::istream& read(char* buffer, size_t size);
    std::ostream& write(char* buffer, size_t size);
    std::vector<char> readAllFile();

private:
    std::fstream file_;
};
