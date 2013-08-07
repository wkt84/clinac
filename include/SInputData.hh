#ifndef inputDataH
#define inputDataH

#include "globals.hh"
#include "G4ThreeVector.hh"

struct SGeneralData
{
	G4int seed;
	G4String fileInputData;
	G4String PSname;
	G4int nMax;
	G4bool bSave;
	G4bool bUsePS;
};

struct SParticle
{
	G4double posx, posy, posz;
	G4double dirx, diry, dirz;
	G4double kinEnergy;
	G4int PDGE;
};

struct SInputData
{
	SGeneralData generalData;
};

#endif
