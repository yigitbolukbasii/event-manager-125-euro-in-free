//Search:
bool CClientManager::InitializeTables()
{
	[...]
	if (!InitializeMonarch())
	{
		sys_err("InitializeMonarch FAILED");
		return false;
	}

//Add below:
#ifdef __EVENT_MANAGER__
	if (!InitializeEventTable())
	{
		sys_err("InitializeEventTable FAILED");
		return false;
	}
#endif


//Add:
#ifdef __EVENT_MANAGER__
bool CClientManager::InitializeEventTable()
{
	char query[4096];
	snprintf(query, sizeof(query), "SELECT id, type, UNIX_TIMESTAMP(start), UNIX_TIMESTAMP(end), value0, value1, completed FROM event%s ORDER BY start", GetTablePostfix());

	std::unique_ptr<SQLMsg> pkMsg(CDBManager::instance().DirectQuery(query));
	SQLResult* pRes = pkMsg->Get();

	if (!m_vec_eventTable.empty())
	{
		sys_log(0, "RELOAD: event");
		m_vec_eventTable.clear();
	}

	m_vec_eventTable.reserve(pRes->uiNumRows);

	MYSQL_ROW data;

	if (pRes->uiNumRows > 0)
	{
		while ((data = mysql_fetch_row(pRes->pSQLResult)))
		{
			TEventTable t;
			memset(&t, 0, sizeof(t));

			int col = 0;

			str_to_number(t.dwID, data[col++]);
			strlcpy(t.szType, data[col++], sizeof(t.szType));
			str_to_number(t.startTime, data[col++]);
			str_to_number(t.endTime, data[col++]);
			str_to_number(t.iValue0, data[col++]);
			str_to_number(t.iValue1, data[col++]);
			str_to_number(t.bCompleted, data[col++]);

			sys_log(0, "EVENT: %s start %lu end %lu", t.szType, t.startTime, t.endTime);

			m_vec_eventTable.push_back(t);
		}
	}

	return true;
}
#endif