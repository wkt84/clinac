// The phantom is devided in voxels. 
// The energy deposit is integrated over the voxels

#ifndef PhantomROGeometry_h
#define PhantomROGeometry_h 

#include "G4VReadOutGeometry.hh"
#include "G4SystemOfUnits.hh"

class PhantomROGeometry : public G4VReadOutGeometry
{
public:
  PhantomROGeometry(G4String aString,
			  G4double phantomDimX,
			  G4double phantomDimY,
			  G4double phantomDimZ,
			  G4int numberOfVoxelsX,
			  G4int numberOfVoxelsY,
			  G4int numberOfVoxelsZ);

  ~PhantomROGeometry();

	void Rebuild();
	void Reset();

private:
  G4VPhysicalVolume* Build();
	void CleanROG();

private:
  const G4double phantomSizeX;
  const G4double phantomSizeY;
  const G4double phantomSizeZ;
  const G4int numberOfVoxelsAlongX;
  const G4int numberOfVoxelsAlongY;
  const G4int numberOfVoxelsAlongZ; 
  G4VPhysicalVolume *ROPhantomYDivisionPhys;
	G4Material* dummyMat;
};
#endif
