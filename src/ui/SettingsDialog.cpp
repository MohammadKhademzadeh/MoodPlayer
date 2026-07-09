#include "SettingsDialog.h"
#include <QVBoxLayout>
#include <QLabel>

SettingsDialog::SettingsDialog(QWidget* parent) : QDialog(parent) {
    setWindowTitle("MoodPlayer Settings");

    auto* layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel("Music folder and player settings will appear here."));
}
