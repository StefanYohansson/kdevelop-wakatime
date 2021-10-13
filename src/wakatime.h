/*
 * This file is part of the WakaTime plugin for KDevelop.
 *
 * Copyright 2021 Stefan Yohansson <sy.fen0@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef WAKATIME_H
#define WAKATIME_H

#include <QDateTime>
#include <interfaces/iplugin.h>
#include <interfaces/idocument.h>
#include <KTextEditor/Cursor>

class WakaTime : public KDevelop::IPlugin
{
    Q_OBJECT

public:
    // KPluginFactory-based plugin wants constructor with this signature
    WakaTime(QObject* parent, const QVariantList& args);

public Q_SLOTS:
	void documentOpened(KDevelop::IDocument *document);
	void documentActivated(KDevelop::IDocument *document);
	void documentSaved(KDevelop::IDocument *document);
	void documentJumpPerformed(KDevelop::IDocument * newDocument, KTextEditor::Cursor newCursor, KDevelop::IDocument * previousDocument, KTextEditor::Cursor previousCursor);

protected:
	QString version;
	QDateTime lastHeartbeat;
	QString lastFile;
	QString getWakatimeBin();
	void checkWakatimeBin();
	QString getFileProject(QUrl url);
	QStringList generateHeartbeat(QString file, QString project, int line, bool write);
	void sendHeartbeat(QStringList args);
	bool isFileIgnored(QString file);
	bool enoughTimePassed(QDateTime date);
};

#endif // WAKATIME_H
