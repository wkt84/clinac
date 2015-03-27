#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"

#include "G4RunManager.hh"

#include "G4Element.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4GenericPolycone.hh"
#include "G4LogicalVolume.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "G4Colour.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4RegionStore.hh"

#include "G4SDManager.hh"
#include "PhantomROGeometry.hh"
#include "PhantomSD.hh"
#include "KillerSD.hh"

#include "BOptrBremSplitting.hh"

DetectorConstruction::DetectorConstruction(G4String &SDName, G4String &KPname, SInputData *inputData)
: experimentalHall_log(0), phantom_log(0), 	targetA_log(0), targetB_log(0), chamber_log(0), det_log(0),
	experimentalHall_phys(0), phantom_phys(0), 
	regVol(0), regVol1(0), regVol2(0), regVol3(0),
	phantomSD(0), phantomROGeometry(0), killerSD(0),
	worldVis(0), targetAVis(0), targetBVis(0), primcolVis(0),
	flatVis(0), xjawVis(0), yjawVis(0), phantomVis(0), inputData(inputData),
	sensitiveDetectorName(SDName), killerPlaneName(KPname)
{
	detectorMessenger = new DetectorMessenger(this);

	//Define half size of the world
	worldSizeX = 1.2*m;
  worldSizeY = 1.2*m;
  worldSizeZ = 1.2*m;

	//Define half size of the phantom
  phantomSizeX = 0.25*m;
  phantomSizeY = 0.25*m;
  phantomSizeZ = 0.25*m;

  //Define the number of Voxels
	numberOfVoxelsAlongX = 100;
	numberOfVoxelsAlongY = 100;
	numberOfVoxelsAlongZ = 100;

}

DetectorConstruction::~DetectorConstruction()
{
	delete detectorMessenger;

	if(phantomROGeometry) delete phantomROGeometry;

	delete worldVis;
	delete targetAVis;
	delete targetBVis;
	delete primcolVis;
	delete flatVis;
	delete xjawVis;
	delete yjawVis;
	delete phantomVis;

	delete regVol;
	delete regVol1;
	delete regVol2;
	delete regVol3;

}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
	DefineMaterials();

	G4VPhysicalVolume* physiworld = CreateGeometry();

	return physiworld;
}

void DetectorConstruction::DefineMaterials()
{

//------------------------------------------------------ materials

  G4double a;  // atomic mass
  G4double z;  // atomic number
  G4double density;

  G4Element* elH =
  new G4Element("Hydrogen", "H", z= 1., a= 1.01*g/mole);

  G4Element* elC =
  new G4Element("Carbon", "C", z= 6., a= 12.00*g/mole);

  G4Element* elO =
  new G4Element("Oxygen", "O", z= 8., a= 16.00*g/mole);

  G4Element* elN =
  new G4Element("Nitrogen", "N", z= 7., a= 14.01*g/mole);

	G4Element* elW =
		new G4Element("Tungsten", "W", z= 74., a= 183.84*g/mole);

  G4Material* Air =
  new G4Material("Air", density= 1.290*mg/cm3, 2);
  Air->AddElement(elO, 30*perCent);
  Air->AddElement(elN, 70*perCent);

  G4Material* Water =
  new G4Material("Water", density= 1.000*g/cm3, 2);
  Water->AddElement(elO, 1);
  Water->AddElement(elH, 2);
	Water->GetIonisation()->SetMeanExcitationEnergy(75.*eV);

	G4Material* W =
		new G4Material("W", density=16.9*g/cm3, 1);
	W->AddElement(elW, 1);

	G4Material* W2 = new G4Material("W2", density=18.0*g/cm3, 1);
	W2->AddElement(elW, 1);
  
	G4NistManager::Instance()->FindOrBuildMaterial("G4_Cu");
}

