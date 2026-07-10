#include "MainWindow.h"
#include <QVBoxLayout>
#include <QFileDialog>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), isMonitoring(false)
{
    setWindowTitle("MoodPlayer - Smart Music");
    resize(450, 320);

    QWidget *central = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(central);

    statusLabel = new QLabel("Status: Idle", this);
    cpuLabel = new QLabel("CPU: 0%", this);
    startStopButton = new QPushButton("Start Monitoring", this);
    auto folderButton = new QPushButton("Choose Music Folder", this);
    auto nextButton = new QPushButton("Next Track", this);
    playButton = new QPushButton("Play", this);

    layout->addWidget(statusLabel);
    layout->addWidget(cpuLabel);
    layout->addWidget(startStopButton);
    layout->addWidget(folderButton);
    layout->addWidget(nextButton);
    layout->addWidget(playButton);

    setCentralWidget(central);

    timer = new QTimer(this);

    connect(timer, &QTimer::timeout, this, &MainWindow::updateSystemStatus);
    connect(startStopButton, &QPushButton::clicked, this, &MainWindow::toggleMonitoring);
    connect(folderButton, &QPushButton::clicked, this, &MainWindow::chooseMusicFolder);
    connect(playButton, &QPushButton::clicked, this, &MainWindow::playMusic);
    connect(nextButton, &QPushButton::clicked, this, [this]() {
        music.playNext();
        statusLabel->setText("Track: " + music.currentTrack());
    });

    QString savedFolder = settings.musicDirectory();
    if (!savedFolder.isEmpty())
        music.setMusicFolder(savedFolder);
}

MainWindow::~MainWindow() {}

void MainWindow::chooseMusicFolder()
{
    QString folder = QFileDialog::getExistingDirectory(this, "Music Folder");
    if (!folder.isEmpty()) {
        music.setMusicFolder(folder);
        settings.setMusicDirectory(folder);
        statusLabel->setText("Music folder loaded");
    }
}

void MainWindow::playMusic()
{
    music.play();
    statusLabel->setText("Playing: " + music.currentTrack());
}

void MainWindow::toggleMonitoring()
{
    if (isMonitoring) {
        timer->stop();
        isMonitoring = false;
        startStopButton->setText("Start Monitoring");
    } else {
        timer->start(3000);
        isMonitoring = true;
        startStopButton->setText("Stop Monitoring");
    }
}

void MainWindow::updateSystemStatus()
{
    double cpu = monitor.cpuUsage();
    QString app = monitor.activeWindow();
    auto mode = MoodEngine::detect(app, cpu);

    cpuLabel->setText(QString("CPU: %1%").arg(cpu, 0, 'f', 1));
    statusLabel->setText(QString("App: %1 | Mode: %2")
        .arg(app)
        .arg(MoodEngine::modeName(mode)));
}
