#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    tview = new TrialView();
    scene->addItem(tview);

    ui->treeWidget->setColumnCount(1);

}

MainWindow::~MainWindow()
{
    delete ui;
}


//opne ASC file and initiates tree build and experiment object build
void MainWindow::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),"",tr("Files (*.asc)"));
    QFile file(fileName);

    if(!file.open(QIODevice::ReadOnly))
        ui->statusBar->showMessage("could not open file!");

    ui->statusBar->showMessage(fileName + " opened!");

    QTextStream in(&file);

    exp.parse_asc(in);

    file.close();
    //update window
    QVector<Trial> trials = exp.getTrials();
    add_root(".." + file.fileName().right(15));

    for(int i=0; i<trials.size();i++)
        add_child(trials[i].getTrialID());

}


//adds the root to the tree
void MainWindow::add_root(QString root)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidget);
    item->setText(0,root);
    ui->treeWidget->addTopLevelItem(item);
}


//adds the child to the tree
void MainWindow::add_child(QString child)
{
    QTreeWidgetItem *item = new QTreeWidgetItem();
    item->setText(0,child);
    ui->treeWidget->addTopLevelItem(item);

}


//when trial is clicked in tree this function builds the visualization of the trial details
void MainWindow::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    QString ID = item->text(0);
    QVector<Ievent> ie = findTrial(ID);

    ui->horizontalSlider->setRange(1,ie.size());

    addTableHeader();
    addTableData(ie);

    //building AOI data and visualization from file
    QVector<AOI> aois;
    getAOIsFromFile(aois);//pass by reference
    addAOITableHeader();
    addAOITableData(aois);

    tview->drawEvents(ie, aois, ui->checkBox->checkState(), ui->checkBox_2->checkState(), ui->checkBox_3->checkState());
    fixationStats();
}


//returns eye-events vector by trial ID
QVector<Ievent> MainWindow::findTrial(QString ID){

    QVector<Trial> trials = exp.getTrials();

    for(int i=0; i<trials.size();i++)
       if(trials[i].getTrialID()== ID)
           return trials[i].getTrial();

    QVector<Ievent> result;
    return result;
}


//moving the slider update the trial view with the trial events until position
void MainWindow::on_horizontalSlider_sliderMoved(int position)
{
    tview->drawEventsSequence( position, ui->checkBox->checkState(),ui->checkBox_2->checkState(), ui->checkBox_3->checkState());
}

QString MainWindow::findAOIPath(){

    for(int i=1; i<ui->tableWidget->rowCount(); i++){
        if(ui->tableWidget->item(i,0)!=0){
            if(ui->tableWidget->item(i,0)->text()=="IAREA")
               return ui->tableWidget->item(i,1)->text();
        }
    }
    return "";
}

void MainWindow::getAOIsFromFile(QVector<AOI>& aoi){

    QString AOI_path = findAOIPath();
    ui->label->setText(AOI_path);

    QString path = AOI_path.mid(4,-1);

    //QVector<AOI> AOIs;
    QStringList tokens;
    QString line;

    QFile actualAOI(path);
    if(actualAOI.open(QIODevice::ReadOnly)){

    QTextStream in(&actualAOI);
    while(!in.atEnd()){

         line = in.readLine();
         tokens.clear();
         tokens = line.split(QRegExp("\\s+"),QString::SkipEmptyParts);

         if(tokens.size()==8){
             AOI *area = new AOI(0,tokens[2],tokens[3].toInt(),tokens[4].toInt(),tokens[5].toInt(),tokens[6].toInt(),tokens[7]);
             aoi.push_back(*area);
         }
         //if the AOI has more than one words
         if(tokens.size()>8){
             QString words;
             for(int i = 7; i<tokens.size(); i++)
                 words+= tokens[i]+" ";

             AOI *area = new AOI(0,tokens[2],tokens[3].toInt(),tokens[4].toInt(),tokens[5].toInt(),tokens[6].toInt(),words);
             aoi.push_back(*area);
         }

    }
    actualAOI.close();
    }

}

