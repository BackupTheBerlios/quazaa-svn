//
// widgetsearchmonitor.h
//
// Copyright © Quazaa Development Team, 2009-2010.
// This file is part of QUAZAA (quazaa.sourceforge.net)
//
// Quazaa is free software; you can redistribute it
// and/or modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 3 of
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

#ifndef WIDGETSEARCHMONITOR_H
#define WIDGETSEARCHMONITOR_H

#include <QMainWindow>

namespace Ui
{
	class WidgetSearchMonitor;
}

class WidgetSearchMonitor : public QMainWindow
{
	Q_OBJECT
public:
	WidgetSearchMonitor(QWidget* parent = 0);
	~WidgetSearchMonitor();
	void saveWidget();

protected:
	void changeEvent(QEvent* e);

private:
	Ui::WidgetSearchMonitor* ui;

private slots:
	void skinChangeEvent();
};

#endif // WIDGETSEARCHMONITOR_H
