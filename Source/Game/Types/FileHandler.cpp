#include "FileHandler.h"
#include "../Utils.h"

FileHandler::FileHandler(std::string_view path)
    : file_{path.data(), std::ios::binary }
{
    if (!file_.is_open())
    {
        std::stringstream ss;
        ss << "Cannot open " << path;
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

std::istream& FileHandler::read(char* buffer, size_t size)
{
    return file_.read(buffer, size);
}
