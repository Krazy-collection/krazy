class FOO_EXPORT Foo1 {
public:

    const QString& publicBar(); //constref

    QString publicBaz(); //ok

protected:

    const QString& protectedBar(); //constref

    QString protectedBaz(); //ok

private:

    const QString& privateBar(); //ok

    QString privateBaz(); //ok

};

class FOO_EXPORT Foo2 {
public:

    const QString& publicBar(); //constref

private:

    const QString& privateBar(); //ok

public:

    const QString& publicBar2(); //constref

protected:

    const QString& protectedBar(); //constref

private:

    const QString& privateBar2(); //ok

public:
    const QString& publicBar3(); //constref
    const QString& publicBar4(); //constref
    const QString& publicBar5(); //constref
};
