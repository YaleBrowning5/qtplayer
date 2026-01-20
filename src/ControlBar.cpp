#include "ControlBar.h"
#include <QHBoxLayout>
#include <QStyle>

ControlBar::ControlBar(QWidget *parent)
    : QWidget(parent)
    , duration(0.0)
    , sliderPressed(false)
    , isPlaying(false)
{
    setupUI();
}

ControlBar::~ControlBar() {
}

void ControlBar::setupUI() {
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(10, 5, 10, 5);
    
    // Play/Pause button
    playPauseButton = new QPushButton("▶", this);
    playPauseButton->setFixedSize(40, 40);
    playPauseButton->setStyleSheet("QPushButton { font-size: 18px; }");
    connect(playPauseButton, &QPushButton::clicked, this, &ControlBar::onPlayPauseClicked);
    
    // Progress slider
    progressSlider = new QSlider(Qt::Horizontal, this);
    progressSlider->setRange(0, 1000);
    progressSlider->setValue(0);
    connect(progressSlider, &QSlider::sliderPressed, this, &ControlBar::onSliderPressed);
    connect(progressSlider, &QSlider::sliderReleased, this, &ControlBar::onSliderReleased);
    connect(progressSlider, &QSlider::sliderMoved, this, &ControlBar::onSliderMoved);
    
    // Time label
    timeLabel = new QLabel("00:00 / 00:00", this);
    timeLabel->setMinimumWidth(120);
    timeLabel->setStyleSheet("QLabel { color: #ffffff; }");
    
    // Speed combo box
    speedComboBox = new QComboBox(this);
    speedComboBox->addItem("0.25x", 0.25);
    speedComboBox->addItem("0.5x", 0.5);
    speedComboBox->addItem("0.75x", 0.75);
    speedComboBox->addItem("1.0x", 1.0);
    speedComboBox->setCurrentIndex(3); // Default to 1.0x
    speedComboBox->setFixedWidth(80);
    connect(speedComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ControlBar::onSpeedChanged);
    
    // Rotate button
    rotateButton = new QPushButton("↻", this);
    rotateButton->setFixedSize(40, 40);
    rotateButton->setStyleSheet("QPushButton { font-size: 20px; }");
    rotateButton->setToolTip("Rotate 90°");
    connect(rotateButton, &QPushButton::clicked, this, &ControlBar::onRotateClicked);
    
    // Fullscreen button
    fullscreenButton = new QPushButton("⛶", this);
    fullscreenButton->setFixedSize(40, 40);
    fullscreenButton->setStyleSheet("QPushButton { font-size: 18px; }");
    fullscreenButton->setToolTip("Fullscreen");
    connect(fullscreenButton, &QPushButton::clicked, this, &ControlBar::onFullscreenClicked);
    
    // Add widgets to layout
    layout->addWidget(playPauseButton);
    layout->addWidget(progressSlider);
    layout->addWidget(timeLabel);
    layout->addWidget(speedComboBox);
    layout->addWidget(rotateButton);
    layout->addWidget(fullscreenButton);
    
    // Set background style
    setStyleSheet("QWidget { background-color: #1e1e1e; }"
                  "QSlider::groove:horizontal { background: #3d3d3d; height: 6px; border-radius: 3px; }"
                  "QSlider::handle:horizontal { background: #ffffff; width: 14px; margin: -4px 0; border-radius: 7px; }"
                  "QSlider::sub-page:horizontal { background: #0078d4; border-radius: 3px; }"
                  "QComboBox { background-color: #2d2d2d; color: #ffffff; border: 1px solid #3d3d3d; padding: 4px; }"
                  "QComboBox::drop-down { border: none; }"
                  "QComboBox QAbstractItemView { background-color: #2d2d2d; color: #ffffff; selection-background-color: #3d3d3d; }");
}

void ControlBar::setDuration(double dur) {
    duration = dur;
    if (!sliderPressed) {
        timeLabel->setText(formatTime(0) + " / " + formatTime(duration));
    }
}

void ControlBar::setPosition(double position) {
    if (!sliderPressed && duration > 0) {
        int value = static_cast<int>((position / duration) * 1000);
        progressSlider->setValue(value);
        timeLabel->setText(formatTime(position) + " / " + formatTime(duration));
    }
}

void ControlBar::setPlayingState(bool playing) {
    isPlaying = playing;
    playPauseButton->setText(playing ? "⏸" : "▶");
}

double ControlBar::getPosition() const {
    if (duration > 0) {
        return (progressSlider->value() / 1000.0) * duration;
    }
    return 0.0;
}

double ControlBar::getPlaybackSpeed() const {
    return speedComboBox->currentData().toDouble();
}

void ControlBar::onPlayPauseClicked() {
    emit playPauseClicked();
}

void ControlBar::onSliderPressed() {
    sliderPressed = true;
}

void ControlBar::onSliderReleased() {
    sliderPressed = false;
    double position = (progressSlider->value() / 1000.0) * duration;
    emit positionChanged(position);
}

void ControlBar::onSliderMoved(int value) {
    if (sliderPressed) {
        double position = (value / 1000.0) * duration;
        timeLabel->setText(formatTime(position) + " / " + formatTime(duration));
    }
}

void ControlBar::onSpeedChanged(int index) {
    double speed = speedComboBox->itemData(index).toDouble();
    emit speedChanged(speed);
}

void ControlBar::onRotateClicked() {
    emit rotateClicked();
}

void ControlBar::onFullscreenClicked() {
    emit fullscreenClicked();
}

QString ControlBar::formatTime(double seconds) const {
    int totalSeconds = static_cast<int>(seconds);
    int hours = totalSeconds / 3600;
    int minutes = (totalSeconds % 3600) / 60;
    int secs = totalSeconds % 60;
    
    if (hours > 0) {
        return QString("%1:%2:%3")
            .arg(hours, 2, 10, QChar('0'))
            .arg(minutes, 2, 10, QChar('0'))
            .arg(secs, 2, 10, QChar('0'));
    } else {
        return QString("%1:%2")
            .arg(minutes, 2, 10, QChar('0'))
            .arg(secs, 2, 10, QChar('0'));
    }
}
