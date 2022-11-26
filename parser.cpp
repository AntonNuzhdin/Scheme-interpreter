#include <parser.h>
#include <error.h>

#include <memory>

std::shared_ptr<Object> ReadRun(Tokenizer* tokenizer) {
    if (tokenizer->IsEnd()) {
        return nullptr;
    }
    auto current = tokenizer->GetToken();
    tokenizer->Next();
    if (std::holds_alternative<ConstantToken>(current)) {
        return std::make_shared<Number>(std::get<ConstantToken>(current).value);
    } else if (std::holds_alternative<SymbolToken>(current)) {
        if (std::get<SymbolToken>(current).name == "quote") {
            return std::make_shared<Symbol>("quote");
        }
        return std::make_shared<Symbol>(std::get<SymbolToken>(current).name);
    } else if (std::holds_alternative<QuoteToken>(current)) {
        return std::make_shared<Cell>(std::make_shared<Symbol>("quote"), Read(tokenizer));
    } else if (std::holds_alternative<BoolToken>(current)) {
        return std::make_shared<Boolean>(std::get<BoolToken>(current).boolean);
    } else if (std::holds_alternative<BracketToken>(current)) {
        if (std::get<BracketToken>(current) == BracketToken::CLOSE) {
            throw SyntaxError{"Error"};
        }
        auto tree = ReadList(tokenizer);
        current = tokenizer->GetToken();
        tokenizer->Next();
        return tree;
    }
    throw SyntaxError{"Invalid syntax"};
}

// ConstantToken, BracketToken, SymbolToken, QuoteToken, DotToken, BoolToken
std::shared_ptr<Object> Read(Tokenizer* tokenizer) {
    if (tokenizer->IsEnd()) {
        throw SyntaxError{"Invalid syntax"};
    }
    return ReadRun(tokenizer);
}

std::shared_ptr<Object> ReadList(Tokenizer* tokenizer) {
    Token current = tokenizer->GetToken();
    if (std::holds_alternative<BracketToken>(current) &&
        std::get<BracketToken>(current) == BracketToken::CLOSE) {  // end of recursion
        return nullptr;
    }

    if (tokenizer->IsEnd()) {
        throw SyntaxError{"Invalid syntax"};
    }
    bool is_pair = false;
    auto first_child = ReadRun(tokenizer);
    std::shared_ptr<Object> second_child = nullptr;
    current = tokenizer->GetToken();  //  here is the dot, bracket or number
    if (std::holds_alternative<DotToken>(current)) {
        tokenizer->Next();
        is_pair = true;
        if (tokenizer->IsEnd()) {
            throw SyntaxError{"Invalid syntax"};
        }
        second_child = ReadRun(tokenizer);
    } else {
        second_child = ReadList(tokenizer);
    }
    current = tokenizer->GetToken();
    if (!std::holds_alternative<BracketToken>(current) ||
        std::get<BracketToken>(current) != BracketToken::CLOSE || tokenizer->IsEnd()) {
        throw SyntaxError{"Invalid syntax"};
    }
    if (is_pair) {
        return std::make_shared<Cell>(first_child, second_child, is_pair);
    }
    return std::make_shared<Cell>(first_child, second_child);
}
