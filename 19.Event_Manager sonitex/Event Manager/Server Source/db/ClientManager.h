//Search:
	bool		InitializeMonarch();
	
//Add below:
#ifdef __EVENT_MANAGER__
	bool		InitializeEventTable();
#endif


//Search:
	std::map<DWORD, building::TObject *>	m_map_pkObjectTable;

//Add below:
#ifdef __EVENT_MANAGER__
	std::vector<TEventTable>		m_vec_eventTable;
#endif


//Search:
	void RequestChannelStatus(CPeer* peer, DWORD dwHandle);

//Add below:
#ifdef __EVENT_MANAGER__
	void UpdateEventStatus(DWORD dwID);
	void EventNotification(TPacketSetEventFlag* p);
#endif