//Search:
PyObject* netSendQuestConfirmPacket(PyObject* poSelf, PyObject* poArgs)
{
	int iAnswer;
	if (!PyTuple_GetInteger(poArgs, 0, &iAnswer))
		return Py_BuildException();
	int iPID;
	if (!PyTuple_GetInteger(poArgs, 1, &iPID))
		return Py_BuildException();

	CPythonNetworkStream& rns=CPythonNetworkStream::Instance();
	rns.SendQuestConfirmPacket(iAnswer, iPID);

	return Py_BuildNone();
}

//Add below:
PyObject* netSendRequestEventQuest(PyObject* poSelf, PyObject* poArgs)
{
	char* szString;
	if (!PyTuple_GetString(poArgs, 0, &szString))
		return Py_BuildException();

	CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
	rns.SendRequestEventQuest(szString);

	return Py_BuildNone();
}


//Search:
PyObject* netGetLoginID(PyObject* poSelf, PyObject* poArgs)
{

	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	return Py_BuildValue("s", rkNetStream.GetLoginID().c_str());
}

//Add
#ifdef ENABLE_EVENT_MANAGER
#include "PythonGameEventManager.h"
PyObject* netSendRequestEventData(PyObject* poSelf, PyObject* poArgs)
{
	int iMonth;
	if (!PyTuple_GetInteger(poArgs, 0, &iMonth))
		return Py_BuildException();

	if ((iMonth >= 0) && (iMonth <= InGameEventManager::MONTH_MAX_NUM))
	{
		CPythonNetworkStream& rns = CPythonNetworkStream::Instance();
		rns.SendRequestEventData(iMonth);
	}

	return Py_BuildNone();
}
#endif


//Search:
		{ "RegisterErrorLog",						netRegisterErrorLog,						METH_VARARGS },
		
//Add below:
		{ "SendRequestEventQuest",					netSendRequestEventQuest,					METH_VARARGS },
#ifdef ENABLE_EVENT_MANAGER
		{ "SendRequestEventData",					netSendRequestEventData,					METH_VARARGS },
#endif