G4VPhysicalVolume* DetectorConstruction::CreateGeometry()
{
  //------------------------------ experimental hall (world volume)

	experimentalHall_log=NULL;
/*
	//Clean old geometry
	G4GeometryManager::GetInstance()->OpenGeometry();
	G4PhysicalVolumeStore::GetInstance()->Clean();
	G4LogicalVolumeStore::GetInstance()->Clean();
	G4SolidStore::GetInstance()->Clean();
*/
	G4RegionStore::GetInstance()->DeRegister(regVol);
	G4RegionStore::GetInstance()->DeRegister(regVol1);
	G4RegionStore::GetInstance()->DeRegister(regVol2);
	G4RegionStore::GetInstance()->DeRegister(regVol3);

	G4VPhysicalVolume* physiworld = CreateWorld();
	experimentalHall_log = physiworld->GetLogicalVolume();

	ConstructAccel(experimentalHall_log);

	ActivateDet();

	return physiworld;
}


void DetectorConstruction::UpdateGeometry()
{
	G4RunManager::GetRunManager()->DefineWorldVolume(CreateGeometry());
}


G4VPhysicalVolume* DetectorConstruction::CreateWorld()
{
  G4Box* experimentalHall_box = new G4Box("expHall_box", worldSizeX, worldSizeY, worldSizeZ);
	experimentalHall_log = new G4LogicalVolume(experimentalHall_box,
			G4Material::GetMaterial("Air"),"expHall_log",0,0,0);
	experimentalHall_phys = new G4PVPlacement(0,G4ThreeVector(),
			experimentalHall_log,"expHall",0,false,0);

	return experimentalHall_phys;
}
 
