#include "mainwindow.h"
#include "ui_mainwindow.h"

//----------------------------------------------------------------------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    logger = new Logger("log.bin");
    counter = 0;

    writerThread = std::thread([this](){
        logger->writeToFile();
    });

    thread1 = std::thread([this]()
    {
        while(true)
        {
            counter++;
            logger->writeToBuffer(array,1,5);
            printf("%u\n", counter.load());
//            std::cout << counter << std::endl;
            std::this_thread::sleep_for(std::chrono::microseconds(10));
        }
    });

    thread2 = std::thread([this]()
    {
        while(true)
        {
            counter++;
            logger->writeToBuffer(array,2,5);
            printf("%u\n", counter.load());
//            std::cout << counter << std::endl;
            std::this_thread::sleep_for(std::chrono::microseconds(10));
        }
    });
    thread3 = std::thread([this]()
    {
        while(true)
        {
            counter++;
            logger->writeToBuffer(array,3,5);
            printf("%u\n", counter.load());
//            std::cout << counter << std::endl;
            std::this_thread::sleep_for(std::chrono::microseconds(10));
        }
    });

    thread4 = std::thread([this]()
    {
        while(true)
        {
            counter++;
            logger->writeToBuffer(array,4,5);
            printf("%u\n", counter.load());
//            std::cout << counter << std::endl;
            std::this_thread::sleep_for(std::chrono::microseconds(10));
        }
    });


    writeBuff_time = startTimer(1000);
}
//----------------------------------------------------------------------------------------------------------------------------
MainWindow::~MainWindow()
{
    delete ui;
    delete logger;
    writerThread.join();
}

void MainWindow::timerEvent(QTimerEvent *event)
{
    int timer_id = event->timerId();

    if (timer_id == writeBuff_time)
    {
        counter++;
        logger->writeToBuffer(array,5,5);
        printf("%u\n", counter.load());
//        std::cout << counter << std::endl;
        std::this_thread::sleep_for(std::chrono::microseconds(10));
    }
}
