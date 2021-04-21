#include "trialview.h"

TrialView::TrialView()
{
    //setFlag(ItemIsMovable);

    drawevents = false;
}

QRectF TrialView::boundingRect() const
{
    return QRectF(0,0,1024,768);
}

void TrialView::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //draw black background
    QRectF rect = boundingRect();
    QBrush brush(Qt::black);
    painter->fillRect(rect,brush);
    painter->drawRect(rect);


    //draw eye-events
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

    QColor yellow("#f0d048");
    QColor white("#ffffff");
    Qt::BrushStyle style = Qt::SolidPattern;
    QBrush brush2(QColor(242, 255, 0, 128), style);
    QBrush brush3(QColor(0, 0, 0, 0), style);

    if(drawevents){
        //loop over events
        //if event is fixation draw ellipse
        //if event is sacad draw line
        int bubble = 5;

        for(int i=0; i<lastevent; i++){
            ievents[i].getIevents(event,image_file,eye,start_time,end_time,duration,start_x,start_y,end_x,end_y,amplitude,peak_velocity, pupil,resx,resy);
            if(event == "EFIX" && drawfixations){
                painter->setBrush(brush2);
                painter->setPen(Qt::yellow);
                if(5*(duration/100) < 5)
                    bubble = 3;
                else
                    bubble = 5*(duration/100);
                painter->drawEllipse(QPoint(start_x,start_y),bubble,bubble);

                painter->setPen(Qt::red);
                QFont font;// = painter.font();
                font.setPixelSize(18);
                painter->setFont(font);
                painter->drawText(QPoint(start_x,start_y), QString::number(i));
            }
            else if (event == "ESACC" && drawsaccades){
                painter->setBrush(brush2);
                painter->setPen(Qt::darkYellow);
                painter->drawLine(QPoint(start_x,start_y),QPoint(end_x,end_y));

                painter->setPen(Qt::darkRed);
                QFont font;// = painter.font();
                font.setPixelSize(18);
                painter->setFont(font);
                painter->drawText(QPoint((start_x+end_x)/2,(start_y+end_y)/2), QString::number(i));

            }
            else if (event == "DRAW_LIST"){
                //draw text image
                int x,y;

                QPixmap pi(getImage(image_file, x, y));
                painter->drawPixmap(x,y,pi.width(),pi.height(),pi);
            }
            else if (event == "IAREA" && drawAOIs){
                QString TrialID;
                QString AOIID;
                int x;
                int y;
                int x2;
                int y2;
                QString word;

                for(int q =0 ; q<AOIs.size();q++){
                    AOIs[q].getAOI(TrialID,AOIID,x,y,x2,y2,word);
                    painter->setPen(Qt::white);
                    painter->setBrush(brush3);
                    painter->drawRect(x,y,x2-x-2,y2-y-2);

                }

            }

        }


    }
}


void TrialView::drawEvents(const QVector<Ievent>& ie, const QVector<AOI>& aoi, bool drawfixs, bool drawsacs, bool drawAOI)
{
    ievents.clear();
    for(int i=0; i < ie.size();i++)
        ievents.push_back( ie[i]);

    AOIs.clear();
    for(int i=0; i < aoi.size();i++)
        AOIs.push_back( aoi[i]);


    drawevents = true;
    drawfixations = drawfixs;
    drawsaccades = drawsacs;
    drawAOIs = drawAOI;
    lastevent = ievents.size();
    update();
}


void TrialView::updateAOIs(const QVector<AOI>& aoi, bool drawfixs, bool drawsacs, bool drawAOI)
{
    AOIs.clear();
    for(int i=0; i < aoi.size();i++)
        AOIs.push_back( aoi[i]);

    drawevents = true;
    drawfixations = drawfixs;
    drawsaccades = drawsacs;
    drawAOIs = drawAOI;
    update();
}

void TrialView::updateIevents(const QVector<Ievent> &ie, bool drawfixs, bool drawsacs, bool drawAOI)
{
    ievents.clear();
    for(int i=0; i < ie.size();i++)
        ievents.push_back( ie[i]);

    drawevents = true;
    drawfixations = drawfixs;
    drawsaccades = drawsacs;
    drawAOIs = drawAOI;
    lastevent = ievents.size();
    update();
}

void TrialView::drawEventsSequence(int endevent, bool drawfixs, bool drawsacs, bool drawAOI)
{
    drawevents = true;
    drawfixations = drawfixs;
    drawsaccades = drawsacs;
    drawAOIs = drawAOI;
    lastevent = endevent;
    update();
}


QString TrialView::getImage(QString image_file, int& x, int& y)
{

    QString path = image_file.mid(4,-1);

    QStringList tokens;
    QString line;
    QString imagepath;
    QString xval, yval;

    QFile actualVCL(path);
    if(actualVCL.open(QIODevice::ReadOnly)){

    QTextStream in(&actualVCL);
    while(!in.atEnd()){

         line = in.readLine();
         tokens.clear();
         tokens = line.split(QRegExp("\\s+"),QString::SkipEmptyParts);

         if(tokens.size()==6){
             imagepath = tokens[3];
             xval = tokens[4];
             yval = tokens[5];
         }

    }
    actualVCL.close();
    }

    x = xval.toInt();
    y = yval.toInt();
    return imagepath.mid(4,-1);

}


void TrialView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
   // pressed = true;
   // update();
    QGraphicsItem::mousePressEvent(event);
}

void TrialView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
   // pressed = false;
    //update();
    QGraphicsItem::mouseReleaseEvent(event);
}

