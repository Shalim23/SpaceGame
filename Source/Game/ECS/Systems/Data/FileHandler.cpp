#include "FileHandler.h"
#include "../../../Utils.h"

FileHandler::FileHandler(std::string_view path)
{
    file_.open(path.data(), std::fstream::in | std::fstream::out | std::ios::binary);
    if (!file_.is_open())
    {
        std::stringstream ss;
        ss << "Cannot open " << path << "!";
        utils::showMessageBox("File open error!", ss.str());
        throw std::exception{};
    }
}

FileHandler::~FileHandler()
{
    if (file_.is_open())
    {
        file_.close();
    }
}

void FileHandler::setPosition(const std::streampos position)
{
    file_.seekg(position);
    if (!file_)
    {
        std::stringstream ss;
        ss << "Failed to set position " << position << "!";
        utils::showMessageBox("File error!", ss.str());
        throw std::exception{};
    }
}

std::istream& FileHandler::read(char* buffer, size_t size)
{
    return file_.read(buffer, size);
}

std::ostream& FileHandler::write(char* buffer, size_t size)
{
    return file_.write(buffer, size);
}

std::vector<char> FileHandler::readAllFile()
{
    std::vector<char> fontRawData(std::istreambuf_iterator<char>(file_), {});
    return fontRawData;
}