//opens AOI file and adds AOI data to table_2
void MainWindow::addAOITableData(QVector<AOI>& aoi){


    QString TrialID;    // acts as a unique key - not used
    QString AOIID;      // sequencial number for each AOI
    int x;              // x value top left
    int y;              // y value top left
    int x2;          // x value bottom right
    int y2;         // y value bottom right
    QString word;       // the actual word which the AOI covers

    for(int i = 0; i < aoi.size() ; i++){
        aoi[i].getAOI(TrialID, AOIID, x, y, x2, y2, word);

        ui->tableWidget_2->setRowCount(i+2);
        QTableWidgetItem *item = new QTableWidgetItem(AOIID);
        ui->tableWidget_2->setItem(i+1,0,item);

        QTableWidgetItem *item2 = new QTableWidgetItem(QString::number(x));
        ui->tableWidget_2->setItem(i+1,1,item2);

        QTableWidgetItem *item3 = new QTableWidgetItem(QString::number(y));
        ui->tableWidget_2->setItem(i+1,2,item3);

        QTableWidgetItem *item4 = new QTableWidgetItem(QString::number(x2));
        ui->tableWidget_2->setItem(i+1,3,item4);

        QTableWidgetItem *item5 = new QTableWidgetItem(QString::number(y2));
        ui->tableWidget_2->setItem(i+1,4,item5);

        QTableWidgetItem *item6 = new QTableWidgetItem(word);
        ui->tableWidget_2->setItem(i+1,5,item6);

    }
    ui->pushButton_7->setEnabled(true);
}


void MainWindow::addTableData(QVector<Ievent>& ie){
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

    for(int i = 0; i < ie.size() ; i++){
        ie[i].getIevents(event,image_file,eye,start_time,end_time,duration,start_x,start_y,end_x,end_y,amplitude,peak_velocity, pupil,resx,resy);
        ui->tableWidget->setRowCount(i+2);
        QTableWidgetItem *item = new QTableWidgetItem(event);
        ui->tableWidget->setItem(i+1,0,item);

        if(image_file==0)
            image_file="0";
        QTableWidgetItem *item2 = new QTableWidgetItem(image_file);
        ui->tableWidget->setItem(i+1,1,item2);

        if(eye==0)
            eye="0";
        QTableWidgetItem *item3 = new QTableWidgetItem(eye);
        ui->tableWidget->setItem(i+1,2,item3);

        QTableWidgetItem *item4 = new QTableWidgetItem(QString::number(start_time));
        ui->tableWidget->setItem(i+1,3,item4);

        QTableWidgetItem *item5 = new QTableWidgetItem(QString::number(end_time));
        ui->tableWidget->setItem(i+1,4,item5);

        QTableWidgetItem *item6 = new QTableWidgetItem(QString::number(duration));
        ui->tableWidget->setItem(i+1,5,item6);

        QTableWidgetItem *item7 = new QTableWidgetItem(QString::number(start_x));
        ui->tableWidget->setItem(i+1,6,item7);

        QTableWidgetItem *item8 = new QTableWidgetItem(QString::number(start_y));
        ui->tableWidget->setItem(i+1,7,item8);

        QTableWidgetItem *item9 = new QTableWidgetItem(QString::number(end_x));
        ui->tableWidget->setItem(i+1,8,item9);

        QTableWidgetItem *item10 = new QTableWidgetItem(QString::number(end_y));
        ui->tableWidget->setItem(i+1,9,item10);

        QTableWidgetItem *item11 = new QTableWidgetItem(QString::number(amplitude));
        ui->tableWidget->setItem(i+1,10,item11);

        QTableWidgetItem *item12 = new QTableWidgetItem(QString::number(peak_velocity));
        ui->tableWidget->setItem(i+1,11,item12);

        QTableWidgetItem *item13 = new QTableWidgetItem(QString::number(pupil));
        ui->tableWidget->setItem(i+1,12,item13);

        QTableWidgetItem *item14 = new QTableWidgetItem(QString::number(resx));
        ui->tableWidget->setItem(i+1,13,item14);

        QTableWidgetItem *item15 = new QTableWidgetItem(QString::number(resy));
        ui->tableWidget->setItem(i+1,14,item15);
    }

    ui->pushButton_6->setEnabled(true);
}


