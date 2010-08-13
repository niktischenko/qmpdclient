/*
 * QMPDClient - An MPD client written in Qt 4.
 * Copyright (C) 2005-2008 Håvard Tautra Knutsen <havtknut@tihlde.org>
 * Copyright (C) 2008 Voker57 <voker57@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "lyricsdialog.h"
#include "config.h"
#include "mpdsong.h"

#include <QNetworkProxy>

LyricsDialog::LyricsDialog(QWidget *parent) : QDialog(parent) {
	setupUi(this);
	if (Config::instance()->proxyEnabled()) {
		QString address = Config::instance()->proxyAddress();
		int port = Config::instance()->proxyPort();
		if (Config::instance()->proxyAuthorization()) {
			QString login = Config::instance()->proxyLogin();
			QString password = Config::instance()->proxyPassword();
			QNetworkProxy::setApplicationProxy(QNetworkProxy(QNetworkProxy::HttpProxy, address, port, login, password));
		}
		QNetworkProxy::setApplicationProxy(QNetworkProxy(QNetworkProxy::HttpProxy, address, port));
	} else {
		QNetworkProxy::setApplicationProxy(QNetworkProxy::NoProxy);
	}
}

void LyricsDialog::show() {
	updateLyrics();
	QDialog::show();
}

void LyricsDialog::updateLyrics() {
	setWindowTitle(QString("Lyrics: %1 by %2").arg(m_title, m_artist));
	artistEdit->setText(m_artist);
	titleEdit->setText(m_title);
	lyricsBrowser->setHtml(tr("Getting lyrics from server..."));
	QUrl req("http://www.lyricsplugin.com/winamp03/plugin/");
	req.addQueryItem("artist", m_artist);
	req.addQueryItem("title", m_title);
	lyricsBrowser->setUrl(req);
}


void LyricsDialog::setSong(const MPDSong &s) {
	m_artist=s.artist();
	m_title=s.title();
	if(!isHidden()) updateLyrics();
}


void LyricsDialog::setUserSong() {
	m_artist = artistEdit->text();
	m_title = titleEdit->text();
	updateLyrics();
}
