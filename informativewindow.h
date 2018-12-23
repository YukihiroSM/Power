#ifndef INFORMATIVEWINDOW_H
#define INFORMATIVEWINDOW_H

#include <QDialog>
#include <QWidget>
#include <QAction>

namespace Ui {
class InformativeWindow;
}

class InformativeWindow : public QDialog
{
    Q_OBJECT

public:
    explicit InformativeWindow(QWidget *parent = nullptr);
    ~InformativeWindow();
signals:
    void baseWindow();
private slots:
    void on_pushButton_clicked();

private:
    Ui::InformativeWindow *ui;
};

#endif // INFORMATIVEWINDOW_H
