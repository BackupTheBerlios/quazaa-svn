/*
** endpoint.h
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

#ifndef ENDPOINT_H
#define RNDPOINT_H

#include <QHostAddress>

class CEndPoint : public QHostAddress
{
protected:
	quint16	m_nPort;

public:
	CEndPoint();
	explicit CEndPoint(quint32 ip4Addr, quint16 nPort = 0);
	explicit CEndPoint(quint8 *ip6Addr, quint16 nPort = 0);
	explicit CEndPoint(const Q_IPV6ADDR &ip6Addr, quint16 nPort = 0);
	explicit CEndPoint(const sockaddr *sockaddr, quint16 nPort = 0);
	explicit CEndPoint(const QString &address, quint16 nPort);
	explicit CEndPoint(const QString &address);
	explicit CEndPoint(const QHostAddress &address, quint16 nPort);
	CEndPoint(const CEndPoint &copy);
	CEndPoint(SpecialAddress address, quint16 nPort = 0);
	void setAddressWithPort(const QString &address);

public:
	void clear();
	QString toStringWithPort() const;
	quint16 port() const;
	void setPort(const quint16 nPort);
};

#endif // ENDPOINT_H
