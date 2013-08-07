#include "KillerSD.hh"
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

KillerSD::KillerSD(G4String name, SInputData *inputData):G4VSensitiveDetector(name)
{
	this->inputData = inputData;
	this->nParticle = 0;
}

KillerSD::~KillerSD()
{
}

void KillerSD::Initialize(G4HCofThisEvent*)
{
}

void KillerSD::setfname()
{
	G4String seedname;
	char a[10];
	sprintf(a,"%d",inputData->generalData.seed);
	seedname = (G4String)a;

	this->filename = "../results/PS/PS" + seedname + ".dat";
	this->particles = new SParticle[inputData->generalData.nMax];
}

void KillerSD::save(G4int nParticle)
{
	std::ofstream out;
	out.open(this->filename,std::ios::out|std::ios::binary|std::ios::app);
	for(int i=0; i<nParticle; i++)
	{
		out.write((char*) &this->particles[i].PDGE, sizeof(int));
		out.write((char*) &this->particles[i].kinEnergy, sizeof(double));
		out.write((char*) &this->particles[i].posx, sizeof(double));
		out.write((char*) &this->particles[i].posy, sizeof(double));
		out.write((char*) &this->particles[i].posz, sizeof(double));
		out.write((char*) &this->particles[i].dirx, sizeof(double));
		out.write((char*) &this->particles[i].diry, sizeof(double));
		out.write((char*) &this->particles[i].dirz, sizeof(double));
	}

	G4cout << nParticle << " particles have been written in " << this->filename << "." << G4endl;

	out.close();
}

G4bool KillerSD::ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist)
{
//	G4cout << ecut/MeV << " " << gcut/MeV << " " << ncut/MeV << " " << pcut/MeV << G4endl;

	static bool bFirst=true;

	G4double kE = aStep->GetTrack()->GetKineticEnergy();

	G4ParticleDefinition *pd = aStep->GetTrack()->GetDefinition();
	G4String pname = pd->GetParticleName();

//	G4cout << inputData->generalData.nMax << G4endl;

	if(pname == "e-"){
		if(kE <= ecut) aStep->GetTrack()->SetTrackStatus(fStopAndKill);
	}else if(pname == "gamma"){
		if(kE <= gcut) aStep->GetTrack()->SetTrackStatus(fStopAndKill);
	}else if(pname == "neutron"){
//		G4cout << "Here is Neutron whose kinetic Energy is " << kE/MeV << " MeV !" << G4endl;
		if(kE <= ncut) aStep->GetTrack()->SetTrackStatus(fStopAndKill);
	}else if(pname == "e+"){
		if(kE <= pcut) aStep->GetTrack()->SetTrackStatus(fStopAndKill);
	}

	if(inputData->generalData.bSave)
	{
		if(bFirst)
		{
			bFirst=false;
			setfname();
		}
		this->particles[this->nParticle].kinEnergy = kE/MeV;
		this->particles[this->nParticle].posx = aStep->GetPreStepPoint()->GetPosition().getX()/mm;
		this->particles[this->nParticle].posy = aStep->GetPreStepPoint()->GetPosition().getY()/mm;
		this->particles[this->nParticle].posz = aStep->GetPreStepPoint()->GetPosition().getZ()/mm;
		this->particles[this->nParticle].dirx = aStep->GetPreStepPoint()->GetMomentumDirection().getX();
		this->particles[this->nParticle].diry = aStep->GetPreStepPoint()->GetMomentumDirection().getY();
		this->particles[this->nParticle].dirz = aStep->GetPreStepPoint()->GetMomentumDirection().getZ();
		this->particles[this->nParticle].PDGE = aStep->GetTrack()->GetDefinition()->GetPDGEncoding();

		this->nParticle++;
		if(this->nParticle == this->inputData->generalData.nMax)
		{
			save(this->nParticle);
			this->nParticle = 0;
		}
	}

	return true;
}

void KillerSD::EndOfEvent(G4HCofThisEvent*)
{
}

void KillerSD::clear()
{
} 

void KillerSD::DrawAll()
{
}

void KillerSD::PrintAll()
{
}

void KillerSD::set(G4double Ecut, G4double Gcut, G4double Ncut, G4double Pcut)
{
	ecut = Ecut;
	gcut = Gcut;
	ncut = Ncut;
	pcut = Pcut;
}
