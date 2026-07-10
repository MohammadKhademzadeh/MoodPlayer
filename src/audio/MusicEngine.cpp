#include "MusicEngine.h"
#include <QDir>
#include <QUrl>

MusicEngine::MusicEngine(QObject* parent) : QObject(parent)
{
    player = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);
    player->setAudioOutput(audioOutput);
}

void MusicEngine::setMusicFolder(const QString& folder) {
    QDir dir(folder);
    QStringList filters{"*.mp3", "*.wav", "*.ogg", "*.flac"};
    tracks = dir.entryList(filters, QDir::Files);
    currentIndex = -1;
}

QStringList MusicEngine::playlist() const {
    return tracks;
}

void MusicEngine::playNext() {
    if (!tracks.isEmpty()) {
        int next = currentIndex + 1;
        if (next >= tracks.size())
            next = 0;
        playTrack(next);
    }
}

void MusicEngine::playTrack(int index) {
    if (index >= 0 && index < tracks.size()) {
        currentIndex = index;
        current = tracks.at(index);
        player->setSource(QUrl::fromLocalFile(current));
        player->play();
    }
}

void MusicEngine::play() {
    player->play();
}

void MusicEngine::pause() {
    player->pause();
}

void MusicEngine::stop() {
    player->stop();
    current.clear();
    currentIndex = -1;
}

QString MusicEngine::currentTrack() const {
    return current;
}
