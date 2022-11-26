//
// Created by Anton Nuzhdin on 08.11.2022.
//
#include "object.h"
#include "error.h"
#include "../advanced/object.h"

void GenerateVectorOfArgs(std::vector<std::shared_ptr<Object>>& args,
                          std::shared_ptr<Object> cell) {
    if (!Is<Symbol>(cell) && !Is<Cell>(cell)) {
        throw RuntimeError{"Error"};
    }
    if (!Is<Symbol>(cell) && !Is<Cell>(As<Cell>(cell)->GetFirst())) {
        if (As<Cell>(cell)->IsPair()) {
            throw RuntimeError{"Error"};
        }
        args.push_back(As<Cell>(cell)->GetFirst());
    } else {
        if (Is<Symbol>(cell) && !Is<Quote>(cell)) {
            throw RuntimeError{"Err"};
        }
        auto cell2 = As<Cell>(As<Cell>(cell)->GetFirst());
        auto function = cell2->GetFunction();
        if (As<Cell>(cell2)->IsPair()) {
            throw RuntimeError{"Error"};
        }
        if (!function) {
            throw RuntimeError{"Error"};
        }
        std::vector<std::shared_ptr<Object>> args2;
        if (Is<Quote>(As<Function>(function))) {
            args.push_back(As<Cell>(cell2)->GetSecond());
        } else {
            if (As<Cell>(cell2)->GetSecond()) {
                GenerateVectorOfArgs(args2, As<Cell>(cell2)->GetSecond());
            }
        }
        auto evaled = As<Function>(function)->Apply(args2);
        args.push_back(evaled);
    }
    if (As<Cell>(cell)->GetSecond() == nullptr) {
        return;
    }
    GenerateVectorOfArgs(args, As<Cell>(cell)->GetSecond());
}

void GenerateVectorOfArgsNoExcept(std::vector<std::shared_ptr<Object>>& args,
                                  std::shared_ptr<Object> cell) {
    if (!Is<Symbol>(cell) && !Is<Cell>(As<Cell>(cell)->GetFirst())) {
        args.push_back(As<Cell>(cell)->GetFirst());
    } else {
        auto cell2 = As<Cell>(As<Cell>(cell)->GetFirst());
        auto function = cell2->GetFunction();
        if (As<Cell>(cell2)->IsPair()) {
            throw RuntimeError{"Error"};
        }
        if (!function) {
            return;
        }
        std::vector<std::shared_ptr<Object>> args2;
        if (Is<Quote>(As<Function>(function))) {
            args.push_back(As<Cell>(cell2)->GetSecond());
        } else {
            if (As<Cell>(cell2)->GetSecond()) {
                GenerateVectorOfArgs(args2, As<Cell>(cell2)->GetSecond());
            }
        }
        auto evaled = As<Function>(function)->Apply(args2);
        args.push_back(evaled);
    }
    if (As<Cell>(cell)->GetSecond() == nullptr) {
        return;
    }
    GenerateVectorOfArgsNoExcept(args, As<Cell>(cell)->GetSecond());
}

std::shared_ptr<Object> CheckIsNumber::Apply(std::vector<std::shared_ptr<Object>>& args) {
    bool checker = true;
    for (auto el : args) {
        checker *= Is<Number>(el);
    }
    return std::make_shared<Boolean>(checker);
}

std::shared_ptr<Object> Equal::Apply(std::vector<std::shared_ptr<Object>>& args) {
    bool checker = true;
    for (size_t i = 1; i < args.size(); ++i) {
        if (!Is<Number>(args[i - 1]) || !Is<Number>(args[i])) {
            throw RuntimeError{"Incorrect types"};
        }
        checker = args[i - 1]->Serialize() == args[i]->Serialize();
    }
    return std::make_shared<Boolean>(checker);
}

std::shared_ptr<Object> More::Apply(std::vector<std::shared_ptr<Object>>& args) {
    bool checker = true;
    for (size_t i = 1; i < args.size(); ++i) {
        if (!Is<Number>(args[i - 1]) || !Is<Number>(args[i])) {
            throw RuntimeError{"Incorrect types"};
        }
        checker = args[i - 1]->Serialize() > args[i]->Serialize();
    }
    return std::make_shared<Boolean>(checker);
}

std::shared_ptr<Object> Less::Apply(std::vector<std::shared_ptr<Object>>& args) {
    bool checker = true;
    for (size_t i = 1; i < args.size(); ++i) {
        if (!Is<Number>(args[i - 1]) || !Is<Number>(args[i])) {
            throw RuntimeError{"Incorrect types"};
        }
        checker = args[i - 1]->Serialize() < args[i]->Serialize();
    }
    return std::make_shared<Boolean>(checker);
}

