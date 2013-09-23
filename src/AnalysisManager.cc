#include <stdlib.h>
#include <fstream>
#include "AnalysisManager.hh"

#include "G4ios.hh"

#include "TROOT.h"
#include "TH1.h"
#include "TH2.h"
#include "TFile.h"

AnalysisManager* AnalysisManager::instance = 0;

AnalysisManager::AnalysisManager() : 
hist_z(0), hist_xz(0), hist_n(0), hist_u(0), hist_d2(0), hist_o(0)
{
//gROOT->Reset();
}

AnalysisManager::~AnalysisManager() 
{ 
  delete hist_z;
  hist_z = 0;

	delete hist_xz;
	hist_xz = 0;

	delete hist_n;
	hist_n = 0;

	delete hist_u;
	hist_u = 0;

	delete hist_d2;
	hist_d2 = 0;

	delete hist_o;
	hist_o = 0;
}

AnalysisManager* AnalysisManager::getInstance()
{
  if (instance == 0) instance = new AnalysisManager;
  return instance;
}

void AnalysisManager::book() 
{ 
  // Instantiate the histogram and ntuple factories
  hist_z = new TH1F("h1", "1D-Dose", 500, 0, 500);
	hist_z->GetXaxis()->SetTitle("depth (mm)");
	hist_z->GetYaxis()->SetTitle("Dose (Gy)");

	hist_xz = new TH2F("h2", "2D-Dose", 500, -250, 250, 500, 0, 500);
	hist_xz->GetXaxis()->SetTitle("x (mm)");
	hist_xz->GetYaxis()->SetTitle("z (mm)");
	hist_xz->GetZaxis()->SetTitle("Dose (Gy)");

	hist_u = new TH2F("h3","2D-Uncertainty", 500, -250, 250, 500, 0, 500);
	hist_u->GetXaxis()->SetTitle("x (mm)");
	hist_u->GetYaxis()->SetTitle("z (mm)");
	hist_u->GetZaxis()->SetTitle("Uncertainty(%)");
	
	hist_n = new TH2F("h4","Count", 500, -250, 250, 500, 0, 500);

	hist_d2 = new TH2F("h5", "dosedose", 500, -250, 250, 500, 0, 500);

	hist_o = new TH2F("h6", "photonuclear", 500, -250, 250, 500, 0, 500);
	hist_o->GetXaxis()->SetTitle("x (mm)");
	hist_o->GetYaxis()->SetTitle("z (mm)");
	hist_o->GetZaxis()->SetTitle("Dose (Gy)");

	number = 0;
	hit = 0;
	square = 0;
}
 
void AnalysisManager::FillHistogramWithEnergy(G4double x, G4double z, G4double energy)
{
  // 2DHistogram: energy deposit in a voxel which center is fixed in position (x,z)  
  hist_xz -> Fill(x, z, energy);
	hist_n -> Fill(x, z);
	hist_d2 -> Fill(x, z, energy*energy);
}

void AnalysisManager::FillPN(G4double x, G4double z, G4double energy)
{
	hist_o -> Fill(x, z, energy);
}


void AnalysisManager::DoseDistribution(G4double z,G4double energy)
{
  // 1DHistogram: energy spectrum of primary particles  
  hist_z -> Fill(z, energy);
}

void AnalysisManager::count(G4double h)
{
	number += 1;
	hit += h;
}

void AnalysisManager::sumsquare(G4double a)
{
	square += a;
}

G4double AnalysisManager::uncertainty()
{
	G4double abs, rel;
	abs = std::sqrt((square-hit*hit/number)/(number*(number-1)));
	rel = abs/(hit/number)*100;

	return rel;
}

void AnalysisManager::finish(G4String& fname) 
{  
	TFile *f=new TFile(fname,"RECREATE", "Geant4 ROOT Analysis");
	hist_z->Write();
	hist_xz->Write();
	hist_n->Write();
	hist_d2->Write();
	hist_o->Write();

	G4int i, j;
	G4double n, d2, ave;
	G4double unc;

	// Calculate Uncertainty
	for(i=1; i<501; i++){
		for(j=1; j<501; j++){
			n = hist_n->GetBinContent(i, j);
			d2 = hist_d2->GetBinContent(i, j);
			ave = hist_xz->GetBinContent(i, j)/n;

			if(n>1){
				unc = 100*std::sqrt((d2-n*ave*ave)/(n*(n-1)))/(hist_xz->GetBinContent(i,j));
				hist_u->Fill((2*i-501)/2, (2*j-1)/2, unc);
			}
		}
	}

	hist_u->Write();

	f->Close();
	delete f;
}

