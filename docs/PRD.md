# Project Requirements Document (PRD)

## 1. Project overview
______ A Linux driver that pretends to be a temperature sensor, and a C++ app that reads it, logs it and warns the user.

## 2. Scope
**In scope:** one device, one ioctl command, one C++ app, a log file on one side.
**Out of scope:**On the other side, real hardware, multiple sensors and a graphical interface.

## 3. Functional requirements
| ID  | Requirement |
|-----|-------------|
| FR1 | create /dev/mysensor when loaded |
| FR2 | read the current temperature |
| FR3 | set a threshold using ioctl |
| FR4 | log each reading with a timestamp |
| FR5 | show an alert when the temperature crosses the threshold |
| FR6 | reject an invalid ioctl command with an error code |

## 4. Non-functional requirements
| ID   | Requirement |
|------|-------------|
| NFR1 |uses a mutex so the timer and read() don't clash |
| NFR2 |loads and unloads cleanly with no errors in dmesg |
| NFR3 |returns proper error codes |
| NFR4 |code is commented and builds with one make|
| NFR5 |app exits cleanly on Ctrl+C|

## 5. System modules
- kernel driver, shared header with ioctl numbers, FileDescriptor class, Logger, AlertManager, test script

## 6. Deliverables
-source code, Stage 1 and 2 documents, design diagrams (UML), test results, GitHub repo, final report.
## 7. Timeline
| Date | Work |
|------|------|
| Oct 3 | setup, Stage 1 and 2 documents, hello-world module, design diagrams, driver skeleton |
| Oct 4 |read, ioctl, mutex, C++ app, testing, report, demo rehearsal |
| Oct 5 | buffer and final push only |

## 8. Risks
- VM lag, so lower the RAM; timer bugs, so compute a value on each read as a fallback; running short of time, so drop optional features first.