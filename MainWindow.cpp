#include "MainWindow.h"
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), isMonitoring(false)
{
    setWindowTitle("MoodPlayer - Smart Music");
    resize(450, 250);

    QWidget *central = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(central);

    statusLabel = new QLabel("Status: Idle", this);
    cpuLabel = new QLabel("CPU: 0%", this);
    startStopButton = new QPushButton("Start Monitoring", this);

    layout->addWidget(statusLabel);
    layout->addWidget(cpuLabel);
    layout->addWidget(startStopButton);

    setCentralWidget(central);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateSystemStatus);
    connect(startStopButton, &QPushButton::clicked, this, &MainWindow::toggleMonitoring);
}

MainWindow::~MainWindow() {}

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
