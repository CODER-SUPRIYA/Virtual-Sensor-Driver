#include <fcntl.h>
#include <poll.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <atomic>
#include <chrono>
#include <cstdint>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>

#include "../include/vsensor_ioctl.h"

static std::atomic<bool> g_run{true};
static void on_sigint(int) { g_run = false; }

/* RAII wrapper around the device file descriptor */
class SensorReader {
    int fd_;
public:
    explicit SensorReader(const std::string& path) {
        fd_ = ::open(path.c_str(), O_RDONLY);
        if (fd_ < 0) throw std::runtime_error("cannot open " + path);
    }
    ~SensorReader() { if (fd_ >= 0) ::close(fd_); }
    SensorReader(const SensorReader&) = delete;
    SensorReader& operator=(const SensorReader&) = delete;

    void setInterval(uint32_t ms) { ioctl(fd_, VSENSOR_SET_INTERVAL, &ms); }
    void setHigh(int32_t mC)      { ioctl(fd_, VSENSOR_SET_HIGH_THR, &mC); }
    void setLow(int32_t mC)       { ioctl(fd_, VSENSOR_SET_LOW_THR, &mC); }

    bool readOne(vsensor_reading& r, int timeout_ms) {
        pollfd p{fd_, POLLIN, 0};
        int n = ::poll(&p, 1, timeout_ms);
        if (n <= 0) return false;
        return ::read(fd_, &r, sizeof(r)) == (ssize_t)sizeof(r);
    }
};

class Logger {
    std::ofstream out_;
public:
    explicit Logger(const std::string& path) : out_(path, std::ios::app) {
        if (!out_) throw std::runtime_error("cannot open log " + path);
    }
    void log(const vsensor_reading& r) {
        std::time_t t = std::time(nullptr);
        out_ << std::put_time(std::localtime(&t), "%F %T") << ", "
             << r.temp_mC / 1000.0 << " C, status=0x"
             << std::hex << r.status << std::dec << "\n";
        out_.flush();
    }
};

class AlertManager {
    unsigned alerts_ = 0;
public:
    void check(const vsensor_reading& r) {
        if (r.status & VSENSOR_STATUS_HIGH) {
            std::cout << "  [ALERT] HIGH temperature!\n"; ++alerts_;
        }
        if (r.status & VSENSOR_STATUS_LOW) {
            std::cout << "  [ALERT] LOW temperature!\n"; ++alerts_;
        }
    }
    unsigned count() const { return alerts_; }
};

int main(int argc, char** argv) {
    int32_t high = 26000, low = 20000;
    uint32_t interval = 500;
    if (argc > 1) high = std::stoi(argv[1]);
    if (argc > 2) low = std::stoi(argv[2]);
    if (argc > 3) interval = std::stoul(argv[3]);

    signal(SIGINT, on_sigint);
    try {
        SensorReader sensor("/dev/vsensor0");
        Logger logger("vsensor.log");
        AlertManager alerts;

        sensor.setInterval(interval);
        sensor.setHigh(high);
        sensor.setLow(low);
        std::cout << "Monitoring (Ctrl+C to stop). high=" << high
                  << " low=" << low << " interval=" << interval << "ms\n";

        vsensor_reading r{};
        while (g_run) {
            if (!sensor.readOne(r, 2000)) continue;
            std::cout << std::fixed << std::setprecision(2)
                      << r.temp_mC / 1000.0 << " C\n";
            logger.log(r);
            alerts.check(r);
        }
        std::cout << "\nStopped. Alerts raised: " << alerts.count() << "\n";
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
