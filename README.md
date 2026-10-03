# Virtual Sensor Driver

A Linux kernel character-device driver that simulates a temperature sensor, plus a C++ user-space monitoring application.

## Features
- Kernel module (`driver/vsensor.c`) creating `/dev/vsensor0`
- Timer-driven simulated readings (delayed workqueue), integer milli-degrees C
- `read()` (blocking and non-blocking), `poll()`, `ioctl()` for interval and thresholds
- `/proc/vsensor` status interface, mutex-protected shared state
- C++17 app (`app/`): `SensorReader` (RAII), `Logger`, `AlertManager`
- Automated test script and valgrind leak check

## Structure
| Folder | Contents |
|---|---|
| `driver/` | Kernel module and Makefile |
| `include/` | Shared header `vsensor_ioctl.h` |
| `app/` | C++ monitoring application |
| `tests/` | `run_tests.sh` |
| `docs/` | Introduction, PRD, design and UML |

## Build and run
```bash
make -C driver
make -C app
sudo insmod driver/vsensor.ko
cat /proc/vsensor
./app/vsensor_app 26000 20000 500   # high_mC low_mC interval_ms
sudo rmmod vsensor
```

## Tests
```bash
bash tests/run_tests.sh
```

## Environment
Ubuntu 25.04 VM (VirtualBox), kernel 7.0.0-38-generic, GCC 15.

## Limitations and future work
- Simulated data only (random walk)
- Single sensor instance
- Future: multiple sensors, sysfs attributes, epoll-based daemon, device tree# Virtual-Sensor-Driver
