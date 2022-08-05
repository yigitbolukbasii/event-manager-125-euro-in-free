//Add:
#ifdef __EVENT_MANAGER__
	bool NPC::OnEventBegin(PC& pc)
	{
		return HandleEvent(pc, QUEST_GAME_EVENT_BEGIN);
	}

	bool NPC::OnEventEnd(PC& pc)
	{
		return HandleEvent(pc, QUEST_GAME_EVENT_END);
	}
#endif
