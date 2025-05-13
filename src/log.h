#ifndef LOG_H
#define LOG_H

#include <fstream>
#include <string>
#include <ctime>

class Logger {
public:
    Logger(const std::string& filename = "render.log") {
        log_file.open(filename, std::ios::app);  // Append mode
    }

    ~Logger() {
        if (log_file.is_open()) {
            log_file.close();
        }
    }

    void log(const std::string& message) {
        if (!log_file.is_open()) return;

        auto now = std::time(nullptr);
        char time_buf[100];
        std::strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));

        log_file << "[" << time_buf << "] " << message << "\n";
    }

private:
    std::ofstream log_file;
};

#endif