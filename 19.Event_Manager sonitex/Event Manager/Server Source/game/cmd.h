//Search:
extern void SendMonarchNotice(BYTE bEmpire, const char * c_pszBuf);

//Add below:

#ifdef __EVENT_MANAGER__
extern void SendEventBeginNotification();
extern void SendEventEndNotification();

extern void BroadcastEventReload();
extern void ReloadEvent();
#endif