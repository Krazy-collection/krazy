include_directories(${CMAKE_CURRENT_SOURCE_DIR})

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
    #DefaultVisitor.cpp
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
    ObjectiveCTypeQualifiers.cpp
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
include_directories(${QT_INCLUDES})

add_library(cpp_parser SHARED ${cplusplus_SRCS})
set_target_properties(cpp_parser PROPERTIES DEFINE_SYMBOL CPLUSPLUS_BUILD_LIB)

if(WIN32)
install(TARGETS cpp_parser ${INSTALL_TARGETS_DEFAULT_ARGS})
endif(WIN32)
