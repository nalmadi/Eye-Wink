#ifndef TRIALVIEW_H
#define TRIALVIEW_H

#include <QPainter>
#include <QGraphicsItem>
#include <QTextStream>

#include <QFile>

#include "ievents.h"
#include "aoi.h"


class TrialView : public QGraphicsItem
{
public:
    TrialView();                    //constructor
    QRectF boundingRect()const;     //needed for drawing and it is fixed to 1024X768
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget); //important function for drawing all visible objects

public slots:
    void drawEvents(const QVector<Ievent>&, const QVector<AOI>&, bool, bool,bool);  //function to draw eye event
    void drawEventsSequence(int, bool, bool,bool);      //draw events until index from ievents vector
    QString getImage(QString, int& , int& );            //finds the file name for the string image of a trial
    void updateAOIs(const QVector<AOI>& , bool , bool , bool);
    void updateIevents(const QVector<Ievent>& , bool , bool , bool);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    QPointF mouse;  // not used at the moment
    bool pressed;   // not used
    bool drawevents;    //indicator for drawing events or not
    bool drawfixations; //indicator for drawing fixations
    bool drawsaccades;  //indicator for drawing saccades
    bool drawAOIs;      //indicator for drawing AOIs
    QVector<Ievent> ievents;    // vector of all ievents in a trial
    QVector<AOI> AOIs;          // vector of all areas of interest
    int lastevent;              // ievents size

};

#endif // TRIALVIEW_H
