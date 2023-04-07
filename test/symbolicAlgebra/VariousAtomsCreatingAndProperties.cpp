#include "SymbolicAlgebraTestBase.h"

/*
TEST_CASE("Abbreviated multiplication formulas for power = 2", "[symbolicAlgebra]")
{
    Atom expression1 = std::make_unique<Power>(
        std::make_unique<Sum>(std::make_unique<Var>("a"), std::make_unique<Var>("b")),
        std::make_unique<NumberInt>(2)
    );

    Atom expression2 = std::make_unique<Power>(
        std::make_unique<Sum>(std::make_unique<Var>("a"), std::make_unique<Product>(std::make_unique<NumberInt>(-1), std::make_unique<Var>("b"))),
        std::make_unique<NumberInt>(2)
    );

    Atom expression3 = std::make_unique<Product>(
        std::make_unique<Sum>(std::make_unique<Var>("a"), std::make_unique<Var>("b")),
        std::make_unique<Sum>(std::make_unique<Var>("a"), std::make_unique<Product>(std::make_unique<NumberInt>(-1), std::make_unique<Var>("b")))
    );

    expression1 = expression1->expand()->simplify();
    expression2 = expression2->expand()->simplify();
    expression3 = expression3->expand()->simplify();

    std::stringstream streamOutput;

    expression1->print(streamOutput);
    REQUIRE(streamOutput.str() == "a^2+2*a*b+b^2");
    streamOutput.str("");

    expression2->print(streamOutput);
    REQUIRE(streamOutput.str() == "a^2-2*a*b+b^2");
    streamOutput.str("");

    expression3->print(streamOutput);
    REQUIRE(streamOutput.str() == "a^2-b^2");
    streamOutput.str("");
}
*/