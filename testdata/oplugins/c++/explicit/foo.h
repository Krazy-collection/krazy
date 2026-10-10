class Foo {
public:

    Foo(); //ok



    Foo(int singleArgument); //issue

    Foo(int singleArgument = 0); //issue

    explicit Foo(int singleArgument); //ok

    explicit Foo(int singleArgument = 0); //ok



    Foo(int argument1, int argument2); //ok

    Foo(int argument1, int argument2 = 0); //issue

    Foo(int argument1 = 0, int argument2 = 0); //issue

    explicit Foo(int argument1, int argument2 = 0); //ok

    explicit Foo(int argument1 = 0, int argument2 = 0); //ok

};

class Bar {
public:

    Bar(); //ok



    Bar(int singleArgument); //issue

    Bar(int singleArgument = 0); //issue

    explicit Bar(int singleArgument); //ok

    explicit Bar(int singleArgument = 0); //ok



    Bar(int argument1, int argument2); //ok

    Bar(int argument1, int argument2 = 0); //issue

    Bar(int argument1 = 0, int argument2 = 0); //issue

    explicit Bar(int argument1, int argument2 = 0); //ok

    explicit Bar(int argument1 = 0, int argument2 = 0); //ok

};
