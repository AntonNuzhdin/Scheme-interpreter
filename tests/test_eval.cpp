#include "scheme_test.h"

TEST_CASE_METHOD(SchemeTest, "Quote") {
    ExpectEq("(quote 1)", "(1 2)");
    ExpectEq("'(1 2)", "(1 2)");
}

TEST_CASE_METHOD(SchemeTest, "Be careful") {
    ExpectSyntaxError("())");
    ExpectRuntimeError("(())");
    ExpectRuntimeError("(+ ())");
    ExpectRuntimeError("('() ())");
    ExpectEq("'(())", "(())");
}
