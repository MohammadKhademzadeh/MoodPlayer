#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QTimer>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void updateSystemStatus();
    void toggleMonitoring();

private:
    QLabel *statusLabel;
    QLabel *cpuLabel;
    QPushButton *startStopButton;
    
    QMediaPlayer *player;
    QAudioOutput *audioOutput;
    QTimer *timer;
    
    bool isMonitoring;
    long prev_idle, prev_total;

    double getCPUUsage();
    QString getActiveWindowClass();
    void decideMusic(const QString& appClass, double cpuUsage);
};

#endif // MAINWINDOW_H