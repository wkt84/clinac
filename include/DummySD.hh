// Dummy sensitive used only to flag sensitivity in cells of RO geometry.

#ifndef DummySD_h
#define DummySD_h 1

#include "G4VSensitiveDetector.hh"
class G4Step;

class DummySD : public G4VSensitiveDetector
{
public:
  DummySD();
  ~DummySD() {};
  
  void Initialize(G4HCofThisEvent* ) {};
  G4bool ProcessHits(G4Step* ,G4TouchableHistory*) {return false;}
  void EndOfEvent(G4HCofThisEvent*) {};
  void clear() {};
  void DrawAll() {};
  void PrintAll() {};
};

DummySD::DummySD() : G4VSensitiveDetector("dummySD")
{}
#endif
