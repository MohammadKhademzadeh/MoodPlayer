#pragma once

#include <QDialog>
#include <QSlider>
#include <QCheckBox>
#include <QLineEdit>
#include <QPushButton>

class SettingsDialog : public QDialog {
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget* parent = nullptr);

private:
    QLineEdit* musicPath;
    QSlider* volumeSlider;
    QCheckBox* autoPlay;
};