std::shared_ptr<Object> MoreOrEqual::Apply(std::vector<std::shared_ptr<Object>>& args) {
    bool checker = true;
    for (size_t i = 1; i < args.size(); ++i) {
        if (!Is<Number>(args[i - 1]) || !Is<Number>(args[i])) {
            throw RuntimeError{"Incorrect types"};
        }
        checker = args[i - 1]->Serialize() >= args[i]->Serialize();
    }
    return std::make_shared<Boolean>(checker);
}

std::shared_ptr<Object> LessOrEqual::Apply(std::vector<std::shared_ptr<Object>>& args) {
    bool checker = true;
    for (size_t i = 1; i < args.size(); ++i) {
        if (!Is<Number>(args[i - 1]) || !Is<Number>(args[i])) {
            throw RuntimeError{"Incorrect types"};
        }
        checker = args[i - 1]->Serialize() <= args[i]->Serialize();
    }
    return std::make_shared<Boolean>(checker);
}

std::shared_ptr<Object> Sum::Apply(std::vector<std::shared_ptr<Object>>& args) {
    int64_t sum = 0;
    for (size_t i = 0; i < args.size(); ++i) {
        if (!Is<Number>(args[i])) {
            throw RuntimeError{"Incorrect types"};
        }
        sum += std::stoi(args[i]->Serialize());
    }
    return std::make_shared<Number>(sum);
}

std::shared_ptr<Object> Sub::Apply(std::vector<std::shared_ptr<Object>>& args) {
    if (args.empty()) {
        throw RuntimeError{"Incorrect args"};
    }
    if (!Is<Number>(args[0])) {
        throw RuntimeError{"Incorrect types"};
    }
    int64_t sub = std::stoi(args[0]->Serialize());
    for (size_t i = 1; i < args.size(); ++i) {
        if (!Is<Number>(args[i])) {
            throw RuntimeError{"Incorrect types"};
        }
        sub -= std::stoi(args[i]->Serialize());
    }
    return std::make_shared<Number>(sub);
}

std::shared_ptr<Object> Multiply::Apply(std::vector<std::shared_ptr<Object>>& args) {
    int64_t mul = 1;
    for (size_t i = 0; i < args.size(); ++i) {
        if (!Is<Number>(args[i])) {
            throw RuntimeError{"Incorrect types"};
        }
        mul *= std::stoi(args[i]->Serialize());
    }
    return std::make_shared<Number>(mul);
}

std::shared_ptr<Object> Divide::Apply(std::vector<std::shared_ptr<Object>>& args) {
    if (args.empty()) {
        throw RuntimeError{"Error"};
    }
    int64_t sub = std::stoi(args[0]->Serialize());
    for (size_t i = 1; i < args.size(); ++i) {
        if (!Is<Number>(args[i])) {
            throw RuntimeError{"Incorrect types"};
        }
        sub /= std::stoi(args[i]->Serialize());
    }
    return std::make_shared<Number>(sub);
}

std::shared_ptr<Object> Max::Apply(std::vector<std::shared_ptr<Object>>& args) {
    if (args.empty()) {
        throw RuntimeError{"You have bo args"};
    }
    if (!Is<Number>(args[0])) {
        throw RuntimeError{"Invalid arguments"};
    }
    int mx = std::stoi(args[0]->Serialize());
    for (size_t i = 1; i < args.size(); ++i) {
        mx = std::max(mx, std::stoi(args[i]->Serialize()));
    }
    return std::make_shared<Number>(mx);
}

std::shared_ptr<Object> Min::Apply(std::vector<std::shared_ptr<Object>>& args) {
    if (args.empty()) {
        throw RuntimeError{"You have bo args"};
    }
    if (!Is<Number>(args[0])) {
        throw RuntimeError{"Invalid arguments"};
    }
    int mn = std::stoi(args[0]->Serialize());
    for (size_t i = 1; i < args.size(); ++i) {
        mn = std::min(mn, std::stoi(args[i]->Serialize()));
    }
    return std::make_shared<Number>(mn);
}

std::shared_ptr<Object> Abs::Apply(std::vector<std::shared_ptr<Object>>& args) {
    if (args.size() != 1) {
        throw RuntimeError{"Invalid arguments"};
    }
    if (!Is<Number>(args[0])) {
        throw RuntimeError{"Invalid arguments"};
    }
    int abs = std::abs(std::stoi(args[0]->Serialize()));
    return std::make_shared<Number>(abs);
}

