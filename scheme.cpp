#include "scheme.h"
#include "tokenizer.h"
#include "object.h"
#include "parser.h"
#include <sstream>
#include <vector>

void Valide(const std::shared_ptr<Object>& ast) {
    if (Is<Cell>(ast) && As<Cell>(ast)->GetFirst() == nullptr) {
        throw RuntimeError{"Error"};
    }
    if (Is<Cell>(As<Cell>(ast))) {
        if (Is<Cell>(As<Cell>(As<Cell>(ast))->GetFirst())) {
            Valide(As<Cell>(ast)->GetFirst());
        }
    }
    if (Is<Cell>(As<Cell>(ast))) {
        if (Is<Cell>(As<Cell>(As<Cell>(ast))->GetSecond())) {
            Valide(As<Cell>(ast)->GetSecond());
        }
    }
}

void ValidAst(const std::shared_ptr<Object>& ast) {
    if (ast == nullptr) {
        throw RuntimeError{"Error"};
    }
    auto cell = As<Cell>(ast)->GetFirst();
    if (cell == nullptr) {
        throw RuntimeError{"Error"};
    }
    while (Is<Cell>(cell)) {
        if (cell == nullptr) {
            throw RuntimeError{"Error"};
        }
        cell = As<Cell>(cell)->GetFirst();
    }
    if (!cell) {
        throw RuntimeError{"Error"};
    }
}

std::string Interpreter::Run(const std::string& code) {
    std::stringstream stream{code};
    Tokenizer tokenizer(&stream);
    auto input_ast = Read(&tokenizer);

    if (!tokenizer.IsEnd()) {
        throw SyntaxError{"Error"};
    }
    if (Is<Cell>(input_ast)) {
        ValidAst(input_ast);
    }

    if (!input_ast) {
        throw RuntimeError{"Error"};
    }

    if (Is<Cell>(input_ast)) {
        auto function = As<Cell>(input_ast)->GetFunction();
        if (As<Cell>(input_ast)->IsPair()) {
            throw RuntimeError{"Error"};
        }
        if (!function) {
            throw RuntimeError{"Error"};
        }
        std::vector<std::shared_ptr<Object>> args;
        if (!Is<Quote>(function)) {
            Valide(input_ast);
        }
        if (Is<And>(function) || Is<Or>(function)) {
            if (As<Cell>(input_ast)->GetSecond()) {
                GenerateVectorOfArgsNoExcept(args, As<Cell>(input_ast)->GetSecond());
            }
        } else if (Is<Quote>(As<Function>(function))) {
            args.push_back(As<Cell>(input_ast)->GetSecond());
        } else {
            if (As<Cell>(input_ast)->GetSecond()) {
                if (!Is<CheckIsPair>(function) &&
                    As<Cell>(As<Cell>(input_ast)->GetSecond())->IsPair()) {
                    throw RuntimeError{"Error"};
                }
                GenerateVectorOfArgs(args, As<Cell>(input_ast)->GetSecond());
            }
        }
        input_ast = As<Function>(function)->Apply(args);
    }
    auto output_ast = input_ast->Eval();
    return output_ast->Serialize();
}
