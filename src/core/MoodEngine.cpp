#include "MoodEngine.h"

MoodEngine::Mode MoodEngine::detect(const QString& activeApp, double cpuUsage) {
    QString app = activeApp.toLower();

    if (app.contains("code") || app.contains("qt") || app.contains("terminal"))
        return Mode::Focus;

    if (cpuUsage > 70.0)
        return Mode::Intense;

    return Mode::Chill;
}

QString MoodEngine::modeName(Mode mode) {
    switch (mode) {
    case Mode::Focus: return "Focus";
    case Mode::Intense: return "Intense";
    case Mode::Chill: return "Chill";
    }
    return "Unknown";
}
