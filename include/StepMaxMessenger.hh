#ifndef StepMaxMessenger_h
#define StepMaxMessenger_h 1

#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "G4UImessenger.hh"

class StepMax;
class G4UIcmdWithABool;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class StepMaxMessenger: public G4UImessenger
{
	public:
	StepMaxMessenger(StepMax*);
	~StepMaxMessenger();

	void SetNewValue(G4UIcommand*, G4String);

	private:
	StepMax* stepMax;
	G4UIcmdWithABool* StepMaxCmd;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
