#ifndef RunAction_h
#define RunAction_h 1

#include "SInputData.hh"
#include "CInputData.hh"

#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "G4UserRunAction.hh"

class G4Timer;
class G4Run;
class TH1D;
class AnalysisManager;
class PhysicsList;

class RunAction : public G4UserRunAction
{
	public:
	RunAction(G4int seed, PhysicsList*);
	~RunAction();


	public:
	void BeginOfRunAction(const G4Run* aRun);
	void EndOfRunAction(const G4Run* aRun);	

	G4String seedname, fname;

	private:
	G4Timer* timer;
	PhysicsList* physics;
};

#endif