std::shared_ptr<Object> Quote::Apply(std::vector<std::shared_ptr<Object>>& args) {
    std::string token;
    for (auto x : args) {
        if (!x) {
            return std::make_shared<Symbol>("()");
        }
        token += x->Serialize();
    }
    return std::make_shared<Symbol>(token);
}

std::shared_ptr<Object> CheckIsPair::Apply(std::vector<std::shared_ptr<Object>>& args) {
    if (!args[0]) {
        return std::make_shared<Boolean>(false);
    }
    if (As<Cell>(args[0])->IsPair()) {
        return std::make_shared<Boolean>(As<Cell>(args[0])->IsPair());
    }
    std::vector<std::shared_ptr<Object>> args2;
    GenerateVectorOfArgs(args2, args[0]);
    return std::make_shared<Boolean>(args2.size() == 2);
}

std::shared_ptr<Object> CheckIsNull::Apply(std::vector<std::shared_ptr<Object>>& args) {
    return std::make_shared<Boolean>(args[0] == nullptr);
}

std::shared_ptr<Object> CheckIsBool::Apply(std::vector<std::shared_ptr<Object>>& args) {
    return std::make_shared<Boolean>(Is<Boolean>(args[0]));
}

std::shared_ptr<Object> CheckIsList::Apply(std::vector<std::shared_ptr<Object>>& args) {
    if (args[0] == nullptr) {
        return std::make_shared<Boolean>(true);
    }
    auto str = args[0]->Serialize();
    bool is_list = true;
    for (auto c : str) {
        if (c == '.') {
            is_list = false;
        }
    }
    return std::make_shared<Boolean>(is_list);
}

std::shared_ptr<Object> Cons::Apply(std::vector<std::shared_ptr<Object>>& args) {
    std::string token = '(' + args[0]->Serialize() + " . " + args[1]->Serialize() + ')';

    return std::make_shared<Symbol>(token);
}

std::shared_ptr<Object> Car::Apply(std::vector<std::shared_ptr<Object>>& args) {
    if (!args[0]) {
        throw RuntimeError{"Error"};
    }
    return std::make_shared<Symbol>(As<Cell>(args[0])->GetFirst()->Serialize());
}

std::shared_ptr<Object> Cdr::Apply(std::vector<std::shared_ptr<Object>>& args) {
    if (!args[0]) {
        throw RuntimeError{"Error"};
    }
    if (As<Cell>(args[0])->GetSecond() == nullptr) {
        return std::make_shared<Symbol>("()");
    }
    std::vector<std::shared_ptr<Object>> args2;
    if (Is<Cell>(As<Cell>(args[0])->GetSecond())) {
        auto input_ast =
            std::make_shared<Cell>(std::make_shared<Symbol>("quote"),
                                   std::make_shared<Cell>(As<Cell>(args[0])->GetSecond(), nullptr));
        auto function = As<Cell>(input_ast)->GetFunction();
        if (!function) {
            throw RuntimeError{"Error"};
        }
        std::vector<std::shared_ptr<Object>> args_inside;
        if (Is<Quote>(As<Function>(function))) {
            args_inside.push_back(As<Cell>(input_ast)->GetSecond());
        } else {
            if (As<Cell>(input_ast)->GetSecond()) {
                GenerateVectorOfArgs(args_inside, As<Cell>(input_ast)->GetSecond());
            }
        }
        auto out = As<Function>(function)->Apply(args_inside);
        return out->Eval();
    }
    return std::make_shared<Symbol>(As<Cell>(args[0])->GetSecond()->Serialize());
}

std::shared_ptr<Object> List::Apply(std::vector<std::shared_ptr<Object>>& args) {
    if (args.empty()) {
        return std::make_shared<Symbol>("()");
    }
    std::string list = "(";
    for (auto x : args) {
        list += x->Serialize();
        list += ' ';
    }
    list.back() = ')';
    return std::make_shared<Symbol>(list);
}

std::shared_ptr<Object> ListRef::Apply(std::vector<std::shared_ptr<Object>>& args) {
    auto cell = As<Cell>(args[0]);
    int index = std::stoi(As<Number>(args[2])->Serialize());
    for (int i = 0; i < index; ++i) {
        if (As<Cell>(cell->GetSecond()) == nullptr) {
            throw RuntimeError{"Error"};
        }
        cell = As<Cell>(cell->GetSecond());
    }
    return cell->GetFirst();
}

