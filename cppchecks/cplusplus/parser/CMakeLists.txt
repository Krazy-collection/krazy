include_directories(${CMAKE_CURRENT_SOURCE_DIR})
message(${CMAKE_SOURCE_DIR} STATUS)

set(cplusplus_SRCS
    AST.cpp
    ASTVisitor.cpp
    Array.cpp
    CheckDeclaration.cpp
    CheckDeclarator.cpp
    CheckExpression.cpp
    CheckName.cpp
    CheckSpecifier.cpp
    CheckStatement.cpp
    Control.cpp
    CoreTypes.cpp
    DefaultVisitor.cpp
    DiagnosticClient.cpp
    FullySpecifiedType.cpp
    Keywords.cpp
    ObjectiveCAtKeywords.cpp
    Lexer.cpp
    LiteralTable.cpp
    Literals.cpp
    MemoryPool.cpp
    Name.cpp
    NameVisitor.cpp
    Names.cpp
    Parser.cpp
    Scope.cpp
    Semantic.cpp
    SemanticCheck.cpp
    Symbol.cpp
    Symbols.cpp
    SymbolVisitor.cpp
    Token.cpp
    TranslationUnit.cpp
    Type.cpp
    TypeVisitor.cpp
    PrettyPrinter.cpp
)

add_library(c++parser SHARED ${cplusplus_SRCS})