#include "PhysicsList.hh"
#include "G4VPhysicsConstructor.hh"
#include "G4PhysListFactory.hh"

#include "G4ProcessManager.hh"

#include "G4ParticleTypes.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "G4EmProcessOptions.hh"
#include "G4LossTableManager.hh"
#include "G4Decay.hh"
#include "G4DecayPhysics.hh"

//#include "G4Gamma.hh"
//#include "G4Electron.hh"
//#include "G4Positron.hh"
//#include "G4Proton.hh"
//#include "G4AntiProton.hh"
//#include "G4Deuteron.hh"
//#include "G4Triton.hh"
//#include "G4He3.hh"
//#include "G4Alpha.hh"
//#include "G4GenericIon.hh"
//#include "G4Neutron.hh"

#include "G4EmStandardPhysics_option3.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4EmPenelopePhysics.hh"
#include "G4EmExtraPhysics.hh"

#include "G4IonBinaryCascadePhysics.hh"
#include "G4HadronPhysicsQGSP_BERT_HP.hh"
#include "G4HadronElasticPhysicsHP.hh"
#include "G4StoppingPhysics.hh"
#include "G4IonPhysics.hh"

#include "G4HadronicProcessStore.hh"
#include "G4NeutronHPManager.hh"

PhysicsList::PhysicsList() : G4VUserPhysicsList()
{
	G4LossTableManager::Instance();
	defaultCutValue = 0.1*mm;
	cutForGamma = defaultCutValue;
	cutForElectron = defaultCutValue;
	cutForPositron = defaultCutValue;

//	stepMaxProcess = 0;
	SetVerboseLevel(0);

	// Set verbose level of hadronic process
//	G4HadronicProcessStore *hadron = G4HadronicProcessStore::Instance();
//	hadron->SetVerbose(0);

	// Set verbose level of neutron HP process
//	G4NeutronHPManager *neutronHP = G4NeutronHPManager::GetInstance();
//	neutronHP->SetVerboseLevel(0);


	stepMaxProcess = 0;

	decPhysicsList = new G4DecayPhysics();
}

PhysicsList::~PhysicsList()
{
	delete decPhysicsList;
	delete emPhysicsList;
	delete emExtraPhysicsList;
	for(size_t i=0; i<hadronPhys.size(); i++) {delete hadronPhys[i];}
}

void PhysicsList::ConstructParticle()
{
  // In this method, static member functions should be called
  // for all particles which you want to use.
  // This ensures that objects of these particle types will be
  // created in the program. 

//// gamma
//G4Gamma::Gamma();
// 
//// leptons
//G4Electron::Electron();
//G4Positron::Positron();
//
//// baryons
//G4Proton::Proton();
//G4AntiProton::AntiProton();
//G4Neutron::Neutron();
//
//// ions
//G4Deuteron::Deuteron();
//G4Triton::Triton();
//G4He3::He3();
//G4Alpha::Alpha();
//G4GenericIon::GenericIonDefinition();

decPhysicsList->ConstructParticle();

}

void PhysicsList::ConstructProcess()
{
  // Define transportation process

  AddTransportation();
	AddStepMax();

  decPhysicsList->ConstructProcess();

	emPhysicsList = new G4EmStandardPhysics_option3(0);
//	emPhysicsList = new G4EmLivermorePhysics(1);
//	emPhysicsList = new G4EmPenelopePhysics(1);

	emExtraPhysicsList = new G4EmExtraPhysics();

	emPhysicsList->ConstructProcess();
	emExtraPhysicsList->ConstructProcess();

	hadronPhys.push_back(new G4HadronElasticPhysicsHP());
	hadronPhys.push_back(new G4HadronPhysicsQGSP_BERT_HP());
	hadronPhys.push_back(new G4StoppingPhysics());
	hadronPhys.push_back(new G4IonPhysics());

  for(size_t i=0; i<hadronPhys.size(); i++) {
		  hadronPhys[i]->ConstructProcess();
  }

	SetVerboseLevel(0);

}