std::shared_ptr<Object> ListTail::Apply(std::vector<std::shared_ptr<Object>>& args) {
    if (!args[0]) {
        throw RuntimeError{"Error"};
    }
    if (As<Cell>(args[0])->GetSecond() == nullptr) {
        return std::make_shared<Symbol>("()");
    }

    auto cell = As<Cell>(args[0]);
    int index = std::stoi(As<Number>(args[2])->Serialize());
    for (int i = 0; i < index; ++i) {
        if (As<Cell>(cell->GetSecond()) == nullptr) {
            if (index - 1 > i) {
                throw RuntimeError{"Error"};
            }
            return std::make_shared<Symbol>("()");
        }
        cell = As<Cell>(cell->GetSecond());
    }

    std::vector<std::shared_ptr<Object>> args2;
    if (Is<Cell>(As<Cell>(args[0])->GetSecond())) {
        auto input_ast = std::make_shared<Cell>(std::make_shared<Symbol>("quote"),
                                                std::make_shared<Cell>(cell, nullptr));
        auto function = As<Cell>(input_ast)->GetFunction();
        if (!function) {
            throw RuntimeError{"Error"};
        }
        std::vector<std::shared_ptr<Object>> args_inside;
        if (Is<Quote>(As<Function>(function))) {
            args_inside.push_back(As<Cell>(input_ast)->GetSecond());
        } else {
            if (As<Cell>(input_ast)->GetSecond()) {
                GenerateVectorOfArgs(args_inside, As<Cell>(input_ast)->GetSecond());
            }
        }
        auto out = As<Function>(function)->Apply(args_inside);
        return out->Eval();
    }
    return std::make_shared<Symbol>(As<Cell>(args[0])->GetSecond()->Serialize());
}

std::shared_ptr<Object> And::Apply(std::vector<std::shared_ptr<Object>>& args) {
    if (args.empty()) {
        return std::make_shared<Symbol>("#t");
    }
    std::string ans;
    for (auto x : args) {
        if (x->Serialize().empty()) {
            continue;
        }
        if (Is<Boolean>(x) && As<Boolean>(x)->Serialize() == "#f") {
            return std::make_shared<Symbol>("#f");
        } else {
            ans = x->Serialize();
        }
    }
    return std::make_shared<Symbol>(ans);
}

std::shared_ptr<Object> Or::Apply(std::vector<std::shared_ptr<Object>>& args) {
    if (args.empty()) {
        return std::make_shared<Symbol>("#f");
    }
    std::string ans;
    for (auto x : args) {
        if (x->Serialize().empty()) {
            continue;
        }
        if (Is<Boolean>(x) && As<Boolean>(x)->Serialize() == "#t") {
            return std::make_shared<Symbol>("#t");
        } else {
            ans = x->Serialize();
        }
    }
    return std::make_shared<Symbol>(ans);
}

std::shared_ptr<Object> Define::Apply(std::shared_ptr<Scope>& scope,
                                      std::vector<std::shared_ptr<Object>>& args) {
    auto name = As<Symbol>(args[0])->Serialize();
    auto value = args[1]->Eval();
    scope->Define(name, value);
    return value;
}

//-------------------------------------------------------------

std::shared_ptr<Object> Cell::GetFirst() const {
    return first_child_;
}
std::shared_ptr<Object> Cell::GetFunction() const {
    if (!Is<Cell>(first_child_) && command_to_function_.contains(first_child_->Serialize())) {
        return command_to_function_.at(first_child_->Serialize());
    }
    return nullptr;
}
std::shared_ptr<Object> Symbol::Eval() {
    if (command_to_function_.contains(str_)) {
        auto function = command_to_function_[str_];
        return function;
    }
    return shared_from_this();
}
std::shared_ptr<Object> Number::Eval() {
    return shared_from_this();
}
std::string Number::Serialize() {
    return std::to_string(number_);
}
std::shared_ptr<Object> Boolean::Eval() {
    return shared_from_this();
}
std::string Boolean::Serialize() {
    if (value_) {
        return "#t";
    } else {
        return "#f";
    }
}
std::shared_ptr<Object> Not::Apply(std::vector<std::shared_ptr<Object>>& args) {
    if ((args.size() > 1 && args[0] != nullptr) || args.empty()) {
        throw RuntimeError{"Error"};
    }
    if (Is<Boolean>(args[0])) {
        if (args[0]->Serialize() == "#t") {
            return std::make_shared<Symbol>(Boolean(false).Serialize());
        }
        return std::make_shared<Symbol>(Boolean(true).Serialize());
    }
    return std::make_shared<Symbol>(Boolean(false).Serialize());
}
