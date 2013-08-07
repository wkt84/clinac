#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"
#include "SInputData.hh"
#include "G4RunManager.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"

class G4ParticleGun;
class G4Event;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    PrimaryGeneratorAction(SInputData* inputData);
    ~PrimaryGeneratorAction();

  public:
    void GeneratePrimaries(G4Event* anEvent);
		void SetParticles();
		void FillParticles();

  private:
		SInputData *inputData;
		G4int nParticle;
		SParticle *particles;
    G4ParticleGun* particleGun;
		G4double dev;
		G4ThreeVector pos, dir;
		G4double kinEnergy;
		G4int PDGE;

		G4ParticleDefinition *gamma;
		G4ParticleDefinition *electron;
		G4ParticleDefinition *positron;
		G4ParticleDefinition *neutron;
};

#endif
