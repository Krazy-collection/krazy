class Measure
{
public:
    /*implicit*/ Measure( qreal value, //ok due to the implicit comment
                          int mode = 1,
                          int orientation = 2 );

    Measure( qreal value, int mode = 1, int orientation = 2 ); // issue
};

class GlobalMeasureScaling
{
public:
    GlobalMeasureScaling();
};
