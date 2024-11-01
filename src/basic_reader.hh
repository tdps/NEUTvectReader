#include <iostream>
#include "tools.h"
#include "stdlib.h"
#include "TVector3.h"
#include <cmath>
#include "neutpart.h"
#include "neutvect.h"
#include "neutvtx.h"

/* File pointers*/
TFile * outfile;

/* Histos */
TH1F * h_mode;
TH1F * h_enu;

void init_histos(){

    h_mode = new TH1F("mode","Neutrino Mode in ROOT file",56,0, 55);

};

void write_histos(){
    outfile->cd();
    h_mode->Write();
    quicksave(h_mode, "../plots/h_mode.png");
}

bool isNeutrino(Int_t id){
    if(abs(id) == 12 || abs(id) == 14 || abs(id) == 16){
        return true;
    }else{
        return false;
    }
}

std::vector<Int_t> GetPrimaryParticles(NeutVect *nvect){
    std::vector<Int_t> primary_particles;
    for(int i = 0; i < nvect->Npart(); i++){ 
        NeutPart *this_particle = nvect->PartInfo(i);
        Int_t parent_index = nvect->ParentIdx(i);
        if(isNeutrino((nvect->PartInfo(parent_index))->fPID)){ //is primary particle
            primary_particles.push_back(this_particle->fPID);
        }
    }
    return primary_particles;
}
