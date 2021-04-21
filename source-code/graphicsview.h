#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H
#include <QGraphicsView>

//this class was created to allow interaction between window and TrialView class
//something is needed to connect this class to a normal QGraphicsView on the window, I can't remember the excat name
//but the QgraphicsView can act as a place holder for this custom class

class GraphicsView : public QGraphicsView
{
public:
    GraphicsView(){}
    GraphicsView(QWidget * parent):QGraphicsView( parent){}

protected:
    void mousePressEvent(QMouseEvent *event)
    {

        QGraphicsView::mousePressEvent(event);
    }

private:
    QString objName;

};

#endif // GRAPHICSVIEW_H
