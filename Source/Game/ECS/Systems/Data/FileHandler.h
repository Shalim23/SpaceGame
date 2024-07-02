#pragma once
#include <fstream>

class FileHandler
{
public:
    FileHandler(std::string_view path);
    ~FileHandler();

    std::istream& read(char* buffer, size_t size);

private:
    std::ifstream file_;
};
