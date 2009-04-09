#include <iostream>

using namespace std;

class A
{
    public:
        virtual ~A() {}

        void someFunc()
        {
            cout << "A::someFunc()" << endl;
        }

        virtual void someVirtualFunc()
        {
            cout << "A::someVirtualFunc()" << endl;
        }

        virtual void anotherVirtualFunc()
        {
            cout << "A::anotherVirtualFunc()" << endl;
        }
};

class B : public A
{
    public:
        virtual ~B() {}
        // Reimplementation of non-virtual method.
        void someFunc()
        {
            cout << "B::someFunc()" << endl;
        }

        virtual void someVirtualFunc()
        {
            cout << "B::someVirtualFunc()" << endl;
        }
};

int main()
{
    B b;
    b.someFunc();
    b.someVirtualFunc();
    b.anotherVirtualFunc();

    A *b2 = new B();
    b2->someFunc();
    b2->someVirtualFunc();
    b2->anotherVirtualFunc();
    delete b2;

    return 0;
}
