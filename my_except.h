#ifndef LAB2_MY_STDEXCEPT_H
#define LAB2_MY_STDEXCEPT_H

#include <exception>
#include <string>
//переименовать
class ParentForErrors: public std::exception {
private:
    std::string message;

protected:
    explicit ParentForErrors(const char *msg): message(msg) {}

public:
    const char* what() const noexcept override {
        return message.c_str();
    }
};
//правило ягни - вам никогда не понадобиться))

class InvalidArgument : public ParentForErrors {
public:
    explicit InvalidArgument(const char *Name) :
    ParentForErrors((std::string("неправильный аргумент:") + Name).c_str()) {}
    explicit InvalidArgument(const char *Name1, const char *Name2 ) :
    ParentForErrors((std::string("неправильные аргументы:") + Name1 + std::string(", ")
        + Name2).c_str()) {}
};

class OutOfRange : public ParentForErrors {
public:
    // для случая "список пуст"
    explicit OutOfRange(const char* msg)
        : ParentForErrors(msg) {}

    explicit OutOfRange(const char* name, int value, int min, int max)
        : ParentForErrors(
            (std::string("значение ") + name + " = " + std::to_string(value) +
             " вне диапазона [" + std::to_string(min) + ", " + std::to_string(max) + "]").c_str()) {}
};



// достигнут конец потока — дальнейшее чтение невозможно
class EndOfStream : public ParentForErrors {
public:
    EndOfStream() : ParentForErrors("достигнут конец потока") {}
    explicit EndOfStream(const char* msg) : ParentForErrors(msg) {}
};

#endif //LAB2_MY_STDEXCEPT_H