/*
//gamma
#include "G4LivermorePhotoElectricModel.hh"
#include "G4PhotoElectricEffect.hh"

#include "G4LivermoreComptonModel.hh"
#include "G4ComptonScattering.hh"

#include "G4LivermoreGammaConversionModel.hh"
#include "G4GammaConversion.hh"

#include "G4GammaNuclearReaction.hh"
#include "G4PhotoNuclearProcess.hh"

//e-
#include "G4eMultipleScattering.hh"
#include "G4UniversalFluctuation.hh"

#include "G4LivermoreIonisationModel.hh"
#include "G4eIonisation.hh"

#include "G4eBremsstrahlung.hh"
#include "G4LivermoreBremsstrahlungModel.hh"

#include "G4GoudsmitSaundersonMscModel.hh"
#include "G4CoulombScattering.hh"
#include "G4WentzelVIModel.hh"

//e+
#include "G4eplusAnnihilation.hh"

// mu
 
#include "G4MuMultipleScattering.hh"
#include "G4MuIonisation.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4MuPairProduction.hh"

// hadrons

#include "G4hMultipleScattering.hh"
#include "G4MscStepLimitType.hh"

#include "G4hBremsstrahlung.hh"
#include "G4hPairProduction.hh"

#include "G4hIonisation.hh"
#include "G4ionIonisation.hh"
#include "G4alphaIonisation.hh"
#include "G4IonParametrisedLossModel.hh"
#include "G4NuclearStopping.hh"
*/