void MainWindow::addAOITableHeader(){

    ui->tableWidget_2->setColumnCount(6);
     ui->tableWidget_2->setRowCount(1);

    //table header
    QTableWidgetItem *item1 = new QTableWidgetItem("Index");
    ui->tableWidget_2->setItem(0,0,item1);

    QTableWidgetItem *item2 = new QTableWidgetItem("X1");
    ui->tableWidget_2->setItem(0,1,item2);

    QTableWidgetItem *item3 = new QTableWidgetItem("Y1");
    ui->tableWidget_2->setItem(0,2,item3);

    QTableWidgetItem *item4 = new QTableWidgetItem("X2");
    ui->tableWidget_2->setItem(0,3,item4);

    QTableWidgetItem *item5 = new QTableWidgetItem("Y2");
    ui->tableWidget_2->setItem(0,4,item5);

    QTableWidgetItem *item6 = new QTableWidgetItem("Word");
    ui->tableWidget_2->setItem(0,5,item6);




}

void MainWindow::addTableHeader(){

    ui->tableWidget->setColumnCount(15);
     ui->tableWidget->setRowCount(1);

    //table header
    QTableWidgetItem *item1 = new QTableWidgetItem("event");
    ui->tableWidget->setItem(0,0,item1);

    QTableWidgetItem *item2 = new QTableWidgetItem("image_file");
    ui->tableWidget->setItem(0,1,item2);

    QTableWidgetItem *item3 = new QTableWidgetItem("eye");
    ui->tableWidget->setItem(0,2,item3);

    QTableWidgetItem *item4 = new QTableWidgetItem("start_time");
    ui->tableWidget->setItem(0,3,item4);

    QTableWidgetItem *item5 = new QTableWidgetItem("end_time");
    ui->tableWidget->setItem(0,4,item5);

    QTableWidgetItem *item6 = new QTableWidgetItem("duration");
    ui->tableWidget->setItem(0,5,item6);

    QTableWidgetItem *item7 = new QTableWidgetItem("start_x");
    ui->tableWidget->setItem(0,6,item7);

    QTableWidgetItem *item8 = new QTableWidgetItem("start_y");
    ui->tableWidget->setItem(0,7,item8);

    QTableWidgetItem *item9 = new QTableWidgetItem("end_x");
    ui->tableWidget->setItem(0,8,item9);

    QTableWidgetItem *item10 = new QTableWidgetItem("end_y");
    ui->tableWidget->setItem(0,9,item10);

    QTableWidgetItem *item11 = new QTableWidgetItem("amplitude");
    ui->tableWidget->setItem(0,10,item11);

    QTableWidgetItem *item12 = new QTableWidgetItem("peak_velocity");
    ui->tableWidget->setItem(0,11,item12);

    QTableWidgetItem *item13 = new QTableWidgetItem("pupil");
    ui->tableWidget->setItem(0,12,item13);

    QTableWidgetItem *item14 = new QTableWidgetItem("resx");
    ui->tableWidget->setItem(0,13,item14);

    QTableWidgetItem *item15 = new QTableWidgetItem("resy");
    ui->tableWidget->setItem(0,14,item15);
}

//update visualization
//converts table2 to AOI vector and sends to drawing
void MainWindow::on_pushButton_3_clicked()
{
    QVector<AOI> aois;
    //loop over table_2 and push data into AOI vector
    for(int i=0; i<ui->tableWidget_2->rowCount();i++){
        if(ui->tableWidget_2->item(i,0)!=0 && ui->tableWidget_2->item(i,1)!=0 && ui->tableWidget_2->item(i,2)!=0 && ui->tableWidget_2->item(i,3)!=0 && ui->tableWidget_2->item(i,4)!=0 && ui->tableWidget_2->item(i,5)!=0){
            AOI *area = new AOI(0,ui->tableWidget_2->item(i,0)->text(),ui->tableWidget_2->item(i,1)->text().toInt(),ui->tableWidget_2->item(i,2)->text().toInt(),ui->tableWidget_2->item(i,3)->text().toInt(),ui->tableWidget_2->item(i,4)->text().toInt(),ui->tableWidget_2->item(i,5)->text());
            aois.push_back(*area);
        }
    }
    tview->updateAOIs(aois, ui->checkBox->checkState(), ui->checkBox_2->checkState(), ui->checkBox_3->checkState());

}

