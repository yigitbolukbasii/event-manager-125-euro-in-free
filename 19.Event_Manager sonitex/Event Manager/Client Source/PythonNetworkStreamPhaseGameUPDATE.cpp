//Replace this function:

bool CPythonNetworkStream::RecvEventInformation()
{
	TPacketGCEventInfo p;

	if (!Recv(sizeof(TPacketGCEventInfo), &p))
	{
		TraceError("CPythonNetworkStream::RecvEventInformation TPacketGCEventInfo Error");
		return false;
	}

	time_t serverTimeStamp = CPythonApplication::Instance().GetServerTimeStamp();
	time_t clientTimeStamp = time(0);
	time_t deltaTime = clientTimeStamp - serverTimeStamp;

	std::vector<InGameEventManager::TEventTable> eventVec;

	int iPacketSize = (p.iSize - sizeof(TPacketGCEventInfo));
	for (; iPacketSize > 0; iPacketSize -= sizeof(TPacketEventData))
	{
		TPacketEventData kElement;
		if (!Recv(sizeof(TPacketEventData), &kElement))
		{
			TraceError("CPythonNetworkStream::RecvEventInformation TPacketEventData Error");
			return false;
		}

		InGameEventManager::TEventTable table;
		table.dwID = kElement.dwID;
		table.bType = kElement.bType;
		table.startTime = kElement.startTime + deltaTime;
		table.endTime = kElement.endTime + deltaTime;
		table.iValue0 = kElement.iValue0;
		table.iValue1 = kElement.iValue1;
		table.bCompleted = kElement.bCompleted;

		eventVec.push_back(table);

	}

	InGameEventManager::Instance().SetRequestEventData(true);
	InGameEventManager::Instance().AddEventData(eventVec);

	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_OpenInGameEvent", Py_BuildValue("()"));

	return true;
}