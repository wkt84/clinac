// --------------------------------------------------------------
//      GEANT 4 - clinac
// --------------------------------------------------------------

#include "Main.hh"

#include "G4RunManager.hh"
#include "G4UImanager.hh"

#include "RunAction.hh"
#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "PrimaryGeneratorAction.hh"
#include "PhantomSD.hh"
#include "G4SDManager.hh"

#include <time.h>

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif


int main(int argc,char** argv)
{

  // Construct the default run manager
  //
  G4RunManager* runManager = new G4RunManager;

	G4String sensitiveDetectorName = "Phantom";
	G4String killerPlaneName = "Killer";
	
	// set mandatory initialization classes
  // Physicsl List
  PhysicsList* physics;
  runManager->SetUserInitialization(physics = new PhysicsList);

	// set main messenger class for the input data
	CInputData *myInput;
	myInput = new CInputData();

  // Detector Construction
  DetectorConstruction* detector = new DetectorConstruction(sensitiveDetectorName, killerPlaneName, &myInput->inputData);
  runManager->SetUserInitialization(detector);


	if(argc==3){
		sscanf(argv[2],"%d",&myInput->inputData.generalData.seed);
	}else{
		myInput->inputData.generalData.seed = 0;
	}

  // set mandatory user action class
  //
  RunAction* run_action = new RunAction(&myInput->inputData, physics);
  runManager->SetUserAction(run_action);

  PrimaryGeneratorAction* gen_action = new PrimaryGeneratorAction(&myInput->inputData);
  runManager->SetUserAction(gen_action);

  // Set the Seed
	G4long seeds[2];
	seeds[0] = myInput->inputData.generalData.seed;
	seeds[1] = time(NULL);

	G4Random::setTheEngine(new CLHEP::RanecuEngine);
	G4Random::setTheSeeds(seeds);

  // Initialize G4 kernel
  //
  runManager->Initialize();

#ifdef G4VIS_USE
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();
#endif 
     
  // Get the pointer to the User Interface manager
  //
  G4UImanager * UImanager = G4UImanager::GetUIpointer();  

  if (argc!=1)   // batch mode  
    {
      G4String command = "/control/execute ";
      G4String fileName = argv[1];
			UImanager->ApplyCommand(command+fileName);
    }
  else           // interactive mode : define UI session
    { 
#ifdef G4UI_USE
      G4UIExecutive * ui = new G4UIExecutive(argc,argv);
#ifdef G4VIS_USE
      UImanager->ApplyCommand("/control/execute vis.mac");     
#endif
      ui->SessionStart();
      delete ui;
#endif
     
#ifdef G4VIS_USE
      delete visManager;
#endif     
    }

  delete runManager;

  return 0;
}


