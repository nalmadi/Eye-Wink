#ifndef EXPERIMENT_H
#define EXPERIMENT_H

#include <QVector>
#include <QStringList>
#include <QTextStream>
#include "trial.h"


class Experiment
{
public:
    Experiment();                       // constructor
    void parse_asc(QTextStream& in);    // function to parse the ASCII file from Eye-link
    QVector<Trial> getTrials();         // accessor for trials


private:
    QVector<Trial> trials;  //each experiment contains many trials

};

#endif // EXPERIMENT_H
