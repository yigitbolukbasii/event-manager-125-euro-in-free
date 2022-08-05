//Search:
		bool SendQuestConfirmPacket(BYTE byAnswer, DWORD dwPID);
		
//Add below:
		bool SendRequestEventQuest(const char* c_szName);
		
		
//Search:
		bool SendOnClickPacket(DWORD vid);
		
//Add below:
#ifdef ENABLE_EVENT_MANAGER
		bool SendRequestEventData(int iMonth);
		bool RecvEventInformation();
		bool RecvEventReload();
#endif

