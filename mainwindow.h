#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "logger.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    Logger *logger = nullptr;
    int writeBuff_time;

    unsigned char array[5] = {1,2,3,4,5};

    void timerEvent(QTimerEvent *event);

    std::atomic<int> counter;       //глобальный атомарный счетчик, необходимый для асинхронного вызова метода writeToBuffer

private:
    Ui::MainWindow *ui;
    std::thread writerThread;
    std::thread thread1;
    std::thread thread2;
    std::thread thread3;
    std::thread thread4;


};

#endif // MAINWINDOW_H
