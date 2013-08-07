#include "DetectorMessenger.hh"

#include "DetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"

DetectorMessenger::DetectorMessenger(DetectorConstruction* Det)
:Detector(Det)
{
	accdet = new G4UIdirectory("/acc/");
	accdet->SetGuidance("accelerator control");

	killdet = new G4UIdirectory("/killer/");
	killdet->SetGuidance("set killer plane parameter");

	Energycmd = new G4UIcmdWithAnInteger("/acc/energy", this);
	Energycmd->SetDefaultValue(15);
	//	Energycmd->setEnergy(15);
	Energycmd->SetGuidance("set initail energy");

	X1cmd = new G4UIcmdWithADoubleAndUnit("/acc/X1", this);
	X1cmd->SetDefaultUnit("mm");
	X1cmd->SetDefaultValue(50.);
	Detector->SetX1(50.*mm);
	X1cmd->AvailableForStates(G4State_PreInit, G4State_Idle);
	X1cmd->SetGuidance("set X1 Jaw Position");

	X2cmd = new G4UIcmdWithADoubleAndUnit("/acc/X2", this);
	X2cmd->SetDefaultUnit("mm");
	X2cmd->SetDefaultValue(50.);
	Detector->SetX2(50.*mm);
	X2cmd->AvailableForStates(G4State_PreInit, G4State_Idle);
	X2cmd->SetGuidance("set X2 Jaw Position");

	Y1cmd = new G4UIcmdWithADoubleAndUnit("/acc/Y1", this);
	Y1cmd->SetDefaultUnit("mm");
	Y1cmd->SetDefaultValue(50.);
	Detector->SetY1(50.*mm);
	Y1cmd->AvailableForStates(G4State_PreInit, G4State_Idle);
	Y1cmd->SetGuidance("set Y1 Jaw Position");

	Y2cmd = new G4UIcmdWithADoubleAndUnit("/acc/Y2", this);
	Y2cmd->SetDefaultUnit("mm");
	Y2cmd->SetDefaultValue(50.);
	Detector->SetY2(50.*mm);
	Y2cmd->AvailableForStates(G4State_Init, G4State_Idle);
	Y2cmd->SetGuidance("set Y2 Jaw Position");

	Updatecmd = new G4UIcmdWithoutParameter("/acc/update", this);
	Updatecmd->AvailableForStates(G4State_Idle);

	Ecutcmd = new G4UIcmdWithADoubleAndUnit("/killer/e-", this);
	Ecutcmd->SetDefaultUnit("MeV");
	Ecutcmd->SetDefaultValue(0.);
	Detector->SetEcut(0.*MeV);
	Ecutcmd->AvailableForStates(G4State_Init, G4State_Idle);
	Ecutcmd->SetGuidance("set electron cut energy on killer plane");
	
	Gcutcmd = new G4UIcmdWithADoubleAndUnit("/killer/gamma", this);
	Gcutcmd->SetDefaultUnit("MeV");
	Gcutcmd->SetDefaultValue(0.);
	Detector->SetGcut(0.*MeV);
	Gcutcmd->AvailableForStates(G4State_Init, G4State_Idle);
	Gcutcmd->SetGuidance("set photon cut energy on killer plane");

	Ncutcmd = new G4UIcmdWithADoubleAndUnit("/killer/neutron", this);
	Ncutcmd->SetDefaultUnit("MeV");
	Ncutcmd->SetDefaultValue(0.);
	Detector->SetNcut(0.*MeV);
	Ncutcmd->AvailableForStates(G4State_Init, G4State_Idle);
	Ncutcmd->SetGuidance("set neutron cut energy on killer plane");

	Pcutcmd = new G4UIcmdWithADoubleAndUnit("/killer/e+", this);
	Pcutcmd->SetDefaultUnit("MeV");
	Pcutcmd->SetDefaultValue(0.);
	Detector->SetPcut(0.*MeV);
	Pcutcmd->AvailableForStates(G4State_Init, G4State_Idle);
	Pcutcmd->SetGuidance("set positron cut energy on killer plane");
}

DetectorMessenger::~DetectorMessenger()
{
		delete Energycmd;
		delete X1cmd;
		delete X2cmd;
		delete Y1cmd;
		delete Y2cmd;
		delete Updatecmd;
		delete Ecutcmd;
		delete Gcutcmd;
		delete Ncutcmd;
		delete Pcutcmd;
		delete accdet;
		delete killdet;
}

void DetectorMessenger::SetNewValue(G4UIcommand *cmd, G4String newValue)
{
	if(cmd==Energycmd)
	{
		Detector->SetEnergy(Energycmd->GetNewIntValue(newValue));
	}else if(cmd==X1cmd)
	{
		X1cmd->GetNewUnitValue(newValue);
		Detector->SetX1(X1cmd->GetNewDoubleValue(newValue));
		Detector->UpdateGeometry();
	}else if(cmd==X2cmd)
	{
		X2cmd->GetNewUnitValue(newValue);
		Detector->SetX2(X2cmd->GetNewDoubleValue(newValue));
		Detector->UpdateGeometry();
	}else if(cmd==Y1cmd)
	{
		Y1cmd->GetNewUnitValue(newValue);
		Detector->SetY1(Y1cmd->GetNewDoubleValue(newValue));
		Detector->UpdateGeometry();
	}else if(cmd==Y2cmd)
	{
		Y2cmd->GetNewUnitValue(newValue);
		Detector->SetY2(Y2cmd->GetNewDoubleValue(newValue));
		Detector->UpdateGeometry();
	}else if(cmd==Updatecmd){
		Detector->UpdateGeometry();
	}else if(cmd==Ecutcmd){
		Ecutcmd->GetNewUnitValue(newValue);
		Detector->SetEcut(Ecutcmd->GetNewDoubleValue(newValue));
		Detector->UpdateGeometry();
	}else if(cmd==Gcutcmd){
		Gcutcmd->GetNewUnitValue(newValue);
		Detector->SetGcut(Gcutcmd->GetNewDoubleValue(newValue));
		Detector->UpdateGeometry();
	}else if(cmd==Ncutcmd){
		Ncutcmd->GetNewUnitValue(newValue);
		Detector->SetNcut(Ncutcmd->GetNewDoubleValue(newValue));
		Detector->UpdateGeometry();
	}else if(cmd==Pcutcmd){
		Pcutcmd->GetNewUnitValue(newValue);
		Detector->SetPcut(Pcutcmd->GetNewDoubleValue(newValue));
		Detector->UpdateGeometry();
	}
}
