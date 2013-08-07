#ifndef DetectorMessenger_h 
#define DetectorMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class DetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAnInteger;
class G4UIcmdWithoutParameter;

class DetectorMessenger: public G4UImessenger
{
  public:
  DetectorMessenger(DetectorConstruction*);
  ~DetectorMessenger();

  void SetNewValue(G4UIcommand*, G4String);

  private:
  DetectorConstruction* Detector;
  
  G4UIdirectory*  accdet;
	G4UIdirectory*  killdet;
  G4UIcmdWithAnInteger *Energycmd;
  G4UIcmdWithADoubleAndUnit *X1cmd, *X2cmd, *Y1cmd, *Y2cmd;
	G4UIcmdWithADoubleAndUnit *Ecutcmd, *Gcutcmd, *Ncutcmd, *Pcutcmd; 
	G4UIcmdWithoutParameter *Updatecmd;
};

#endif
