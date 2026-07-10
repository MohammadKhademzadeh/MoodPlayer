#include "MainWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QPainter>
#include <QCloseEvent>
#include <QApplication>
#include <QLabel>

// --- Custom CPU Gauge Widget (The Cool Visuals) ---
class CpuGauge : public QWidget {
public:
    CpuGauge(QWidget* parent = nullptr) : QWidget(parent), m_cpu(0) {
        setFixedSize(160, 160);
        setStyleSheet("background: transparent;");
    }
    void setCpu(double cpu) {
        m_cpu = cpu;
        update(); // Redraw
    }
protected:
    void paintEvent(QPaintEvent*) override {
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);
        
        int side = qMin(width(), height());
        p.translate(width() / 2, height() / 2);
        p.scale(side / 250.0, side / 250.0);

        // Background Arc
        p.setPen(QPen(QColor(45, 48, 58), 14));
        p.drawArc(-100, -100, 200, 200, 0, 360 * 16);

        // Dynamic Color based on CPU (Mood)
        QColor color = m_cpu > 70 ? QColor(255, 80, 100) : (m_cpu > 40 ? QColor(255, 180, 50) : QColor(80, 220, 150));
        p.setPen(QPen(color, 14, Qt::SolidLine, Qt::RoundCap));
        int spanAngle = static_cast<int>(m_cpu * 16 * 3.6);
        p.drawArc(-100, -100, 200, 200, 90 * 16, -spanAngle);

        // Text
        p.setPen(QColor(230, 230, 240));
        QFont f = p.font();
        f.setPointSize(28);
        f.setBold(true);
        p.setFont(f);
        p.drawText(QRect(-100, -30, 200, 50), Qt::AlignCenter, QString("%1%").arg(m_cpu, 0, 'f', 0));
        
        f.setPointSize(11);
        p.setFont(f);
        p.drawText(QRect(-100, 25, 200, 20), Qt::AlignCenter, "CPU USAGE");
    }
private:
    double m_cpu;
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), isMonitoring(false), cpuGauge(nullptr), trayIcon(nullptr)
{
    setWindowTitle("MoodPlayer");
    resize(700, 450);
    setupUI();
    applyTheme();

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateSystemStatus);
    
    connect(startStopButton, &QPushButton::clicked, this, &MainWindow::toggleMonitoring);
    connect(playButton, &QPushButton::clicked, this, &MainWindow::playMusic);
    connect(pauseButton, &QPushButton::clicked, this, &MainWindow::pauseMusic);
    connect(stopButton, &QPushButton::clicked, this, &MainWindow::stopMusic);
    connect(playlistWidget, &QListWidget::itemDoubleClicked, this, &MainWindow::playSelectedTrack);

    // Load settings
    QString savedFolder = settings.musicDirectory();
    if (!savedFolder.isEmpty()) {
        music.setMusicFolder(savedFolder);
        playlistWidget->addItems(music.playlist());
    }

    setupSystemTray();
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI() {
    QWidget *central = new QWidget(this);
    central->setObjectName("CentralWidget");
    QHBoxLayout *mainLayout = new QHBoxLayout(central);
    mainLayout->setContentsMargins(25, 25, 25, 25);
    mainLayout->setSpacing(30);

    // --- Left Side: Controls & Status ---
    QVBoxLayout *leftLayout = new QVBoxLayout();
    
    QLabel *titleLabel = new QLabel("MoodPlayer");
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(26);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet("color: #5BC0EB; background: transparent; margin: 0;");
    
    statusLabel = new QLabel("Status: Idle");
    statusLabel->setStyleSheet("font-size: 14px; color: #A0A5B5; background: transparent; margin-bottom: 15px;");
    
    leftLayout->addWidget(titleLabel);
    leftLayout->addWidget(statusLabel);
    leftLayout->addSpacing(10);

    // Control Buttons
    QHBoxLayout *controlLayout = new QHBoxLayout();
    startStopButton = new QPushButton("Start Monitoring");
    startStopButton->setObjectName("PrimaryBtn");
    
    auto folderButton = new QPushButton("Select Music Folder");
    connect(folderButton, &QPushButton::clicked, this, &MainWindow::chooseMusicFolder);
    
    controlLayout->addWidget(startStopButton);
    controlLayout->addWidget(folderButton);
    leftLayout->addLayout(controlLayout);
    leftLayout->addSpacing(10);

    // Playback Buttons
    QHBoxLayout *playbackLayout = new QHBoxLayout();
    playButton = new QPushButton("Play");
    pauseButton = new QPushButton("Pause");
    stopButton = new QPushButton("Stop");
    
    playbackLayout->addWidget(playButton);
    playbackLayout->addWidget(pauseButton);
    playbackLayout->addWidget(stopButton);
    leftLayout->addLayout(playbackLayout);

    // Spotify Button
    spotifyButton = new QPushButton("Connect Spotify");
    spotifyButton->setObjectName("SpotifyBtn");
    connect(spotifyButton, &QPushButton::clicked, this, &MainWindow::connectSpotify);
    leftLayout->addWidget(spotifyButton);
    
    leftLayout->addStretch();

    // --- Right Side: CPU Gauge and Playlist ---
    QVBoxLayout *rightLayout = new QVBoxLayout();
    
    cpuGauge = new CpuGauge(this);
    QHBoxLayout *gaugeLayout = new QHBoxLayout();
    gaugeLayout->addStretch();
    gaugeLayout->addWidget(cpuGauge);
    gaugeLayout->addStretch();
    rightLayout->addLayout(gaugeLayout);
    rightLayout->addSpacing(15);

    playlistWidget = new QListWidget();
    QLabel *plTitle = new QLabel("Local Playlist:");
    plTitle->setStyleSheet("background: transparent; font-weight: bold; color: #E0E0E0;");
    rightLayout->addWidget(plTitle);
    rightLayout->addWidget(playlistWidget);

    mainLayout->addLayout(leftLayout, 4);
    mainLayout->addLayout(rightLayout, 3);

    setCentralWidget(central);
}

