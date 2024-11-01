/*

This script was originally written by J. McKean and shared with me. 
Input: <neutvect.root>
Output: <root file with crs plot> 


*/

#include "neutvect.h"
#include "neutpart.h"
#include "neutvtx.h"


#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TH1D.h"
#include "TH2F.h"
#include "TH2D.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TLegend.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

void scale_by_energy(TH1F*& hist){
  for (int i = 0; i < hist->GetNbinsX()+1; i++) {
          double energy = hist->GetBinCenter(i);
          hist->SetBinContent(i, hist->GetBinContent(i)/energy);
  }
}

int main(int argc, char const *argv[]){
  
  // Error handling for the number of arguments 
  if(argc < 3){
    std::cout << "[ERROR]: Expected at least one argument." << std::endl;
    std::cout << "[USAGE]: " << argv[0] << " <neutvect.root> <out.root>" << std::endl;
  return 1;
  }

  // Error handling in case the root file cannot be read
  TFile fin(argv[1],"READ");
  if(fin.IsZombie()){
    std::cout << "[ERROR]: Failed to read root file: " << argv[1] << std::endl;
  return 1;
  }


  // Get pointer for neuttree object which is in the root file
  // Also do some error handling in case it is not there
  TTree *neuttree = 0;
  fin.GetObject("neuttree", neuttree);
  if(!neuttree){
    std::cout << "[ERROR]: Failed to read \"neuttree\" from: " << argv[1] << std::endl;
  return 1;
  }

  // Get vector branch from neuttree (need to set the pointer to nullptr first)
  NeutVect *neutev = 0;
  neuttree->SetBranchAddress("vectorbranch", &neutev);

  // Get Num Entires for event loop
  Long64_t nevs = neuttree->GetEntries();
  std::cout << "[INFO]: Reading tree with " << nevs << " NEUT events." << std::endl;

  TH1F *totcrs = new TH1F("totalxs","Total Cross Section",300,0,3000);
  TH1F *evrtrt = new TH1F("enu_histo", "E_{#nu} histo", 300,0,3000);

  TH1F *crs_qe = new TH1F("qexs","QE Cross Section",300,0,3000);
  TH1F *crs_cc1pi = new TH1F("cc1pixs","CC1#pi Cross Section",300,0,3000);
  TH1F *crs_ccINCL = new TH1F("ccINCL","CC-Incl. Cross Section",300,0,3000);

  int counter = 0;

  for (Long64_t ent_it = 0; ent_it < nevs; ++ent_it) {
    neuttree->GetEntry(ent_it);
    int mode = abs(neutev->Mode);
    double enu = neutev->PartInfo(0)->fP.E();
    evrtrt->Fill(enu);
    totcrs->Fill(enu,neutev->Totcrs);

    if(mode == 1){ // CCQE
      crs_qe->Fill(enu,neutev->Totcrs);
      // totcrs->Fill(enu,neutev->Totcrs);
    } else if (mode == 11 || mode == 12 || mode == 13) { // CC1pi
      crs_cc1pi->Fill(enu,neutev->Totcrs);
      // totcrs->Fill(enu,neutev->Totcrs);
    }
    
    if (mode < 26) { // CC2p2h
      crs_ccINCL->Fill(enu, neutev->Totcrs);
      // totcrs->Fill(enu,neutev->Totcrs);
    }
  }
  totcrs->Divide(evrtrt);
  crs_qe->Divide(evrtrt);
  crs_cc1pi->Divide(evrtrt);
  crs_ccINCL->Divide(evrtrt);

  double scale_factor = totcrs->Integral()/nevs;
  totcrs->Scale(scale_factor);
  crs_qe->Scale(scale_factor);
  crs_cc1pi->Scale(scale_factor);
  crs_ccINCL->Scale(scale_factor);

  scale_by_energy(totcrs);
  scale_by_energy(crs_qe);
  scale_by_energy(crs_cc1pi);
  scale_by_energy(crs_ccINCL);

 
  auto legend = new TLegend(0.6,0.6,0.8,0.8);
  

  TFile* outputFileRoot = TFile::Open(argv[2], "RECREATE");
  TCanvas c1("c1", "canvas");
  c1.cd();
  totcrs->Draw("HIST");

  crs_qe->SetLineColor(kRed);
  crs_qe->Draw("HIST SAME");

  crs_cc1pi->SetLineColor(kBlue);
  crs_cc1pi->Draw("HIST SAME");

  crs_ccINCL->SetLineColor(kOrange);
  crs_ccINCL->Draw("HIST SAME");

  crs_ccINCL->GetXaxis()->SetTitle("E_{#nu} [MeV]");
  crs_ccINCL->GetYaxis()->SetTitle("")

  legend->AddEntry(totcrs, "Total Cross Section", "f");
  legend->AddEntry(crs_qe, "CCQE", "f");
  legend->AddEntry(crs_cc1pi, "CC1#pi", "f");
  legend->AddEntry(crs_ccINCL, "CC Incl.", "f");
  legend->Draw();
  c1.Write();

  outputFileRoot->Close();


  legend->Delete();
  totcrs->Delete();
  evrtrt->Delete();
  crs_qe->Delete(); 
  crs_cc1pi->Delete();
  crs_ccINCL->Delete();
}






