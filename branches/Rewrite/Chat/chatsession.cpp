/*
** chatsession.cpp
**
** Copyright © Quazaa Development Team, 2009-2011.
** This file is part of QUAZAA (quazaa.sourceforge.net)
**
** Quazaa is free software; this file may be used under the terms of the GNU
** General Public License version 3.0 or later as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.
**
** Quazaa is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
**
** Please review the following information to ensure the GNU General Public
** License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** You should have received a copy of the GNU General Public License version
** 3.0 along with Quazaa; if not, write to the Free Software Foundation,
** Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "chatsession.h"
#include "chatcore.h"
#include "quazaasettings.h"
#include "widgetprivatemessage.h"
#include "winmain.h"

CChatSession::CChatSession(QObject *parent) :
	CNetworkConnection(parent)
{
	m_nState = csNull;
	m_nProtocol = dpNull;
	m_pWidget = 0;

	m_bShareaza = false;
}
CChatSession::~CChatSession()
{
	ChatCore.Remove(this);
}

// called from GUI thread
void CChatSession::Connect()
{
	MainWindow->OpenChat(this);
	m_nState = csConnecting;
	ChatCore.Add(this);

	if( thread() != &ChatThread )
		moveToThread(&ChatThread);
}


void CChatSession::OnTimer(quint32 tNow)
{
	if( m_nState < csConnected )
	{
		if( tNow - m_tConnected > quazaaSettings.Connection.TimeoutConnect )
		{
			m_nState = csClosed;
			CNetworkConnection::Close();
			emit systemMessage("Timed out connecting to remote host");
		}
	}
	else if( m_nState == csConnected )
	{

	}
}

void CChatSession::OnConnect()
{

}
void CChatSession::OnDisconnect()
{
	m_nState = csClosed;
	emit systemMessage("Connection lost");
}
void CChatSession::OnRead()
{

}
void CChatSession::OnError(QAbstractSocket::SocketError e)
{
	Q_UNUSED(e);
}

void CChatSession::OnStateChange(QAbstractSocket::SocketState s)
{
	Q_UNUSED(s);
}

void CChatSession::SetupWidget(WidgetPrivateMessage *pWg)
{
	m_pWidget = pWg;

	connect(this, SIGNAL(guidChanged(QUuid)), m_pWidget, SLOT(OnGUIDChanged(QUuid)));
	connect(this, SIGNAL(nickChanged(QString)), m_pWidget, SLOT(OnNickChanged(QString)));
	connect(this, SIGNAL(incomingMessage(QString,bool)), m_pWidget, SLOT(OnIncomingMessage(QString,bool)));
	connect(this, SIGNAL(systemMessage(QString)), m_pWidget, SLOT(OnSystemMessage(QString)));
	connect(m_pWidget, SIGNAL(SendMessageS(QString,bool)), this, SLOT(SendMessage(QString,bool)));
	connect(m_pWidget, SIGNAL(SendMessageS(QTextDocument*,bool)), this, SLOT(SendMessage(QTextDocument*,bool)));
	connect(m_pWidget, SIGNAL(destroyed()), this, SLOT(deleteLater()));
}
