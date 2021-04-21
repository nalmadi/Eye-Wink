#ifndef AOI_H
#define AOI_H

#include <QString>

class AOI
{
public:
    //default constructor
    AOI();

    //constructor
    AOI(QString iTrialID,
    QString iAOIID,
    int ix,
    int iy,
    int ix2,
    int iy2,
    QString iword);

    //accessor for AOI by reference
    void getAOI(QString& iTrialID,
             QString& iAOIID,
             int& ix,
             int& iy,
             int& ix2,
             int& iy2,
             QString& iword);


private:
    QString TrialID;    // acts as a unique key - not used
    QString AOIID;      // sequencial number for each AOI
    int x;              // x value top left
    int y;              // y value top left
    int x2;          // x value bottom right
    int y2;         // y value bottom right
    QString word;       // the actual word which the AOI covers


};

#endif // AOI_H
