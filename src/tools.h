//
// Created by Tristan Schefke on 2/8/24.
//

#ifndef EVENTSELECTION_TOOLS_H
#define EVENTSELECTION_TOOLS_H
#include <iostream>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <string>
#include <map>
#include <array>

#include "TROOT.h"
#include "TChain.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TObjString.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
//#include "t2ksk_eventSelections.hh"
#include "TLine.h"
#include "TArrow.h"
#include "TStyle.h"
#include "TLatex.h"
#include "THStack.h"
#include "TPaveText.h"
#include "TLegend.h"
#include "SetT2KStyle.H"
#include "TGraph.h"

static std::map<unsigned int, const char *> neut_mode{
        {1, "CCQE"},
        {2, "2p2h"},
        {11, "CCRes1pi1p"},
        {12,"CCRes1pi01p"},
        {13, "CCRes1pi1n"},
        {15, "CCDif1pi+"},
        {16,"CCDif1pi-"},
        {17, "CCRes1gamma"},
        {21,"CCNpi"},
        {22,"CCRes1eta0"},
        {23,"CCRes1K0"},
        {26,"CCDIS"},
        {31, "NCRes1pi0"},
        {32, "NCRes1pi0"},
        {33, "NCRes1pi-"},
        {34, "NCRes1pi+"},
        {35, "NCDif1pi0"},
        {36, "NCCoh1pi0"},
        {38, "NCRes1gamma"},
        {39, "NCRes1gamma"},
        {41, "NCNpi"},
        {42, "NCRes1eta0"},
        {43, "NCRes1eta0"},
        {44, "NCRes1K0"},
        {45, "NCRes1K+"},
        {46, "NCDIS"},
        {51, "NCEL"},
        {52, "NCEL"}
};

static std::map<unsigned int, const char *> t2ksk_samples{
        {0, "1Re"},
        {1, "1Rmu"},
        {2, "1Re1de"},
        {3, "MRe1de"},
        {4, "MRmu1de"},
        {5, "MRmu2de"},
        {6, "1Rncpi0"},
        {7, "2Rncpi0"},

};

const int _numSKsamples = 8;
const int _numNeutModes = 27;

short _cut_line_col = kBlue;
short _cut_line_width = 3;
short _cut_line_style = 10;

TCanvas *tmp = new TCanvas("tmp", "tmp", 1600, 1600);

std::string ToUpper(std::string& arrowopt){
    TString tstr(arrowopt);
    tstr.ToUpper();
    return tstr.Data();
}

template <typename T, typename F>
void writeCSVTable(const char * filename, const T arr[], const F arrNames[], size_t size) {
    std::ofstream file(filename, std::ios_base::out);
    if (!file.is_open()) {
        std::cerr << "Cannot open file: " << filename << std::endl;
        return;
    }

    for (int i = 0; i < 2; i++) {
        for (int t = 0; t < size; t++) {
            if (i == 0) {
                if (t == size - 1) {
                    file << arrNames[t] << "\n";
                } else {
                    file << arrNames[t] << ",";
                }
            } else {
                if (t == size - 1) {
                    file << arr[t] << "\n";
                } else {
                    file << arr[t] << ",";
                }
            }
        }
    }
    file.close();
    std::cout << "CSV file written to " << filename << std::endl;
}

bool makedirs(TString outdirs){
    if(!std::filesystem::is_directory(outdirs.Data())){ //if it is not a directory, create it
        try {
            std::filesystem::create_directories(outdirs.Data());
        }catch (const std::exception& e){
            std::cerr << "Unable to create directory: " << e.what() << std::endl;
            return false;
        }
    }
    return true;
}

