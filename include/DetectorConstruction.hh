#ifndef DetectorConstruction_H
#define DetectorConstruction_H 1

class G4LogicalVolume;
class G4VPhysicalVolume;

#include "G4VUserDetectorConstruction.hh"
#include "G4NistManager.hh"
#include "G4VisAttributes.hh"
#include "G4Material.hh"
#include "G4Region.hh"
#include "G4ProductionCuts.hh"
#include "SInputData.hh"

class DetectorMessenger;
class PhantomSD;
class PhantomROGeometry;
class KillerSD;

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:

	DetectorConstruction(G4String&, G4String&, SInputData *inputData);
	~DetectorConstruction();

	public:

	//Material Definition
	void DefineMaterials();

	//Geometry Definition
	G4VPhysicalVolume* CreateWorld();
	void ConstructAccel(G4LogicalVolume* logicWorld);
	G4VPhysicalVolume* CreateGeometry();
	void UpdateGeometry();

	//Activate Detector
	void ActivateDet();

	//Command Interface
	inline void SetEnergy(G4int val){this->Energy=val;};
	inline void SetX1(G4double val){this->X1=val;};
	inline void SetX2(G4double val){this->X2=val;};
	inline void SetY1(G4double val){this->Y1=val;};
	inline void SetY2(G4double val){this->Y2=val;};
	inline void SetEcut(G4double val){this->Ecut=val;};
	inline void SetGcut(G4double val){this->Gcut=val;};
	inline void SetNcut(G4double val){this->Ncut=val;};
	inline void SetPcut(G4double val){this->Pcut=val;};

	G4VPhysicalVolume* Construct();

	private:

	// Logical volumes
	//
	G4LogicalVolume* experimentalHall_log;
	G4LogicalVolume* phantom_log;
	G4LogicalVolume* targetA_log;
	G4LogicalVolume* targetB_log;
	G4LogicalVolume* chamber_log;
	G4LogicalVolume* det_log;
	G4LogicalVolume* KP_log;

	// Physical volumes
	G4VPhysicalVolume* experimentalHall_phys;
	G4VPhysicalVolume* phantom_phys;
	G4VPhysicalVolume* KP_phys;

	// Regions for cuts
	G4Region* regVol;
	G4Region* regVol1;
	G4Region* regVol2;
	G4Region* regVol3;

	// Physical volumes
/*
	G4VPhysicalVolume* experimentalHall_phys;
	G4VPhysicalVolume* phantom_phys;
	G4VPhysicalVolume* targetA_phys;
	G4VPhysicalVolume* targetB_phys;
	G4VPhysicalVolume* chamber_phys;
	G4VPhysicalVolume* det_phys;
*/
	DetectorMessenger* detectorMessenger;

	PhantomSD* phantomSD; //pointer to sensitive detector
	PhantomROGeometry* phantomROGeometry; //pointer to ROGeometry
	KillerSD* killerSD; //pointer to KillerPlane

	G4double phantomSizeX; //Phantom XSize
	G4double phantomSizeY; //Phantom YSize
	G4double phantomSizeZ; //Phantom ZSize  
	G4int numberOfVoxelsAlongX; //Number of voxels along x axis
	G4int numberOfVoxelsAlongY; //Number of voxels along y axis 
	G4int numberOfVoxelsAlongZ; //Number of voxels along z axis 
	G4double worldSizeX ; //World XSize
	G4double worldSizeY ; //World YSize
	G4double worldSizeZ ; //World XSize
	G4String sensitiveDetectorName;
	G4String killerPlaneName;

	SInputData *inputData;


	private:

	G4int Energy;
	G4double X1;
	G4double X2;
	G4double Y1;
	G4double Y2;
	G4double Ecut;
	G4double Gcut;
	G4double Ncut;
	G4double Pcut;

	//Visualization Attributes
	G4VisAttributes* worldVis;
	G4VisAttributes* targetAVis;
	G4VisAttributes* targetBVis;
	G4VisAttributes* primcolVis;
	G4VisAttributes* flatVis;
	G4VisAttributes* xjawVis;
	G4VisAttributes* yjawVis;
	G4VisAttributes* phantomVis;
};

#endif
