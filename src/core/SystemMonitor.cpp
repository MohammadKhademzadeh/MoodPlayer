#include "SystemMonitor.h"
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

SystemMonitor::SystemMonitor() : previousIdle(0), previousTotal(0) {}

double SystemMonitor::cpuUsage() {
    QFile file("/proc/stat");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return 0.0;

    QTextStream in(&file);
    QStringList values = in.readLine().split(" ", Qt::SkipEmptyParts);
    file.close();

    long user = values[1].toLong();
    long nice = values[2].toLong();
    long system = values[3].toLong();
    long idle = values[4].toLong();

    long total = user + nice + system + idle;
    long diffTotal = total - previousTotal;
    long diffIdle = idle - previousIdle;

    previousTotal = total;
    previousIdle = idle;

    if (diffTotal <= 0)
        return 0.0;

    return 100.0 * (diffTotal - diffIdle) / diffTotal;
}

QString SystemMonitor::activeWindow() {
    Display* display = XOpenDisplay(nullptr);
    if (!display)
        return "Unknown";

    Window window;
    int revert;
    XGetInputFocus(display, &window, &revert);

    XClassHint hint;
    QString result = "Unknown";

    if (XGetClassHint(display, window, &hint)) {
        result = QString::fromLatin1(hint.res_class);
        XFree(hint.res_name);
        XFree(hint.res_class);
    }

    XCloseDisplay(display);
    return result;
}
