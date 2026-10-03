# Virtual Sensor Driver: Final Project Report

**Author:** [Your full name]
**Training:** Linux, Device Drivers, System Programming and C++ (20-day program)
**Date:** 04 Oct 2026
**Repository:** https://github.com/CODER-SUPRIYA/Virtual-Sensor-Driver

## 1. Introduction and objective
Embedded and driver developers often need to test software before real hardware exists. This project builds a Linux kernel driver that simulates a temperature sensor, plus a C++ user-space application that monitors it. It demonstrates the full path from user space through the system call interface and VFS to a kernel driver.

## 2. Requirements (summary)
- Driver creates `/dev/vsensor0` and generates simulated readings at a configurable interval.
- `read()` (blocking and non-blocking), `poll()` and `ioctl()` (interval, high and low thresholds).
- `/proc/vsensor` status interface.
- C++ app that logs readings with timestamps and raises alerts when thresholds are crossed.
- Non-functional: no memory leaks, safe repeated load/unload, thread-safe shared state, readable code.
Full details are in `docs/PRD.md`.

## 3. Design
Architecture, class, sequence and state diagrams are in `docs/design.md`.
Key decisions:
- Temperatures are integers in milli-degrees C, because floating point is not allowed in kernel code.
- A delayed workqueue generates samples, because the timer API differs between kernel versions.
- `miscdevice` creates the device node automatically.
- A mutex protects shared state, since the work item, `read()` and `ioctl()` run concurrently.
- A wait queue plus `poll()` lets the app sleep until fresh data exists instead of busy-waiting.
- `struct vsensor_reading` (16 bytes) is shared by kernel and app via `include/vsensor_ioctl.h`.

## 4. Implementation
| Part | File | Notes |
|---|---|---|
| Kernel driver | `driver/vsensor.c` | misc device, delayed work, read/poll/ioctl, procfs |
| Shared header | `include/vsensor_ioctl.h` | struct, ioctl numbers, status flags |
| C++ app | `app/main.cpp` | `SensorReader` (RAII), `Logger`, `AlertManager` |
| Tests | `tests/run_tests.sh` | 12 automated checks |

Concepts applied: kernel vs user space, system calls, file operations, concurrency and synchronization, blocking I/O, signals (graceful Ctrl+C), RAII and C++17, little-endian layout and struct alignment of the 16-byte reading.

## 5. Testing and results
- Unit and integration checks (`bash tests/run_tests.sh`): **12 passed, 0 failed**. They cover builds, module load/unload, device and procfs nodes, 16-byte reads, continuous sampling, log writing and HIGH alerts.
- Memory check: Valgrind reported **0 errors and no leaks** (all heap blocks freed).
- System test: the app ran against the live driver, logged timestamped readings and raised alerts above the 26.0 C threshold (34 alerts in one run).
- Environment: Ubuntu 25.04 VM, kernel 7.0.0-38-generic, GCC 15.

## 6. Problems and solutions
(See `progress_log.md`.) Main ones: Makefile `$(PWD)` vs `$(CURDIR)`, tab handling in Makefiles, kernel timer API differences, a false test failure caused by `timeout` exit codes, and stage tags initially placed before merging.

## 7. Achievements
- Working kernel module and user-space application end to end.
- Automated tests and a clean memory check.
- Professional workflow: branches, pull requests, six stage tags and a progress log.

## 8. Limitations
- Data is simulated (bounded random walk).
- Single sensor instance, single reader state.
- Tested only on one kernel version.

## 9. Future improvements
Multiple sensors, sysfs attributes, device-tree binding, an epoll-based daemon, a ring buffer for history, and signed module support.