int CommonStyle(){
    // T2K style
    TString localStyleName = "CommonStyle";

    // -- WhichStyle -- //
    // 1 = presentation large fonts
    // 2 = presentation small fonts
    // 3 = publication/paper

    Int_t localWhichStyle = 2;
    TStyle* commonStyle = SetCommonStyle(localStyleName);
    gROOT->SetStyle(commonStyle->GetName());

    // -- margin -- //
    gStyle->SetPadTopMargin(0.05);
    gStyle->SetPadBottomMargin(0.22);
    gStyle->SetPadRightMargin(0.045);

    // more space if no colz option
    gStyle->SetPadLeftMargin(0.22);
    //
    // -- title/lable offset -- //
    gStyle->SetTitleOffset(1.02, "x");
    gStyle->SetTitleOffset(1.05, "y");
    gStyle->SetLabelOffset(0.02, "x");
    gStyle->SetLabelOffset(0.02, "y");

    // -- title/label size -- //
    gStyle->SetTitleSize(0.04, "x");
    gStyle->SetTitleSize(0.04, "y");
    gStyle->SetTitleSize(0.04, "z");
    gStyle->SetLabelSize(0.034,"x");
    gStyle->SetLabelSize(0.034,"y");
    gStyle->SetLabelSize(0.034,"z");

    // -- statistic and title info --
    gStyle->SetOptTitle(0);
    //gStyle->SetOptStat(1111);

    // -- lines -- //
    // gStyle->SetLineWidth(4);

    // -- fills/color -- //
    gStyle->SetFrameFillColor(0); // white color for backgroud
    gStyle->SetFillColor(38);
    gStyle->SetFillStyle(1001);
    //
    // -- color scheme -- //
    // - "warm"/red-ish - //
    const Int_t NRGBs = 3;
    const Int_t NCont = 500;
    Double_t red[] = {1.00, 1.00, 0.25 };
    Double_t green[] = {1.00, 0.00, 0.00 };
    Double_t blue[] = {0.00, 0.00, 0.00 };
    Double_t stops[] = { 0.25, 0.75, 1.00 };
    TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
    gStyle->SetNumberContours(NCont);
    // - rainbow - //
     gStyle->SetPalette(1); // use the rainbow color set //

    // -- horizontal error bars back -- //
    // gStyle->SetErrorX(0.5);

    // -- transparent stuff -- //
    // gStyle->SetFillStyle(1);
    return 0;
}

//*** Root-specific functions that are useful***//
TH1F * DrawOverflow(TH1F *h){
    Int_t maxbin = h->GetNbinsX();
    Int_t eventsOver = h->GetBinContent(maxbin+1);
    Float_t errOver = h->GetBinError(maxbin+1);
    Int_t eventsUnder = h->GetBinContent(0);
    Float_t errUnder = h->GetBinError(0);
    Int_t lastBin = h->GetBinContent(maxbin);
    Float_t errLastBin = h->GetBinError(maxbin);
    Int_t firstBin = h->GetBinContent(1);
    Float_t errFirstBin = h->GetBinError(1);

    Int_t newLastBin = lastBin+eventsOver;
    Float_t newErrLastBin = std::sqrt(errOver*errOver + errLastBin*errLastBin);
    Int_t newFirstBin = firstBin+eventsUnder;
    Float_t newErrFirstBin = std::sqrt(errUnder*errUnder + errFirstBin*errFirstBin);

    //remove old maxbin/minbin
    h->SetBinContent(maxbin+1, 0.0);
    h->SetBinError(maxbin+1, 0.0);
    h->SetBinContent(0, 0.0);
    h->SetBinError(0, 0.0);
    //make new maxbin/minbins.
    h->SetBinContent(maxbin, newLastBin);
    h->SetBinError(maxbin, newErrLastBin);
    h->SetBinContent(1, newFirstBin);
    h->SetBinError(1, newErrFirstBin);
    return h;
}

