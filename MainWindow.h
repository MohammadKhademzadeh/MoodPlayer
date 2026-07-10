#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
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

private:
    QLabel *statusLabel;
    QLabel *cpuLabel;
    QPushButton *startStopButton;

    QTimer *timer;
    bool isMonitoring;

    SystemMonitor monitor;
    MusicEngine music;
    SettingsManager settings;
};

#endif
