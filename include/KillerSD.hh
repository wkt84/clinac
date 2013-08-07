#ifndef KillerSD_h
#define KillerSD_h 1

#include "G4VSensitiveDetector.hh"
#include "SInputData.hh"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class KillerSD : public G4VSensitiveDetector
{
public:
  KillerSD(G4String name, SInputData *inputData);
  ~KillerSD();

  void Initialize(G4HCofThisEvent*);
  G4bool ProcessHits(G4Step* aStep,G4TouchableHistory* ROhist);
  void EndOfEvent(G4HCofThisEvent* HCE);
  void clear();
  void DrawAll();
  void PrintAll();
	void save(G4int);
	void setfname();

	void set(G4double, G4double, G4double, G4double);

private:
  G4double ecut, gcut, ncut, pcut;
	SInputData *inputData;
	SParticle *particles;
	G4double posx, posy, posz;
	G4double dirx, diry, dirz;
	G4String filename;
	G4int nParticle;
};
#endif


