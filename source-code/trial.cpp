#include "trial.h"

Trial::Trial()
{

}

Trial::Trial(QString ID)
{
    TrialID = ID;
}

void Trial::add_event(QString ievent_line)
{
    //parse ievent
    //if event is important add it to events vector
    //if event is not important ignore it
    QStringList tokens;
    tokens = ievent_line.split(QRegExp("\\s+"),QString::SkipEmptyParts);

    int i=0;

    if(tokens[i]=="MSG" && tokens.size()>5){
        if(tokens[4]=="DRAW_LIST"){
            Ievent *ie = new Ievent("DRAW_LIST",tokens[5],0,0,0,0,0,0,0,0,0,0,0,0,0);
            Ievents.push_back(*ie);
        }
        else if (tokens[4]=="IAREA"){
            Ievent *ie = new Ievent("IAREA",tokens[6],0,0,0,0,0,0,0,0,0,0,0,0,0);
            Ievents.push_back(*ie);
        }

        }
        else if(tokens[i]=="EFIX"){
            Ievent *ie = new Ievent("EFIX",0,tokens[i+1], tokens[i+2], tokens[i+3], tokens[i+4], tokens[i+5], tokens[i+6],0,0,0,0,tokens[i+7],0,0);
            Ievents.push_back(*ie);
        }
        else if(tokens[i]=="ESACC"){
            Ievent *ie = new Ievent("ESACC",0,tokens[i+1], tokens[i+2], tokens[i+3], tokens[i+4], tokens[i+5], tokens[i+6],tokens[i+7],tokens[i+8],tokens[i+9],tokens[i+10],0,0,0);
            Ievents.push_back(*ie);
        }
        else if(tokens[i]=="EBLINK"){
            Ievent *ie = new Ievent("EBLINK",0,tokens[i+1], tokens[i+2], tokens[i+3], tokens[i+4],0,0,0,0,0,0,0,0,0);
            Ievents.push_back(*ie);
        }

}

QVector<Ievent> Trial::getTrial()
{
    return Ievents;
}

QString Trial::getTrialID()
{
    return TrialID;
}
