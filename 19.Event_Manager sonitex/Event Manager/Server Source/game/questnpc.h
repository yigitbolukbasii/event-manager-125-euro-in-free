//Search:
			bool	HasChat();

//Add below:
#ifdef __EVENT_MANAGER__
			bool	OnEventBegin(PC& pc);
			bool	OnEventEnd(PC& pc);
#endif