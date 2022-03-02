#ifndef PLAYER_H
#define PLAYER_H

#include <QtWidgets/QMainWindow>
#include "ui_Player.h"
#include <QtMultimedia/QMediaPlayer>
#include <QTimer>
#include <QString>
#include <QFileDialog>
#include <QListWidgetItem>
#include "Lyrics.h"
#include <QSystemTrayIcon>
#include <QIcon>
#include <QMenu>
#include <QCloseEvent>

class Player : public QMainWindow
{
    Q_OBJECT

public:
    Player(QWidget *parent = Q_NULLPTR);
    QString setTime(qint64 time);
    ~Player();
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
    int onExit();
    int onSystemTrayClicked(QSystemTrayIcon::ActivationReason reason);
    void onPlay();
    void onPause();
    void onNext();
    void onPrevious();
private:
    void closeEvent(QCloseEvent* event) override;
private:
    Ui::PlayerClass ui;
    QMediaPlayer mediaPlayer;
    QTimer* timer;
    QString songPath;
    int playRow;
    qint64 playTime;
    Lyrics lyric;
    int lyricID = 0;
    QSystemTrayIcon pIcon;
};

#endif