#pragma once

#include <QObject>
#include <QStringList>
#include <QMediaPlayer>
#include <QAudioOutput>

class MusicEngine : public QObject {
    Q_OBJECT

public:
    explicit MusicEngine(QObject* parent = nullptr);

    void setMusicFolder(const QString& folder);
    QStringList playlist() const;
    void playNext();
    void playTrack(int index);
    void stop();
    void play();
    void pause();
    QString currentTrack() const;

private:
    QStringList tracks;
    QString current;
    QMediaPlayer* player;
    QAudioOutput* audioOutput;
    int currentIndex = -1;
};
