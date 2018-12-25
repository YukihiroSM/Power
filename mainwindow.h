#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <stdint.h>
#include <QMainWindow>
#include <QDebug>
#include <QWidget>
#include <QTimer>
#include <QTime>
#include <QMessageBox>
#include <QProcess>

#include <Windows.h>
#include <winnt.h>
#include <poclass.h>
#include <powrprof.h>
#include <physicalmonitorenumerationapi.h>
#include <highlevelmonitorconfigurationapi.h>

#include <string>
#include <ioapiset.h>
#include <ntddvdeo.h>
#include <Psapi.h>

#include <auto.h>
#include <informativewindow.h>
#include <dialog.h>
#include <userinfo.h>

using namespace std;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_pushButton_6_clicked();

    void on_toolButton_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_2_clicked();

    void timerAlarm();

    bool on_horizontalSlider_valueChanged(int value);

    void on_action_4_triggered();

    void on_pushButton_3_clicked();

    void on_action_3_triggered();

    void on_pushButton_clicked();

    void on_action_5_triggered();

    void on_action_triggered();

    void on_pushButton_10_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();
    void SetAutoMode();
    void on_pushButton_5_clicked();

private:
    Ui::MainWindow *ui;
    Ui::Auto *prev;
    Auto *secWindow = new Auto();
    InformativeWindow *thirdWindow;
    Dialog* fourthWindow;
    QTimer *timer;
    HMONITOR hMonitor;
    DISPLAY_BRIGHTNESS _displayBrightness;
    LPDWORD * pdwMinimumBrightness;
    LPDWORD * pdwCurrentBrightness;
    LPDWORD * pdwMaximumBrightness;
    UserInfo * fiveWindow;
    GUID ** CurrentPowerPolicy;
    int mode;
    SYSTEM_BATTERY_STATE batStats;

    QTimer *timer2;
};
void SetAutoMode(int, int, int, int);
#endif // MAINWINDOW_H