Double_t getMCNormalization(TString filename, TString prod_version){

    if(!strcmp(prod_version.Data(),"t2kmc19b")){
        //std::cout << "Loading MC Normalizations for t2kmc 19b" << std::endl;
        Double_t norm = 1.0;

        std::map<std::string, Double_t> Numerator_fhc;
        std::map<std::string, Double_t> Denominator_fhc;
        std::map<std::string, Double_t> Numerator_rhc;
        std::map<std::string, Double_t> Denominator_rhc;

        Numerator_fhc["numu_x_numu"] = 1373.872;
        Numerator_fhc["numu_x_nue"] = 1458.340;
        Numerator_fhc["numubar_x_numubar"] = 48.495;
        Numerator_fhc["numubar_x_nuebar"] = 50.212;
        Numerator_fhc["nue_x_nue"] = 26.225;
        Numerator_fhc["nuebar_x_nuebar"] = 2.749;


        Denominator_fhc["numu_x_numu"] = 634441;
        Denominator_fhc["numu_x_nue"] = 127017;
        Denominator_fhc["numubar_x_numubar"] = 635545;
        Denominator_fhc["numubar_x_nuebar"] = 126509;
        Denominator_fhc["nue_x_nue"] = 127010;
        Denominator_fhc["nuebar_x_nuebar"] = 126865;


        Numerator_rhc["numu_x_numu"] = 205.214;
        Numerator_rhc["numu_x_nue"] = 211.817;
        Numerator_rhc["numubar_x_numubar"] = 338.497;
        Numerator_rhc["numubar_x_nuebar"] = 357.710;
        Numerator_rhc["nue_x_nue"] = 8.793;
        Numerator_rhc["nuebar_x_nuebar"] = 6.522;

        Denominator_rhc["numu_x_numu"] = 634250;
        Denominator_rhc["numu_x_nue"] = 126923;
        Denominator_rhc["numubar_x_numubar"] = 634817;
        Denominator_rhc["numubar_x_nuebar"] = 126928;
        Denominator_rhc["nue_x_nue"] = 126829;
        Denominator_rhc["nuebar_x_nuebar"] = 126982;

        TString OscCh;
        TString mode;
        TObjArray* filename_tokens = filename.Tokenize(".");
        OscCh = ((TObjString *) filename_tokens->At(filename_tokens->GetEntries()-2))->GetString();
        mode = ((TObjString *) filename_tokens->At(filename_tokens->GetEntries()-3))->GetString();
        if(mode.EqualTo("fhc")){
            norm = Numerator_fhc[OscCh.Data()]/Denominator_fhc[OscCh.Data()];
        }else if(mode.EqualTo("rhc")){
            norm = Numerator_rhc[OscCh.Data()]/Denominator_rhc[OscCh.Data()];
        }else{
            std::cerr << "Unable to identify beammode from filename. Parsed member was " << mode.Data() << "." << std::endl;
            return norm;
        }
        return norm;
    }
    else if(!strcmp(prod_version.Data(), "t2kmc23c")){

        //std::cout << "Loading MC Normalizations for t2kmc 23c" << std::endl;
        Double_t norm = 1.0;

        std::map<std::string, Double_t> Numerator_fhc;
        std::map<std::string, Double_t> Denominator_fhc;
        std::map<std::string, Double_t> Numerator_rhc;
        std::map<std::string, Double_t> Denominator_rhc;
        /*
        Numerator_fhc["numu_x_numu"] = 1497.03;
        Numerator_fhc["numu_x_nue"] = 1585.2;
        Numerator_fhc["numubar_x_numubar"] = 53.1443;
        Numerator_fhc["numubar_x_nuebar"] = 55.0006;
        Numerator_fhc["nue_x_nue"] = 27.9482;
        Numerator_fhc["nuebar_x_nuebar"] = 2.93363;


        Denominator_fhc["numu_x_numu"] = 698441;
        Denominator_fhc["numu_x_nue"] = 140181;
        Denominator_fhc["numubar_x_numubar"] = 697254;
        Denominator_fhc["numubar_x_nuebar"] = 139642;
        Denominator_fhc["nue_x_nue"] = 139776;
        Denominator_fhc["nuebar_x_nuebar"] = 139519;


        Numerator_rhc["numu_x_numu"] = 1;
        Numerator_rhc["numu_x_nue"] = 1;
        Numerator_rhc["numubar_x_numubar"] = 1;
        Numerator_rhc["numubar_x_nuebar"] = 1;
        Numerator_rhc["nue_x_nue"] = 1;
        Numerator_rhc["nuebar_x_nuebar"] = 1;


        Denominator_rhc["numu_x_numu"] = 1;
        Denominator_rhc["numu_x_nue"] = 1;
        Denominator_rhc["numubar_x_numubar"] = 1;
        Denominator_rhc["numubar_x_nuebar"] = 1;
        Denominator_rhc["nue_x_nue"] = 1;
        Denominator_rhc["nuebar_x_nuebar"] = 1;
        */


        Numerator_fhc["numu_x_numu"] = 1497.03;
        Numerator_fhc["numu_x_nue"] = 1585.2;
        Numerator_fhc["numubar_x_numubar"] = 53.1443;
        Numerator_fhc["numubar_x_nuebar"] = 55.0006;
        Numerator_fhc["nue_x_nue"] = 27.9482;
        Numerator_fhc["nuebar_x_nuebar"] = 2.93363;


        Denominator_fhc["numu_x_numu"] = 63570;
        Denominator_fhc["numu_x_nue"] = 12721;
        Denominator_fhc["numubar_x_numubar"] = 63473;
        Denominator_fhc["numubar_x_nuebar"] = 12662;
        Denominator_fhc["nue_x_nue"] = 12674;
        Denominator_fhc["nuebar_x_nuebar"] = 12817;


        Numerator_rhc["numu_x_numu"] = 1;
        Numerator_rhc["numu_x_nue"] = 1;
        Numerator_rhc["numubar_x_numubar"] = 1;
        Numerator_rhc["numubar_x_nuebar"] = 1;
        Numerator_rhc["nue_x_nue"] = 1;
        Numerator_rhc["nuebar_x_nuebar"] = 1;


        Denominator_rhc["numu_x_numu"] = 1;
        Denominator_rhc["numu_x_nue"] = 1;
        Denominator_rhc["numubar_x_numubar"] = 1;
        Denominator_rhc["numubar_x_nuebar"] = 1;
        Denominator_rhc["nue_x_nue"] = 1;
        Denominator_rhc["nuebar_x_nuebar"] = 1;

        TString OscCh;
        TString mode;
        TObjArray* filename_tokens = filename.Tokenize(".");
        OscCh = ((TObjString *) filename_tokens->At(filename_tokens->GetEntries()-2))->GetString();
        mode = ((TObjString *) filename_tokens->At(filename_tokens->GetEntries()-3))->GetString();
        if(mode.EqualTo("fhc")){
            norm = Numerator_fhc[OscCh.Data()]/Denominator_fhc[OscCh.Data()];
            return norm;
        }else if(mode.EqualTo("rhc")){
            norm = Numerator_rhc[OscCh.Data()]/Denominator_rhc[OscCh.Data()];
            return norm;
        }else{
            std::cerr << "Unable to identify beammode from filename. Parsed member was " << mode.Data() << "." << std::endl;
            return norm;
        }
    }
    else if(!strcmp(prod_version.Data(), "skCoil")){

        //std::cout << "Loading MC Normalizations for t2kmc 23c" << std::endl;
        Double_t norm = 1.0;

        std::map<std::string, Double_t> Numerator_fhc;
        std::map<std::string, Double_t> Denominator_fhc;
        std::map<std::string, Double_t> Numerator_rhc;
        std::map<std::string, Double_t> Denominator_rhc;

        Numerator_fhc["numu_x_numu"] = 1497.03;
        Numerator_fhc["numu_x_nue"] = 1585.2;
        Numerator_fhc["numubar_x_numubar"] = 53.1443;
        Numerator_fhc["numubar_x_nuebar"] = 55.0006;
        Numerator_fhc["nue_x_nue"] = 27.9482;
        Numerator_fhc["nuebar_x_nuebar"] = 2.93363;


        Denominator_fhc["numu_x_numu"] = 63570;
        Denominator_fhc["numu_x_nue"] = 12721;
        Denominator_fhc["numubar_x_numubar"] = 63473;
        Denominator_fhc["numubar_x_nuebar"] = 12662;
        Denominator_fhc["nue_x_nue"] = 12674;
        Denominator_fhc["nuebar_x_nuebar"] = 12817;


        Numerator_rhc["numu_x_numu"] = 1;
        Numerator_rhc["numu_x_nue"] = 1;
        Numerator_rhc["numubar_x_numubar"] = 1;
        Numerator_rhc["numubar_x_nuebar"] = 1;
        Numerator_rhc["nue_x_nue"] = 1;
        Numerator_rhc["nuebar_x_nuebar"] = 1;


        Denominator_rhc["numu_x_numu"] = 1;
        Denominator_rhc["numu_x_nue"] = 1;
        Denominator_rhc["numubar_x_numubar"] = 1;
        Denominator_rhc["numubar_x_nuebar"] = 1;
        Denominator_rhc["nue_x_nue"] = 1;
        Denominator_rhc["nuebar_x_nuebar"] = 1;

        TString OscCh;
        TString mode;
        TObjArray* filename_tokens = filename.Tokenize(".");
        OscCh = ((TObjString *) filename_tokens->At(filename_tokens->GetEntries()-2))->GetString();
        mode = ((TObjString *) filename_tokens->At(filename_tokens->GetEntries()-3))->GetString();
        if(mode.EqualTo("fhc")){
            norm = Numerator_fhc[OscCh.Data()]/Denominator_fhc[OscCh.Data()];
            return norm;
        }else if(mode.EqualTo("rhc")){
            norm = Numerator_rhc[OscCh.Data()]/Denominator_rhc[OscCh.Data()];
            return norm;
        }else{
            std::cerr << "Unable to identify beammode from filename. Parsed member was " << mode.Data() << "." << std::endl;
            return norm;
        }
    }
    else{
        std::cerr << "Unable to identify production value from filename. Parsed member was " << prod_version.Data() << "." << std::endl;
        return 1.0;
    }
}

