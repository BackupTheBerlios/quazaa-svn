/*
** widgetchatinput.h
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

#ifndef WIDGETCHATINPUT_H
#define WIDGETCHATINPUT_H

#include <QtGui>
#include "widgetreturnemittextedit.h"
#include "widgetsmileylist.h"

#include "network.h"

namespace Ui {
	class WidgetChatInput;
}

class WidgetChatInput : public QMainWindow {
	Q_OBJECT
public:
	WidgetChatInput(QWidget *parent = 0, bool isIRC = false);
	~WidgetChatInput();

signals:
	void messageSent(QTextDocument *text);
	void closing();

protected:
	void changeEvent(QEvent *e);
	WidgetReturnEmitTextEdit *textEditInput;

private:
	Ui::WidgetChatInput *ui;
	QCheckBox *checkBoxSendOnEnter;
	QToolButton *toolButtonSmilies;
	WidgetSmileyList *widgetSmileyList;
	QToolButton *toolButtonPickColor;
	QToolButton *toolButtonPrivateMessage;
	bool bIsIRC;

public slots:
	void setText(QString text);

private slots:
	void on_actionUnderline_toggled(bool checked);
	void on_actionItalic_toggled(bool checked);
	void on_actionBold_toggled(bool checked);
	void on_toolButtonSend_clicked();
	void onTextFormatChange(QTextCharFormat newFormat);
	void pickColor();
	void addPrivateMessage();
	void updateToolbar();
};

#endif // WIDGETCHATINPUT_H
