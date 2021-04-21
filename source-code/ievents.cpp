#include "ievents.h"

Ievent::Ievent(){


}
//constructor
Ievent::Ievent(QString ievent, QString iimage_file, QString ieye, QString istart_time, QString iend_time, QString iduration, QString istart_x, QString istart_y, QString iend_x, QString iend_y, QString iamplitude, QString ipeak_velocity, QString ipupil, QString iresx, QString iresy)
{
    event =  ievent;
    image_file = iimage_file;
    eye =  ieye;
    start_time = istart_time.toInt();
    end_time = iend_time.toInt();
    duration = iduration.toInt();
    start_x = istart_x.toFloat();
    start_y = istart_y.toFloat();
    end_x = iend_x.toFloat();
    end_y = iend_y.toFloat();
    amplitude = iamplitude.toFloat();
    peak_velocity = ipeak_velocity.toFloat();
    pupil = ipupil.toFloat();
    resx = iresx.toFloat();
    resy = iresy.toFloat();
}

//accessor by reference
void Ievent::getIevents(QString &ievent, QString &iimage_file, QString &ieye, int &istart_time, int &iend_time, int &iduration, float &istart_x, float &istart_y, float &iend_x, float &iend_y, float &iamplitude, float &ipeak_velocity, float &ipupil, float &iresx, float &iresy)
{
    ievent = event;
    iimage_file = image_file;
    ieye = eye;
    istart_time = start_time;
    iend_time = end_time;
    iduration = duration;
    istart_x = start_x;
    istart_y = start_y;
    iend_x = end_x;
    iend_y = end_y;
    iamplitude = amplitude;
    ipeak_velocity = peak_velocity;
    ipupil = pupil;
    iresx = resx;
    iresy = resy;
}


