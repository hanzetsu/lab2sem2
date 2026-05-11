#pragma once

#include <stdexcept>
#include <string>

class DataStructureException : public std::runtime_error
{
public:
    explicit DataStructureException(const std::string &message)
        : std::runtime_error(message) {}
};

class IndexOutOfRange : public DataStructureException
{
public:
    IndexOutOfRange(std::size_t index, std::size_t size, const std::string &context)
        : DataStructureException(buildMessage(index, size, context)),
          m_index(index), m_size(size) {}

    std::size_t getIndex() const noexcept { return m_index; }
    std::size_t getSize() const noexcept { return m_size; }

private:
    static std::string buildMessage(std::size_t index, std::size_t size, const std::string &context)
    {
        if (size == 0)
        {
            return "Ошибка в " + context + ": индекс " + std::to_string(index) +
                   " выходит за границы пустой последовательности";
        }
        return "Ошибка в " + context + ": индекс " + std::to_string(index) +
               " вне допустимого диапазона [0, " + std::to_string(size - 1) + "]";
    }

    std::size_t m_index;
    std::size_t m_size;
};

class EmptyStructureException : public DataStructureException
{
public:
    explicit EmptyStructureException(const std::string &context)
        : DataStructureException("Ошибка в " + context + ": последовательность пуста") {}
};

class InvalidArgument : public DataStructureException
{
public:
    explicit InvalidArgument(const std::string &message)
        : DataStructureException(message) {}
};