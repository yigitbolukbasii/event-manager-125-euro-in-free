//Search:
		void		QuestConfirm(LPCHARACTER ch, const void* pvData);
		
//Add below:
		void		RequestEventQuest(LPCHARACTER ch, const void* pvData);
		
		
//Search:
		void		Roulette(LPCHARACTER ch, const char* c_pData);

//Add below:
#ifdef __EVENT_MANAGER__
		void		RequestEventData(LPCHARACTER ch, const char* c_pData);
#endif


//Search:
	void		RespondChannelStatus(LPDESC desc, const char* pcData);

//Add below
#ifdef __EVENT_MANAGER__
	void		EventNotification(const char* c_pData);
#endif


//Search:
		void		IamAwake(LPDESC d, const char * c_pData);

//Add below:
#ifdef __EVENT_MANAGER__
		void		Event(const char* c_pData);
#endif