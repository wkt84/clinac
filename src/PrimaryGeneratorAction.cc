#include "PrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "globals.hh"
#include "Randomize.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction(SInputData *inputData)
{
  G4int n_particle = 1;
  particleGun = new G4ParticleGun(n_particle);

  G4ParticleTable *ParticleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  particleGun->SetParticleDefinition(ParticleTable->FindParticle(particleName="e-"));
  particleGun->SetParticleEnergy(15.0*MeV);
  particleGun->SetParticlePosition(G4ThreeVector(0.0, 0.0, -1.0*m));
	dev = 1.443847*mm;

	this->inputData = inputData;
	this->nParticle = 0;
	this->gamma = ParticleTable->FindParticle("gamma");
	this->electron = ParticleTable->FindParticle("e-");
	this->positron = ParticleTable->FindParticle("e+");
	this->neutron = ParticleTable->FindParticle("neutron");
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete particleGun;
}

void PrimaryGeneratorAction::SetParticles()
{
	static bool bFirstTime=true;
	if(bFirstTime)
	{
		bFirstTime = false;
		this->particles = new SParticle[this->inputData->generalData.nMax];
		FillParticles();
	}
	if(this->nParticle == this->inputData->generalData.nMax)
	{
		FillParticles();
		this->nParticle = 0;
	}

	this->kinEnergy = this->particles[this->nParticle].kinEnergy;
	this->pos.set(this->particles[this->nParticle].posx, this->particles[this->nParticle].posy, this->particles[this->nParticle].posz);
	this->dir.set(this->particles[this->nParticle].dirx, this->particles[this->nParticle].diry, this->particles[this->nParticle].dirz);

//	G4cout << this->particles[this->nParticle].PDGE << G4endl;

	switch(this->particles[this->nParticle].PDGE)
	{
		case -11:
			particleGun->SetParticleDefinition(this->positron);
			break;
		case 11:
			particleGun->SetParticleDefinition(this->electron);
			break;
		case 22:
			particleGun->SetParticleDefinition(this->gamma);
			break;
		case 2112:
			particleGun->SetParticleDefinition(this->neutron);
			break;
	}
	this->nParticle++;
}

void PrimaryGeneratorAction::FillParticles()
{
	static int currentFilePosition=0;
	static int currentFileSize=0;
	int startDataFilePosition;
	std::ifstream in;
	in.open(this->inputData->generalData.PSname, std::ios::binary|std::ios::in);
	if (in==0)
	{
		std::cout <<"ERROR phase space file: "<< this->inputData->generalData.PSname << " NOT found. Run abort "<< G4endl;
		G4RunManager::GetRunManager()->AbortRun(true);
	}

	static bool bFirstTime=true;
	if (bFirstTime)
	{
		in.seekg(-1,std::ios::end);
		currentFileSize=in.tellg();
		in.seekg(0,std::ios::beg);
		bFirstTime=false;
	}

	startDataFilePosition=in.tellg();
	if (currentFilePosition>0)
	{in.seekg(currentFilePosition, std::ios::beg);}
	int i;
	G4double x,y,z;
	G4int d;
	G4String s;

	static bool checkFileRewind=false;
	static bool bRewindTheFile=false;
	static int nPhSpFileRewind=0;

	for (i=0;i<this->inputData->generalData.nMax;i++)
	{
		if (bRewindTheFile) // to read the phase space file again to fill the container
		{
			in.close();
		 	in.open(this->inputData->generalData.PSname, std::ios::binary|std::ios::in);
			in.seekg(startDataFilePosition, std::ios::beg);
			checkFileRewind=true;
			bRewindTheFile=false;
			std::cout<<"\n################\nI have reached the end of the phase space file "<<++nPhSpFileRewind <<" times, I rewind the file\n" << G4endl;
			std::cout <<"loaded " <<i <<"/"<< this->inputData->generalData.nMax <<" particles" << G4endl;
		}
		in.read((char*)&d, sizeof(int));
		this->particles[i].PDGE = d;

		in.read((char*)&x, sizeof(double));
		this->particles[i].kinEnergy = x;

		in.read((char*)&x, sizeof(double));
		in.read((char*)&y, sizeof(double));
		in.read((char*)&z, sizeof(double));
		this->particles[i].posx = x;
		this->particles[i].posy = y;
		this->particles[i].posz = z;

		in.read((char*)&x, sizeof(double));
		in.read((char*)&y, sizeof(double));
		in.read((char*)&z, sizeof(double));
		this->particles[i].dirx = x;
		this->particles[i].diry = y;
		this->particles[i].dirz = z;

		if (in.tellg()== (currentFileSize+1))	{bRewindTheFile=true;}
		if (checkFileRewind)	{checkFileRewind=false;}
	}
	std::cout <<"loaded " << i <<"/"<< this->inputData->generalData.nMax <<" particles" << G4endl;
	currentFilePosition=in.tellg(); // to remind the actual position in the phase space file
	if (currentFilePosition>=currentFileSize) // to read the phase space file again
	{currentFilePosition=startDataFilePosition;} 
	in.close();
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
	if(!this->inputData->generalData.bUsePS)
	{
		G4ThreeVector v(0.0,0.0,1.0);
		particleGun->SetParticleMomentumDirection(v);
		//Mean Energy=14.5MeV, 3%FWHM, by Sheikh-Bagheri, Med. Phys. 29. p379
		G4double energy = G4RandGauss::shoot(14.5, 0.1847);
		//		particleGun->SetParticleEnergy(3*MeV);
		particleGun->SetParticleEnergy(energy*MeV);

		//Radial intensity distribution is 0.17 cm (FWHM) by Sheikh-Bagheri
		if(dev>0.){
			G4double s_proj = dev;
			G4double x0, y0;
			do{
				x0 = G4RandGauss::shoot(0, s_proj);
				y0 = G4RandGauss::shoot(0, s_proj);
			}while((x0<-0.6*cm)||(x0>0.6*cm)||(y0<-0.6*cm)||(y0>0.6*cm));
			particleGun->SetParticlePosition(G4ThreeVector(x0,y0,-1.0*m));
		}
		particleGun->GeneratePrimaryVertex(anEvent);
	}else{
		SetParticles();
		particleGun->SetParticleEnergy(this->kinEnergy*MeV);
		particleGun->SetParticlePosition(this->pos*mm);
		particleGun->SetParticleMomentumDirection((G4ParticleMomentum)this->dir);
		particleGun->GeneratePrimaryVertex(anEvent);
	}
}
