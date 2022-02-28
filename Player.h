#ifndef PLAYER_H
#define PLAYER_H

#include <QtWidgets/QMainWindow>
#include "ui_Player.h"
#include <QtMultimedia/QMediaPlayer>
#include <QTimer>
#include <QString>
#include <QFileDialog>
#include <QListWidgetItem>

class Player : public QMainWindow
{
    Q_OBJECT

public:
    Player(QWidget *parent = Q_NULLPTR);
    QString setTime(qint64 time);

private slots:
    void onbtnPlayClicked();
    void onProgressBarMoved(int pos);
    void setProgressBarValue();
    void onSliderVolumeMoved(int vol);
    int onMenuAddSong();
    void onListItemDoubleClicked(QListWidgetItem* item);
    void onbtnPreviousClicked();
	void onbtnNextClicked();
    void onbtnVolumeClicked();
    void onVolumeSliderReleased();
    void getDuration();
    void setPlayTime();
private:
    Ui::PlayerClass ui;
    QMediaPlayer mediaPlayer;
    QTimer* timer;
    QString songPath;
    int playRow;
    qint64 playTime;
};

#endif