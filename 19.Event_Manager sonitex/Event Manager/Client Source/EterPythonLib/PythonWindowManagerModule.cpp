//Add:
PyObject* wndSetAniImgScale(PyObject* poSelf, PyObject* poArgs)
{
	UI::CWindow* pWindow;
	if (!PyTuple_GetWindow(poArgs, 0, &pWindow))
		return Py_BuildException();
	float fScaleX;
	if (!PyTuple_GetFloat(poArgs, 1, &fScaleX))
		return Py_BuildException();
	float fScaleY;
	if (!PyTuple_GetFloat(poArgs, 2, &fScaleY))
		return Py_BuildException();

	((UI::CAniImageBox*)pWindow)->SetAniImgScale(fScaleX, fScaleY);

	return Py_BuildNone();
}

//Search:
		// AniImageBox
		{ "SetDelay",					wndImageSetDelay,					METH_VARARGS },
		{ "AppendImage",				wndImageAppendImage,				METH_VARARGS },
		
//Add below:
		{ "SetAniImgScale",				wndSetAniImgScale,					METH_VARARGS },