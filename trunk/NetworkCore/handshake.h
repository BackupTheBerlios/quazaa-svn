//
// handshake.h
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

#ifndef HANDSHAKE_H
#define HANDSHAKE_H

#include <QObject>
#include "networkconnection.h"

class CHandshake: public CNetworkConnection
{
	Q_OBJECT

public:
	CHandshake(QObject* parent = 0);
	~CHandshake();

	void OnTimer(quint32 tNow);

public slots:
	void OnRead();

};

#endif // HANDSHAKE_H
