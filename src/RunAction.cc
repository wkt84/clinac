// Make this appear first!
#include "G4Timer.hh"

#include "RunAction.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4ios.hh"

#include "TROOT.h"
#include "TH1.h"
#include "TFile.h"

#include "AnalysisManager.hh"
#include "PhysicsList.hh"
#include "StepMax.hh"

RunAction::RunAction(G4int seed, PhysicsList* phys)
	:physics(phys)
{
  timer = new G4Timer;

	char a[10];
	sprintf(a,"%d", seed);
	seedname=(G4String)a;
}

RunAction::~RunAction()
{
  delete timer;
}

void RunAction::BeginOfRunAction(const G4Run* aRun)
{
  //set StepMax
	G4double stepMax = 0.1*mm;
	physics->GetStepMaxProcess()->SetMaxStep(stepMax);
	
	G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;
  timer->Start();

	G4int runNb = aRun->GetRunID();
	if(runNb == 0){
		AnalysisManager* analysis = AnalysisManager::getInstance();
		analysis->book();
	}
}

void RunAction::EndOfRunAction(const G4Run* aRun)
{
  timer->Stop();
  G4cout << "number of event = " << aRun->GetNumberOfEvent() << G4endl;
  
  G4double time = timer->GetUserElapsed();
  G4cout << "elapsed time[s] : " << time << G4endl;

	fname = "../results/dose/hist" + seedname + ".root";

	AnalysisManager* analysis = AnalysisManager::getInstance();
	analysis->finish(fname);

}

