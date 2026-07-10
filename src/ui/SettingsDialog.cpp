#include "SettingsDialog.h"
#include <QVBoxLayout>
#include <QLabel>

SettingsDialog::SettingsDialog(QWidget* parent) : QDialog(parent) {
    setWindowTitle("MoodPlayer Settings");

    auto* layout = new QVBoxLayout(this);

    musicPath = new QLineEdit(this);
    musicPath->setPlaceholderText("Default music folder");

    volumeSlider = new QSlider(Qt::Horizontal, this);
    volumeSlider->setRange(0, 100);
    volumeSlider->setValue(70);

    autoPlay = new QCheckBox("Auto play on startup", this);

    layout->addWidget(new QLabel("Music folder"));
    layout->addWidget(musicPath);
    layout->addWidget(new QLabel("Volume"));
    layout->addWidget(volumeSlider);
    layout->addWidget(autoPlay);
}