//add AOI at row - from lineedit
void MainWindow::on_pushButton_4_clicked()
{
    if(ui->lineEdit->text()!=0)
        if(ui->lineEdit->text().toInt()>0 && ui->lineEdit->text().toInt()<ui->tableWidget_2->rowCount()+1)
            ui->tableWidget_2->insertRow(ui->lineEdit->text().toInt());

    for(int i=1; i<ui->tableWidget_2->rowCount();i++){
        QTableWidgetItem *item = new QTableWidgetItem(QString::number(i));
        ui->tableWidget_2->setItem(i,0,item);
    }

}
//remove AOI at row - from lineedit2
void MainWindow::on_pushButton_5_clicked()
{
    if(ui->lineEdit_2->text()!=0)
        if(ui->lineEdit_2->text().toInt()>0 && ui->lineEdit_2->text().toInt()<ui->tableWidget_2->rowCount()+1)
            ui->tableWidget_2->removeRow(ui->lineEdit_2->text().toInt());

    for(int i=1; i<ui->tableWidget_2->rowCount();i++){
        QTableWidgetItem *item = new QTableWidgetItem(QString::number(i));
        ui->tableWidget_2->setItem(i,0,item);
    }
}


//update fixation visualization
void MainWindow::on_pushButton_8_clicked()
{
    QVector<Ievent> ie;
    //loop over table and push data into ie vector
    for(int i=1; i<ui->tableWidget->rowCount();i++){
            Ievent *event = new Ievent(ui->tableWidget->item(i,0)->text(),
                                       ui->tableWidget->item(i,1)->text(),
                                       ui->tableWidget->item(i,2)->text(),
                                       ui->tableWidget->item(i,3)->text(),
                                       ui->tableWidget->item(i,4)->text(),
                                       ui->tableWidget->item(i,5)->text(),
                                       ui->tableWidget->item(i,6)->text(),
                                       ui->tableWidget->item(i,7)->text(),
                                       ui->tableWidget->item(i,8)->text(),
                                       ui->tableWidget->item(i,9)->text(),
                                       ui->tableWidget->item(i,10)->text(),
                                       ui->tableWidget->item(i,11)->text(),
                                       ui->tableWidget->item(i,12)->text(),
                                       ui->tableWidget->item(i,13)->text(),
                                       ui->tableWidget->item(i,14)->text());
            ie.push_back(*event);

    }
    tview->updateIevents(ie, ui->checkBox->checkState(), ui->checkBox_2->checkState(), ui->checkBox_3->checkState());
    ui->horizontalSlider->setRange(1,ie.size());

}

//remove fixations less than lineedit 4
//delete row idea
void MainWindow::on_pushButton_10_clicked()
{
    if(ui->lineEdit_4->text()!=0)
        for(int i=3; i<ui->tableWidget->rowCount(); i++)
        if(ui->lineEdit_4->text().toInt() > 0 && ui->lineEdit_4->text().toInt() > ui->tableWidget->item(i,5)->text().toInt() && ui->tableWidget->item(i,0)->text()=="EFIX")
            ui->tableWidget->removeRow(i);
    //update statistics
    fixationStats();
}

//remove fixations more than lineedit 3
//delete row idea
void MainWindow::on_pushButton_9_clicked()
{
    if(ui->lineEdit_3->text()!=0)
        for(int i=3; i<ui->tableWidget->rowCount(); i++)
        if(ui->lineEdit_3->text().toInt() > 0 && ui->lineEdit_3->text().toInt() < ui->tableWidget->item(i,5)->text().toInt() && ui->tableWidget->item(i,0)->text()=="EFIX")
            ui->tableWidget->removeRow(i);
    //update statistics
    fixationStats();
}

