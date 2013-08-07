#include "PhantomROGeometry.hh"
#include "DummySD.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4SDManager.hh"
#include "G4Box.hh"
#include "G4ThreeVector.hh"
#include "G4Material.hh"
#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"


PhantomROGeometry::PhantomROGeometry(G4String aString,
		G4double phantomDimX,
		G4double phantomDimY,
		G4double phantomDimZ,
		G4int numberOfVoxelsX,
		G4int numberOfVoxelsY,
		G4int numberOfVoxelsZ):
	G4VReadOutGeometry(aString),
	phantomSizeX(phantomDimX),
	phantomSizeY(phantomDimY),
	phantomSizeZ(phantomDimZ),
	numberOfVoxelsAlongX(numberOfVoxelsX),
	numberOfVoxelsAlongY(numberOfVoxelsY),
	numberOfVoxelsAlongZ(numberOfVoxelsZ)
{
}

PhantomROGeometry::~PhantomROGeometry()
{
}

G4VPhysicalVolume* PhantomROGeometry::Build()
{
 
  // A dummy material is used to fill the volumes of the readout geometry.
	// (It will be allowed to set a NULL pointer in volumes of such virtual
	// division in future, since this material is irrelevant for tracking.)

	G4Material* dummyMat = new G4Material(name="dummyMat", 
			1., 1.*g/mole, 1.*g/cm3);

	G4double worldSizeX = 1.2*m;
	G4double worldSizeY = 1.2*m;
	G4double worldSizeZ = 1.2*m;

	G4double halfPhantomSizeX = phantomSizeX;
	G4double halfPhantomSizeZ = phantomSizeZ;
	G4double halfPhantomSizeY = phantomSizeY;

	// variables for x division ...
	G4double halfVoxelSizeX = halfPhantomSizeX/numberOfVoxelsAlongX;
	G4double halfVoxelSizeY = halfPhantomSizeY/numberOfVoxelsAlongY;
	G4double halfVoxelSizeZ = halfPhantomSizeZ/numberOfVoxelsAlongZ;
	G4double halfSizeX = halfPhantomSizeX;
	G4double halfSizeY = halfPhantomSizeY;
	G4double halfSizeZ = halfPhantomSizeZ;
	G4double voxelThicknessX = 2*halfVoxelSizeX;
	G4double voxelThicknessY = 2*halfVoxelSizeY;
	G4double voxelThicknessZ = 2*halfVoxelSizeZ;

	// world volume of ROGeometry ...
	G4Box *ROWorld = new G4Box("ROWorld",
			worldSizeX,
			worldSizeY,
			worldSizeZ);

	G4LogicalVolume *ROWorldLog = new G4LogicalVolume(ROWorld,
			dummyMat,
			"ROWorldLog",
			0,0,0);

	G4VPhysicalVolume *ROWorldPhys = new G4PVPlacement(0,
			G4ThreeVector(),
			"ROWorldPhys",
			ROWorldLog,
			0,false,0);

	// Phantom ROGeometry ... 
	G4Box *ROPhantom = new G4Box("ROPhantom", 
			halfPhantomSizeX, 
			halfPhantomSizeY, 
			halfPhantomSizeZ);

	G4LogicalVolume *ROPhantomLog = new G4LogicalVolume(ROPhantom,
			dummyMat,
			"ROPhantomLog",
			0,0,0);

	G4VPhysicalVolume *ROPhantomPhys = new G4PVPlacement(0,
			G4ThreeVector(0,0,halfPhantomSizeZ),
			"PhantomPhys",
			ROPhantomLog,
			ROWorldPhys,
			false,0);

	// ROGeometry: Voxel division

	// X division first... 
	G4Box *ROPhantomXDivision = new G4Box("ROPhantomXDivision",
			halfVoxelSizeX,
			halfSizeY,
			halfSizeZ);

	G4LogicalVolume *ROPhantomXDivisionLog = new G4LogicalVolume(ROPhantomXDivision,
			dummyMat,
			"ROPhantomXDivisionLog",
			0,0,0);

	G4VPhysicalVolume *ROPhantomXDivisionPhys = new G4PVReplica("ROPhantomXDivisionPhys",
			ROPhantomXDivisionLog,
			ROPhantomPhys,
			kXAxis,
			numberOfVoxelsAlongX,
			voxelThicknessX);
	// ...then Z division

	G4Box *ROPhantomZDivision = new G4Box("ROPhantomZDivision",
			halfVoxelSizeX,
			halfSizeY, 
			halfVoxelSizeZ);

	G4LogicalVolume *ROPhantomZDivisionLog = new G4LogicalVolume(ROPhantomZDivision,
			dummyMat,
			"ROPhantomZDivisionLog",
			0,0,0);

	G4VPhysicalVolume *ROPhantomZDivisionPhys = new G4PVReplica("ROPhantomZDivisionPhys",
			ROPhantomZDivisionLog,
			ROPhantomXDivisionPhys,
			kZAxis,
			numberOfVoxelsAlongZ,
			voxelThicknessZ);
	// ...then Y  division

	G4Box *ROPhantomYDivision = new G4Box("ROPhantomYDivision",
			halfVoxelSizeX, 
			halfVoxelSizeY,
			halfVoxelSizeZ);

	G4LogicalVolume *ROPhantomYDivisionLog = new G4LogicalVolume(ROPhantomYDivision,
			dummyMat,
			"ROPhantomYDivisionLog",
			0,0,0);

	ROPhantomYDivisionPhys = new G4PVReplica("ROPhantomYDivisionPhys",
			ROPhantomYDivisionLog,
			ROPhantomZDivisionPhys,
			kYAxis,
			numberOfVoxelsAlongY,
			voxelThicknessY);
	

	DummySD *dummySD = new DummySD();
  ROPhantomYDivisionLog -> SetSensitiveDetector(dummySD);

  return ROWorldPhys;
}

void PhantomROGeometry::CleanROG(){
	G4GeometryManager::GetInstance()->OpenGeometry();
	G4PhysicalVolumeStore::GetInstance()->Clean();
	G4LogicalVolumeStore::GetInstance()->Clean();
	G4SolidStore::GetInstance()->Clean();
}

void PhantomROGeometry::Rebuild(){
	CleanROG();
	G4cout << "CleanROG was suceeded" << G4endl;
	Build();
	G4cout << "Build was suceeded" << G4endl;
}