Double_t getMCNormalization_v2(TString filename, TString prod_version){
    Double_t  norm = 1.0;
    TString normalization_directory = TString::Format("/home/tschefke/OA2024_eventSelection/MCNormalizations/%s", prod_version.Data());

    TString OscCh;
    TString mode;
    TObjArray* filename_tokens = filename.Tokenize(".");
    OscCh = ((TObjString *) filename_tokens->At(filename_tokens->GetEntries()-2))->GetString();
    mode = ((TObjString *) filename_tokens->At(filename_tokens->GetEntries()-3))->GetString();

    TString mcNorm_file = TString::Format("%s/%s/MCNormalizations_%s.txt", normalization_directory.Data(), mode.Data(), OscCh.Data());

    std::ifstream infile(mcNorm_file.Data());
    std::string line;
    if(infile.is_open()){
        while(std::getline(infile, line)){
            std::istringstream iss(line);
            std::string str, number_str, den_str, num_str;
            Double_t Numerator, Denominator;

            if(iss >> str >> number_str){

                size_t pos;
                if((pos = number_str.find('/')) != std::string::npos){
                    den_str = number_str.substr(pos+1);
                    num_str = number_str.substr(0, pos);
                }
                Numerator = std::stod(num_str);
                Denominator = std::stod(den_str);
                norm = Numerator/Denominator;
            }
        }
    }else{
        std::cerr << "Cannot find Normalization file requested: " << mcNorm_file << std::endl;
    }
    infile.close();

    return norm;
}

