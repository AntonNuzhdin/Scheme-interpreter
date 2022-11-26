#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <map>
#include <string>
#include <vector>
#include "error.h"

class Object : public std::enable_shared_from_this<Object> {
public:
    virtual std::shared_ptr<Object> Eval() {
    }
    virtual std::string Serialize() {
    }
    virtual ~Object() = default;
};

class Function : public Object {
public:
    virtual std::shared_ptr<Object> Apply(std::vector<std::shared_ptr<Object>>&) {
    }
};

// Runtime type checking and convertion.
// This can be helpful: https://en.cppreference.com/w/cpp/memory/shared_ptr/pointer_cast

template <class T>
std::shared_ptr<T> As(const std::shared_ptr<Object>& obj) {
    return std::dynamic_pointer_cast<T>(obj);
}

template <class T>
bool Is(const std::shared_ptr<Object>& obj) {
    auto as = As<T>(obj);
    return as != nullptr;
}

///////////////////////////////////////////////////////////////////////////////

void GenerateVectorOfArgs(std::vector<std::shared_ptr<Object>>& args, std::shared_ptr<Object> cell);

void GenerateVectorOfArgsNoExcept(std::vector<std::shared_ptr<Object>>& args,
                                  std::shared_ptr<Object> cell);

class CheckIsNumber : public Function {
    std::shared_ptr<Object> Apply(std::vector<std::shared_ptr<Object>>& args) override;
};

class Equal : public Function {
    std::shared_ptr<Object> Apply(std::vector<std::shared_ptr<Object>>& args) override;
};

class More : public Function {
    std::shared_ptr<Object> Apply(std::vector<std::shared_ptr<Object>>& args) override;
};

class Less : public Function {
    std::shared_ptr<Object> Apply(std::vector<std::shared_ptr<Object>>& args) override;
};

class MoreOrEqual : public Function {
    std::shared_ptr<Object> Apply(std::vector<std::shared_ptr<Object>>& args) override;
};

class LessOrEqual : public Function {
    std::shared_ptr<Object> Apply(std::vector<std::shared_ptr<Object>>& args) override;
};

class Sum : public Function {
    std::shared_ptr<Object> Apply(std::vector<std::shared_ptr<Object>>& args) override;
};

class Sub : public Function {
    std::shared_ptr<Object> Apply(std::vector<std::shared_ptr<Object>>& args) override;
};

class Multiply : public Function {
    std::shared_ptr<Object> Apply(std::vector<std::shared_ptr<Object>>& args) override;
};

class Divide : public Function {
    std::shared_ptr<Object> Apply(std::vector<std::shared_ptr<Object>>& args) override;
};

class Max : public Function {
    std::shared_ptr<Object> Apply(std::vector<std::shared_ptr<Object>>& args) override;
};

class Min : public Function {
    std::shared_ptr<Object> Apply(std::vector<std::shared_ptr<Object>>& args) override;
};

class Abs : public Function {
    std::shared_ptr<Object> Apply(std::vector<std::shared_ptr<Object>>& args) override;
};

class Quote : public Function {
public:
    std::shared_ptr<Object> Apply(std::vector<std::shared_ptr<Object>>& args) override;
};

class CheckIsPair : public Function {
    std::shared_ptr<Object> Apply(std::vector<std::shared_ptr<Object>>& args) override;
};

class CheckIsNull : public Function {
    std::shared_ptr<Object> Apply(std::vector<std::shared_ptr<Object>>& args) override;
};

class CheckIsList : public Function {
    std::shared_ptr<Object> Apply(std::vector<std::shared_ptr<Object>>& args) override;
};

class Cons : public Function {
    std::shared_ptr<Object> Apply(std::vector<std::shared_ptr<Object>>& args) override;
};

class Car : public Function {
    std::shared_ptr<Object> Apply(std::vector<std::shared_ptr<Object>>& args) override;
};

class Cdr : public Function {
    std::shared_ptr<Object> Apply(std::vector<std::shared_ptr<Object>>& args) override;
};

class List : public Function {
    std::shared_ptr<Object> Apply(std::vector<std::shared_ptr<Object>>& args) override;
};

class CheckIsBool : public Function {
    std::shared_ptr<Object> Apply(std::vector<std::shared_ptr<Object>>& args) override;
};

class Not : public Function {
    std::shared_ptr<Object> Apply(std::vector<std::shared_ptr<Object>>& args) override;
};

class ListRef : public Function {
    std::shared_ptr<Object> Apply(std::vector<std::shared_ptr<Object>>& args) override;
};

class ListTail : public Function {
    std::shared_ptr<Object> Apply(std::vector<std::shared_ptr<Object>>& args) override;
};

class And : public Function {
    std::shared_ptr<Object> Apply(std::vector<std::shared_ptr<Object>>& args) override;
};

class Or : public Function {
    std::shared_ptr<Object> Apply(std::vector<std::shared_ptr<Object>>& args) override;
};

class Number : public Object {
public:
    explicit Number(int64_t number) : number_(number) {
    }

