#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <utility>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    myOpenGL = new GLWidget;
    ui->verticalLayout1->addWidget(myOpenGL);

    //clock_t start = clock();
    ui->Edit_RobotInitPosX->setText("-100");
    ui->Edit_RobotInitPosY->setText("-100");
    ui->Edit_TargetInitPosX->setText("100");
    ui->Edit_TargetInitPosY->setText("100");
    ui->Edit_Iteration->setText("100");

    ui->Edit_GA_Gen->setText("0");
    ui->Edit_GA_Population->setText("10");
    ui->Edit_GA_CO_Rate->setText("0.6");
    ui->Edit_GA_MU_Rate->setText("0.1");
    ui->Edit_GA_Generation->setText("10");
    ui->Edit_GA_Delay->setEnabled(0);
    ui->Edit_GA_Delay->setText("100");

    ui->spinBox_Obstacle->setRange(0,10);
    ui->spinBox_Obstacle->setValue(2);
    ui->spinBox_Node->setRange(0,100);
    ui->spinBox_Node->setValue(8);
    ui->statusBar->showMessage(tr("Robot Soccer Simulator - ERSOW PENS 2017____Ready!!!"), 120000);

    ui->Label_InitResult->setText("Node : 0 - Obs : 0");
    ui->Label_ViaPointTime->setText("Time : 0 s");
    ui->Label_Result_Jalur->setText("Jalur Terbaik : 0 px");
    ui->Label_Result_Generasi->setText("Dicapai Pada Generasi ke - 0");
    ui->Label_Result_Time->setText("Execution Time : 0 s");

    srand(time(NULL));    
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SortedNode()
{
    for(int i = 0; i < (nodeCounter - 1); i++){
        for(int j = 0; j < (nodeCounter - 1); j++){
            if(MyNode[j].x > MyNode[j+1].x){                
                std::swap(MyNode[j], MyNode[j+1]);
            }
        }
    }
}

float MainWindow::AcakRill()
{
    return rand()/float(RAND_MAX);
}

float MainWindow::GetDistance(int populasi)
{
    float distance = 0;
    int temp[2];
    temp[0] = MyNode[0].x;
    temp[1] = MyNode[0].y;
    for(int i = 1; i <= UkuranKromosom; i++){
        if(i != 1){
            if(Kromosom[populasi][i] == 1){
                distance += sqrt(powf(MyNode[i-1].x - temp[0], 2) + powf(MyNode[i-1].y - temp[1], 2));
                temp[0] = MyNode[i-1].x;
                temp[1] = MyNode[i-1].y;
            }
        }
    }
    return distance;
}

QString MainWindow::TampilKromosom(int i)
{
    QString xx;
    QTextStream(&xx)<<"Pop["<<i<<"] : "<<Kromosom[i][1]<<Kromosom[i][2]<<Kromosom[i][3]<<Kromosom[i][4]<<" "<<Kromosom[i][5]<<Kromosom[i][6]<<Kromosom[i][7]<<Kromosom[i][8]<<" "<<Kromosom[i][9]<<Kromosom[i][10]<<Kromosom[i][11]<<Kromosom[i][12]<<" "<<Kromosom[i][13]<<Kromosom[i][14]<<Kromosom[i][15]<<Kromosom[i][16];
    return xx;
}

void MainWindow::Inisialisasi(int LebarKromosom)
{
    QString xx;
    ui->List_InitPopulation->clear();

    for(int i = 1; i <= UkuranPopulasi; i++){
        for(int j = 1; j <= 16; j++){
            if(j==1 || j == LebarKromosom)  Kromosom[i][j] = 1;
            else if(j > LebarKromosom)      Kromosom[i][j] = 0;
            else{
                if(AcakRill() < 0.5)        Kromosom[i][j] = 0;
                else                        Kromosom[i][j] = 1;
            }
        }
        xx.clear();
        QTextStream(&xx)<<"Pop["<<i<<"] : "<<Kromosom[i][1]<<Kromosom[i][2]<<Kromosom[i][3]<<Kromosom[i][4]<<" "<<Kromosom[i][5]<<Kromosom[i][6]<<Kromosom[i][7]<<Kromosom[i][8]<<" "<<Kromosom[i][9]<<Kromosom[i][10]<<Kromosom[i][11]<<Kromosom[i][12]<<" "<<Kromosom[i][13]<<Kromosom[i][14]<<Kromosom[i][15]<<Kromosom[i][16];
        ui->List_InitPopulation->addItem(xx);
    }
}