//Cut drawing methods pulled from highland2/highlandTools (https://git.t2k.org/nd280/highland2Software/highland2/highlandTools)
void DrawCutArrow(double xmin, double ymin, double xmax, double ymax) {
    TArrow* arr = new TArrow(xmin, ymin, xmax, ymax, 0.015, "|>");
    arr->SetLineColor(_cut_line_col);
    arr->SetFillColor(_cut_line_col);
    arr->SetLineWidth(_cut_line_width);
    arr->SetLineStyle(_cut_line_style);
    arr->Draw();
}

void DrawCutLine(double xmin, double ymin, double xmax, double ymax) {
    //*********************************************************
    TLine* l = new TLine(xmin, ymin, xmax, ymax);

    l->SetLineColor(_cut_line_col);
    l->SetLineWidth(_cut_line_width);
    l->SetLineStyle(_cut_line_style);
    l->Draw();
}

void DrawCutLineVertical(double xval, bool addarrow, std::string arrowopt, double arrowpos) {

    double ymin = gPad->PadtoY(gPad->GetUymin());
    double ymax = arrowpos * (gPad->PadtoY(gPad->GetUymax()));
    DrawCutLine(xval, ymin, xval, ymax);

    if (addarrow) {
        std::string uarrowdir = ToUpper(arrowopt);
        double deltax = xval-gPad->PadtoX( gPad->XtoPad(xval)-(gPad->GetUxmax()-gPad->GetUxmin())/20.);

        if (uarrowdir.find("L") != std::string::npos) {
            deltax *= -1;
        }

        // It needs this to work in LogY scale
        double yarr = gPad->PadtoY((gPad->GetUymax()-gPad->GetUymin())*arrowpos+gPad->GetUymin());
        double xarrmax = xval + deltax;
        DrawCutArrow(xval, yarr, xarrmax, yarr);
    }
}

void CenterHistoTitles(TH1 *thisHisto){
    thisHisto->GetXaxis()->CenterTitle();
    thisHisto->GetYaxis()->CenterTitle();
    thisHisto->GetZaxis()->CenterTitle();
}

void quicktitle(TNamed *thisObj){
    const char * titletext = thisObj->GetTitle();
    //std::cout << titletext << std::endl;
    TPaveText *title = new TPaveText(0.4, 0.9, 0.9, 0.95, "rNDC");
    title->AddText(titletext);
    title->SetTextFont(42);
    title->SetTextSize(0.02);
    title->SetFillColorAlpha(19, 0.9);
    title->SetTextAlign(32);
    title->Draw();
}

