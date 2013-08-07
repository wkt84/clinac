#include "CInputData.hh"

CInputData::CInputData(void)
{
	this->MainMessenger = new CMainMessenger(this);
}

CInputData::~CInputData(void)
{
}
