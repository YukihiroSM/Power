#ifndef USERINFO_H
#define USERINFO_H

#include <QDialog>

namespace Ui {
class UserInfo;
}

class UserInfo : public QDialog
{
    Q_OBJECT

public:
    explicit UserInfo(QWidget *parent = nullptr);
    ~UserInfo();
signals:
    void baseWindow();
private slots:
    void on_pushButton_clicked();
private:
    Ui::UserInfo *ui;
};

#endif // USERINFO_H