//make stats about fixations
void MainWindow::fixationStats()
{
    ui->textEdit->clear();

    int fix = 0, sac = 0, max_fix = 0, min_fix = 1000, max_sac = 0, min_sac = 1000, fix_sum = 0, sac_sum = 0;

    for(int i=3; i<ui->tableWidget->rowCount(); i++){
        if(ui->tableWidget->item(i,0)->text()=="EFIX"){
            fix++;
            fix_sum += ui->tableWidget->item(i,5)->text().toInt();

            if(ui->tableWidget->item(i,5)->text().toInt() > max_fix)
                max_fix = ui->tableWidget->item(i,5)->text().toInt();

            if(ui->tableWidget->item(i,5)->text().toInt() < min_fix)
                min_fix = ui->tableWidget->item(i,5)->text().toInt();
        }
        if(ui->tableWidget->item(i,0)->text()=="ESACC"){
            sac++;
            sac_sum += ui->tableWidget->item(i,5)->text().toInt();
            if(ui->tableWidget->item(i,5)->text().toInt() > max_sac)
                max_sac = ui->tableWidget->item(i,5)->text().toInt();

            if(ui->tableWidget->item(i,5)->text().toInt() < min_sac)
                min_sac = ui->tableWidget->item(i,5)->text().toInt();
        }
    }
    ui->textEdit->append("----------------------------------");
    ui->textEdit->append("number of fixations = "+QString::number(fix));
    ui->textEdit->append("number of saccades = "+QString::number(sac));

    ui->textEdit->append("\nmax fixation = "+QString::number(max_fix));
    ui->textEdit->append("min fixation = "+QString::number(min_fix));
    ui->textEdit->append("avg fixation = "+QString::number(fix_sum/fix));

    ui->textEdit->append("\nmax saccade = "+QString::number(max_sac));
    ui->textEdit->append("min saccade = "+QString::number(min_sac));
    ui->textEdit->append("avg saccade = "+QString::number(sac_sum/sac));
}


//save eye events into CSV file from table
void MainWindow::on_pushButton_6_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),"",tr("Files (*.CSV)"));
    QFile file(fileName);

    if(!file.open(QIODevice::ReadWrite))
        ui->statusBar->showMessage("Save Error");

    QTextStream out(&file);
    int r=ui->tableWidget->rowCount(),c=ui->tableWidget->columnCount();

    for (int x = 0; x < r; x++){
        for (int i = 0; i < c; i++){
            if(ui->tableWidget->item(x,i)==0)
                out<<",";
            else
                out<<ui->tableWidget->item(x,i)->text()<<",";
        }
    out<<"\r\n";
    }

    file.close();

}

//save eye AOIs + analysis into CSV file from table2
void MainWindow::on_pushButton_7_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),"",tr("Files (*.CSV)"));
    QFile file(fileName);

    if(!file.open(QIODevice::ReadWrite))
        ui->statusBar->showMessage("Save Error");

    QTextStream out(&file);
    int r=ui->tableWidget_2->rowCount(),c=ui->tableWidget_2->columnCount();

    for (int x = 0; x < r; x++){
        for (int i = 0; i < c; i++){
            if(ui->tableWidget_2->item(x,i)==0)
                out<<",";
            else
                out<<ui->tableWidget_2->item(x,i)->text()<<",";
        }
    out<<"\r\n";
    }

    file.close();
}


//overwrite AOI file - tabs and 0 RECTANGLE
void MainWindow::on_pushButton_2_clicked()
{
    QString AOI_path = ui->tableWidget->item(2,1)->text();

    QString path = AOI_path.mid(4,-1);
    ui->statusBar->showMessage(path + " saved");


    QFile file(path);

    if(!file.open(QIODevice::ReadWrite | QIODevice::Truncate))
        ui->statusBar->showMessage("Save AOI Error");

    QTextStream out(&file);
    int r=ui->tableWidget_2->rowCount();

    for (int x = 1; x < r; x++){
        out<<"0 RECTANGLE\t";
        for (int i = 0; i < 6; i++){
            out<<ui->tableWidget_2->item(x,i)->text()<<"\t";
        }
    out<<"\r\n";
    }

    file.close();


}