    std::shared_ptr<Object> Eval() override;

    std::string Serialize() override;

    int GetValue() const {
        return number_;
    }

private:
    int64_t number_;
};

class Symbol : public Object {
public:
    explicit Symbol(const std::string& str) : str_(str) {
    }
    std::shared_ptr<Object> Eval() override;

    std::string Serialize() override {
        return str_;
    }
    const std::string& GetName() const {
        return str_;
    }

private:
    std::unordered_map<std::string, std::shared_ptr<Function>> command_to_function_ = {
        {"number?", std::make_shared<CheckIsNumber>()}};

    std::string str_;
};

class Boolean : public Object {
public:
    explicit Boolean(const bool value) : value_(value) {
    }
    std::shared_ptr<Object> Eval() override;

    std::string Serialize() override;

private:
    bool value_;
};

class Cell : public Object {
public:
    explicit Cell(std::shared_ptr<Object> first, std::shared_ptr<Object> second,
                  bool is_pair = false)
        : first_child_(first), second_child_(second), is_pair_(is_pair) {
    }
    std::shared_ptr<Object> Eval() override {
        throw RuntimeError{"Error"};
    }
    std::string Serialize() override {
        std::string token;
        token += '(';
        SerializeInside(shared_from_this(), token);
        token += ')';
        return token;
    }

    std::string SerializeHelper() {
        std::string token;
        SerializeInside(shared_from_this(), token);
        return token;
    }

    std::shared_ptr<Object> GetFirst() const;

    std::shared_ptr<Object> GetSecond() const {
        return second_child_;
    }

    std::shared_ptr<Object> GetFunction() const;

    bool IsPair() const {
        return is_pair_;
    }

private:
    void SerializeInside(std::shared_ptr<Object> cell, std::string& token) {
        if (Is<Cell>(As<Cell>(cell)->GetFirst())) {
            if (!token.empty() && token.back() != '(') {
                token += ' ';
            }
            token += As<Cell>(As<Cell>(cell)->GetFirst())->SerializeHelper();
            if (As<Cell>(As<Cell>(cell)->GetFirst())->IsPair()) {
                token += " . ";
            }
        } else {
            if (!token.empty() && token.back() != '(') {
                token += ' ';
            }
            if (!As<Cell>(cell)->GetFirst()) {
                token += "()";
            } else {
                token += As<Cell>(cell)->GetFirst()->Serialize();
            }
            //            token += As<Cell>(cell)->GetFirst()->Serialize();
        }
        if (Is<Cell>(As<Cell>(cell)->GetSecond())) {
            if (!token.empty() && token.back() != '(') {
                token += ' ';
            }
            token += As<Cell>(As<Cell>(cell)->GetSecond())->SerializeHelper();
            //            SerializeInside(As<Cell>(cell)->GetSecond(), token);
        } else {
            if (!Is<Cell>(cell)) {
                token += cell->Serialize();
            } else {
                if (!As<Cell>(cell)->GetSecond()) {
                    return;
                }
                if (!Is<Cell>(As<Cell>(cell)->GetSecond())) {
                    if (As<Cell>(cell)->IsPair()) {
                        token += " . ";
                    } else if (!token.empty() && token.back() != '(') {
                        token += ' ';
                    }

                    token += As<Cell>(cell)->GetSecond()->Serialize();
                    return;
                }
                token += As<Cell>(As<Cell>(cell)->GetSecond())->SerializeHelper();
            }
        }
    }

    std::map<std::string, std::shared_ptr<Function>> command_to_function_ = {
        {"number?", std::make_shared<CheckIsNumber>()},
        {"=", std::make_shared<Equal>()},
        {">", std::make_shared<More>()},
        {"<", std::make_shared<Less>()},
        {"<=", std::make_shared<LessOrEqual>()},
        {">=", std::make_shared<MoreOrEqual>()},
        {"+", std::make_shared<Sum>()},
        {"-", std::make_shared<Sub>()},
        {"*", std::make_shared<Multiply>()},
        {"/", std::make_shared<Divide>()},
        {"max", std::make_shared<Max>()},
        {"min", std::make_shared<Min>()},
        {"abs", std::make_shared<Abs>()},
        {"quote", std::make_shared<Quote>()},
        {"pair?", std::make_shared<CheckIsPair>()},
        {"null?", std::make_shared<CheckIsNull>()},
        {"list?", std::make_shared<CheckIsList>()},
        {"cons", std::make_shared<Cons>()},
        {"car", std::make_shared<Car>()},
        {"cdr", std::make_shared<Cdr>()},
        {"list", std::make_shared<List>()},
        {"boolean?", std::make_shared<CheckIsBool>()},
        {"not", std::make_shared<Not>()},
        {"list-ref", std::make_shared<ListRef>()},
        {"list-tail", std::make_shared<ListTail>()},
        {"and", std::make_shared<And>()},
        {"or", std::make_shared<Or>()}};
    bool is_pair_;
    std::shared_ptr<Object> first_child_;
    std::shared_ptr<Object> second_child_;
};
