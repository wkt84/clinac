#include "StepMax.hh"
#include "StepMaxMessenger.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

	StepMax::StepMax(const G4String& processName, G4ProcessType type)
: G4VDiscreteProcess(processName,type), MaxChargedStep(DBL_MAX), apply(true)
{
	pMess = new StepMaxMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

StepMax::~StepMax() { delete pMess; }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool StepMax::IsApplicable(const G4ParticleDefinition& particle)
{
	return (particle.GetPDGCharge() != 0. && !particle.IsShortLived());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void StepMax::SetMaxStep(G4double step) {MaxChargedStep = step;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void StepMax::ApplyMaxStep(G4bool value) {apply = value;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double StepMax::PostStepGetPhysicalInteractionLength( const G4Track& aTrack,
		G4double,
		G4ForceCondition* condition )
{
	// condition is set to "Not Forced"
	*condition = NotForced;

	G4double ProposedStep = DBL_MAX;

	if((MaxChargedStep > 0.) &&
			(aTrack.GetVolume() != 0) &&
			(aTrack.GetVolume()->GetName() == "phantom_phys"))
		ProposedStep = MaxChargedStep;

	return ProposedStep;


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VParticleChange* StepMax::PostStepDoIt(const G4Track& aTrack, const G4Step&)
{
	// do nothing
	aParticleChange.Initialize(aTrack);
	return &aParticleChange;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


