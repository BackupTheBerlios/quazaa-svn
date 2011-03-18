/*
** dialogprivatemessage.cpp
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

#include "dialogprivatemessages.h"
#include "ui_dialogprivatemessages.h"

#include "widgetprivatemessage.h"
#include "chatcore.h"

#include <QTextDocument>

DialogPrivateMessages::DialogPrivateMessages(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::DialogPrivateMessages)
{
	ui->setupUi(this);
	widgetChatInput = new WidgetChatInput(this);
	ui->horizontalLayoutChatInput->addWidget(widgetChatInput);
	connect(widgetChatInput, SIGNAL(messageSent(QTextDocument*)), this, SLOT(onMessageSent(QTextDocument*)));
	m_pCurrentWidget = 0;
	qRegisterMetaType<QUuid>("QUuid");
}

DialogPrivateMessages::~DialogPrivateMessages()
{
	delete widgetChatInput;
	delete ui;
}

WidgetPrivateMessage* DialogPrivateMessages::FindByGUID(QUuid oGUID)
{
	foreach( WidgetPrivateMessage* pWg, m_lChatWindows )
	{
		if( oGUID == pWg->m_oGUID )
			return pWg;
	}

	return 0;
}

void DialogPrivateMessages::changeEvent(QEvent *e)
{
	QDialog::changeEvent(e);
	switch (e->type()) {
	case QEvent::LanguageChange:
		ui->retranslateUi(this);
		break;
	default:
		break;
	}
}

void DialogPrivateMessages::OpenChat(CChatSession* pSess)
{
	ChatCore.m_pSection.lock();
	WidgetPrivateMessage* pWg = FindByGUID(pSess->m_oGUID);
	if( pWg == 0 )
	{
		pWg = new WidgetPrivateMessage(this);
		m_lChatWindows.append(pWg);
		int idx = ui->tabWidgetPrivateMessages->addTab(pWg, pSess->m_oAddress.toStringWithPort());
		m_pCurrentWidget = pWg;
		ui->tabWidgetPrivateMessages->setCurrentIndex(idx);
	}

	pSess->SetupWidget(pWg);
	pWg->show();
	ChatCore.m_pSection.unlock();
	raise();
}

void DialogPrivateMessages::onMessageSent(QTextDocument *pDoc)
{
	QString sMessage = pDoc->toPlainText();

	if( sMessage.isEmpty() )
		return;

	if( sMessage.left(2) != "//" )
	{
		if( sMessage.left(1) == "/" ) // is it a command?
		{
			QString sCmd, sParam;
			int nSpace = sMessage.indexOf(" ");
			if( nSpace == -1 )
				nSpace = sMessage.length();

			sCmd = sMessage.left(nSpace).toLower();
			sParam = sMessage.mid(nSpace + 1);

			if( sCmd == "/me" )
			{
				if( !sParam.isEmpty() )
					m_pCurrentWidget->SendMessage(sParam, true);
			}
			else
			{
				m_pCurrentWidget->OnSystemMessage("Unknown command");
			}

			return;
		}
	}
	m_pCurrentWidget->SendMessage(pDoc, false);
}

void DialogPrivateMessages::on_tabWidgetPrivateMessages_currentChanged(int index)
{
	if( ui->tabWidgetPrivateMessages->count() > 0 )
		m_pCurrentWidget = m_lChatWindows[index];
}

void DialogPrivateMessages::on_tabWidgetPrivateMessages_tabCloseRequested(int index)
{
	ui->tabWidgetPrivateMessages->removeTab(index);
	m_lChatWindows.removeAt(index);

	if( m_lChatWindows.isEmpty() )
	{
		close();
	}
}
