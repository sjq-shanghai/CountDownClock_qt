#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QTimer>
#include <QProcess>
#include <QSettings>
#include <QFileDialog>

#include "pausabletimer.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void stop();

public slots:
    void on_timerUpdate(int, int);
    void on_timerFinished();


    void on_startPushButton_clicked();
    void on_pausePushButton_clicked();
    void on_stopPushButton_clicked();

private slots:
    void on_exitPushButton_clicked();

private:
    Ui::MainWindow *ui;
    QString settingsFile;

    PausableTimer* timer;

    QString osName();

    int timeInSeconds(int hours, int minutes, int seconds);
    QString secondsInTimeString(int seconds);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    //记录鼠标，窗口位置
    QPoint windowPos;
    QPoint mousePos;
    QPoint dPos;
};

#endif // MAINWINDOW_H
