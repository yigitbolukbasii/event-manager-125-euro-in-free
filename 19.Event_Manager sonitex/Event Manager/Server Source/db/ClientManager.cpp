//Search:
void CClientManager::QUERY_BOOT(CPeer* peer, TPacketGDBoot * p)
{
	[...]
		sizeof(WORD) + sizeof(WORD) + sizeof(building::TObject) * m_map_pkObjectTable.size() +
		
//Add below:
#ifdef __EVENT_MANAGER__
		sizeof(WORD) + sizeof(WORD) + sizeof(TEventTable) * m_vec_eventTable.size() +
#endif


//Search:
void CClientManager::QUERY_BOOT(CPeer* peer, TPacketGDBoot * p)
{
	[...]
	sys_log(0, "sizeof(TObject) = %d", sizeof(building::TObject));
	
//Add below:
#ifdef __EVENT_MANAGER__
	sys_log(0, "sizeof(TEventTable) = %d", sizeof(TEventTable));
#endif


//Search:
void CClientManager::QUERY_BOOT(CPeer* peer, TPacketGDBoot * p)
{
	[...]
	while (it != m_map_pkObjectTable.end())
		peer->Encode((it++)->second, sizeof(building::TObject));
	
//Add below:
#ifdef __EVENT_MANAGER__
	peer->EncodeWORD(sizeof(TEventTable));
	peer->EncodeWORD(m_vec_eventTable.size());
	peer->Encode(&m_vec_eventTable[0], sizeof(TEventTable) * m_vec_eventTable.size());
#endif


//Search:
void CClientManager::QUERY_RELOAD_PROTO()
{
	[...]
			sizeof(WORD) + sizeof(TMobTable) * m_vec_mobTable.size());
			
//Replace/Add below:
			sizeof(WORD) + sizeof(TMobTable) * m_vec_mobTable.size() +
#ifdef __EVENT_MANAGER__
			sizeof(WORD) + sizeof(TEventTable) * m_vec_eventTable.size());
#endif


//Search:
		tmp->EncodeWORD(m_vec_mobTable.size());
		tmp->Encode(&m_vec_mobTable[0], sizeof(TMobTable) * m_vec_mobTable.size());

//Add below:
#ifdef __EVENT_MANAGER__
		tmp->EncodeWORD(m_vec_eventTable.size());
		tmp->Encode(&m_vec_eventTable[0], sizeof(TEventTable) * m_vec_eventTable.size());
#endif


//Search:
			case HEADER_GD_REQUEST_CHANNELSTATUS:
				RequestChannelStatus(peer, dwHandle);
				break;
				
//Add below:
#ifdef __EVENT_MANAGER__
			case HEADER_GD_UPDATE_EVENT_STATUS:
				UpdateEventStatus(*(DWORD*)data);
				break;

			case HEADER_GD_EVENT_NOTIFICATION:
				EventNotification((TPacketSetEventFlag*)data);
				break;
#endif


//Add:
#ifdef __EVENT_MANAGER__
void CClientManager::UpdateEventStatus(DWORD dwID)
{
	char buf[128];
	snprintf(buf, sizeof(buf), "UPDATE event%s SET completed=TRUE WHERE id=%u", GetTablePostfix(), dwID);
	CDBManager::instance().AsyncQuery(buf);

	for (auto it = m_vec_eventTable.begin(); it != m_vec_eventTable.end(); ++it)
	{
		if (it->dwID == dwID)
			it->bCompleted = true;
	}

	ForwardPacket(HEADER_DG_UPDATE_EVENT_STATUS, &dwID, sizeof(DWORD));
}

void CClientManager::EventNotification(TPacketSetEventFlag* p)
{
	ForwardPacket(HEADER_DG_EVENT_NOTIFICATION, p, sizeof(TPacketSetEventFlag));

	bool bChanged = false;

	auto it = m_map_lEventFlag.find(p->szFlagName);
	if (it == m_map_lEventFlag.end())
	{
		bChanged = true;
		m_map_lEventFlag.insert(std::make_pair(std::string(p->szFlagName), p->lValue));
	}
	else if (it->second != p->lValue)
	{
		bChanged = true;
		it->second = p->lValue;
	}

	if (bChanged)
	{
		char szQuery[1024];
		snprintf(szQuery, sizeof(szQuery),
			"REPLACE INTO quest%s (dwPID, szName, szState, lValue) VALUES(0, '%s', '', %ld)",
			GetTablePostfix(), p->szFlagName, p->lValue);
		szQuery[1023] = '\0';

		CDBManager::instance().AsyncQuery(szQuery);
		sys_log(0, "HEADER_GD_SET_EVENT_FLAG : Changed CClientmanager::SetEventFlag(%s %d) ", p->szFlagName, p->lValue);
		return;
	}
	sys_log(0, "HEADER_GD_SET_EVENT_FLAG : No Changed CClientmanager::SetEventFlag(%s %d) ", p->szFlagName, p->lValue);
}
#endif