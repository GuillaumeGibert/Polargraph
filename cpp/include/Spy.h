#ifndef SPY_H
#define SPY_H

#include <QObject>
#include <iostream>


class Spy : public QObject
{
	Q_OBJECT

public:
    Spy();
    ~Spy();

public slots:
    void setSignalValues(float, std::vector<float>);
	
};

#endif // SPY_H
