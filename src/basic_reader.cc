#include "basic_reader.hh"

void basic_reader(const char * input_vector){
    TString in(input_vector);

    NeutVtx *nvtx;
    NeutVect *nvect;

    TFile *f = new TFile(input_vector, "READ");
    if(f == NULL){
        std::cout << "Failed to open " << input_vector << std::endl;
        return;
    }
    std::cout << "[basic_reader] Input File: " << f->GetName() <<std::endl;

    TTree *tn = (TTree *) f->Get("neuttree");

    nvtx = new NeutVtx();
    nvect = new NeutVect();

    tn->SetBranchAddress("vertexbranch", &nvtx);
    tn->SetBranchAddress("vectorbranch", &nvect);
    Int_t nevents = tn->GetEntries();
    std::cout << "Number of Events: " << nevents <<std::endl;
    init_histos();
    outfile = TFile::Open("../summary_plots_fhc.root", "RECREATE");
    for (int j = 0; j < 100000; j++){
        tn->GetEntry(j);
        h_mode->Fill(nvect->Mode);
        
        /***************************************************************/
        std::cout << "Event #        :" << nvect->EventNo << "\n";
        std::cout << "Target A       :" << nvect->TargetA << "\n";
        std::cout << "Target Z       :" << nvect->TargetZ << "\n";
        std::cout << "VNuclIni       :" << nvect->VNuclIni << "\n";
        std::cout << "VNuclFin       :" << nvect->VNuclFin << "\n";
        std::cout << "PF Surface     :" << nvect->PFSurf   << "\n";
        std::cout << "PF Maximum     :" << nvect->PFMax    << "\n";
        std::cout << "Flux ID        :" << nvect->FluxID   << "\n";

        std::cout << "Intr. mode     :" << nvect->Mode   << "\n";

        //loop through event info in nvect



        /*
        for(int i = 0; i < nvect->Npart(); i++){
                std::cout << "i=" << i << "\n";
                std::cout << "Vertex         =" << nvect->VertexID(i) << "\n";
                std::cout << "Parent Index   =" << nvect->ParentIdx(i) << "\n";

                std::cout << "Particle Code  = " << (nvect->PartInfo(i))->fPID   << "\n";
                std::cout << "Particle Mass  = " << (nvect->PartInfo(i))->fMass   << "\n";
                std::cout << "Particle Mom.  =(" << (nvect->PartInfo(i))->fP.Px() << ","
                    << (nvect->PartInfo(i))->fP.Py() << "," 
                    << (nvect->PartInfo(i))->fP.Pz() << ","
                    << (nvect->PartInfo(i))->fP.E()  << ")"
                    << "\n";
        }
        */
    }

    std::cout << "Writing Histos" << std::endl;
    write_histos();
   
    outfile->Close();
    f->Close();
    return;
}

int main(){
    basic_reader("../output/fhc_13a_1e6_rs.root");
    return 1;
}