void MainWindow::Evaluasi(int IndexGenerasi)
{
    QString xx;
    ui->List_Fitness->clear();
    for(int i = 1; i <= UkuranPopulasi; i++){
        xx.clear();
        Jarak[IndexGenerasi][i] = GetDistance(i);
        Objektif[IndexGenerasi][i] = 1/GetDistance(i);
        QTextStream(&xx)<<"Pop["<<i<<"] :"<<Objektif[IndexGenerasi][i];
        ui->List_Fitness->addItem(xx);
    }
}

void MainWindow::Historis(int IndexGenerasi)
{
    for(int i = 1; i <= UkuranPopulasi; i++){
        if(MaxObjectif < Objektif[IndexGenerasi][i]){
            MaxObjectif = Objektif[IndexGenerasi][i];
            BestPathDistance = Jarak[IndexGenerasi][i];
            myNumPath = 0;

            for(int j = 1; j <= UkuranKromosom; j++){
                KromosomMax[j] = Kromosom[i][j];
                if(Kromosom[i][j] == 1){
                    myOpenGL->Path[myNumPath].x = MyNode[j-1].x;
                    myOpenGL->Path[myNumPath].y = MyNode[j-1].y;
                    myNumPath++;
                }
            }
            myOpenGL->numPath = myNumPath;
            GenerasiMax = IndexGenerasi;
        }
    }
}

void MainWindow::Seleksi(int IndexGenerasi)
{
    float JumlahProb[101], Fitness[101], Prob[101];
    int Terseleksi[101];
    float JumlahFitness = 0, Acak = 0;
    Vektor KromosomSeleksi[101];

    JumlahFitness = 0;
    for(int i = 1; i <= UkuranPopulasi; i++){
        Fitness[i] = Objektif[IndexGenerasi][i];
        JumlahFitness += Fitness[i];
    }
    JumlahProb[0] = 0;
    for(int i = 1; i <= UkuranPopulasi; i++){
        Prob[i] = Fitness[i]/JumlahFitness;
        JumlahProb[i] = JumlahProb[i-1]+Prob[i];
    }
    int j=0;
    for(int i = 1; i <= UkuranPopulasi; i++){
        Acak = AcakRill();
        j = 1;
        while(Acak>JumlahProb[j])
            j++;
        Terseleksi[i] = j;
    }
    for(int i = 1; i <= UkuranPopulasi; i++){
        for(int j = 0; j <= UkuranKromosom; j++){
            KromosomSeleksi[i][j] = Kromosom[Terseleksi[i]][j];
        }
    }

    ui->List_Selection->clear();
    QString xx;
    for(int i=1; i<=UkuranPopulasi; i++){
        for(int j=1; j<=UkuranKromosom; j++){
            Kromosom[i][j] = KromosomSeleksi[i][j];
        }
        xx.clear();
        xx = TampilKromosom(i);
        ui->List_Selection->addItem(xx);
    }
}

void MainWindow::PilihInduk(MainWindow::Pasangan &Ortu, int &CacahInduk)
{
    CacahInduk = 0;
    for(int i = 1; i <= UkuranPopulasi; i++){
        if(AcakRill() <= ProbabilitasPenyilangan){
            cacah++;
            CacahInduk = CacahInduk + 1;
            if(CacahInduk%2 == 1)   Ortu[1][(CacahInduk/2)+1] = i;
            else                    Ortu[2][(CacahInduk/2)]   = i;
        }
    }
    if(CacahInduk%2==1)
        CacahInduk = CacahInduk - 1;
}

void MainWindow::Ganti(int nOrtu, int nGen, MainWindow::Pasangan Ortu)
{
    for(int k = 1; k <= nOrtu/2; k++){
        for(int j = 1; j <= nGen; j++){
            Kromosom[Ortu[1][k]][j] = KromosomSilang[2*k-1][j];
            Kromosom[Ortu[2][k]][j] = KromosomSilang[2*k][j];
        }
    }
}