void DetectorConstruction::ConstructAccel(G4LogicalVolume* experimentalHall_log)
{
//------------------------------ a phantom
  G4Box* phantom_box= new G4Box("phantom_box", phantomSizeX, phantomSizeY, phantomSizeZ);
  phantom_log = new G4LogicalVolume(phantom_box, G4Material::GetMaterial("Water"), "phantom_log", 0,0,0);
  phantom_log -> SetVisAttributes(new G4VisAttributes(TRUE, G4Colour(0.,1.,1.)));
 
  G4double phantompos_x = 0.0*m;
  G4double phantompos_y = 0.0*m;
  G4double phantompos_z = phantomSizeZ;
	phantom_phys	= new G4PVPlacement(0, G4ThreeVector(phantompos_x, phantompos_y, phantompos_z), phantom_log, "phantom_phys", experimentalHall_log, false, 0);

	//---------------------------- a Killer Plane
	G4Box* KP_box = new G4Box("KP_box", phantomSizeX, phantomSizeY, 0.5*mm);
	KP_log = new G4LogicalVolume(KP_box, G4Material::GetMaterial("Air"), "KP_log", 0,0,0);
	KP_log -> SetVisAttributes(new G4VisAttributes(TRUE, G4Colour(0.3,0.7,0.1)));
	KP_phys = new G4PVPlacement(0, G4ThreeVector(phantompos_x, phantompos_y, -500.*mm), KP_log, "KP_phys", experimentalHall_log, false, 0);
  

  //    volumes
  //    beam line along z axis
//------------------------target 15MV------------------------
  G4double targetADim_x = 0.6*cm;
  G4double targetADim_y = 0.6*cm;
  G4double targetADim_z = 0.03175*cm;
  G4Box* targetA_box = new G4Box("targetA_box",targetADim_x,targetADim_y,targetADim_z);
  G4LogicalVolume *targetA_log = new G4LogicalVolume(targetA_box,G4Material::GetMaterial("W2"),"targetA_log",0,0,0);
  G4double targetAPos_x = 0.0*m;
  G4double targetAPos_y = 0.0*m;
  G4double targetAPos_z = -99.96825*cm;
  G4VPhysicalVolume *targetA_phys=0;	
  targetA_phys = new G4PVPlacement(0,
            G4ThreeVector(targetAPos_x,targetAPos_y,targetAPos_z),
            targetA_log,"targetA", experimentalHall_log,false,0);

  G4double targetBDim_x = 0.6*cm;
  G4double targetBDim_y = 0.6*cm;
  G4double targetBDim_z = 0.39624*cm;
  G4Box* targetB_box = new G4Box("targetB_box",targetBDim_x,targetBDim_y,targetBDim_z);
  G4LogicalVolume *targetB_log = new G4LogicalVolume(targetB_box,G4Material::GetMaterial("G4_Cu"),"targetB_log",0,0,0);
	G4double targetBPos_x = 0.0*m;
	G4double targetBPos_y = 0.0*m;
	G4double targetBPos_z = -99.54026*cm;
	G4VPhysicalVolume *targetB_phys=0;
	targetB_phys = new G4PVPlacement(0,
			G4ThreeVector(targetBPos_x,targetBPos_y,targetBPos_z),
			targetB_log,"targetB", experimentalHall_log,false,0);


	// ***********  REGIONS for CUTS
	G4ProductionCuts* cuts = new G4ProductionCuts;
	cuts->SetProductionCut(0.1*cm);

	regVol= new G4Region("targetR");
	regVol->SetProductionCuts(cuts);

	targetA_log->SetRegion(regVol);
	regVol->AddRootLogicalVolume(targetA_log);
	targetB_log->SetRegion(regVol);
	regVol->AddRootLogicalVolume(targetB_log);

	// Visualization attributes

	targetAVis = new G4VisAttributes(G4Colour(1.,0.,1.));
	targetAVis->SetVisibility(true);
// 	simpleWSVisAtt->SetForceSolid(true);
	targetBVis = new G4VisAttributes(G4Colour(0.,1.,1.));
	targetBVis->SetVisibility(true);
// 	simpleCuSVisAtt->SetForceSolid(true);
	targetA_log->SetVisAttributes(targetAVis);
	targetB_log->SetVisAttributes(targetBVis);

  //------------------------------------------------------ volumes

// primary collimator
   G4Colour  green (0.0, 1.0, 0.0);
 
 //---------rotation matrix first collimator --------

  G4RotationMatrix*  rotateMatrix=new G4RotationMatrix();
  rotateMatrix->rotateX(180.0*deg);

  //-------------------- the first collimator upper----------------

  
  G4double innerRadiusOfTheTubeEx = 1.0*cm;
  G4double outerRadiusOfTheTubeEx = 8.*cm;
  G4double hightOfTheTubeEx = 1.0*cm;
  G4double startAngleOfTheTubeEx = 0.*deg;
  G4double spanningAngleOfTheTubeEx = 360.*deg;
	G4Tubs* UpperCollimator = new G4Tubs("UpperCollimator",innerRadiusOfTheTubeEx,
			outerRadiusOfTheTubeEx,hightOfTheTubeEx,
			startAngleOfTheTubeEx,spanningAngleOfTheTubeEx);
  G4LogicalVolume *UpperCollimator_log = new G4LogicalVolume(UpperCollimator,G4Material::GetMaterial("W2"),"UpperCollimator_log",0,0,0);

  G4double UpperCollimatorPosX = 0.*cm;
  G4double UpperCollimatorPosY = 0.*cm;
  G4double UpperCollimatorPosZ = -99.*cm;
  G4VPhysicalVolume *UpperCollimator_phys=0;
  UpperCollimator_phys = new G4PVPlacement(0,
					   G4ThreeVector(UpperCollimatorPosX,UpperCollimatorPosY,
							 UpperCollimatorPosZ), UpperCollimator_log,"UpperCollimator",experimentalHall_log,false,0);

 
  //-------------------- the first collimator lower----------------

  G4double  pRmin1 = 0.724*cm;
 
  G4double  pRmax1 = 8.*cm;
  G4double  pRmin2 = 2.09*cm;
  G4double  pRmax2 = 8.*cm;
  G4double  hightOfTheCone =2.765*cm;
  G4double  startAngleOfTheCone = 0.*deg;
  G4double  spanningAngleOfTheCone = 360.*deg;

  G4Cons* collim_cone = new G4Cons("collim_cone",pRmin1,pRmax1,pRmin2,
				   pRmax2,hightOfTheCone,startAngleOfTheCone,
				   spanningAngleOfTheCone);
  G4LogicalVolume *collim_log = new G4LogicalVolume(collim_cone,G4Material::GetMaterial("W2"),"collim_log",0,0,0);

  G4double CminusCPos_x = 0.*cm;
  G4double CminusCPos_y = 0.*cm;
  G4double CminusCPos_z = -95.235*cm;
  G4VPhysicalVolume *CylMinusCone_phys =0;
  CylMinusCone_phys = new G4PVPlacement(0,
					G4ThreeVector(CminusCPos_x,CminusCPos_y,CminusCPos_z),
					collim_log,"CylMinusCone", experimentalHall_log,false,0);
  
//--------- Visualization attributes -------------------------------
   primcolVis = new G4VisAttributes(G4Colour(1.,0.,1.));
   primcolVis->SetVisibility(true);
//    simpleTungstenWVisAtt->SetForceSolid(true);
   collim_log->SetVisAttributes(primcolVis);
   UpperCollimator_log->SetVisAttributes(primcolVis);

	// ***********  REGIONS for CUTS
	 regVol1= new G4Region("PrymCollR");
	 G4ProductionCuts* cuts1 = new G4ProductionCuts;
	 cuts1->SetProductionCut(0.5*cm);
	 regVol1->SetProductionCuts(cuts1);

	 collim_log->SetRegion(regVol1);
	 regVol1->AddRootLogicalVolume(collim_log);

	 UpperCollimator_log->SetRegion(regVol1);
	 regVol1->AddRootLogicalVolume(UpperCollimator_log);



//Flattning Filter*********************************************
	 G4double z0;
	 G4ThreeVector centre;

	 const G4double z1[23] = {-1.88976*cm, -1.82372*cm, -1.73228*cm, -1.65862*cm, -1.57988*cm, -1.50622*cm, -1.38938*cm, -1.28524*cm, -1.08458*cm, -0.89916*cm, -0.72898*cm, -0.5715*cm, -0.42672*cm, -0.30226*cm, -0.2286*cm, -0.17018*cm, -0.11938*cm, -0.0762*cm, -0.0762*cm, -0.3175*cm, -0.3175*cm, 0.000*cm, 0.000*cm};
	 const G4double r1[23] = {0.000*cm, 0.1016*cm, 0.19812*cm, 0.2667*cm, 0.33274*cm, 0.4064*cm, 0.5207*cm, 0.62992*cm, 0.87122*cm, 1.10744*cm, 1.34874*cm, 1.59512*cm, 1.84658*cm, 2.10566*cm, 2.2352*cm, 2.36728*cm, 2.49682*cm, 2.63398*cm, 3.175*cm, 3.429*cm, 3.81*cm, 3.81*cm, 0.000*cm};

	 G4GenericPolycone *FFL1A_1Cone = new G4GenericPolycone("FFL1A_1", 0.*deg, 360.*deg, 23, r1, z1);

	 z0=-872.525*mm;
	 centre.set(0.,0.,z0);
	 G4LogicalVolume *FFL1A_1LV = new G4LogicalVolume(FFL1A_1Cone, G4Material::GetMaterial("W"), "FFL1A_1LV", 0, 0, 0);
	 G4VPhysicalVolume *FFL1A_1PV=0;
	 FFL1A_1PV=new G4PVPlacement(0, centre, FFL1A_1LV, "FFLA_1PV", experimentalHall_log, false, 0);

	 flatVis = new G4VisAttributes(G4Colour::Red());
	 flatVis->SetVisibility(true);
	 // 	simpleAlSVisAtt->SetForceSolid(true);
	 FFL1A_1LV->SetVisAttributes(flatVis);

	 // region for cuts
	 regVol2= new G4Region("flatfilterR");
	 G4ProductionCuts* cuts2 = new G4ProductionCuts;
	 cuts2->SetProductionCut(0.01*cm);
	 regVol2->SetProductionCuts(cuts2);
	 FFL1A_1LV->SetRegion(regVol2);
	 regVol2->AddRootLogicalVolume(FFL1A_1LV);


	 // JawX --------------------------------------------------------------------------

	 G4Box *JawX = new G4Box("JawX", 45.*mm, 93.*mm, 78/2*mm);
	 G4LogicalVolume *JawXLV = new G4LogicalVolume(JawX, G4Material::GetMaterial("W"), "JawXLV", 0, 0, 0);
	 G4double thetaX1, thetaX2, distX1, distX2, betaX1, betaX2, trajX1, trajX2;
	 thetaX1 = fabs(atan(X1/(1000.*mm)));
	 thetaX2 = fabs(atan(X2/(1000.*mm)));
	 trajX1 = 36.63/cos(thetaX1);
	 trajX2 = 36.63/cos(thetaX2);
	 betaX1 = fabs(atan(4.5/(3.9+trajX1)));
	 betaX2 = fabs(atan(4.5/(3.9+trajX2)));
	 distX1 = std::sqrt(4.5*4.5+(3.9+trajX1)*(3.9+trajX1))*cm;
	 distX2 = std::sqrt(4.5*4.5+(3.9+trajX2)*(3.9+trajX2))*cm;
	 G4VPhysicalVolume *JawX1=0;
	 G4VPhysicalVolume *JawX2=0;
	 G4RotationMatrix *cRotationX1=new G4RotationMatrix();
	 G4RotationMatrix *cRotationX2=new G4RotationMatrix();
	 cRotationX1->rotateY(thetaX1);
	 cRotationX2->rotateY(-thetaX2);
	 JawX1= new G4PVPlacement(cRotationX1, G4ThreeVector(-distX1*sin(thetaX1+betaX1),0, -100.*cm+distX1*cos(thetaX1+betaX1)),JawXLV,"JawX1",experimentalHall_log,false,0);
	 JawX2= new G4PVPlacement(cRotationX2, G4ThreeVector(distX2*sin(thetaX2+betaX2),0, -100.*cm+distX2*cos(thetaX2+betaX2)),JawXLV,"JawX2", experimentalHall_log, false, 1);
	 xjawVis = new G4VisAttributes(G4Colour(1.,0.5,1.));
	 xjawVis->SetVisibility(true);
	 JawXLV->SetVisAttributes(xjawVis);

// JawY  -----------------------------------------------------------------------

	 G4Box *JawY = new G4Box("JawY", 93.*mm, 45.*mm, 77.7/2*mm);
	 G4LogicalVolume *JawYLV = new G4LogicalVolume(JawY, G4Material::GetMaterial("W"), "JawYLV", 0, 0, 0);
	 G4double thetaY1, thetaY2, distY, betaY;
	 //	Y1 = 50.*mm;
	 //	Y2 = 50.*mm;
	 thetaY1 = fabs(atan(Y1/(1000.*mm)));
	 thetaY2 = fabs(atan(Y2/(1000.*mm)));
	 betaY = fabs(atan(4.5/31.765));
	 distY = std::sqrt(4.5*4.5+31.765*31.765)*cm;
	 G4VPhysicalVolume *JawY1=0;
	 G4VPhysicalVolume *JawY2=0;
	 G4RotationMatrix *cRotationY1=new G4RotationMatrix();
	 G4RotationMatrix *cRotationY2=new G4RotationMatrix();
	 cRotationY1->rotateX(thetaY1);
	 cRotationY2->rotateX(-thetaY2);
	 JawY1= new G4PVPlacement(cRotationY1, G4ThreeVector(0,distY*sin(thetaY1+betaY), -100.*cm+distY*cos(thetaY1+betaY)),JawYLV,"JawY1",experimentalHall_log,false,0);
	 JawY2= new G4PVPlacement(cRotationY2, G4ThreeVector(0,-distY*sin(thetaY2+betaY), -100.*cm+distY*cos(thetaY2+betaY)),JawYLV,"JawY2", experimentalHall_log, false, 1);
	 yjawVis = new G4VisAttributes(green);
	 yjawVis->SetVisibility(true);
	 JawYLV->SetVisAttributes(yjawVis);

	 regVol3= new G4Region("Jaws");
	 G4ProductionCuts* cuts3 = new G4ProductionCuts;
	 cuts3->SetProductionCut(0.1*cm);
	 regVol3->SetProductionCuts(cuts3);

	 JawXLV->SetRegion(regVol3);
	 regVol3->AddRootLogicalVolume(JawXLV);
	 JawYLV->SetRegion(regVol3);
	 regVol3->AddRootLogicalVolume(JawYLV);

/*
	 //detector --------------------------------------------------------------------

	 G4Box* sheath=new G4Box("sheath_box",25.*cm,1*cm,1*cm);
	 G4LogicalVolume* sheathLV=new G4LogicalVolume(sheath,G4Material::GetMaterial("Water"),"sheathLV",0,0,0);
	 G4VPhysicalVolume* sheathPV=new G4PVPlacement(0,G4ThreeVector(0,0,-15.*cm),sheathLV,"sheathPV",phantom_log,false,0);
	 sheathLV->SetVisAttributes(G4VisAttributes::Invisible);
	 
	 //
	 G4double chamber_r = 1.0*cm;
	 G4double chamber_z = 0.1*cm;
	 G4Box* chamber_tubs
		 = new G4Box("chamber_tubs", chamber_z, chamber_r, chamber_r);
	 chamber_log = new G4LogicalVolume(chamber_tubs,
			 G4Material::GetMaterial("Water"),"chamber_log",0,0,0);

	 G4VPhysicalVolume* chamber_phys=0;
	 chamber_phys = new G4PVReplica("chamber",chamber_log,sheathPV,kXAxis,250,0.2*cm,0);
	 chamber_log->SetVisAttributes(new G4VisAttributes(G4Colour(0.5,0.5,1)));
//	 chamber_log->SetVisAttributes(G4VisAttributes::Invisible);
*/
}

