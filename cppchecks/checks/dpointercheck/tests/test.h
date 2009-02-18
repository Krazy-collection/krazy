
class QString;

class TestClass
{
  public:
    TestClass();

  protected:
    class Blaat
    {

    };

  private:
    class TestClassPrivate;
    TestClassPrivate *d;
    MyClass x1;                // VALUE
    MyClass const x2;          // CONST_VALUE
    MyClass &x3;               // REFERENCE_TO_VALUE
    MyClass const &x4;         // REFERENCE_TO_CONST_VALUE
    MyClass & const x5;        // CONST_REFERENCE_TO_VALUE
    MyClass const & const x6;  // CONST_REFERENCE_TO_CONST_VALUE
    MyClass *x7;               // POINTER_TO_VALUE
    MyClass const *x8;         // POINTER_TO_CONST_VALUE
    MyClass * const x9;        // CONST_POINTER_TO_VALUE
    MyClass const * const x10; // CONST_POINTER_TO_CONST_VALUE
};

class AnotherTestClass
{
  public:
    AnotherTestClass();

  private:
    MyClass y1;                // VALUE
    MyClass const y2;          // CONST_VALUE
    MyClass &y3;               // REFERENCE_TO_VALUE
    MyClass const &y4;         // REFERENCE_TO_CONST_VALUE
    MyClass & const y5;        // CONST_REFERENCE_TO_VALUE
    MyClass const & const y6;  // CONST_REFERENCE_TO_CONST_VALUE
    MyClass *y7;               // POINTER_TO_VALUE
    MyClass const *y8;         // POINTER_TO_CONST_VALUE
    MyClass * const y9;        // CONST_POINTER_TO_VALUE
    MyClass const * const y10; // CONST_POINTER_TO_CONST_VALUE
};
