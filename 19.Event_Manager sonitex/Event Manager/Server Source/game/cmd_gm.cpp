//Search:
void BroadcastMonarchNotice(BYTE bEmpire, const char * c_pszBuf)
{
	TPacketGGMonarchNotice p;
	p.bHeader = HEADER_GG_MONARCH_NOTICE;
	p.bEmpire = bEmpire;
	p.lSize = strlen(c_pszBuf) + 1;

	TEMP_BUFFER buf;
	buf.write(&p, sizeof(p));
	buf.write(c_pszBuf, p.lSize);

	P2P_MANAGER::instance().Send(buf.read_peek(), buf.size());

	SendMonarchNotice(bEmpire, c_pszBuf);
}

//Add below:
#ifdef __EVENT_MANAGER__
struct send_event_begin_notification
{
	void operator() (LPDESC d)
	{
		if (!d->GetCharacter()) 
			return;

		quest::CQuestManager::instance().EventBegin(d->GetCharacter()->GetPlayerID());
	}
};

struct send_event_end_notification
{
	void operator() (LPDESC d)
	{
		if (!d->GetCharacter()) 
			return;

		quest::CQuestManager::instance().EventEnd(d->GetCharacter()->GetPlayerID());
	}
};

void SendEventBeginNotification()
{
	const DESC_MANAGER::DESC_SET& c_ref_set = DESC_MANAGER::instance().GetClientSet();
	std::for_each(c_ref_set.begin(), c_ref_set.end(), send_event_begin_notification());
}

void SendEventEndNotification()
{
	const DESC_MANAGER::DESC_SET& c_ref_set = DESC_MANAGER::instance().GetClientSet();
	std::for_each(c_ref_set.begin(), c_ref_set.end(), send_event_end_notification());
}

struct send_event_reload
{
	void operator() (LPDESC d)
	{
		if (!d->GetCharacter()) 
			return;

		TPacketGCEventReload p;
		p.bHeader = HEADER_GC_EVENT_RELOAD;

		d->Packet(&p, sizeof(p));
	}
};

void BroadcastEventReload()
{
	const DESC_MANAGER::DESC_SET& c_ref_set = DESC_MANAGER::instance().GetClientSet();
	std::for_each(c_ref_set.begin(), c_ref_set.end(), send_event_reload());
}

void ReloadEvent()
{
	TPacketGGReloadEvent p;
	p.bHeader = HEADER_GG_EVENT_RELOAD;

	P2P_MANAGER::instance().Send(&p, sizeof(p));

	BroadcastEventReload();
}
#endif
