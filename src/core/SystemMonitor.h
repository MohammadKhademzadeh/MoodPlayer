#pragma once

#include <QString>

class SystemMonitor {
public:
    SystemMonitor();

    double cpuUsage();
    QString activeWindow();

private:
    long previousIdle;
    long previousTotal;
};
