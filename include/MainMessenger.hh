#ifndef CMainMessengerH
#define CMainMessengerH

#include "Main.hh"
#include "G4UImessenger.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAString.hh"

#include "CInputData.hh"

class CMain;
class CInputData;
class G4UIcmdWithAnInteger;
class G4UIcmdWithABool;
class G4UIcmdWithAString;

class CMainMessenger : public G4UImessenger
{
	public:
	CMainMessenger(CInputData *CInput);
	~CMainMessenger(void);
	void SetNewValue(G4UIcommand* cmd, G4String newValue);

	private:
	CInputData *CInput;
	CMain *Main;

	G4UIcmdWithAnInteger *nMax;
	G4UIcmdWithABool *bSave;
	G4UIcmdWithABool *bUsePS;
	G4UIcmdWithAString *PSname;
};

#endif
