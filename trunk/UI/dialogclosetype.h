//
// dialogclosetype.h
//
// Copyright � Quazaa Development Team, 2009-2010.
// This file is part of QUAZAA (quazaa.sourceforge.net)
//
// Quazaa is free software; you can redistribute it
// and/or modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2 of
// the License, or (at your option) any later version.
//
// Quazaa is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Quazaa; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//

#ifndef DIALOGCLOSETYPE_H
#define DIALOGCLOSETYPE_H

#include <QtGui/QDialog>

namespace Ui
{
	class DialogCloseType;
}

class DialogCloseType : public QDialog
{
	Q_OBJECT
public:
	DialogCloseType(QWidget* parent = 0);
	~DialogCloseType();

protected:
	void changeEvent(QEvent* e);

private:
	Ui::DialogCloseType* m_ui;

signals:
	void closed();

private slots:
	void on_pushButtonOK_clicked();
	void skinChangeEvent();
};

#endif // DIALOGCLOSETYPE_H
