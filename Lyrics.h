#ifndef LYRICS_H
#define LYRICS_H

#include <QWidget>
#include <QString>
#include <QList>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QFile>
#include <QIODevice>
#include <QDebug>

class Lyrics
{
public:
	Lyrics(QString path);
	Lyrics() = default;
	bool readLyrics(QString path);
	bool analyzeLyrics(QString line);
	QList<QString> getLyricText() const;
	QList<int> getLyricTime() const;
private:
	QString lyricPath;
	QList<QString> textList;
	QList<int> timeList;
};

#endif