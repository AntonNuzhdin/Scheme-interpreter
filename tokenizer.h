#pragma once

#include <variant>
#include <optional>
#include <istream>
#include <vector>
#include <set>
#include <unordered_map>

struct BoolToken {
    bool boolean;
    bool operator==(const BoolToken& other) const;
};

struct SymbolToken {
    std::string name;
    bool operator==(const SymbolToken& other) const;
};

struct QuoteToken {
    bool operator==(const QuoteToken&) const;
};

struct DotToken {
    bool operator==(const DotToken&) const;
};

enum class BracketToken { OPEN, CLOSE };

struct ConstantToken {
    int value;

    bool operator==(const ConstantToken& other) const;
};

using Token =
    std::variant<ConstantToken, BracketToken, SymbolToken, QuoteToken, DotToken, BoolToken>;

class Tokenizer {
public:
    Tokenizer(std::istream* in);

    bool IsEnd();

    void Next();

    Token GetToken() {
        return cur_;
    }

private:
    std::set<char> begin_ = {'<', '>', '=', '*', '/', '#'};
    std::set<char> special_tokens_ = {'(', ')', '.', '\'', '+', '-'};
    Token cur_;
    std::istream* in_stream_;
    bool is_end_ = false;

private:
    void GoToValid();
    int GetDigit(int c);
    bool CheckCorrectnessOfSymbolBegin(int c);
    bool CheckCorrectnessOfSymbolContains(int c);
    std::string GetSymbols(int& c);
};