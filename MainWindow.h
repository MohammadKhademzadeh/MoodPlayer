#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QListWidget>
#include <QTimer>
#include <QSystemTrayIcon>
#include "src/core/SystemMonitor.h"
#include "src/core/MoodEngine.h"
#include "src/audio/MusicEngine.h"
#include "src/core/SettingsManager.h"
#include "src/integrations/SpotifyConnector.h"

class CpuGauge; // Forward declaration

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void updateSystemStatus();
    void toggleMonitoring();
    void chooseMusicFolder();
    void playMusic();
    void pauseMusic();
    void stopMusic();
    void playSelectedTrack(QListWidgetItem *item);
    void connectSpotify();

private:
    void setupSystemTray();
    void setupUI();
    void applyTheme();

    QLabel *statusLabel;
    QPushButton *startStopButton;
    QPushButton *playButton;
    QPushButton *pauseButton;
    QPushButton *stopButton;
    QPushButton *spotifyButton;
    QListWidget *playlistWidget;

    QTimer *timer;
    bool isMonitoring;
    CpuGauge *cpuGauge;
    QSystemTrayIcon *trayIcon;

    SystemMonitor monitor;
    MusicEngine music;
    SettingsManager settings;
    SpotifyConnector spotify;
};

#endif
