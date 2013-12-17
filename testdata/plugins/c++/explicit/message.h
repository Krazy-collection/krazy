class Message
{
  public:
#ifdef Q_COMPILER_RVALUE_REFS
  Message(  Message && other); //ok
#else
    Message(const Message &other);  //ok
#endif
    ~Message();
};