//run AOI analysis
void MainWindow::on_pushButton_11_clicked()
{
    ui->tableWidget_2->setColumnCount(6);

    ui->tableWidget_2->setColumnCount(11);
    QTableWidgetItem *item = new QTableWidgetItem("First fix time");
    ui->tableWidget_2->setItem(0,6,item);

    QTableWidgetItem *item2 = new QTableWidgetItem("AOI skipped");
    ui->tableWidget_2->setItem(0,7,item2);

    QTableWidgetItem *item3 = new QTableWidgetItem("Total time");
    ui->tableWidget_2->setItem(0,8,item3);

    QTableWidgetItem *item4 = new QTableWidgetItem("Total fix #");
    ui->tableWidget_2->setItem(0,9,item4);

    QTableWidgetItem *item5 = new QTableWidgetItem("Regression");
    ui->tableWidget_2->setItem(0,10,item5);

    firstFixationAnalysis();
    totalTimeAnalysis();
    regressionAnalysis();

    //regression
    if(!ui->checkBox_7->checkState()){
        ui->tableWidget_2->removeColumn(10);
       // ui->tableWidget_2->setColumnCount(ui->tableWidget_2->columnCount()-1);
    }

    //tital fixation time + number
    if(!ui->checkBox_4->checkState()){
        ui->tableWidget_2->removeColumn(8);
        ui->tableWidget_2->removeColumn(8);
        //ui->tableWidget_2->setColumnCount(ui->tableWidget_2->columnCount()-2);
    }

    //skipping informaiton
    if(!ui->checkBox_6->checkState()){
        ui->tableWidget_2->removeColumn(7);
       // ui->tableWidget_2->setColumnCount(ui->tableWidget_2->columnCount()-1);
    }


    //first fixation time
    if(!ui->checkBox_5->checkState()){
        ui->tableWidget_2->removeColumn(6);
        //ui->tableWidget_2->setColumnCount(ui->tableWidget_2->columnCount()-1);
    }

}

//adds first fixation time to AOI analysis in table2
void MainWindow::firstFixationAnalysis()
{
    int x1,y1,x2,y2;
    int fixx,fixy,duration;
    bool found = false;

    //going over every AOI
    for(int i = 1; i < ui->tableWidget_2->rowCount(); i++){
        found = false;
        x1 = ui->tableWidget_2->item(i,1)->text().toInt();
        y1 = ui->tableWidget_2->item(i,2)->text().toInt();
        x2 = ui->tableWidget_2->item(i,3)->text().toInt();
        y2 = ui->tableWidget_2->item(i,4)->text().toInt();

        //going over fixations
        int f = 3;
        while( f < ui->tableWidget->rowCount() && !found){

            if(ui->tableWidget->item(f,0)->text()=="EFIX"){
                fixx = ui->tableWidget->item(f,6)->text().toFloat();
                fixy = ui->tableWidget->item(f,7)->text().toFloat();
                duration = ui->tableWidget->item(f,5)->text().toInt();

                if( x1<fixx && fixx<x2 && y1<fixy && fixy<y2 ){
                    QTableWidgetItem *item = new QTableWidgetItem(QString::number(duration));
                    ui->tableWidget_2->setItem(i,6,item);
                    found = true;
                }

            }
        f++;
        }
        if(found == false){
            QTableWidgetItem *item = new QTableWidgetItem("skipped");
            ui->tableWidget_2->setItem(i,7,item);

        }

    }

}


