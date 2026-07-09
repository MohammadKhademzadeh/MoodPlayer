#include "PlayerController.h"

PlayerController::PlayerController(QObject* parent) : QObject(parent) {}

void PlayerController::setFolder(const QString& folder) {
    engine.setMusicFolder(folder);
}

QString PlayerController::currentSong() const {
    return engine.currentTrack();
}

void PlayerController::next() {
    engine.playNext();
}
