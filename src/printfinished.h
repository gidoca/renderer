#ifndef MAIN_H
#define MAIN_H

#include <QObject>
#include <QTime>

class PrintFinished : public QObject
{
    Q_OBJECT

public:
    PrintFinished(QTime & timeElapsed) : timeElapsed(timeElapsed) {}

public Q_SLOTS:
    void printTime();

private:
    QTime & timeElapsed;
};


#endif // MAIN_H