/*
void PhysicsList::ConstructEM()
{
//	theParticleIterator->reset();
//	while((*theParticleIterator)()){
//		G4ParticleDefinition* particle = theParticleIterator->value();
//		G4ProcessManager* pmanager = particle->GetProcessManager();
//		pmanager->SetVerboseLevel(0);
//		G4String particleName = particle->GetParticleName();
//		G4double LivermoreHighEnergyLimit = 1*GeV;
//
//		if (particleName == "gamma"){
//		//gamma
//			G4PhotoElectricEffect* thePhotoElectricEffect = new G4PhotoElectricEffect();
//			G4LivermorePhotoElectricModel* theLivermorePhotoElectricModel = new G4LivermorePhotoElectricModel();
//			theLivermorePhotoElectricModel->SetHighEnergyLimit(LivermoreHighEnergyLimit);
//			thePhotoElectricEffect->AddEmModel(0, theLivermorePhotoElectricModel);
//			pmanager->AddDiscreteProcess(thePhotoElectricEffect);
//			
//			G4ComptonScattering* theComptonScattering = new G4ComptonScattering();
//			G4LivermoreComptonModel* theLivermoreComptonModel = new G4LivermoreComptonModel();
//			theLivermoreComptonModel->SetHighEnergyLimit(LivermoreHighEnergyLimit);
//			theComptonScattering->AddEmModel(0, theLivermoreComptonModel);
//			pmanager->AddDiscreteProcess(theComptonScattering);
//			
//			G4GammaConversion* theGammaConversion = new G4GammaConversion();
//			G4LivermoreGammaConversionModel* theLivermoreGammaConversionModel = new G4LivermoreGammaConversionModel();
//			theLivermoreGammaConversionModel->SetHighEnergyLimit(LivermoreHighEnergyLimit);
//			theGammaConversion->AddEmModel(0, theLivermoreGammaConversionModel);
//			pmanager->AddDiscreteProcess(theGammaConversion);
//			
//			G4GammaNuclearReaction* lowEGammaModel = new G4GammaNuclearReaction();
//			lowEGammaModel->SetMaxEnergy(1.0*GeV);
//			G4PhotoNuclearProcess* thePhotoNuclearProcess = new G4PhotoNuclearProcess();
//			thePhotoNuclearProcess->RegisterMe(lowEGammaModel);
//			pmanager->AddDiscreteProcess(thePhotoNuclearProcess);
//		}else if (particleName == "e-"){
//		//e-
//			G4eMultipleScattering* msc = new G4eMultipleScattering();
//		//	msc->AddEmModel(0, new G4UrbanMscModel93());
//			msc->AddEmModel(0, new G4GoudsmitSaundersonMscModel());
//			msc->SetStepLimitType(fUseDistanceToBoundary);
//			pmanager->AddProcess(msc, -1, 1, 1);
//			// Ionisation
//			G4eIonisation* eIoni = new G4eIonisation();
//			G4LivermoreIonisationModel* theIoniLivermore = new G4LivermoreIonisationModel();
//			theIoniLivermore->SetHighEnergyLimit(1*MeV);
//			eIoni->AddEmModel(0, theIoniLivermore, new G4UniversalFluctuation() );
//			eIoni->SetStepFunction(0.2, 100*um); //     
//			pmanager->AddProcess(eIoni, -1, 2, 2);
//			// Bremsstrahlung
//			G4eBremsstrahlung* eBrem = new G4eBremsstrahlung();
//			G4LivermoreBremsstrahlungModel* theBremLivermore = new G4LivermoreBremsstrahlungModel();
//			theBremLivermore->SetHighEnergyLimit(LivermoreHighEnergyLimit);
//			eBrem->AddEmModel(0, theBremLivermore);
//			pmanager->AddProcess(eBrem, -1,-3, 3);
//		} else if (particleName == "e+") {
//		// e+ : Identical to G4EmStandardPhysics_option3
//
//			G4eMultipleScattering* msc = new G4eMultipleScattering();
//		//	msc->AddEmModel(0, new G4UrbanMscModel93());
//			msc->AddEmModel(0, new G4GoudsmitSaundersonMscModel());
//			msc->SetStepLimitType(fUseDistanceToBoundary);
//			pmanager->AddProcess(msc, -1, 1, 1);
//			G4eIonisation* eIoni = new G4eIonisation();
//			eIoni->SetStepFunction(0.2, 100*um);
//			pmanager->AddProcess(eIoni,                 -1, 2, 2);
//			pmanager->AddProcess(new G4eBremsstrahlung, -1,-3, 3);
//			pmanager->AddProcess(new G4eplusAnnihilation,0,-1, 4);
//		} else if (particleName == "mu+" || particleName == "mu-"    ) {
//		// Identical to G4EmStandardPhysics_option3
//
//			G4MuMultipleScattering* msc = new G4MuMultipleScattering();
//		    msc->AddEmModel(0, new G4WentzelVIModel());
//		    pmanager->AddProcess(msc,                       -1, 1, 1);
//		
//	     G4MuIonisation* muIoni = new G4MuIonisation();
//	     muIoni->SetStepFunction(0.2, 50*um);
//	
//      pmanager->AddProcess(muIoni,                    -1, 2, 2);
//      pmanager->AddProcess(new G4MuBremsstrahlung,    -1,-3, 3);
//      pmanager->AddProcess(new G4MuPairProduction,    -1,-4, 4);
//      pmanager->AddDiscreteProcess(new G4CoulombScattering());
//
//    } else if (particleName == "GenericIon") {
//						  pmanager->AddProcess(new G4hMultipleScattering, -1, 1, 1);
//						
//					   G4ionIonisation* ionIoni = new G4ionIonisation();
//					   ionIoni->SetEmModel(new G4IonParametrisedLossModel());
//					   ionIoni->SetStepFunction(0.1, 10*um);
//					   pmanager->AddProcess(ionIoni,                   -1, 2, 2);
//					   pmanager->AddProcess(new G4NuclearStopping(),   -1, 3,-1);
//	} else if (particleName == "alpha" || particleName == "He3"){
//		pmanager->AddProcess(new G4hMultipleScattering, -1, 1, 1);
//	 
//		G4ionIonisation* ionIoni = new G4ionIonisation();
//		ionIoni->SetStepFunction(0.1, 20*um);
//		pmanager->AddProcess(ionIoni,                   -1, 2, 2);
//		pmanager->AddProcess(new G4NuclearStopping(),   -1, 3,-1);
//	} else if ( particleName == "pi+" ||
//	               particleName == "pi-" ||
//	            particleName == "kaon+" ||
//	                particleName == "kaon-" ||
//	                particleName == "proton" ) {
//			 
//			       // Identical to G4EmStandardPhysics_option3
//		 
//		       pmanager->AddProcess(new G4hMultipleScattering, -1, 1, 1);
//		 
//		       G4hIonisation* hIoni = new G4hIonisation();
//		       hIoni->SetStepFunction(0.2, 50*um);
//		 
//		       pmanager->AddProcess(hIoni,                     -1, 2, 2);
//		       pmanager->AddProcess(new G4hBremsstrahlung,     -1,-3, 3);
//		       pmanager->AddProcess(new G4hPairProduction,     -1,-4, 4);
//	} else if ( particleName == "B+" ||
//            particleName == "B-" ||
//            particleName == "D+" ||
//            particleName == "D-" ||
//            particleName == "Ds+" ||
//            particleName == "Ds-" ||
//            particleName == "anti_He3" ||
//            particleName == "anti_alpha" ||
//            particleName == "anti_deuteron" ||
//            particleName == "anti_lambda_c+" ||
//            particleName == "anti_omega-" ||
//            particleName == "anti_proton" ||
//            particleName == "anti_sigma_c+" ||
//            particleName == "anti_sigma_c++" ||
//            particleName == "anti_sigma+" ||
//            particleName == "anti_sigma-" ||
//            particleName == "anti_triton" ||
//            particleName == "anti_xi_c+" ||
//            particleName == "anti_xi-" ||
//            particleName == "deuteron" ||
//            particleName == "lambda_c+" ||
//            particleName == "omega-" ||
//            particleName == "sigma_c+" ||
//            particleName == "sigma_c++" ||
//            particleName == "sigma+" ||
//            particleName == "sigma-" ||
//            particleName == "tau+" ||
//            particleName == "tau-" ||
//            particleName == "triton" ||
//            particleName == "xi_c+" ||
//            particleName == "xi-" ) {
//			pmanager->AddProcess(new G4hMultipleScattering, -1, 1, 1);
//			pmanager->AddProcess(new G4hIonisation,         -1, 2, 2);
//	}
//	}
//
}
*/

