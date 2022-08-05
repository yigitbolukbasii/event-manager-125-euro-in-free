//Search:
void CPythonNetworkStream::GamePhase()
	[...]
			case HEADER_GC_DRAGON_SOUL_REFINE:
				ret = RecvDragonSoulRefine();
				break;

//Add below:
#ifdef ENABLE_EVENT_MANAGER
			case HEADER_GC_EVENT_INFO:
				ret = RecvEventInformation();
				break;

			case HEADER_GC_EVENT_RELOAD:
				ret = RecvEventReload();
				break;
#endif


//Add
#ifdef ENABLE_EVENT_MANAGER
bool CPythonNetworkStream::SendRequestEventData(int iMonth)
{
	// Return if data has been already requested
	if (InGameEventManager::Instance().GetRequestEventData())
		return false;

	SPacketCGRequestEventData p;
	p.bHeader = HEADER_CG_REQUEST_EVENT_DATA;
	p.bMonth = BYTE(iMonth);

	if (!Send(sizeof(p), &p))
	{
		Tracen("Send SendRequestEventData Packet Error");
		return false;
	}

	Tracef("SendRequestEventData\n");
	return SendSequence();
}

bool CPythonNetworkStream::RecvEventInformation()
{
	TPacketGCEventInfo p;

	if (!Recv(sizeof(TPacketGCEventInfo), &p))
	{
		TraceError("CPythonNetworkStream::RecvEventInformation TPacketGCEventInfo Error");
		return false;
	}

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
		table.startTime = kElement.startTime;
		table.endTime = kElement.endTime;
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

bool CPythonNetworkStream::RecvEventReload()
{
	TPacketGCEventReload p;

	if (!Recv(sizeof(TPacketGCEventReload), &p))
	{
		TraceError("CPythonNetworkStream::RecvEventReload TPacketGCEventReload Error");
		return false;
	}

	InGameEventManager::Instance().SetRequestEventData(false);
	PyCallClassMemberFunc(m_apoPhaseWnd[PHASE_WINDOW_GAME], "BINARY_CloseInGameEvent", Py_BuildValue("()"));
	return true;
}
#endif

bool CPythonNetworkStream::SendRequestEventQuest(const char* c_szString)
{
	TPacketCGRequestEventQuest Packet;
	Packet.bHeader = HEADER_CG_REQUEST_EVENT_QUEST;
	strncpy(Packet.szName, c_szString, QUEST_NAME_MAX_NUM);

	if (!Send(sizeof(Packet), &Packet))
	{
		Tracen("SendRequestEventQuest Error");
		return false;
	}

	return SendSequence();
}