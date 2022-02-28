#include "Player.h"

Player::Player(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	timer = new QTimer();
	timer->setInterval(2000);
	timer->start();
	ui.VolumeSlider->setVisible(false);
	QString fileName = "E:/Song";
	QDir directory(fileName);
	QStringList nameFilters;
	nameFilters << "*.mp3" << "*.flac";
	QStringList files = directory.entryList(nameFilters, QDir::Files | QDir::Readable, QDir::Name);
	ui.SongList->addItems(files);
	songPath = fileName;
	connect(ui.btnPlay, SIGNAL(clicked()), this, SLOT(onbtnPlayClicked()));
	connect(ui.progressBar, SIGNAL(sliderMoved(int)), this, SLOT(onProgressBarMoved(int)));
	connect(ui.VolumeSlider, SIGNAL(sliderMoved(int)), this, SLOT(onSliderVolumeMoved(int)));
	connect(ui.VolumeSlider, SIGNAL(sliderReleased()), this, SLOT(onVolumeSliderReleased()));
	connect(ui.actionAddSong, SIGNAL(triggered()), this, SLOT(onMenuAddSong()));
	connect(ui.SongList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(onListItemDoubleClicked(QListWidgetItem*)));
	connect(ui.btnPrevious, SIGNAL(clicked()), this, SLOT(onbtnPreviousClicked()));
	connect(ui.btnNext, SIGNAL(clicked()), this, SLOT(onbtnNextClicked()));
	connect(ui.btnVolume, SIGNAL(clicked()), this, SLOT(onbtnVolumeClicked()));
}

void Player::onbtnPlayClicked()
{
		if(QMediaPlayer::PlayingState==mediaPlayer.state())
		{
			mediaPlayer.pause();
			ui.btnPlay->setText("Play");
		}
		else
		{
			mediaPlayer.play();
			ui.btnPlay->setText("Pause");
		}
}

void Player::onProgressBarMoved(int pos)
{
	mediaPlayer.setPosition(mediaPlayer.duration() * pos / 100);
}

void Player::setProgressBarValue()
{
	ui.progressBar->setValue(mediaPlayer.position() * 100 / mediaPlayer.duration());
	if(mediaPlayer.position()>=mediaPlayer.duration())
	{
		onbtnNextClicked();
	}
}

void Player::onSliderVolumeMoved(int vol)
{
	mediaPlayer.setVolume(vol);
}

int Player::onMenuAddSong()
{
	QString fileName = QFileDialog::getExistingDirectory(NULL, "Select Directory", ".", NULL);
	QDir dir(fileName);
	QStringList nameFilters;
	nameFilters << "*.mp3" << "*.flac";
	QStringList files = dir.entryList(nameFilters, QDir::Files | QDir::Readable, QDir::Name);
	ui.SongList->addItems(files);
	songPath = fileName;
	return 0;
}

void Player::onListItemDoubleClicked(QListWidgetItem* item)
{
	playRow = ui.SongList->row(item);
	mediaPlayer.stop();
	mediaPlayer.setMedia(QUrl::fromLocalFile(songPath + "/" + item->text()));
	mediaPlayer.play();
	ui.progressBar->setValue(mediaPlayer.position());
	ui.btnPlay->setText("Pause");
	connect(timer, SIGNAL(timeout()), this, SLOT(setProgressBarValue()));
	connect(&mediaPlayer, SIGNAL(durationChanged(qint64)), this, SLOT(getDuration()));
	connect(&mediaPlayer, SIGNAL(positionChanged(qint64)), this, SLOT(setPlayTime()));
}

void Player::onbtnPreviousClicked()
{
	if(playRow==0)
	{
		playRow = ui.SongList->count() - 1;
	}
	else
	{
		playRow--;
	}
	QListWidgetItem* item = ui.SongList->item(playRow);
	item->setSelected(true);
	mediaPlayer.stop();
	mediaPlayer.setMedia(QUrl::fromLocalFile(songPath + "/" + item->text()));
	mediaPlayer.play();
}

void Player::onbtnNextClicked()
{
	if(playRow+1==ui.SongList->count())
	{
		playRow = 0;
	}
	else
	{
		playRow++;
	}
	QListWidgetItem* item = ui.SongList->item(playRow);
	item->setSelected(true);
	mediaPlayer.stop();
	mediaPlayer.setMedia(QUrl::fromLocalFile(songPath + "/" + item->text()));
	mediaPlayer.play();
}

void Player::onbtnVolumeClicked()
{
	if(ui.VolumeSlider->isVisible())
	{
		ui.VolumeSlider->setVisible(false);
	}
	else
	{
		ui.VolumeSlider->setVisible(true);
	}
}

void Player::onVolumeSliderReleased()
{
	ui.VolumeSlider->setVisible(false);
}

QString Player::setTime(qint64 time)
{
	int hour;
	int minute;
	int second;
	time /= 1000;
	hour = time / 3600;
	minute = (time - hour * 3600) / 60;
	second = time - hour * 3600 - minute * 60;
	return QString("%1:%2:%3").arg(hour).arg(minute).arg(second);
}

void Player::getDuration()
{
	playTime = mediaPlayer.duration();
	ui.TotalTime->setText(setTime(playTime));
}

void Player::setPlayTime()
{
	ui.playTime->setText(setTime(mediaPlayer.position()));
}