#include "StepMax.hh"

void PhysicsList::AddStepMax()
{
  // Step limitation seen as a process
  stepMaxProcess = new StepMax();

  theParticleIterator->reset();
  while ((*theParticleIterator)()){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();

    if (stepMaxProcess->IsApplicable(*particle) && pmanager)
      {
    pmanager ->AddDiscreteProcess(stepMaxProcess);
      }
  }
}


/////////////////////////////////////////////////////////////////////////////
void PhysicsList::SetCuts()
{

  if (verboseLevel >0){
    G4cout << "PhysicsList::SetCuts:";
    G4cout << "CutLength : " << G4BestUnit(defaultCutValue,"Length") << G4endl;
  }

  // set cut values for gamma at first and for e- second and next for e+,
  // because some processes for e+/e- need cut values for gamma
  SetCutValue(cutForGamma, "gamma");
  SetCutValue(cutForElectron, "e-");
  SetCutValue(cutForPositron, "e+");
  SetCutValue(1.*um, "proton");

  if (verboseLevel>0) DumpCutValuesTable();
}
/*
/////////////////////////////////////////////////////////////////////////////
void PhysicsList::SetCutForGamma(G4double cut)
{
  cutForGamma = cut;
  SetParticleCuts(cutForGamma, G4Gamma::Gamma());
}

/////////////////////////////////////////////////////////////////////////////
void PhysicsList::SetCutForElectron(G4double cut)
{
  cutForElectron = cut;
  SetParticleCuts(cutForElectron, G4Electron::Electron());
}

/////////////////////////////////////////////////////////////////////////////
void PhysicsList::SetCutForPositron(G4double cut)
{
  cutForPositron = cut;
  SetParticleCuts(cutForPositron, G4Positron::Positron());
}
*/
/*
void PhysicsList::SetCuts()
{
  // uppress error messages even in case e/gamma/proton do not exist            
  G4int temp = GetVerboseLevel();                                                SetVerboseLevel(0);                                                           
  //  " G4VUserPhysicsList::SetCutsWithDefault" method sets 
  //   the default cut value for all particle types 
  SetCutsWithDefault();   

  // Retrieve verbose level
  SetVerboseLevel(temp);  
}
*/
