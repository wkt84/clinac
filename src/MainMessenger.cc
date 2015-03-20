#include "MainMessenger.hh"

CMainMessenger::CMainMessenger(CInputData *CInput)
{
	this->CInput = CInput;

	this->nMax = new G4UIcmdWithAnInteger("/general/nMax",this);
	this->nMax->SetDefaultValue(10000);
	this->nMax->SetGuidance("maximum particle stored in RAM before saving");
	this->CInput->setNMax(10000);

	this->bSave = new G4UIcmdWithABool("/general/bSave",this);
	this->bSave->SetDefaultValue(false);
	this->bSave->SetGuidance("Save phase space data or not");
	this->CInput->setBSave(false);

	this->bUsePS = new G4UIcmdWithABool("/general/bUsePS",this);
	this->bUsePS->SetDefaultValue(false);
	this->bUsePS->SetGuidance("Use phase space data as primaries or not");
	this->CInput->setBUsePS(false);

	this->PSname = new G4UIcmdWithAString("/general/PSname",this);
	this->PSname->SetDefaultValue("");
	this->PSname->SetGuidance("File name of phase space data");
	this->CInput->setPSname("");

	this->dosefile = new G4UIcmdWithAString("/general/dosefile", this);
	this->dosefile->SetDefaultValue("");
	this->dosefile->SetGuidance("File name of dose data");
	this->CInput->setDosefile("");
}

CMainMessenger::~CMainMessenger(void)
{
	delete PSname;
	delete bUsePS;
	delete bSave;
	delete nMax;
	delete dosefile;
}

void CMainMessenger::SetNewValue(G4UIcommand *cmd, G4String newValue)
{
	if(cmd==this->nMax){
		this->CInput->setNMax(this->nMax->GetNewIntValue(newValue));
	}
	if(cmd==this->bSave){
		this->CInput->setBSave(this->bSave->GetNewBoolValue(newValue));
	}
	if(cmd==this->bUsePS){
		this->CInput->setBUsePS(this->bUsePS->GetNewBoolValue(newValue));
	}
	if(cmd==this->PSname){
		this->CInput->setPSname(newValue);
	}
	if(cmd==this->dosefile){
		this->CInput->setDosefile(newValue);
	}
}
