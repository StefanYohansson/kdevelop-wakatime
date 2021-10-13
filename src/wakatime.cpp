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

#include "wakatime.h"

#include <QStandardPaths>
#include <QDir>
#include <QProcess>

#include <debug.h>

#include <KLocalizedString>
#include <KPluginFactory>


#include <interfaces/icore.h>
#include <interfaces/idocumentcontroller.h>
#include <interfaces/iprojectcontroller.h>

K_PLUGIN_FACTORY_WITH_JSON(WakaTimeFactory, "wakatime.json",
registerPlugin<WakaTime>(); )

WakaTime::WakaTime(QObject *parent, const QVariantList& args)
    : KDevelop::IPlugin(QStringLiteral("WakaTime"), parent)
{
    Q_UNUSED(args);

	checkWakatimeBin();

	version = QString("0.0.1");

	connect(KDevelop::ICore::self()->documentController(),
			&KDevelop::IDocumentController::documentOpened,
			this,
			&WakaTime::documentOpened);
	connect(KDevelop::ICore::self()->documentController(),
			&KDevelop::IDocumentController::documentActivated,
			this,
			&WakaTime::documentActivated);
	connect(KDevelop::ICore::self()->documentController(),
			&KDevelop::IDocumentController::documentSaved,
			this,
			&WakaTime::documentSaved);
	connect(KDevelop::ICore::self()->documentController(),
			&KDevelop::IDocumentController::documentJumpPerformed,
			this,
			&WakaTime::documentJumpPerformed);
}

QString WakaTime::getFileProject(QUrl url)
{
	KDevelop::IProjectController *projectController = KDevelop::ICore::self()->projectController();

	// Extract file name and path.
	QString text = url.fileName();
	QString path = projectController->prettyFilePath(url, KDevelop::IProjectController::FormatPlain);

	const bool isPartOfOpenProject = QDir::isRelativePath(path);
	if (path.endsWith(QLatin1Char('/'))) {
		path.chop(1);
	}
	if (isPartOfOpenProject) {
		const int projectNameSize = path.indexOf(QLatin1Char(':'));

		return (projectNameSize < 0) ? path : path.left(projectNameSize);
	} else {
		return QString("");
	}
}

bool WakaTime::enoughTimePassed(QDateTime time)
{
	return lastHeartbeat.secsTo(time) >= 120000;
}

void WakaTime::documentActivated(KDevelop::IDocument* document)
{
	if (isFileIgnored(document->url().fileName())) {
		return;
	}

	QDateTime now = QDateTime::currentDateTime();
	if (lastFile != document->url().path() || enoughTimePassed(now)) {
		QStringList args = generateHeartbeat(document->url().path(), getFileProject(document->url()), document->cursorPosition().line(), false);
		sendHeartbeat(args);
		lastHeartbeat = now;
		lastFile = document->url().path();
	}
}

void WakaTime::documentOpened(KDevelop::IDocument* document)
{
	if (isFileIgnored(document->url().fileName())) {
		return;
	}

	QDateTime now = QDateTime::currentDateTime();
	if (enoughTimePassed(now)) {
		QStringList args = generateHeartbeat(document->url().path(), getFileProject(document->url()), document->cursorPosition().line(), false);
		sendHeartbeat(args);
		lastHeartbeat = now;
		lastFile = document->url().path();
	}
}

void WakaTime::documentJumpPerformed(KDevelop::IDocument* newDocument, KTextEditor::Cursor newCursor, KDevelop::IDocument* previousDocument, KTextEditor::Cursor previousCursor)
{
	if (isFileIgnored(newDocument->url().fileName())) {
		return;
	}

	QDateTime now = QDateTime::currentDateTime();
	if (enoughTimePassed(now)) {
		QStringList args = generateHeartbeat(newDocument->url().path(), getFileProject(newDocument->url()), newCursor.line(), false);
		sendHeartbeat(args);
		lastHeartbeat = now;
		lastFile = newDocument->url().path();
	}
}

void WakaTime::documentSaved(KDevelop::IDocument* document)
{
	if (isFileIgnored(document->url().fileName())) {
		return;
	}

	QDateTime now = QDateTime::currentDateTime();
	QStringList args = generateHeartbeat(document->url().path(), getFileProject(document->url()), document->cursorPosition().line(), true);
	sendHeartbeat(args);
	lastHeartbeat = now;
	lastFile = document->url().path();
}

QString WakaTime::getWakatimeBin()
{
	return QStandardPaths::findExecutable(QStringLiteral("wakatime"));
}

void WakaTime::checkWakatimeBin()
{
	if (getWakatimeBin().isEmpty()) {
		qCDebug(PLUGIN_KDEVWAKATIME) << "Unable to find wakatime executable. Is it installed on the system?";
		setErrorDescription(i18n("Unable to find wakatime executable. Is it installed on the system?"));
		return;
	}
}

bool WakaTime::isFileIgnored(QString file)
{
	return false;
}

QStringList WakaTime::generateHeartbeat(QString file, QString project, int line, bool write)
{
	QStringList args;

	if (!project.isEmpty()) {
		args << "--alternate-project" << project;
	}

	args << "--entity" << file;

	QString plugin = QString("kdevelop-wakatime/");
	plugin.append(version);
	args << "--plugin" << plugin;

	if (write) {
		args << "--write";
	}

	if (line != 0) {
		args << "--lineno" << QString::number(line);
	}

	QString config = QStandardPaths::locate(QStandardPaths::HomeLocation, QStringLiteral(".wakatime.cfg"));
	if (!config.isEmpty()) {
		args << "--config" << config;
	}

	return args;
}

void WakaTime::sendHeartbeat(QStringList args)
{
	QString program = getWakatimeBin();
	QProcess *sender = new QProcess(this);
	sender->setProcessChannelMode(QProcess::MergedChannels);
	qCDebug(PLUGIN_KDEVWAKATIME) << program << args;
	sender->startDetached(program, args);

	if (!sender->waitForFinished()) {
		qCDebug(PLUGIN_KDEVWAKATIME) << "Hearbeat failed:" << sender->errorString();
	} else {
		if (sender->exitCode() == 102) {
			qCDebug(PLUGIN_KDEVWAKATIME) << "WakaTime Offline, coding activity will sync when online.";
			setErrorDescription(i18n("WakaTime Offline, coding activity will sync when online."));
		} else if (sender->exitCode() == 103) {
			qCDebug(PLUGIN_KDEVWAKATIME) << "An error occured while parsing $WAKATIME_HOME/.wakatime.cfg. Check $WAKATIME_HOME/.wakatime.log for more info.";
			setErrorDescription(i18n("An error occured while parsing $WAKATIME_HOME/.wakatime.cfg. Check $WAKATIME_HOME/.wakatime.log for more info."));
		} else if (sender->exitCode() == 104) {
			qCDebug(PLUGIN_KDEVWAKATIME) << "Invalid API Key. Make sure your API Key is correct!";
			setErrorDescription(i18n("Invalid API Key. Make sure your API Key is correct!"));
		}
	}
}

// needed for QObject class created from K_PLUGIN_FACTORY_WITH_JSON
#include "wakatime.moc"
