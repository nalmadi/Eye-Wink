#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QTextStream>
#include <QString>
#include <QFileDialog>
#include <QTreeWidgetItem>

#include "trialview.h"
#include "graphicsview.h"
#include "experiment.h"
#include "trial.h"
#include "ievents.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void add_root(QString root);    //for tree
    void add_child(QString child);  //for tree
    void addTableHeader();      //adds the header to the events table
    void addTableData(QVector<Ievent>& ie);
    void getAOIsFromFile(QVector<AOI>& aoi);
    void addAOITableHeader();
    void addAOITableData(QVector<AOI>& aoi);

    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);

    QVector<Ievent> findTrial(QString ID);//returns events of a trial by trial ID


    void on_horizontalSlider_sliderMoved(int position); //this is for the slider to construct the sequency of events until position (0-#events)

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_9_clicked();

    void fixationStats();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_11_clicked();

    //adds first fixation time to AOI in table2
    void firstFixationAnalysis();

    //adds total time data to table2
    void totalTimeAnalysis();

    //regression analysis
    void regressionAnalysis();

    //finds AOI row in table
    QString findAOIPath();


private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    GraphicsView *graph;
    TrialView *tview;
    Experiment exp;

};

#endif // MAINWINDOW_H
