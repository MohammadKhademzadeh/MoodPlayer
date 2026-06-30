#include "MainWindow.h"
#include <QVBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QUrl>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), isMonitoring(false) {
    setWindowTitle("Mood Player - Smart Music for Linux");
    resize(400, 200);

    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    statusLabel = new QLabel("App Status: Idle", this);
    cpuLabel = new QLabel("CPU: 0%", this);
    startStopButton = new QPushButton("Start Monitoring", this);

    layout->addWidget(statusLabel);
    layout->addWidget(cpuLabel);
    layout->addWidget(startStopButton);

    setCentralWidget(centralWidget);

    // تنظیمات پخش کننده موسیقی
    player = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);
    player->setAudioOutput(audioOutput);
    audioOutput->setVolume(0.5);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateSystemStatus);
    connect(startStopButton, &QPushButton::clicked, this, &MainWindow::toggleMonitoring);

    // مقادیر اولیه برای محاسبه CPU
    prev_idle = 0;
    prev_total = 0;
}

MainWindow::~MainWindow() {}

void MainWindow::toggleMonitoring() {
    if (isMonitoring) {
        timer->stop();
        isMonitoring = false;
        startStopButton->setText("Start Monitoring");
        player->stop();
    } else {
        timer->start(3000); // هر ۳ ثانیه چک می‌کند
        isMonitoring = true;
        startStopButton->setText("Stop Monitoring");
    }
}

double MainWindow::getCPUUsage() {
    QFile file("/proc/stat");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return 0.0;

    QTextStream in(&file);
    QString line = in.readLine();
    file.close();

    QStringList values = line.split(" ", Qt::SkipEmptyParts);
    // user, nice, system, idle, iowait, irq, softirq, steal
    long user = values[1].toLong();
    long nice = values[2].toLong();
    long system = values[3].toLong();
    long idle = values[4].toLong();
    long iowait = values[5].toLong();
    long irq = values[6].toLong();
    long softirq = values[7].toLong();

    long total = user + nice + system + idle + iowait + irq + softirq;
    long total_diff = total - prev_total;
    long idle_diff = idle - prev_idle;

    double cpu_usage = 0.0;
    if (total_diff > 0) {
        cpu_usage = 100.0 * (total_diff - idle_diff) / total_diff;
    }

    prev_total = total;
    prev_idle = idle;

    return cpu_usage;
}

QString MainWindow::getActiveWindowClass() {
    Display *display = XOpenDisplay(nullptr);
    if (!display) return "Unknown";

    Window focused;
    int revert;
    XGetInputFocus(display, &focused, &revert);

    XClassHint hint;
    QString appClass = "Unknown";
    if (XGetClassHint(display, focused, &hint)) {
        appClass = QString::fromLatin1(hint.res_class);
        XFree(hint.res_name);
        XFree(hint.res_class);
    }

    XCloseDisplay(display);
    return appClass;
}

void MainWindow::updateSystemStatus() {
    double cpu = getCPUUsage();
    QString appClass = getActiveWindowClass();

    cpuLabel->setText(QString("CPU: %1%").arg(cpu, 0, 'f', 1));
    statusLabel->setText(QString("Active App: %1").arg(appClass));

    decideMusic(appClass, cpu);
}

void MainWindow::decideMusic(const QString& appClass, double cpuUsage) {
    QString currentMusic = player->source().toString();

    // منطق تصمیم‌گیری شما اینجا نوشته می‌شود
    // مثال: اگر برنامه VS Code باز بود (کلاس آن معمولا Code است)
    if (appClass.toLower().contains("code")) {
        statusLabel->setText(statusLabel->text() + " | Mode: Focus");
        // آدرس فایل آهنگ آرام را اینجا بگذارید
        QString focusMusic = "file:///path/to/your/focus_music.mp3"; 
        if (currentMusic != focusMusic) {
            player->setSource(QUrl(focusMusic));
            player->play();
        }
    } 
    // اگر پردازنده درگیر بود (فعالیت سنگین)
    else if (cpuUsage > 70.0) {
        statusLabel->setText(statusLabel->text() + " | Mode: Intense");
        QString intenseMusic = "file:///path/to/your/intense_music.mp3";
        if (currentMusic != intenseMusic) {
            player->setSource(QUrl(intenseMusic));
            player->play();
        }
    }
    // در غیر این صورت (فعالیت سبک)
    else {
        statusLabel->setText(statusLabel->text() + " | Mode: Chill");
        QString chillMusic = "file:///path/to/your/chill_music.mp3";
        if (currentMusic != chillMusic) {
            player->setSource(QUrl(chillMusic));
            player->play();
        }
    }
}