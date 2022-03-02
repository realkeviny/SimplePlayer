#include "Lyrics.h"


QList<QString> Lyrics::getLyricText() const
{
	return textList;
}

QList<int> Lyrics::getLyricTime() const
{
	return timeList;
}

Lyrics::Lyrics(QString path)
{
	this->lyricPath = path;
}


bool Lyrics::analyzeLyrics(QString line)
{
	if(line ==NULL||line.isEmpty())
	{
		qDebug() << "Unavailable!";
		return false;
	}
	QRegularExpression regularExp("\\[(\\d+)?:(\\d+)?\\](.*)?");
	int index = 0;
	QRegularExpressionMatch check;
	check = regularExp.match(line, index);
	if(check.hasMatch())
	{
		int totalTime = 0;
		totalTime = check.captured(1).toInt() * 60000 + check.captured(2).toInt() * 1000;//毫秒时间计算
		QString currentLyricText = QString::fromStdString(check.captured(4).toStdString());//歌词文本
		textList.push_back(currentLyricText);
		timeList.push_back(totalTime);
		return time;
	}
	return false;
}

bool Lyrics::readLyrics(QString path)
{
	QFile lyricFile(lyricPath);
	lyricFile.open(QIODevice::ReadOnly | QIODevice::Text);
	QString line = "";
	while((line=lyricFile.readLine())>0)
	{
		qDebug() << line;
		analyzeLyrics(line);
	}
	return true;
}