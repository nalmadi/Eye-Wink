#ifndef TRIAL_H
#define TRIAL_H

#include <QVector>
#include <QStringList>

#include "ievents.h"


class Trial
{
public:
    Trial();            //constructor
    Trial(QString ID);  //constructor by trial ID
    void add_event(QString ievent); //adds eye event to trial
    QVector<Ievent> getTrial();     //gets vector of events for a trial
    QString getTrialID();           // returns trial ID


private:
    QString TrialID;
    QVector<Ievent> Ievents;

};

#endif // TRIAL_H
