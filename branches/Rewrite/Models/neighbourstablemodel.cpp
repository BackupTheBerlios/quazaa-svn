/*
** neighbourstablemodel.cpp
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

#include "neighbourstablemodel.h"

#include <QColor>
#include <QSize>
#include <QIcon>
#include <QMutexLocker>
#include <QFont>
#include "neighbour.h"
#include "g2node.h"
#include "network.h"
#include "neighbours.h"
#include "geoiplist.h"
#include <qabstractitemview.h>


CNeighboursTableModel::Neighbour::Neighbour(CNeighbour *pNeighbour) : pNode(pNeighbour)
{
	quint32 tNow = time(0);

	oAddress = pNode->GetAddress();
	tConnected = tNow - pNode->m_tConnected;
	nBandwidthIn = pNode->m_mInput.Usage();
	nBandwidthOut = pNode->m_mOutput.Usage();
	nBytesReceived = pNode->m_mInput.m_nTotal;
	nBytesSent = pNode->m_mOutput.m_nTotal;
	nCompressionIn = pNode->GetTotalInDecompressed();
	nCompressionOut = pNode->GetTotalOutCompressed();
	nLeafCount = 0;
	nLeafMax = 0;
	nPacketsIn = pNode->m_nPacketsIn;
	nPacketsOut = pNode->m_nPacketsOut;
	nRTT = pNode->m_tRTT;
	nState = pNode->m_nState;
	nType = G2_UNKNOWN;
	sUserAgent = pNode->m_sUserAgent;
	sCountry = GeoIP.findCountryCode(pNode->m_oAddress);
	iCountry = QIcon(":/Resource/Flags/" + sCountry.toLower() + ".png");

	switch( pNode->m_nProtocol )
	{
	case dpGnutella2:
		nLeafCount = ((CG2Node*)pNode)->m_nLeafCount;
		nLeafMax = ((CG2Node*)pNode)->m_nLeafMax;
		iNetwork = QIcon(":/Resource/Networks/Gnutella2.png");
		nType = ((CG2Node*)pNode)->m_nType;
		break;
	default:
		break;
	}
}

bool CNeighboursTableModel::Neighbour::update(int row, int col, QModelIndexList &to_update, CNeighboursTableModel *model)
{
	if( !Neighbours.NeighbourExists(pNode) )
		return false;

	bool bRet = false;

	quint32 tNow = time(0);

	if( oAddress != pNode->GetAddress() )
	{
		to_update.append(model->index(row, ADDRESS));
		oAddress = pNode->GetAddress();

		if( col == ADDRESS )
			bRet = true;
	}

	tConnected = tNow - pNode->m_tConnected;
	to_update.append(model->index(row, TIME));
	if( col == TIME )
		bRet = true;

	nBandwidthIn = pNode->m_mInput.Usage();
	nBandwidthOut = pNode->m_mOutput.Usage();
	to_update.append(model->index(row, BANDWIDTH));
	if( col == BANDWIDTH )
		bRet = true;

	nBytesReceived = pNode->m_mInput.m_nTotal;
	nBytesSent = pNode->m_mOutput.m_nTotal;
	nCompressionIn = pNode->GetTotalInDecompressed();
	nCompressionOut = pNode->GetTotalOutCompressed();
	to_update.append(model->index(row, BYTES));
	if( col == BYTES )
		bRet = true;

	if( nPacketsIn != pNode->m_nPacketsIn || nPacketsOut != pNode->m_nPacketsOut )
	{
		nPacketsIn = pNode->m_nPacketsIn;
		nPacketsOut = pNode->m_nPacketsOut;
		to_update.append(model->index(row, PACKETS));
		if( col == PACKETS )
			bRet = true;
	}

	if( nRTT != pNode->m_tRTT )
	{
		nRTT = pNode->m_tRTT;
		to_update.append(model->index(row, PING));
		if( col == PING )
			bRet = true;
	}

	if( nState != pNode->m_nState )
	{
		nState = pNode->m_nState;
		to_update.append(model->index(row, STATUS));
		if( col == STATUS )
			bRet = true;
	}

	if( sUserAgent != pNode->m_sUserAgent )
	{
		sUserAgent = pNode->m_sUserAgent;
		to_update.append(model->index(row, USER_AGENT));
		if( col == USER_AGENT )
			bRet = true;
	}

	switch( pNode->m_nProtocol )
	{
	case dpGnutella2:
		if( nLeafCount != ((CG2Node*)pNode)->m_nLeafCount || nLeafMax != ((CG2Node*)pNode)->m_nLeafMax )
		{
			nLeafCount = ((CG2Node*)pNode)->m_nLeafCount;
			nLeafMax = ((CG2Node*)pNode)->m_nLeafMax;
			to_update.append(model->index(row, LEAVES));
			if( col == LEAVES )
				bRet = true;
		}

		if( nType != ((CG2Node*)pNode)->m_nType )
		{
			nType = ((CG2Node*)pNode)->m_nType;
			to_update.append(model->index(row, MODE));
			if( col == MODE )
				bRet = true;
		}
		break;
	default:
		break;
	}

	return bRet;
}

QVariant CNeighboursTableModel::Neighbour::data(int col) const
{
	switch(col)
	{
		case ADDRESS:
			return const_cast<CEndPoint*>(&oAddress)->toStringWithPort();
		case STATUS:
			return StateToString(nState);
		case TIME:
			return QString().sprintf("%.2u:%.2u:%.2u", tConnected / 3600, (tConnected % 3600 / 60), (tConnected % 3600) % 60);
		case BANDWIDTH:
			return QString().sprintf("%1.3f / %1.3f", nBandwidthIn / 1024.0f, nBandwidthOut / 1024.0f);
		case BYTES:
			return QString().sprintf("%1.3f / %1.3f KB (%1.2f / %1.2f %%)", nBytesReceived / 1024.0f, nBytesSent / 1024.0f, 100.0f * nBytesReceived / nCompressionIn, 100.0f * nBytesSent / nCompressionOut);
		case PACKETS:
			return QString().sprintf("%u - %u", nPacketsIn, nPacketsOut);
		case MODE:
			return TypeToString(nType);
		case LEAVES:
			if(nType == G2_HUB)
			{
				return QString().sprintf("%u / %u", nLeafCount, nLeafMax);
			}
			else
			{
				return QString();
			}
		case PING:
			return QString("%1 ms").arg(nRTT);
		case USER_AGENT:
			return sUserAgent;
		case COUNTRY:
			return GeoIP.countryNameFromCode(sCountry);
	}


	return QVariant();
}

bool CNeighboursTableModel::Neighbour::lessThan(int col, CNeighboursTableModel::Neighbour *pOther) const
{
	switch(col)
	{
	case ADDRESS:
		return oAddress.toString() < pOther->oAddress.toString();
	case STATUS:
		return nState < pOther->nState;
	case TIME:
		return tConnected < pOther->tConnected;
	case BANDWIDTH:
		return (nBandwidthIn + nBandwidthOut) < (pOther->nBandwidthIn + pOther->nBandwidthOut);
	case BYTES:
		return (nBytesReceived + nBytesSent) < (pOther->nBytesReceived + pOther->nBytesSent);
	case PACKETS:
		return (nPacketsIn + nPacketsOut) < (pOther->nPacketsIn + pOther->nPacketsOut);
	case MODE:
		return TypeToString(nType) < TypeToString(pOther->nType);
	case LEAVES:
		return nLeafCount < pOther->nLeafCount;
	case PING:
		if( nState < nsConnected )
			return false;
		if( pOther->nState < nsConnected )
			return true;
		return nRTT < pOther->nRTT;
	case USER_AGENT:
		return sUserAgent < pOther->sUserAgent;
	case COUNTRY:
		return GeoIP.countryNameFromCode(sCountry) < GeoIP.countryNameFromCode(pOther->sCountry);
	default:
		return false;
	}

}
QString CNeighboursTableModel::Neighbour::StateToString(int s) const
{
	switch(s)
	{
		case nsClosed:
			return "closed";
		case nsConnecting:
			return "connecting";
		case nsHandshaking:
			return "handshaking";
		case nsConnected:
			return "connected";
		case nsClosing:
			return "closing";
		case nsError:
			return "error";
	}

	return QString();
}
QString CNeighboursTableModel::Neighbour::TypeToString(G2NodeType t) const
{
	if(t == G2_HUB)
	{
		return "G2 Hub";
	}
	else if(t == G2_LEAF)
	{
		return "G2 Leaf";
	}
	else
	{
		return "";
	}
}


CNeighboursTableModel::CNeighboursTableModel(QObject* parent, QWidget* container) :
	QAbstractTableModel(parent)
{
	m_oContainer = container;
	m_nSortColumn = -1;
	m_bNeedSorting = false;

	connect(&Neighbours, SIGNAL(NeighbourAdded(CNeighbour*)), this, SLOT(AddNode(CNeighbour*)), Qt::QueuedConnection);
	connect(&Neighbours, SIGNAL(NeighbourRemoved(CNeighbour*)), this, SLOT(RemoveNode(CNeighbour*)), Qt::QueuedConnection);
}

CNeighboursTableModel::~CNeighboursTableModel()
{
	qDeleteAll(m_lNodes);
	m_lNodes.clear();
}

int CNeighboursTableModel::rowCount(const QModelIndex& parent) const
{
	if(parent.isValid())
	{
		return 0;
	}
	else
	{
		return m_lNodes.count();
	}
}

int CNeighboursTableModel::columnCount(const QModelIndex& parent) const
{
	if(parent.isValid())
	{
		return 0;
	}
	else
	{
		return _NO_OF_COLUMNS;
	}
}

QVariant CNeighboursTableModel::data(const QModelIndex& index, int role) const
{
	if(!index.isValid())
	{
		return QVariant();
	}

	if(index.row() > m_lNodes.size() || index.row() < 0)
	{
		return QVariant();
	}

	const Neighbour* nbr = m_lNodes.at(index.row());

	if( role == Qt::DisplayRole )
	{
		return nbr->data(index.column());
	}
	else if(role == Qt::DecorationRole)
	{
		if(index.column() == ADDRESS)
		{
			return nbr->iNetwork;
		}
		else if(index.column() == COUNTRY)
		{
			return nbr->iCountry;
		}
	}
	else if(role == Qt::ForegroundRole)
	{
		switch(nbr->nState)
		{
			case nsConnected:
				//return skinSettings.listsColorSpecial;
				break;
			case nsConnecting:
				//return skinSettings.listsColorActive;
				break;
			default:
				//return skinSettings.listsColorNormal;
				break;
		}
	}
	else if(role == Qt::FontRole)
	{
		QFont font = qApp->font(m_oContainer);
		switch(nbr->nState)
		{
			case nsConnected:
				//font.setWeight(skinSettings.listsWeightSpecial);
				return font;
				break;
			case nsConnecting:
				//font.setWeight(skinSettings.listsWeightActive);
				return font;
				break;
			default:
				//font.setWeight(skinSettings.listsWeightNormal);
				return font;
				break;
		}
	}

	return QVariant();
}

QVariant CNeighboursTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if( orientation != Qt::Horizontal )
		return QVariant();


	if(role == Qt::DisplayRole)
	{
		switch(section)
		{
			case ADDRESS:
				return tr("Address");
			case STATUS:
				return tr("Status");
			case TIME:
				return tr("Time");
			case BANDWIDTH:
				return tr("Bandwidth");
			case BYTES:
				return tr("Bytes");
			case PACKETS:
				return tr("Packets");
			case MODE:
				return tr("Mode");
			case LEAVES:
				return tr("Leaves");
			case PING:
				return tr("Ping");
			case USER_AGENT:
				return tr("User Agent");
			case COUNTRY:
				return tr("Country");
		}
	}
	else if(role == Qt::ToolTipRole)
	{
		switch(section)
		{
			case ADDRESS:
				return tr("The address of remote host");
			case STATUS:
				return tr("Connection status");
			case TIME:
				return tr("When this host was connected");
			case BANDWIDTH:
				return tr("Current download/upload bandwidth (and dl/ul compression)");
			case BYTES:
				return tr("How much data we downloaded/uploaded");
			case PACKETS:
				return tr("Number of received/sent packets");
			case MODE:
				return tr("Type of connection");
			case LEAVES:
				return tr("Number of nodes connected to the host");
			case PING:
				return tr("How fast remote host can reply");
			case USER_AGENT:
				return tr("Software operating on remote host");
			case COUNTRY:
				return tr("Country");
		}
	}

	return QVariant();
}
QModelIndex CNeighboursTableModel::index(int row, int column, const QModelIndex &parent) const
{
	if (parent.isValid())
		return QModelIndex();

	if (row < 0 || row >= m_lNodes.count())
		return QModelIndex();
	else
		return createIndex(row, column, m_lNodes[row]);
}

class CNeighboursTableModelCmp
{
public:
	CNeighboursTableModelCmp(int col,Qt::SortOrder o) : column(col),order(o)
	{}

	bool operator()(CNeighboursTableModel::Neighbour* a,CNeighboursTableModel::Neighbour* b)
	{
		if( order == Qt::AscendingOrder )
		{
			return a->lessThan(column, b);
		}
		else
		{
			return b->lessThan(column, a);
		}
	}

	int column;
	Qt::SortOrder order;
};
void CNeighboursTableModel::sort(int column, Qt::SortOrder order)
{
	m_nSortColumn = column;
	m_nSortOrder = order;

	emit layoutAboutToBeChanged();
	qStableSort(m_lNodes.begin(), m_lNodes.end(), CNeighboursTableModelCmp(column, order));
	emit layoutChanged();
}

CNeighbour* CNeighboursTableModel::NodeFromIndex(const QModelIndex &index)
{
	if (index.isValid() && index.row() < m_lNodes.count() && index.row() >= 0)
		return m_lNodes[index.row()]->pNode;
	else
		return 0;
}

void CNeighboursTableModel::AddNode(CNeighbour* pNode)
{
	Neighbours.m_pSection.lock();
	if( Neighbours.NeighbourExists(pNode) )
	{
		beginInsertRows(QModelIndex(), m_lNodes.size(), m_lNodes.size());
		m_lNodes.append(new Neighbour(pNode));
		endInsertRows();
		m_bNeedSorting = true;
	}
	Neighbours.m_pSection.unlock();
}

void CNeighboursTableModel::RemoveNode(CNeighbour* pNode)
{
	for(int i = 0, nMax = m_lNodes.size(); i < nMax; i++)
	{
		if(m_lNodes[i]->pNode == pNode)
		{
			beginRemoveRows(QModelIndex(), i, i);
			delete m_lNodes[i];
			m_lNodes.remove(i, 1);
			endRemoveRows();
			m_bNeedSorting = true;
			break;
		}
	}
}

void CNeighboursTableModel::UpdateAll()
{
	QModelIndexList uplist;
	bool bSort = m_bNeedSorting;

	if(Neighbours.m_pSection.tryLock())
	{
		for( int i = 0, max = m_lNodes.count(); i < max; ++i )
		{
			if( m_lNodes[i]->update(i, m_nSortColumn, uplist, this) )
				bSort = true;
		}

		Neighbours.m_pSection.unlock();
	}

	if( bSort )
	{
		sort(m_nSortColumn, m_nSortOrder);
	}
	else
	{
		if( !uplist.isEmpty() )
		{
			foreach( QModelIndex idx, uplist )
			{
				QAbstractItemView* pView = qobject_cast<QAbstractItemView*>(m_oContainer);

				if( pView )
				{
					pView->update(idx);
				}
			}
		}
	}
}