void MainWindow::applyTheme() {
    setStyleSheet(R"(
        QMainWindow {
            background-color: #1A1D24;
        }
        QWidget#CentralWidget {
            background-color: #22252D;
            border-radius: 16px;
            border: 1px solid #2D323E;
        }
        QLabel {
            color: #E0E0E0;
            background: transparent;
            font-family: 'Segoe UI', 'Vazirmatn', sans-serif;
        }
        QPushButton {
            background-color: #2D323E;
            color: #E0E0E0;
            border: 1px solid #3A3F4B;
            border-radius: 8px;
            padding: 10px 15px;
            font-weight: bold;
        }
        QPushButton:hover {
            background-color: #3A3F4B;
            border: 1px solid #5BC0EB;
            color: #5BC0EB;
        }
        QPushButton:pressed {
            background-color: #5BC0EB;
            color: #1A1D24;
        }
        QPushButton#PrimaryBtn {
            background-color: #27AE60;
            border: none;
            color: white;
        }
        QPushButton#PrimaryBtn:hover { background-color: #2ECC71; color: white; }
        
        QPushButton#SpotifyBtn {
            background-color: #1DB954;
            border: none;
            color: white;
        }
        QPushButton#SpotifyBtn:hover { background-color: #1ED760; color: white; }

        QListWidget {
            background-color: #1E2128;
            color: #E0E0E0;
            border: 1px solid #2D323E;
            border-radius: 8px;
            outline: none;
            padding: 5px;
        }
        QListWidget::item {
            padding: 8px;
            border-radius: 4px;
        }
        QListWidget::item:hover { background-color: #2D323E; }
        QListWidget::item:selected { background-color: #5BC0EB; color: #1A1D24; }
    )");
}

void MainWindow::setupSystemTray() {
    trayIcon = new QSystemTrayIcon(QIcon::fromTheme("media-playback-start"), this);
    QMenu *trayMenu = new QMenu(this);
    
    QAction *showAction = trayMenu->addAction("Show MoodPlayer");
    connect(showAction, &QAction::triggered, this, &QWidget::show);
    
    QAction *quitAction = trayMenu->addAction("Quit");
    connect(quitAction, &QAction::triggered, qApp, &QApplication::quit);
    
    trayIcon->setContextMenu(trayMenu);
    trayIcon->show();
}

void MainWindow::closeEvent(QCloseEvent *event) {
    if (trayIcon && trayIcon->isVisible()) {
        hide();
        event->ignore(); // Keep running in background
    }
}

void MainWindow::chooseMusicFolder() {
    QString folder = QFileDialog::getExistingDirectory(this, "Select Music Folder");
    if (!folder.isEmpty()) {
        music.setMusicFolder(folder);
        settings.setMusicDirectory(folder);
        playlistWidget->clear();
        playlistWidget->addItems(music.playlist());
        statusLabel->setText("Music folder loaded");
    }
}

void MainWindow::playSelectedTrack(QListWidgetItem *item) {
    int index = playlistWidget->row(item);
    music.playTrack(index);
    statusLabel->setText("Playing: " + music.currentTrack());
}

void MainWindow::playMusic() { music.play(); statusLabel->setText("Playing..."); }
void MainWindow::pauseMusic() { music.pause(); statusLabel->setText("Paused"); }
void MainWindow::stopMusic() { music.stop(); statusLabel->setText("Stopped"); }

void MainWindow::toggleMonitoring() {
    if (isMonitoring) {
        timer->stop();
        isMonitoring = false;
        startStopButton->setText("Start Monitoring");
        startStopButton->setObjectName("PrimaryBtn");
        startStopButton->style()->unpolish(startStopButton);
        startStopButton->style()->polish(startStopButton);
    } else {
        timer->start(2000); // Update every 2 seconds for smooth gauge
        isMonitoring = true;
        startStopButton->setText("Stop Monitoring");
        startStopButton->setStyleSheet("background-color: #E74C3C; border: none; color: white;");
    }
}

void MainWindow::updateSystemStatus() {
    double cpu = monitor.cpuUsage();
    QString app = monitor.activeWindow();
    auto mode = MoodEngine::detect(app, cpu);

    cpuGauge->setCpu(cpu); // Update visual gauge
    
    QString modeName = MoodEngine::modeName(mode);
    statusLabel->setText(QString("%1 | Active: %2").arg(modeName).arg(app));
    
    // UX Improvement: Let MusicEngine decide & crossfade
    // music.setMood(mode); 
}

void MainWindow::connectSpotify() {
    // Spotify Logic here
}
