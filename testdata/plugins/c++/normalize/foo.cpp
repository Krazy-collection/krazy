connect(this, SIGNAL( someSignal()), //normalize
        this, SLOT( someSlot()));  //normalize

connect(this, SIGNAL(someSignal( )), //normalize
        this, SLOT(someSlot( )));  //normalize

connect(this, SIGNAL(someSignal() ), //normalize
        this, SLOT(someSlot() ));  //normalize

connect(this, SIGNAL(someSignal(const QString)), //normalize
        this, SLOT(someSlot(const QString)));  //normalize

connect(this, SIGNAL(someSignal(const QString&)), //normalize
        this, SLOT(someSlot(const QString&)));  //normalize

connect(this, SIGNAL(someSignal(QString, QString)), //normalize
        this, SLOT(someSlot(QString, QString)));  //normalize

connect(this, SIGNAL(someSignal(QString,const QString)), //normalize
        this, SLOT(someSlot(QString,const QString)));  //normalize

connect(this, SIGNAL(someSignal(QString,const QString&)), //normalize
        this, SLOT(someSlot(QString,const QString&)));  //normalize

connect(this, SIGNAL(someSignal(QString&, QString)), //normalize
        this, SLOT(someSlot(QString&, QString)));  //normalize

connect(this, SIGNAL(someSignal(QString&,const QString)), //normalize
        this, SLOT(someSlot(QString&,const QString)));  //normalize

connect(this, SIGNAL(someSignal(QString&,const QString&)), //normalize
        this, SLOT(someSlot(QString&,const QString&)));  //normalize

connect(this, SIGNAL(someSignal(QObject*, QString)), //normalize
        this, SLOT(someSlot(QObject*, QString)));  //normalize

connect(this, SIGNAL(someSignal(QObject*,const QString)), //normalize
        this, SLOT(someSlot(QObject*,const QString)));  //normalize

connect(this, SIGNAL(someSignal(QObject*,const QString&)), //normalize
        this, SLOT(someSlot(QObject*,const QString&)));  //normalize

connect(this, SIGNAL(someSignal(const QObject*, QString)), //normalize
        this, SLOT(someSlot(const QObject*, QString)));  //normalize

connect(this, SIGNAL(someSignal(const QObject*,const QString)), //normalize
        this, SLOT(someSlot(const QObject*,const QString)));  //normalize

connect(this, SIGNAL(someSignal(const QObject*,const QString&)), //normalize
        this, SLOT(someSlot(const QObject*,const QString&)));  //normalize

connect(this, SIGNAL(someSignal(QString,QString) ), //normalize
        this, SLOT(someSlot(QString, QString) ));  //normalize

connect(this, SIGNAL(someSignal(QString&,QString) ), //normalize
        this, SLOT(someSlot(QString&, QString) ));  //normalize

connect(this, SIGNAL(someSignal(QObject*,QString) ), //normalize
        this, SLOT(someSlot(QObject*, QString) ));  //normalize

connect(this, SIGNAL(someSignal(const QObject*,QString) ), //normalize
        this, SLOT(someSlot(const QObject*,QString) ));  //normalize




connect(this, SIGNAL(someSignal()), //ok
        this, SLOT(someSlot()));  //ok

connect(this, SIGNAL(someSignal(QString)), //ok
        this, SLOT(someSlot(QString)));  //ok

connect(this, SIGNAL(someSignal(QString&)), //ok
        this, SLOT(someSlot(QString&)));  //ok

connect(this, SIGNAL(someSignal(QObject*)), //ok
        this, SLOT(someSlot(QObject*)));  //ok

connect(this, SIGNAL(someSignal(const QObject*)), //ok
        this, SLOT(someSlot(const QObject*)));  //ok

connect(this, SIGNAL(someSignal(QString,QString)), //ok
        this, SLOT(someSlot(QString,QString)));  //ok

connect(this, SIGNAL(someSignal(QString,QString&)), //ok
        this, SLOT(someSlot(QString,QString&)));  //ok

connect(this, SIGNAL(someSignal(QString,QObject*)), //ok
        this, SLOT(someSlot(QString,QObject*)));  //ok

connect(this, SIGNAL(someSignal(QString,const QObject*)), //ok
        this, SLOT(someSlot(QString,const QObject*)));  //ok

connect(this, SIGNAL(someSignal(QString&,QString)), //ok
        this, SLOT(someSlot(QString&,QString)));  //ok

connect(this, SIGNAL(someSignal(QString&,QString&)), //ok
        this, SLOT(someSlot(QString&,QString&)));  //ok

connect(this, SIGNAL(someSignal(QString&,QObject*)), //ok
        this, SLOT(someSlot(QString&,QObject*)));  //ok

connect(this, SIGNAL(someSignal(QString&,const QObject*)), //ok
        this, SLOT(someSlot(QString&,const QObject*)));  //ok

connect(this, SIGNAL(someSignal(QObject*,QString)), //ok
        this, SLOT(someSlot(QObject*,QString)));  //ok

connect(this, SIGNAL(someSignal(QObject*,QString&)), //ok
        this, SLOT(someSlot(QObject*,QString&)));  //ok

connect(this, SIGNAL(someSignal(QObject*,QObject*)), //ok
        this, SLOT(someSlot(QObject*,QObject*)));  //ok

connect(this, SIGNAL(someSignal(QObject*,const QObject*)), //ok
        this, SLOT(someSlot(QObject*,const QObject*)));  //ok

connect(this, SIGNAL(someSignal(const QObject*,QString)), //ok
        this, SLOT(someSlot(const QObject*,QString)));  //ok

connect(this, SIGNAL(someSignal(const QObject*,QString&)), //ok
        this, SLOT(someSlot(const QObject*,QString&)));  //ok

connect(this, SIGNAL(someSignal(const QObject*,QObject*)), //ok
        this, SLOT(someSlot(const QObject*,QObject*)));  //ok

connect(this, SIGNAL(someSignal(const QObject*,const Object*)), //ok
        this, SLOT(someSlot(const QObject*,const QObject*)));  //ok
