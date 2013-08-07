#include "PhantomSD.hh"
#include "PhantomROGeometry.hh"
#include "AnalysisManager.hh"
#include "DetectorConstruction.hh"
#include "G4Track.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Step.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4SDManager.hh"
#include "G4ParticleDefinition.hh"

PhantomSD::PhantomSD(G4String name):G4VSensitiveDetector(name)
{
}

PhantomSD::~PhantomSD()
{
  
}

void PhantomSD::Initialize(G4HCofThisEvent*)
{
	i = 0;
}

G4bool PhantomSD::ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist)
{

//
// The energy deposit of the hits is stored in histograms and ntuples
//

	if(!ROhist)
		return false;

	// Check the volume
	if(aStep -> GetPreStepPoint() -> GetPhysicalVolume() -> GetName() != "phantom_phys")
		return false;

	G4double energyDeposit = aStep -> GetTotalEnergyDeposit();

	G4String pname = aStep->GetTrack()->GetDefinition()->GetParticleName();
/*	if(pname!="gamma" && pname!="e-" && pname!="e+"){
	G4cout << pname << "! ";
	i = 1;
	}
*/
	// Check that the energy deposit is not null
	if(energyDeposit == 0.)
		return false;

	if(energyDeposit != 0)                       
	{            
		// Read Voxel indexes: 
		// i is the x index, 
		// j is the y index
		// k is the z index
		G4int j = ROhist -> GetReplicaNumber();
		G4int k = ROhist -> GetReplicaNumber(1);
		G4int i = ROhist -> GetReplicaNumber(2);

		G4int numberOfVoxel = 500;
		G4double voxelWidthX = 1.*mm;
		G4double voxelWidthY = 1.*mm;
		G4double voxelWidthZ = 1.*mm;

		G4double density = aStep->GetPreStepPoint()->GetPhysicalVolume()->GetLogicalVolume()->GetMaterial()->GetDensity();
		G4double volume = voxelWidthX * voxelWidthY * voxelWidthZ;
		G4double mass = density * volume;

		energyDeposit /= mass;

		// Retrieve the coordinates of the center of the voxel where
		// the energy deposit is located
		x = ( - numberOfVoxel + 1+ 2*i )* voxelWidthX/2; 
		y = ( - numberOfVoxel + 1+ 2*j )* voxelWidthY/2;
		z = ( 2*k + 1 ) * voxelWidthZ/2;

		AnalysisManager* analysis = AnalysisManager::getInstance();

		if (y < 8.0 * mm && y > -8.0 * mm)
		{ 
			// Fill a 2D histogram with the energy deposit in the plane
			// containing the source
			analysis -> FillHistogramWithEnergy(x,z,energyDeposit/(joule/kg));

			// Fill a 2D histogram by photonuclear particles
			if(pname!="gamma" && pname!="e-" && pname!="e+")
				analysis -> FillPN(x, z, energyDeposit/(joule/kg));

			//  Fill 1D histogram with the energy deposit 
			// along the axis perpendicular to the main axis of the source
			if (x < 5.0 * mm && x > -5.0 * mm)                
				analysis -> DoseDistribution(z,energyDeposit/(joule/kg));
		} 
	}
	return true;
}

void PhantomSD::EndOfEvent(G4HCofThisEvent*)
{
	if(i==1) G4cout << G4endl;
}

void PhantomSD::clear()
{
} 

void PhantomSD::DrawAll()
{
}

void PhantomSD::PrintAll()
{
}



