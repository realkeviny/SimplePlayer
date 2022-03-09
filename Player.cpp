#include "Player.h"
#include <qsizepolicy.h>
#include <QTimer>

Player::Player(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	timerID = startTimer(1000);//启动定时器，指定时间间隔(毫秒)
	ui.VolumeSlider->setVisible(false);

	QString fileName = "E:/Song";
	QDir directory(fileName);
	QStringList nameFilters;
	nameFilters << "*.mp3" << "*.flac";
	QStringList files = directory.entryList(nameFilters, QDir::Files | QDir::Readable, QDir::Name);
	ui.SongList->addItems(files);
	songPath = fileName;

	pIcon.setToolTip("Player is running!");
	pIcon.setIcon(QIcon("F:\\C++\\Qt\\Player\\Player.ico"));

	QSize sh = ui.SongList->sizeHint();
	QSizePolicy p = ui.SongList->sizePolicy();
	QSizePolicy::Policy vp = p.horizontalPolicy();

	QHBoxLayout* Layout = new QHBoxLayout(this);

	//设置托盘菜单
	QMenu* menus = new QMenu();
	menus->addAction(ui.actionPlay);
	menus->addAction(ui.actionPause);
	menus->addAction(ui.actionNext);
	menus->addAction(ui.actionPrevious);
	menus->addSeparator();
	menus->addAction(ui.actionExit);
	pIcon.setContextMenu(menus);
	pIcon.show();

	connect(ui.btnPlay, SIGNAL(clicked()), this, SLOT(onbtnPlayClicked()));
	connect(ui.progressBar, SIGNAL(sliderMoved(int)), this, SLOT(onProgressBarMoved(int)));
	connect(ui.VolumeSlider, SIGNAL(sliderMoved(int)), this, SLOT(onSliderVolumeMoved(int)));
	connect(ui.VolumeSlider, SIGNAL(sliderReleased()), this, SLOT(onVolumeSliderReleased()));
	connect(ui.actionAddSong, SIGNAL(triggered()), this, SLOT(onMenuAddSong()));
	connect(ui.SongList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(onListItemDoubleClicked(QListWidgetItem*)));
	connect(ui.btnPrevious, SIGNAL(clicked()), this, SLOT(onbtnPreviousClicked()));
	connect(ui.btnNext, SIGNAL(clicked()), this, SLOT(onbtnNextClicked()));
	connect(ui.btnVolume, SIGNAL(clicked()), this, SLOT(onbtnVolumeClicked()));
	connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(onExit()));
	connect(&pIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(onSystemTrayClicked(QSystemTrayIcon::ActivationReason)));
	connect(ui.actionPlay, SIGNAL(triggered()), this, SLOT(onPlay()));
	connect(ui.actionPause, SIGNAL(triggered()), this, SLOT(onPause()));
	connect(ui.actionNext, SIGNAL(triggered()), this, SLOT(onNext()));
	connect(ui.actionPrevious, SIGNAL(triggered()), this, SLOT(onPrevious()));
}

Player::~Player()
{
	
}

void Player::timerEvent(QTimerEvent* event)
{
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
	QString name = item->text().left(item->text().lastIndexOf("."));
	if (!lyric.readLyrics(songPath + "/" + name + ".lrc"))
	{
		ui.LabelLyricDetection->setText("无歌词！");
		ui.LabelLyricFileCheck->setText("检查歌词文件存在性！");
	}
	lyricID = 0;
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
	int hour = 0;
	int minute = 0;
	int second = 0;
	time /= 1000;
	hour = time / 3600;
	minute = (time - hour * 3600) / 60;
	second = time - hour * 3600 - minute * 60;
	QTime now(hour,minute,second);
	QString text = now.toString("HH:mm:ss");
	return text.arg(hour).arg(minute).arg(second);
}

void Player::getDuration()
{
	playTime = mediaPlayer.duration();
	ui.TotalTime->setText(setTime(playTime));
}

void Player::setPlayTime()
{
	if(!lyric.getLyricTime().empty()&&mediaPlayer.position()>=lyric.getLyricTime().at(lyricID)&&lyricID<lyric.getLyricTime().size()-1)
	{
		ui.LabelLyricDetection->setText(lyric.getLyricText().at(lyricID));
		ui.LabelLyricFileCheck->setText(lyric.getLyricText().at(lyricID + 1));
		lyricID++;
	}
	ui.playTime->setText(setTime(mediaPlayer.position()));
}

int Player::onExit()
{
	QApplication::exit();
	return 0;
}

void Player::closeEvent(QCloseEvent* event)
{
	this->hide();
	event->ignore();
}

int Player::onSystemTrayClicked(QSystemTrayIcon::ActivationReason reason)
{
	if(reason == QSystemTrayIcon::Trigger||reason == QSystemTrayIcon::DoubleClick)
	{
		//双击显示主窗口
		this->showNormal();
	}
	return 0;
}

void Player::onPlay()
{
	mediaPlayer.play();
}

void Player::onPause()
{
	mediaPlayer.pause();
}

void Player::onNext()
{
	if (playRow + 1 == ui.SongList->count())
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

void Player::onPrevious()
{
	if (playRow == 0)
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

