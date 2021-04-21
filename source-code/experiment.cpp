#include "experiment.h"

Experiment::Experiment()
{

}

void Experiment::parse_asc(QTextStream& in)
{
    //find TRIALID and create trial object with ID
    //pass all lines from file to trial Object until ENDTRIAL or TRIAL_RESULT
    //repeat until all trials in file are read

    QStringList tokens;
    QString line;
    bool trialEnd = false;
    bool trialstarted = false;

    while(!in.atEnd()){
              line = in.readLine();
              tokens = line.split(QRegExp("\\s+"),QString::SkipEmptyParts);

             // for (int i=0; i< tokens.size(); i++){
              if(tokens.size()>2){
                  if(tokens[2]=="TRIALID" && !trialstarted){
                      Trial *newTrial = new Trial(tokens[3]);
                      trials.push_back(*newTrial);
                      trialstarted = true;
                  }
                  else if(tokens[2]=="TRIAL_RESULT"){
                          trialEnd = true;
                          trialstarted = false;
                  }
                  else if(trialstarted){
                          trials.last().add_event(line);
                  }

             }

    }
}

QVector<Trial> Experiment::getTrials()
{
    return trials;
}
