#ifndef CInputDataH
#define CInputDataH

#include "G4UImessenger.hh"
#include "MainMessenger.hh"

class CMainMessenger;

class CInputData
{
	public:
	CInputData(void);
	~CInputData(void);

	SInputData inputData;
	CMainMessenger *MainMessenger;

	inline void setNMax(G4int val){this->inputData.generalData.nMax=val;};
	inline void setBSave(G4bool val){this->inputData.generalData.bSave=val;};
	inline void setBUsePS(G4bool val){this->inputData.generalData.bUsePS=val;};
	inline void setPSname(G4String val){this->inputData.generalData.PSname=val;};
	inline void setDosefile(G4String val){this->inputData.generalData.dosefile=val;};
	
	private:
};

#endif
