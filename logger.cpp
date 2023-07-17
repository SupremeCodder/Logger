#include "logger.h"

Logger::Logger(std::string filename, QObject *parent) : QObject(parent)
{
    file.open(filename);
}
//----------------------------------------------------------------------------------------------------------------------------
void Logger::writeToBuffer(unsigned char* data, quint8 id, quint8 length)
{
    DataStructure data_toBuff;
    data_toBuff.id = id;
    data_toBuff.length = length;
    data_toBuff.pointer = data;

    auto now_c = std::chrono::high_resolution_clock::now();

    auto msV = std::chrono::time_point_cast<std::chrono::milliseconds>(now_c);
    auto usV = std::chrono::time_point_cast<std::chrono::microseconds>(now_c);

    std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(msV.time_since_epoch()) % 1000;
    std::chrono::microseconds us = std::chrono::duration_cast<std::chrono::microseconds>(usV.time_since_epoch()) % 1000;

//    std::cout << us.count() << std::endl;
//    std::cout << ms.count() << std::endl;

    data_toBuff.ms = ms;
    data_toBuff.us = us;

    time_t now = time(0);
    tm *ltm = localtime(&now);
    data_toBuff.ltm = ltm;

    std::unique_lock<std::mutex> lock(mtx);

    buffer.push_back(data_toBuff);
    cv.notify_one();
}
//----------------------------------------------------------------------------------------------------------------------------
void Logger::writeToFile()
{
    while (true)
    {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this]{return !buffer.empty(); });

        DataStructure data_fromBuff;

        data_fromBuff.id = buffer.begin()->id;
        data_fromBuff.length = buffer.begin()->length;
        data_fromBuff.pointer = buffer.begin()->pointer;
        data_fromBuff.ltm = buffer.begin()->ltm;
        data_fromBuff.ms = buffer.begin()->ms;
        data_fromBuff.us = buffer.begin()->us;
        buffer.erase(buffer.begin());

        if (file.is_open())
        {
            file << "Time: " << data_fromBuff.ltm->tm_hour << ":" << data_fromBuff.ltm->tm_min << ":" << data_fromBuff.ltm->tm_sec
                                                           << ":" << data_fromBuff.ms.count() << ":" << data_fromBuff.us.count()
                 << "  ID: " << static_cast<int>(data_fromBuff.id)
                 << "  Length: " << static_cast<int>(data_fromBuff.length) <<  std::endl;

            for (int i = 0; i < data_fromBuff.length; i++)
            {
                file << "Data[" << i << "]: " << static_cast<int>(*(data_fromBuff.pointer + i)) << std::endl;
            }
        }
    }

}
//----------------------------------------------------------------------------------------------------------------------------
Logger::~Logger()
{
    file.close();
}
