
class Foo: public QObject
Q_OBJECT
{
  public:
    Foo();
  signals:
    fooSignal();
  slots:
    fooSlot(); 
}
