#include "StepMaxMessenger.hh"

#include "StepMax.hh"
#include "G4UIcmdWithABool.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

	StepMaxMessenger::StepMaxMessenger(StepMax* stepM)
:stepMax(stepM)
{ 
	StepMaxCmd = new G4UIcmdWithABool("/applyStepMax",this);
	StepMaxCmd->SetGuidance("apply StepMax computed from histograms");
	StepMaxCmd->SetParameterName("mxStep",true);
	StepMaxCmd->SetDefaultValue(true);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

StepMaxMessenger::~StepMaxMessenger()
{
	delete StepMaxCmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void StepMaxMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{ 
	if (command == StepMaxCmd)
	{ stepMax->ApplyMaxStep(StepMaxCmd->GetNewBoolValue(newValue));}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