void MainWindow::Penyilangan()
{
    Pasangan Induk;
    int Posisi = 0, BanyakInduk = 0;

    PilihInduk(Induk, BanyakInduk);

    if(BanyakInduk > 0){
        Posisi = rand()%(UkuranKromosom-1)+1;

        for(int i=1; i<=BanyakInduk/2; i++){
            for(int j=1; j<=Posisi; j++){
                KromosomSilang[2*i-1][j] = Kromosom[Induk[2][i]][j];
                KromosomSilang[2*i][j] = Kromosom[Induk[1][i]][j];
            }

            for(int j=Posisi+1; j<=UkuranKromosom; j++){
                KromosomSilang[2*i-1][j] = Kromosom[Induk[1][i]][j];
                KromosomSilang[2*i][j] = Kromosom[Induk[2][i]][j];
            }
        }
        Ganti(BanyakInduk, UkuranKromosom, Induk);
    }
    QString xx;
    ui->List_CrossOver->clear();
    for(int i = 1; i <= UkuranPopulasi; i++){
        xx.clear();
        xx = TampilKromosom(i);
        ui->List_CrossOver->addItem(xx);
    }
}

void MainWindow::Mutasi()
{
    for(int i=1; i<=UkuranPopulasi; i++){
        for(int j=1; j<=UkuranKromosom; j++){
            if(j==1 || j==UkuranKromosom){
                Kromosom[i][j] = 1;
            }else{
                if(AcakRill() <= ProbabilitasMutasi){
                    if(Kromosom[i][j]==1)	Kromosom[i][j] = 0;
                    else					Kromosom[i][j] = 1;
                }
            }
        }
    }
    QString xx;
    ui->List_Mutation->clear();
    for(int i = 1; i <= UkuranPopulasi; i++){
        xx.clear();
        xx = TampilKromosom(i);
        ui->List_Mutation->addItem(xx);
    }
}

void MainWindow::on_actionExit_triggered()
{
    this->window()->close();
}


void MainWindow::on_Button_GenerateData_clicked()
{
    QString xx;

    myOpenGL->MyTeam[0].x = RobotPosition[0] = ui->Edit_RobotInitPosX->text().toInt();
    myOpenGL->MyTeam[0].y = RobotPosition[1] = ui->Edit_RobotInitPosY->text().toInt();
    myOpenGL->Ball.x = BallPosition[0] = ui->Edit_TargetInitPosX->text().toInt();
    myOpenGL->Ball.y = BallPosition[1] = ui->Edit_TargetInitPosY->text().toInt();
    maxIteration = ui->Edit_Iteration->text().toInt();
    myObstacleNumber = ui->spinBox_Obstacle->text().toInt();
    myNodeNumber = ui->spinBox_Node->text().toInt();

//    qDebug()<<"Robot X : "<<RobotPosition[0]<<" - Robot Y : "<<RobotPosition[1];
//    qDebug()<<"Ball  X : "<<BallPosition[0]<<" - Ball Y  : "<<BallPosition[1];
//    qDebug()<<"Obs Num : "<<myObstacleNumber<<" - Node Num: "<<myNodeNumber;

    myOpenGL->MyTeam[0].x = RobotPosition[0];
    myOpenGL->MyTeam[0].y = RobotPosition[1];
    myOpenGL->numPlayer = 1;
    ObsCounter = 0;
    for(int i = 0; i < maxIteration; i++){
        int posX = (rand()%600);//rand()%abs(BallPosition[0] - RobotPosition[0]);
        int posY = (rand()%400);
        bool ok = true;
        for(int j = 0; j < ObsCounter; j++){
            int dist = sqrt(powf((posX-MyObstacle[j].x),2)+powf((posY-MyObstacle[j].y),2));
            if(dist < 25){
                ok=false; break;
            }
        }
        if(ok && myObstacleNumber){
            myOpenGL->Others[i].x = MyObstacle[i].x = posX - 300;
            myOpenGL->Others[i].y = MyObstacle[i].y = posY - 200;
            ObsCounter++;
        }
        if(ObsCounter >= myObstacleNumber)  break;
    }
    myOpenGL->numObstacle = ObsCounter;

    clock_t start = clock();
    MyNode[0].x = RobotPosition[0];
    MyNode[0].y = RobotPosition[1];
    MyNode[myNodeNumber - 1].x = BallPosition[0];
    MyNode[myNodeNumber - 1].y = BallPosition[1];

    myNodeNumber -= 2;
    nodeCounter = 0;

    if(myNodeNumber > 0){
        float delta = (abs(BallPosition[0]-RobotPosition[0]))/(myNodeNumber+1);
        for(int i = 1; i <= maxIteration; i++){
            int posX = RobotPosition[0] + (i%(myNodeNumber+1))*delta;
            int posY = (rand()%400)-200;

            bool ok = true;
            for(int j = 0; j < myObstacleNumber; j++){
                int dist = sqrt(powf((posX-MyObstacle[j+1].x),2)+powf((posY-MyObstacle[j+1].y),2));
                int minY = abs(posY - MyObstacle[j+1].y);
                if(dist < 100 || minY < 50){
                    ok = false; break;
                }
            }

            if(ok){
                for(int j=0; j<nodeCounter; j++){
                    int dist = sqrt(powf((posX-MyNode[j].x),2)+powf((posY-MyNode[j].y),2));
                    if((dist < 25) || (posX == MyNode[j].x)){
                        ok=false; break;
                    }
                }
            }
            if(ok && myNodeNumber){
                MyNode[nodeCounter+1].x = posX;
                MyNode[nodeCounter+1].y = posY;
                nodeCounter++;
            }
            if(nodeCounter >= myNodeNumber) break;
        }
        clock_t stop = clock();

        nodeCounter += 2;
        myOpenGL->numNode = nodeCounter;
//        for(int i = 0; i < nodeCounter; i++){
//            qDebug()<<"Node : "<<MyNode[i].x<<","<<MyNode[i].y;
//        }
        SortedNode();

        ui->List_viaPoint->clear();
        for(int i = 0; i < nodeCounter; i++){
            QString zz;
            QTextStream(&zz)<<"Point["<<i<<"] : "<<MyNode[i].x<<","<<MyNode[i].y;
            ui->List_viaPoint->addItem(zz);
            if(i != 0 || i != nodeCounter-1){
                myOpenGL->viaPoint[i].x = MyNode[i].x;
                myOpenGL->viaPoint[i].y = MyNode[i].y;
            }
        }
        xx.clear();
        QTextStream(&xx)<<"Node : "<<nodeCounter<<" - Obs : "<<ObsCounter;
        ui->Label_InitResult->setText(xx);
        xx.clear();
        QTextStream(&xx)<<"Time : "<<QString::number((double) (stop - start)/CLOCKS_PER_SEC,'f',6)<<" s";
        ui->Label_ViaPointTime->setText(xx);
        /*for(int i = 1; i < nodeCounter - 2; i++){
            qDebug()<<"viaPoint : "<<myOpenGL->viaPoint[i].x<<","<<myOpenGL->viaPoint[i].y;
        }*/
        ui->Edit_GA_Gen->setText(QString::number(nodeCounter));
    }
}