int AddGridLinesToPad(TPad *thisPad) {
    thisPad->SetGridx();
    thisPad->SetGridy();
    return 0;
}

void plotMean(TH1 *h1, bool add_legend=true){
    Float_t meanval = h1->GetMean();
    double ymin = 0.75*gPad->PadtoY(gPad->GetUymin());
    double ymax = h1->GetBinContent(h1->GetMaximumBin())+0.1;
    TLine *line = new TLine(meanval, ymin, meanval, ymax);
    line->SetLineColor(2);
    line->SetLineWidth(_cut_line_width);
    line->SetLineStyle(_cut_line_style);
    line->Draw();
    if(add_legend){
        TLegend * leg = new TLegend(0.11, 0.65, 0.51, 0.85);
        TString str = TString::Format("Mean: %f", meanval);
        leg->AddEntry(h1, TString::Format("Events passing selection: %.0f", h1->GetEntries()).Data());
        leg->AddEntry(line, str.Data(), "lf");
        leg->SetBorderSize(1);
        leg->SetTextSize(0.02);
        leg->SetFillColorAlpha(19, 0.9);
        leg->Draw();
    }
}

void drawBinEdges(TH1 * h1){

    for(int i = 0; i <= h1->GetNbinsX()+1; ++i){
        double binEdge = h1->GetBinLowEdge(i);
        Double_t minY = h1->GetYaxis()->GetBinLowEdge(h1->GetYaxis()->GetFirst());

        TLine *divider = new TLine(binEdge, minY, binEdge, h1->GetBinContent(i));
        divider->SetLineColor(1);
        divider->SetLineWidth(2);
        divider->Draw("same");
    }
}

void quicksave(TH1 * h1, const char * name, bool drawTitle=false, bool drawMean=false, bool binEdges=false, bool writeToRoot=false){

    tmp->cd();
    tmp->SetBottomMargin(0.12);
    tmp->SetLeftMargin(0.2);
    tmp->SetRightMargin(0.035);
    tmp->SetGrid();
    tmp->SetTitle("");
    h1->GetXaxis()->SetTitleOffset(1.05);
    h1->GetYaxis()->SetTitleOffset(2);
    h1->Draw("hist");
    if(drawTitle) quicktitle(h1);
    if (drawMean) plotMean(h1, true);
    if(binEdges) drawBinEdges(h1);
    AddGridLinesToPad(tmp);
    if(writeToRoot) tmp->Write();
    tmp->SaveAs(name);
    tmp->Clear();
    h1->Clear();
}

void quicksave(TGraph * g1, const char * name, bool drawTitle=false){
    tmp->cd();
    tmp->SetBottomMargin(0.15);
    tmp->SetGrid();
    g1->SetStats(kFALSE);
    g1->Draw("AP");
    if(drawTitle){
        //Try to supppress any existing title in graph
        //g1->SetTitle("");
        quicktitle(g1);
    }
    AddGridLinesToPad(tmp);
    tmp->SaveAs(name);
    tmp->Clear();
    g1->Clear();
}

void quicksave(TH2 * h2, const char * name, bool drawTitle=false){
    tmp->cd();
    h2->Draw("colz");
    if(drawTitle) quicktitle(h2);
    tmp->SaveAs(name);
    tmp->Clear();
    h2->Clear();
}

void quicksave(THStack *hs, const char *name, bool drawTitle=false){
    tmp->cd();
    tmp->SetBottomMargin(0.12);
    tmp->SetLeftMargin(0.2);
    tmp->SetRightMargin(0.035);
    tmp->SetGrid();
    tmp->SetTitle("");
   // hs->GetXaxis()->SetTitleOffset(1.05);
    //hs->GetYaxis()->SetTitleOffset(2);
    hs->Draw("hist");
    if(drawTitle) quicktitle(hs);
    AddGridLinesToPad(tmp);
    tmp->SaveAs(name);
    tmp->Clear();
    hs->Clear();
}

void checkDenominator(TH1 * h1){
    for(Int_t i = 0; i < h1->GetNbinsX(); i++){
        Double_t t = h1->GetBinContent(i);
        h1->SetBinContent(i, t+1e-12);
    }
}

#endif //EVENTSELECTION_TOOLS_H
