static QString foo; // wrong - object might not be constructed
static QString bar("hello"); // wrong - object might not be constructed
static int foo = myInitializer(); // myInitializer() might not be called

static int foo(); //ok func decl
static int foo(const QString &); //ok func decl

static const int i = 42;  //ok
static const int ii[3] = {1, 2, 3}; //ok
static const char myString[] = "hello"; //ok
static const MyStruct s = {3, 4.4, "hello"}; //ok
K_GLOBAL_STATIC(QFile, myFile); //ok