//adds total time data to table2
void MainWindow::totalTimeAnalysis()
{
    int x1,y1,x2,y2;
    int fixx,fixy,duration;
    int total_time;
    int fix_num;

    //going over every AOI
    for(int i = 1; i < ui->tableWidget_2->rowCount(); i++){
        total_time = 0;
        fix_num = 0;
        x1 = ui->tableWidget_2->item(i,1)->text().toInt();
        y1 = ui->tableWidget_2->item(i,2)->text().toInt();
        x2 = ui->tableWidget_2->item(i,3)->text().toInt();
        y2 = ui->tableWidget_2->item(i,4)->text().toInt();

        //going over fixations
        int f = 3;
        while( f < ui->tableWidget->rowCount()){

            if(ui->tableWidget->item(f,0)->text()=="EFIX"){
                fixx = ui->tableWidget->item(f,6)->text().toFloat();
                fixy = ui->tableWidget->item(f,7)->text().toFloat();
                duration = ui->tableWidget->item(f,5)->text().toInt();

                if( x1<fixx && fixx<x2 && y1<fixy && fixy<y2 ){
                   total_time += duration;
                   fix_num++;
                }

            }
        f++;
        }
        QTableWidgetItem *item = new QTableWidgetItem(QString::number(total_time));
        ui->tableWidget_2->setItem(i,8,item);

        QTableWidgetItem *item2 = new QTableWidgetItem(QString::number(fix_num));
        ui->tableWidget_2->setItem(i,9,item2);
    }
}


//regression analysis:
//regression in/out/return sweep
//regression inter-AOI/intra-AOI
void MainWindow::regressionAnalysis(){

    int x1,y1,x2,y2;
    int startx, starty, endx, endy;


    //going over every AOI
    for(int i = 1; i < ui->tableWidget_2->rowCount(); i++){

        x1 = ui->tableWidget_2->item(i,1)->text().toInt();
        y1 = ui->tableWidget_2->item(i,2)->text().toInt();
        x2 = ui->tableWidget_2->item(i,3)->text().toInt();
        y2 = ui->tableWidget_2->item(i,4)->text().toInt();

        //going over fixations
        int f = 3;
        while( f < ui->tableWidget->rowCount()){

            if(ui->tableWidget->item(f,0)->text()=="ESACC"){
                startx = ui->tableWidget->item(f,6)->text().toFloat();
                starty = ui->tableWidget->item(f,7)->text().toFloat();
                endx = ui->tableWidget->item(f,8)->text().toFloat();
                endy = ui->tableWidget->item(f,9)->text().toFloat();
                //duration = ui->tableWidget->item(f,5)->text().toInt();

                if(startx > endx){//regression
                    if( x1<startx && startx<x2 && y1<starty && starty<y2 && x1<endx && endx<x2 && y1<endy && endy<y2){
                        //intra-AOI regression
                        QTableWidgetItem *item = new QTableWidgetItem("intra-AOI");
                        ui->tableWidget_2->setItem(i,10,item);
                    }
                    else
                        if(x1<startx && startx<x2 && y1<starty && starty<y2 && x1>endx && endx<x2 ){
                            //inter-AOI-out regression
                            if(i==ui->tableWidget_2->rowCount()-1){
                                QTableWidgetItem *item = new QTableWidgetItem("return-sweep");
                                ui->tableWidget_2->setItem(i,10,item);
                            }
                            else{
                                QTableWidgetItem *item = new QTableWidgetItem("inter-AOI-out");
                                ui->tableWidget_2->setItem(i,10,item);
                            }
                        }
                        else
                            if(x1<startx && startx>x2 && x1<endx && endx<x2 && y1<endy && endy<y2){
                                //inter-AOI-in regression
                                QTableWidgetItem *item = new QTableWidgetItem("inter-AOI-in");
                                ui->tableWidget_2->setItem(i,10,item);
                            }
                }
            }
        f++;
        }
//        QTableWidgetItem *item = new QTableWidgetItem(QString::number(total_time));
//        ui->tableWidget_2->setItem(i,8,item);

//        QTableWidgetItem *item2 = new QTableWidgetItem(QString::number(fix_num));
//        ui->tableWidget_2->setItem(i,9,item2);
    }



}


