#pragma once

#include <QString>

class MoodEngine {
public:
    enum class Mode { Focus, Intense, Chill };

    static Mode detect(const QString& activeApp, double cpuUsage);
    static QString modeName(Mode mode);
};
