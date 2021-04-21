#include "aoi.h"

//constructor
AOI::AOI()
{

}

//constructor
AOI::AOI(QString iTrialID, QString iAOIID, int ix, int iy, int ix2, int iy2, QString iword)
{
    TrialID = iTrialID;
    AOIID = iAOIID;
    x = ix;
    y = iy;
    x2 = ix2;
    y2 = iy2;
    word = iword;
}

//accessor for AOI by reference
void AOI::getAOI(QString &iTrialID, QString &iAOIID, int &ix, int &iy, int &ix2, int &iy2, QString &iword)
{
    iTrialID = TrialID;
    iAOIID = AOIID;
    ix = x;
    iy = y;
    ix2 = x2;
    iy2 = y2;
    iword = word;
}
