#pragma once

#include <catch.hpp>

#include <error.h>
#include <scheme.h>

class SchemeTest {
public:
    void ExpectEq(std::string expression, const std::string& result) {
        REQUIRE(interpreter_.Run(expression) == result);
    }

    void ExpectNoError(std::string expression) {
        REQUIRE_NOTHROW(interpreter_.Run(expression));
    }

    void ExpectSyntaxError(std::string expression) {
        REQUIRE_THROWS_AS(interpreter_.Run(expression), SyntaxError);
    }

    void ExpectRuntimeError(std::string expression) {
        REQUIRE_THROWS_AS(interpreter_.Run(expression), RuntimeError);
    }

    void ExpectNameError(std::string expression) {
        REQUIRE_THROWS_AS(interpreter_.Run(expression), NameError);
    }

private:
    Interpreter interpreter_;
};
