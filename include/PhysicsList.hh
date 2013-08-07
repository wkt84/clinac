#ifndef PhysicsList_h
#define PhysicsList_h 1

#include "G4VUserPhysicsList.hh"
#include "globals.hh"
//#include "G4VPhysicsConstructor.hh"

#include "G4EmConfigurator.hh"

class G4VPhysicsConstructor;
class StepMax;

class PhysicsList: public G4VUserPhysicsList
{
	public:
	PhysicsList();
	~PhysicsList();

	public:
	// Construct particle and physics process
	void ConstructParticle();
	void ConstructProcess();
	void SetCuts();
	void SetCutForGamma(G4double);
	void SetCutForElectron(G4double);
	void SetCutForPositron(G4double);

	void AddStepMax();
	StepMax* GetStepMaxProcess() {return stepMaxProcess;};

	//Construct Physics processes and register them
	void ConstructEM();
	//	void ConstructPN();

	private:
	G4EmConfigurator em_config;
	G4double cutForGamma;
	G4double cutForElectron;
	G4double cutForPositron;

	G4VPhysicsConstructor* decPhysicsList;
	std::vector<G4VPhysicsConstructor*>  hadronPhys;

  StepMax* stepMaxProcess;
};

#endif
