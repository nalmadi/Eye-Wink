#ifndef IEVENTS_H
#define IEVENTS_H
#include <QString>

class Ievent
{
public:
    Ievent();

    //this is a constructor
    Ievent(QString ievent,
           QString iimage_file,
           QString ieye,
           QString istart_time,
           QString iend_time,
           QString iduration,
           QString istart_x,
           QString istart_y,
           QString iend_x,
           QString iend_y,
           QString iamplitude,
           QString ipeak_velocity,
           QString ipupil,
           QString iresx,
           QString iresy);

    //accessor by reference
    void getIevents(QString& ievent,
    QString& iimage_file,
    QString& ieye,
    int& istart_time,
    int& iend_time,
    int& iduration,
    float& istart_x,
    float& istart_y,
    float& iend_x,
    float& iend_y,
    float& iamplitude,
    float& ipeak_velocity,
    float& ipupil,
    float& iresx,
    float& iresy);

private:
    QString event; // fixation, saccade, blink
    QString image_file;
    QString eye;
    int start_time;
    int end_time;
    int duration;
    float start_x;
    float start_y;
    float end_x;
    float end_y;
    float amplitude; // in degrees
    float peak_velocity;
    float pupil; // pupil
    float resx; // resolution
    float resy; // resolution

};

#endif // IEVENTS_H
