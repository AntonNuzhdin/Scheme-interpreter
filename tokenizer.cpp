#include <tokenizer.h>
#include "error.h"

Tokenizer::Tokenizer(std::istream *in) {
    in_stream_ = in;
    Next();
}

void Tokenizer::GoToValid() {
    while (isspace(in_stream_->peek())) {
        in_stream_->get();
    }
    if (in_stream_->peek() == EOF) {
        is_end_ = true;
        return;
    }
}

int Tokenizer::GetDigit(int c) {
    int digit = c - '0';
    c = in_stream_->peek();
    while (c <= '9' && c >= '0') {
        in_stream_->get();
        digit *= 10;
        digit += c - '0';
        c = in_stream_->peek();
    }
    return digit;
}

bool Tokenizer::CheckCorrectnessOfSymbolBegin(int c) {
    return isalpha(static_cast<char>(c)) || begin_.contains(c);
}

bool Tokenizer::CheckCorrectnessOfSymbolContains(int c) {
    return isalpha(static_cast<char>(c)) || begin_.contains(c) || isdigit(static_cast<char>(c)) ||
           c == '?' || c == '!' || c == '-';
}

std::string Tokenizer::GetSymbols(int &c) {
    std::string token;
    token += c;
    c = in_stream_->peek();
    while (!isspace(c) && c != EOF) {
        if (c == ')') {
            break;
        }
        in_stream_->get();

        if (CheckCorrectnessOfSymbolContains(c)) {
            token += c;
            c = in_stream_->peek();

        } else {
            throw SyntaxError("Error");
        }
    }
    return token;
}

void Tokenizer::Next() {
    GoToValid();
    if (is_end_) {
        return;
    }
    int c = in_stream_->peek();
    while (!isspace(c) && c != EOF) {
        in_stream_->get();
        if (isdigit(static_cast<char>(c))) {
            cur_ = ConstantToken{GetDigit(c)};
            return;
        } else if (c == '-' && in_stream_->peek() <= '9' && in_stream_->peek() >= '0') {
            c = in_stream_->peek();
            in_stream_->get();
            cur_ = ConstantToken{-GetDigit(c)};
            return;
        } else if (c == '+' && in_stream_->peek() <= '9' && in_stream_->peek() >= '0') {
            c = in_stream_->peek();
            in_stream_->get();
            cur_ = ConstantToken{GetDigit(c)};
            return;
        } else if (c == '#' && (in_stream_->peek() == 't' || in_stream_->peek() == 'f')) {
            cur_ = BoolToken{(in_stream_->peek() == 't')};
            in_stream_->get();
            return;
        } else if (special_tokens_.contains(static_cast<char>(c))) {
            if (c == '.') {
                cur_ = DotToken{};
            } else if (c == '(') {
                cur_ = BracketToken::OPEN;
            } else if (c == ')') {
                cur_ = BracketToken::CLOSE;
            } else if (c == '\'') {
                cur_ = QuoteToken{};
            } else if (c == '+') {
                cur_ = SymbolToken{"+"};
            } else if (c == '-') {
                cur_ = SymbolToken{"-"};
            }
            return;
        } else if (CheckCorrectnessOfSymbolBegin(c)) {
            cur_ = SymbolToken{GetSymbols(c)};
            return;
        } else {
            throw SyntaxError("Error");
        }
    }
}
bool Tokenizer::IsEnd() {
    return is_end_;
}
bool ConstantToken::operator==(const ConstantToken &other) const {
    return value == other.value;
}
bool DotToken::operator==(const DotToken &) const {
    return true;
}
bool SymbolToken::operator==(const SymbolToken &other) const {
    return name == other.name;
}
bool QuoteToken::operator==(const QuoteToken &) const {
    return true;
}
bool BoolToken::operator==(const BoolToken &other) const {
    return boolean == other.boolean;
}
