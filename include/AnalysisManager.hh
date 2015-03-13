#ifndef G4ANALYSISMANAGER_HH
#define G4ANALYSISMANAGER_HH

#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include <vector>
#include "G4ThreeVector.hh"

class TH1F;
class TH2F;

class AnalysisManager
{

public:
  ~AnalysisManager();
  static AnalysisManager* getInstance();

  void book();
	void DoseDistribution(G4double, G4double);
  void FillHistogramWithEnergy(G4double,G4double,G4double);
	void FillPN(G4double, G4double, G4double);
  void finish(G4String&);
	void count(G4double);
	void sumsquare(G4double);
	G4double uncertainty();

private:
  AnalysisManager();
  static AnalysisManager* instance;

	TH1F* hist_z; 
	TH2F* hist_xz;
	TH2F* hist_n;
	TH2F* hist_u;
	TH2F* hist_d2;
	TH2F* hist_o;

	G4int number;
	G4double hit;
	G4double square;


public:
	G4int printnumber() const {return number;}
	G4double printsquare() const {return square;}
	G4double printhit() const {return hit;}


};

#endif

