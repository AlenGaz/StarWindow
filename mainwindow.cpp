#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QDebug>
#include<QFile>
//ssh git 123123

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->lineEdit_2,SIGNAL(textChanged(QString)),this,SLOT(on_lineEdit_textChanged(QString)));
//****************************remove comments when database problem is fixed ...Simple..  ******************
//    QSqlDatabase db;
//    QSqlDatabase::addDatabase("QPSQL");
//    db.setHostName("basen.oru.se");
//    db.setDatabaseName("grupp2basen");
//    db.setUserName("grupp2");
//    db.setPassword("kaffebil");
//    db.setPort(5432);
//    qDebug()<<db.open();
//QSqlQuery *qr=new QSqlQuery(db);
//    qr->prepare("select * from grupp2basen");
//    qr->exec();
//    QSqlQueryModel *model;
//    model->setQuery(*qr);
//    ui->starsView->setModel(model);
//****************************************************************************************

//**********************************Comment this when the database problem is fixed***********************
    QStandardItemModel *model=new QStandardItemModel(this);
    QFile *CSVFile=new QFile;
    CSVFile->setFileName("stars.csv");
    CSVFile->open(QIODevice::ReadOnly);
    QStringList rowData,allRows;
 rowData.clear();allRows.clear();
    allRows=QString(CSVFile->readAll()).split('\n');
    model->setHorizontalHeaderLabels(allRows.first().split(','));
    model->setColumnCount(allRows.first().split(',').size());
    allRows.removeFirst();
    model->setRowCount(allRows.size());
    for(int x=0;x<allRows.size();x++){
        rowData=QString(allRows.at(x)).simplified().split(',');
        if(rowData.size()>=2){
        for(int y=0;y<rowData.size();y++)
        model->setItem(int(x),int(y),new QStandardItem(rowData[y]));
        }
    }
    ui->starsView->setModel(model);

CSVFile->close();
//********************************************************************

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    for(int i=0;i<ui->starsView->model()->rowCount();i++){

        QString selectedCol1=ui->starsView->model()->index(i,ui->comboBox->currentIndex()).data().toString();//Get what is the 1st parameter to be searched (id\name\class etc.)
        QString selectedCol2=ui->starsView->model()->index(i,ui->comboBox_2->currentIndex()).data().toString();//Get what is the 2nd parameter to be searched (id\name\class etc.)
        QStringList pointCoords=ui->pointCoords->text().split(',');//will return (x y z)
        QString line1Data=ui->lineEdit->text();//get the 1st search string
        QString line2Data=ui->lineEdit_2->text();//get the 2nd search string

        double fromMagnitude,toMagnitude,isMag1valid=true,fromMagnitude2,toMagnitude2,isMag2valid=true;
        double magnitude=ui->starsView->model()->index(i,3).data().toString().remove("\"").toDouble();//get magnitude of the current star.
        double ra=ui->starsView->model()->index(i,5).data().toString().remove("\"").toDouble();   //Get both astronomical coordinates(ra,dec)
        double dec=ui->starsView->model()->index(i,6).data().toString().remove("\"").toDouble();


        double dist=findDistance(ra,dec,pointCoords);//find distance between the given and star coordinates. Check function for detail
        double reqDist=ui->distFromPoint->value();//get the required distance to sort the table.(minimum req. distance)

        //each of the following will return a true or false value.
        bool distValid=(dist<=reqDist);//check if distance is valid.
        bool firstMatch=selectedCol1.contains(line1Data,Qt::CaseInsensitive);//check if first string from textbox is matched(id\name\class etc.)
        bool secondMatch=selectedCol2.contains(line2Data,Qt::CaseInsensitive);//check if second string from textbox is matched(id\name\class etc.)

        if(ui->comboBox->currentIndex()==3)//check if magnitude is selected in first combobox;
        {
             fromMagnitude=ui->magnitudeFrom->value();//get to and from magnitude.
             toMagnitude=ui->magnitudeTo->value();
             isMag1valid=(magnitude>=fromMagnitude&&magnitude<=toMagnitude);//check if it is valid(true if valid)
        }

        if(ui->comboBox_2->currentIndex()==3)//check if magnitude is selected in second combobox;
        {
             fromMagnitude2=ui->magnitudeFrom2->value();//get to and from magnitude from second search parameter.
             toMagnitude2=ui->magnitudeTo2->value();
             isMag2valid=(magnitude>=fromMagnitude2&&magnitude<=toMagnitude2);//check if it is valid(true if valid)
        }


        if(distValid&&firstMatch&&secondMatch&&isMag1valid&&isMag2valid)//if all above conditions are sarisfied then show the row.
            ui->starsView->showRow(i);
        else
            ui->starsView->hideRow(i);//else hide the row.
    }
}

double MainWindow::findDistance(double ra, double dec, QStringList pointCoords)
{
    double x1,y1,z1,x2,y2,z2,dist;
    //convert astronomical coordinates to cartesian coordinates.(formula)
    x1=qCos(qDegreesToRadians(ra))*qCos(qDegreesToRadians(dec));
    y1=qSin(qDegreesToRadians(ra))*qCos(qDegreesToRadians(dec));
    z1=qSin(qDegreesToRadians(dec));

    //get input point coordinates
    x2=pointCoords.at(0).toDouble();
    y2=pointCoords.at(1).toDouble();
    z2=pointCoords.at(2).toDouble();

    //formula to find distance between two coordinates.
    dist=qSqrt(qPow(x2-x1,2)+qPow(y2-y1,2)+qPow(z2-z1,2));

    qDebug()<<dist;
return dist;
}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    //if magnitude is selected then show the magnitude input range box.
    if(index==3)
        ui->stackedWidget->setCurrentIndex(1);
    else
        ui->stackedWidget->setCurrentIndex(0);//else show simple input textbox(to enter name,class,id etc.)
}

void MainWindow::on_comboBox_2_currentIndexChanged(int index)
{
    //if magnitude is selected,then show the magnitude input range box.(To , From)
    if(index==3)//index 3 is magnitude
        ui->stackedWidget_2->setCurrentIndex(1);
    else
        ui->stackedWidget_2->setCurrentIndex(0);//else show simple input textbox (to enter name,class,id etc.)
}
