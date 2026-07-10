#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QListWidget>
#include <QTimer>
#include "src/core/SystemMonitor.h"
#include "src/core/MoodEngine.h"
#include "src/audio/MusicEngine.h"
#include "src/core/SettingsManager.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void updateSystemStatus();
    void toggleMonitoring();
    void chooseMusicFolder();
    void playMusic();
    void pauseMusic();
    void stopMusic();
    void playSelectedTrack(QListWidgetItem *item);

private:
    QLabel *statusLabel;
    QLabel *cpuLabel;
    QPushButton *startStopButton;
    QPushButton *playButton;
    QPushButton *pauseButton;
    QPushButton *stopButton;
    QListWidget *playlistWidget;

    QTimer *timer;
    bool isMonitoring;

    SystemMonitor monitor;
    MusicEngine music;
    SettingsManager settings;
};

#endif
