#pragma once

#include <QObject>
#include "../audio/MusicEngine.h"

class PlayerController : public QObject {
    Q_OBJECT

public:
    explicit PlayerController(QObject* parent = nullptr);

    void setFolder(const QString& folder);
    QString currentSong() const;
    void next();

private:
    MusicEngine engine;
};
