#ifndef PhantomSD_h
#define PhantomSD_h 1

#include "G4VSensitiveDetector.hh"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class PhantomSD : public G4VSensitiveDetector
{
public:
  PhantomSD(G4String name);
  ~PhantomSD();

  void Initialize(G4HCofThisEvent*);
  G4bool ProcessHits(G4Step* aStep,G4TouchableHistory* ROhist);
  void EndOfEvent(G4HCofThisEvent* HCE);
  void clear();
  void DrawAll();
  void PrintAll();

private:
  G4double x;
  G4double y;
  G4double z;
	G4double edep;
	G4int i;
};
#endif