void MainWindow::on_Button_GA_Process_clicked()
{
    UkuranKromosom = ui->Edit_GA_Gen->text().toInt();
    UkuranPopulasi = ui->Edit_GA_Population->text().toInt();
    ProbabilitasPenyilangan = ui->Edit_GA_CO_Rate->text().toDouble();
    ProbabilitasMutasi = ui->Edit_GA_MU_Rate->text().toDouble();
    BanyakGenerasi = ui->Edit_GA_Generation->text().toInt();

    MaxObjectif = ShortedPath = myOpenGL->numPath = cacah = 0;

    clock_t start = clock();
    Inisialisasi(UkuranKromosom);
    for(int Generasi = 1; Generasi <= BanyakGenerasi; Generasi++){
        Evaluasi(Generasi);
        Historis(Generasi);
        Seleksi(Generasi);
        Penyilangan();
        Mutasi();

        if(ui->checkBox_Delay->isChecked()){
            QEventLoop loop;
            QTimer::singleShot(ui->Edit_GA_Delay->text().toInt(), &loop, SLOT(quit()));
            loop.exec();
        }
    }
    clock_t stop = clock();

    QString ii;
    QTextStream(&ii)<<"Jalur Terbaik : "<<BestPathDistance<<" px";
    ui->Label_Result_Jalur->setText(ii);

    ii.clear();
    QTextStream(&ii)<<"Dicapai Pada Generasi ke - "<<GenerasiMax;
    ui->Label_Result_Generasi->setText(ii);

    ii.clear();
    QTextStream(&ii)<<"Execution Time : "<<QString::number((double) (stop - start)/CLOCKS_PER_SEC,'f',6)<<" s";
    ui->Label_Result_Time->setText(ii);
}

void MainWindow::on_checkBox_Delay_stateChanged(int arg1)
{
    if(arg1 == 2)
        ui->Edit_GA_Delay->setEnabled(1);
    else if(arg1 == 0)
        ui->Edit_GA_Delay->setEnabled(0);
}