void DetectorConstruction::ActivateDet()
{
	//sensitive detectors

	G4SDManager* SDman = G4SDManager::GetSDMpointer();
	G4VSensitiveDetector* sdet = SDman->FindSensitiveDetector(sensitiveDetectorName, false);
	G4VSensitiveDetector* kdet = SDman->FindSensitiveDetector(killerPlaneName, false);


	if(!phantomROGeometry){
//		G4cout << "no phantom SD" << G4endl;
		G4String ROGeometryName = "PhantomROGeometry";
		phantomROGeometry = new PhantomROGeometry(ROGeometryName,
				phantomSizeX, phantomSizeY, phantomSizeZ,
				numberOfVoxelsAlongX, numberOfVoxelsAlongY, numberOfVoxelsAlongZ);
		phantomROGeometry->BuildROGeometry();
	}

	if(!sdet){
//		G4cout << "no detector" << G4endl;
		phantomSD = new PhantomSD(sensitiveDetectorName);
		phantomSD->SetROgeometry(phantomROGeometry);
		SDman->AddNewDetector(phantomSD);

		phantom_log->SetSensitiveDetector(phantomSD);
	}else{
//		G4cout << "detector exist" << G4endl;
		phantomSD->SetROgeometry(phantomROGeometry);
		phantom_log->SetSensitiveDetector(phantomSD);
	}

	if(!kdet){
		killerSD = new KillerSD(killerPlaneName, this->inputData);
		killerSD->set(Ecut, Gcut, Ncut, Pcut);
		SDman->AddNewDetector(killerSD);
		KP_log->SetSensitiveDetector(killerSD);
	}else{
		killerSD->set(Ecut,Gcut,Ncut,Pcut);
		KP_log->SetSensitiveDetector(killerSD);
	}

	// Setting of splitting
	G4LogicalVolume* logicTargetA = G4LogicalVolumeStore::GetInstance()->GetVolume("targetA_log");
	G4LogicalVolume* logicTargetB = G4LogicalVolumeStore::GetInstance()->GetVolume("targetB_log");

	BOptrBremSplitting* bremSplittingOperator = new BOptrBremSplitting();
	bremSplittingOperator->AttachTo(logicTargetA);
	bremSplittingOperator->AttachTo(logicTargetB);
	
}
