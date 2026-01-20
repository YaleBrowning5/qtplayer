#ifndef CONTROLBAR_H
#define CONTROLBAR_H

#include <QWidget>
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QComboBox>
#include <QHBoxLayout>

class ControlBar : public QWidget {
    Q_OBJECT

public:
    explicit ControlBar(QWidget *parent = nullptr);
    ~ControlBar();

    // Update controls
    void setDuration(double duration);
    void setPosition(double position);
    void setPlayingState(bool playing);
    
    // Get values
    double getPosition() const;
    double getPlaybackSpeed() const;

signals:
    void playPauseClicked();
    void positionChanged(double position);
    void speedChanged(double speed);
    void rotateClicked();
    void fullscreenClicked();

private slots:
    void onPlayPauseClicked();
    void onSliderPressed();
    void onSliderReleased();
    void onSliderMoved(int value);
    void onSpeedChanged(int index);
    void onRotateClicked();
    void onFullscreenClicked();

private:
    // Controls
    QPushButton *playPauseButton;
    QSlider *progressSlider;
    QLabel *timeLabel;
    QComboBox *speedComboBox;
    QPushButton *rotateButton;
    QPushButton *fullscreenButton;
    
    // State
    double duration;
    bool sliderPressed;
    bool isPlaying;
    
    // Helper functions
    void setupUI();
    QString formatTime(double seconds) const;
};

#endif // CONTROLBAR_H
