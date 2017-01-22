#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QEventLoop>

#include "glwidget.h"
#include <cstdlib>
#include <ctime>
#include <cmath>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
    //Structure
    struct MyData{
        int x;
        int y;
    };

    MyData MyObstacle[100];
    MyData MyNode[100];
    MyData MySwap[1];

    typedef int Vektor[101];
    typedef Vektor Solusi[101];
    typedef Vektor Pasangan[3];

    Solusi Kromosom, KromosomSilang;
    float Objektif[1001][101], Jarak[1001][101];
    double MaxObjectif, BestPathDistance, ProbabilitasPenyilangan, ProbabilitasMutasi;
    int KromosomMax[101];
    int UkuranPopulasi, UkuranKromosom, BanyakGenerasi, GenerasiMax;
    double ShortedPath;
    int RobotPosition[2], BallPosition[2];
    int myObstacleNumber, myNodeNumber, maxIteration;
    int nodeCounter, ObsCounter, myNumPath;
    float cacah;

private:
    Ui::MainWindow *ui;
    GLWidget *myOpenGL;

    void SortedNode(void);
    float AcakRill(void);
    float GetDistance(int populasi);
    QString TampilKromosom(int i);

    void Inisialisasi(int LebarKromosom);
    void Evaluasi(int IndexGenerasi);
    void Historis(int IndexGenerasi);
    void Seleksi(int IndexGenerasi);
    void PilihInduk(Pasangan& Ortu, int& CacahInduk);
    void Ganti(int nOrtu, int nGen, Pasangan Ortu);
    void Penyilangan(void);
    void Mutasi(void);
private slots:
    void on_actionExit_triggered();
    void on_Button_GenerateData_clicked();
    void on_Button_GA_Process_clicked();
    void on_checkBox_Delay_stateChanged(int arg1);
};

#endif // MAINWINDOW_H
