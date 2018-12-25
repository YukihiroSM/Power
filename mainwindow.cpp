#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <powersetting.h>
#include <QDesktopServices>
#include <QUrl>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //Initializing auto_window
    secWindow = new Auto();

    //If secWindow -> baseWindow, so this -> show...

    connect(secWindow, &Auto::baseWindow, this, &MainWindow::show);
    connect(secWindow, &Auto::butt2clicked, this, &MainWindow::SetAutoMode);

    thirdWindow = new InformativeWindow();
    connect(thirdWindow, &InformativeWindow::baseWindow, this, &MainWindow::show);


    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(timerAlarm()));
    timer->start(1000);

    timer2 = new QTimer();
    connect(timer2, SIGNAL(timeout()), this, SLOT(timerAlarmAuto()));
    timer2->start(1000);

    fourthWindow = new Dialog();
    connect(fourthWindow, &Dialog::baseWindow, this, &MainWindow::show);

    fiveWindow = new UserInfo();
    connect(fiveWindow, &UserInfo::baseWindow, this, &MainWindow::show);

    _displayBrightness.ucDisplayPolicy = (UCHAR)ui->horizontalSlider->value();
    _displayBrightness.ucACBrightness = (UCHAR)ui->horizontalSlider->value();  //for testing purposes
    _displayBrightness.ucDCBrightness = (UCHAR)ui->horizontalSlider->value();

    DWORD ret = NULL;
    DWORD disp_nOutBufferSize = sizeof(_displayBrightness);
    HANDLE h = CreateFile(L"\\\\.\\LCD", GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, 0, nullptr);
    DeviceIoControl(h, IOCTL_VIDEO_SET_DISPLAY_BRIGHTNESS, (DISPLAY_BRIGHTNESS*)&_displayBrightness, disp_nOutBufferSize, nullptr, 0, &ret, nullptr);
    PowerGetActiveScheme(NULL, CurrentPowerPolicy);
    if(**CurrentPowerPolicy == GUID_MIN_POWER_SAVINGS)
    {
        ui->label_9->setText("Hight perfomance Mode");
    }
    else if(**CurrentPowerPolicy == GUID_MAX_POWER_SAVINGS)
    {
        ui->label_9->setText("Power Saving Mode");
    }
    else if (**CurrentPowerPolicy == GUID_TYPICAL_POWER_SAVINGS)
    {
        ui->label_9->setText("Balance Mode");
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_6_clicked()
{
    secWindow->show();
    this->close();
}

void MainWindow::on_pushButton_4_clicked()
{
    fourthWindow->show();
    this->close();
}

void MainWindow::on_toolButton_clicked()
{
    thirdWindow->show();
    this->close();
}

void MainWindow::on_pushButton_2_clicked()
{
    QMessageBox::StandardButton ans = QMessageBox::question(this, "PowerControl", "Усі не збережені данні будуть знищені.\n"
                          "Ви впевнені, що бажаєте вийти?", QMessageBox::Yes | QMessageBox::No);
    if(ans == QMessageBox::Yes) {

        qDebug() << "Exit" << endl;
        this->close();
    }
}

bool MainWindow::on_horizontalSlider_valueChanged(int value)
{
    _displayBrightness.ucDisplayPolicy = (UCHAR)value;
    _displayBrightness.ucACBrightness = (UCHAR)value;  //for testing purposes
    _displayBrightness.ucDCBrightness = (UCHAR)value;

    DWORD ret = NULL;

    DWORD nOutBufferSize = sizeof(_displayBrightness);
    HANDLE h = CreateFile(L"\\\\.\\LCD",
        GENERIC_READ | GENERIC_WRITE,
        0,
        nullptr,
        OPEN_EXISTING,
        0, nullptr);

    if (h == INVALID_HANDLE_VALUE) {
        //Does not reach here
        return false;
    }

    if (!DeviceIoControl(h, IOCTL_VIDEO_SET_DISPLAY_BRIGHTNESS, (DISPLAY_BRIGHTNESS*)&_displayBrightness, nOutBufferSize, nullptr, 0, &ret,
                    nullptr))
    {
        // GetLastError() returns error code 6 - Invalid handle
        return false;
    }
    else {
        return true;
    }
}

void MainWindow::timerAlarm()
{
    PowerGetActiveScheme(NULL, CurrentPowerPolicy);
    if(**CurrentPowerPolicy == GUID_MIN_POWER_SAVINGS)
    {
        qDebug() << "MinPowSav";
        mode = 1;
    }
    else if(**CurrentPowerPolicy == GUID_MAX_POWER_SAVINGS)
    {
        qDebug() << "MaxPowSav";
        mode = 3;
    }
    else if (**CurrentPowerPolicy == GUID_TYPICAL_POWER_SAVINGS)
    {
        qDebug() << "Typ";
        mode = 2;
    }

    CallNtPowerInformation(SystemBatteryState, nullptr, 0, &batStats, 512);

    if(batStats.BatteryPresent != TRUE)
        {
            QMessageBox::critical(this, "Помилка пошуку.", "Неможливо знайти жодної батареї.", QMessageBox::Ok);
            this->close();
        }

        QString text = QString::number(batStats.RemainingCapacity);
        //QMessageBox::information(this, "Title", text, QMessageBox::Ok);
        DWORD cap = batStats.RemainingCapacity;
        double Percent = ((double)cap/(double)batStats.MaxCapacity);
        string on = "background-image: url(:/img/100.png);";
        string off = "background-image: url(:/img/b100.png);";
        if(Percent < 0.25 && Percent >= 0)
        {
            on = "background-image: url(:/img/0.png);";
            off = "background-image: url(:/img/b0.png);";
        }
        else if(Percent < 0.5 && Percent >= 0.25)
        {
            on = "background-image: url(:/img/25.png);";
            off = "background-image: url(:/img/b25.png);";
        }
        else if(Percent < 0.75 && Percent >= 0.5)
        {
            on = "background-image: url(:/img/50.png);";
            off = "background-image: url(:/img/b50.png);";
        }
        else if(Percent < 1 && Percent >= 0.75)
        {
            on = "background-image: url(:/img/75.png);";
            off = "background-image: url(:/img/b75.png);";
        }
        else if(Percent == 1)
        {
            on = "background-image: url(:/img/100.png);";
            off = "background-image: url(:/img/b100.png);";
        }

        if(batStats.AcOnLine == TRUE)
        {
            ui->info->clear();
            ui->frame->styleSheet().clear();
            ui->frame->setStyleSheet(QString::fromStdString(on));
            ui->frame->show();
            ui->info->setText("Поточний стан: відбувається заряджання батареї від зовнішньої мережі.\n"
                              "Приблизна ємність, що є у данний момент на батареї:" + text + " mAh\n" +  QString::number(Percent*100) + "%");
            QFont f("MS Shell dlg 2", 12);
            ui->info->setFont(f);
            ui->info->show();
        }
        else if(batStats.AcOnLine == FALSE)
        {
            ui->info->clear();
            ui->frame->styleSheet().clear();
            ui->frame->setStyleSheet(QString::fromStdString(off));
            ui->frame->show();
            ui->info->setText("Поточний стан: живлення за рахунок батареї.\n"
                              "Приблизна ємність, що залишилась на батареї:" + text + " mAh\n" + QString::number(Percent*100) + "%\n"
                              "Час, що залишився на батареї: " + QString::number(batStats.EstimatedTime/60) + " хвилин.");
            QFont f("MS Shell dlg 2", 12);
            ui->info->setFont(f);
            ui->info->show();
        }

}

void MainWindow::on_action_3_triggered() //Default
{
    ui->horizontalSlider->setValue(50);
    PowerRestoreDefaultPowerSchemes();
    secWindow->setDef();
}


void MainWindow::on_action_4_triggered()
{
    this->hide();
    secWindow = new Auto(this);
    secWindow->show();
}

void MainWindow::on_pushButton_3_clicked() //default
{
    PowerRestoreDefaultPowerSchemes();
    ui->horizontalSlider->setValue(50);
}


void MainWindow::on_pushButton_clicked()
{
    QMessageBox::StandardButton ans = QMessageBox::question(this, "PowerControl", "Усі не збережені данні будуть знищені.\n"
                          "Ви впевнені, що бажаєте вийти?", QMessageBox::Yes | QMessageBox::No);
    if(ans == QMessageBox::Yes) {

        qDebug() << "Exit" << endl;
        this->close();
    }
}

void MainWindow::on_action_5_triggered()
{
    QMessageBox::StandardButton ans = QMessageBox::question(this, "PowerControl", "Усі не збережені данні будуть знищені.\n"
                          "Ви впевнені, що бажаєте вийти?", QMessageBox::Yes | QMessageBox::No);
    if(ans == QMessageBox::Yes) {

        qDebug() << "Exit" << endl;
        this->close();
    }
}

void MainWindow::on_action_triggered()
{
    QString path = QCoreApplication:: applicationDirPath();
    QDesktopServices::openUrl(QUrl(path + "/instructions.pdf"));
}



void MainWindow::on_pushButton_10_clicked()
{
    QMessageBox::StandardButton ans = QMessageBox::question(this, "PowerControl", "Усі не збережені данні будуть знищені.\n"
                          "Ви впевнені, що бажаєте вийти?", QMessageBox::Yes | QMessageBox::No);
    if(ans == QMessageBox::Yes) {

        qDebug() << "Exit" << endl;
        this->close();
    }
}

void MainWindow::on_pushButton_7_clicked()
{
    PowerSetActiveScheme(NULL, &GUID_MAX_POWER_SAVINGS);
    ui->horizontalSlider->setValue(0);
    qDebug() << "PowerSaving Activated";
    ui->label_9->clear();
    ui->label_9->setText("Power Saving Mode");
    ui->label_9->show();
}

void MainWindow::on_pushButton_8_clicked()
{
    PowerSetActiveScheme(NULL, &GUID_TYPICAL_POWER_SAVINGS);
    ui->horizontalSlider->setValue(60);
    qDebug() << "Typ Scheme Activated";
    ui->label_9->clear();
    ui->label_9->setText("Balance Mode");
    ui->label_9->show();
}

void MainWindow::on_pushButton_9_clicked()
{
    PowerSetActiveScheme(NULL, &GUID_MIN_POWER_SAVINGS);
    ui->horizontalSlider->setValue(100);
    qDebug() << "High Scheme Activated";
    ui->label_9->clear();
    ui->label_9->setText("Hight perfomance Mode");
    ui->label_9->show();
}

void MainWindow::SetAutoMode()
{
    this->show();
    int elect = secWindow->getElect();
    int batt = secWindow->getBatt();
    int perc = secWindow->getProc();
    int low = secWindow->getMode();

    if(perc == -2)
    {
        if(batStats.Charging == TRUE && batStats.AcOnLine == TRUE)
        {
            if(elect == 0)
                on_pushButton_9_clicked();
            if(elect == 1)
                on_pushButton_8_clicked();
            if(elect == 2)
                on_pushButton_7_clicked();
        }
        if(batStats.Charging == FALSE && batStats.AcOnLine == FALSE)
        {
            if(batt == 0)
                on_pushButton_9_clicked();
            if(batt == 1)
                on_pushButton_8_clicked();
            if(batt == 2)
                on_pushButton_7_clicked();
        }
    }
    else if(perc > 0)
    {
        DWORD cap = batStats.RemainingCapacity;
        double Percent = ((double)cap/(double)batStats.MaxCapacity);
        if( Percent <= perc)
         {   if(low == 0)
                on_pushButton_9_clicked();
            if(low == 1)
                on_pushButton_8_clicked();
            if(low == 2)
                on_pushButton_7_clicked();
           }
    }
}


void MainWindow::on_pushButton_5_clicked()
{
    ui->formGroupBox->hide();
    ui->label->setText("<html><head/><body><p align=\"center\"><span style=\" font-size:16pt;\">Дякуємо за ваш відгук. Його буде розглянуто, очікуйте відповіді.</span></p></body></html>");
}
