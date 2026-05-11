#pragma once

#include <stdexcept>
#include <string>

// Базовое исключение для всех ошибок структур данных
class DataStructureException : public std::runtime_error
{
public:
    explicit DataStructureException(const std::string &message)
        : std::runtime_error(message) {}
};

// Исключение: выход за границы последовательности
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

// Исключение: пустая структура (вызов GetFirst/GetLast у пустой последовательности)
class EmptyStructureException : public DataStructureException
{
public:
    explicit EmptyStructureException(const std::string &context)
        : DataStructureException("Ошибка в " + context + ": последовательность пуста") {}
};

// Исключение: недопустимый аргумент (отрицательный размер, нулевой указатель и т.д.)
class InvalidArgument : public DataStructureException
{
public:
    explicit InvalidArgument(const std::string &message)
        : DataStructureException(message) {}
};