#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <fstream>
#include <iostream>
#include <ctime>
#include <chrono>
#include <vector>
#include <thread>
#include <atomic>
#include <condition_variable>

struct DataStructure
{
    quint8 id;
    quint8 length;
    unsigned char* pointer;
    tm *ltm;
    std::chrono::milliseconds ms;
    std::chrono::microseconds us;

};

class Logger : public QObject
{
    Q_OBJECT

public:
    Logger(std::string filename, QObject *parent = 0);
    ~Logger();

    std::vector<DataStructure> buffer;
    std::condition_variable cv;
    std::mutex mtx;

    void writeToBuffer(unsigned char* data, quint8 id, quint8 length);
    void writeToFile();
private:
    std::ofstream file;
};

#endif // LOGGER_H
