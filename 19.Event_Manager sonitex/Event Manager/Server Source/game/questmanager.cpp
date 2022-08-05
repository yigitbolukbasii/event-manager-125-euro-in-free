//Search:
		m_mapEventName.insert(TEventNameMap::value_type("item_informer", QUEST_ITEM_INFORMER_EVENT));

//Add below:
#ifdef __EVENT_MANAGER__
		m_mapEventName.insert(TEventNameMap::value_type("event_begin", QUEST_GAME_EVENT_BEGIN));		// Event for when a game event is started
		m_mapEventName.insert(TEventNameMap::value_type("event_end", QUEST_GAME_EVENT_END));			// Event for when a game event is ended
#endif


//Add:
#ifdef __EVENT_MANAGER__
	void CQuestManager::EventBegin(unsigned int pc)
	{
		PC* pPC;

		if ((pPC = GetPC(pc)))
		{
			if (!CheckQuestLoaded(pPC))
				return;

			m_mapNPC[QUEST_NO_NPC].OnEventBegin(*pPC);
		}
		else
			sys_err("QUEST no such pc id : %d", pc);
	}

	void CQuestManager::EventEnd(unsigned int pc)
	{
		PC* pPC;

		if ((pPC = GetPC(pc)))
		{
			if (!CheckQuestLoaded(pPC))
				return;

			m_mapNPC[QUEST_NO_NPC].OnEventEnd(*pPC);
		}
		else
			sys_err("QUEST no such pc id : %d", pc);
	}
#endif

	void CQuestManager::RequestEventQuest(const std::string& quest_name, unsigned int pc)
	{
		if (GetPCForce(pc)->IsRunning())
			return;

		const int questIdx = GetQuestIndexByName(quest_name);
		if (questIdx)
			QuestButton(pc, questIdx);
	}