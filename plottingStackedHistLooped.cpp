#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TBrowser.h"
#include "TH2.h"
#include "TRandom.h"
#include "string"
#include "TDirectory.h"
#include "TCanvas.h"
#include "THStack.h"
#include "TStyle.h"


#include <iostream>
#include <cmath>
#include <vector>


void plotStackedHistLooped (){
    TDirectory *curdir = gDirectory;
    TString runType;

    THStack *hs = new THStack("hs", "");
    TH1F *histDY;
    TH1F *histWW;
    TH1F *histTT;
    TCanvas canvasForEE ("Canvas for elec elec");
    TCanvas canvasForMuE ("Canvas for mu elec");
    TCanvas canvasForMuMu ("Canvas for mu mu");


    TFile *inDY = TFile::Open("/home/ananda/Documents/UCSB/Research_Particle_Phys/afiles/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8-Result/hist.root","read");
    TFile *inWW = TFile::Open("/home/ananda/Documents/UCSB/Research_Particle_Phys/afiles/WWTo2L2Nu_TuneCP5_13TeV-powheg-pythia8-Result/hist.root","read");
    TFile *inTT = TFile::Open("/home/ananda/Documents/UCSB/Research_Particle_Phys/afiles/TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8-Result/hist.root","read");

    gStyle -> SetOptStat(0);

    hs ->SetTitle("");
//    hs->GetYaxis()->SetTitle("Events");
    /******************************************************************************************************************/
    //for electron
    for (int i = 0; i < 3; i++)
    {
        switch (i) {
            case 0:
                runType = "hpxEE";
                break;
            case 1:
                runType = "hpxEMu";
                break;
            case 2:
                runType = "hpxMuMu";
                break;
            default:
                break;
        }
        inDY -> cd();
        histDY = (TH1F*)inDY -> Get(runType);
        if (!histDY)
            std::cout << "Dy hist DNE" << std::endl;
        curdir -> cd ();

        inWW -> cd();
        histWW = (TH1F*)inWW -> Get(runType);
        if (!histWW)
            std::cout << "WW hist DNE" << std::endl;
        curdir -> cd();

        inTT -> cd();
        histTT = (TH1F*)inTT -> Get(runType);
        if (!histTT)
            std::cout << "TT hist DNE" << std::endl;
        curdir -> cd();

        histTT ->SetFillColor(kRed);
        histWW ->SetFillColor(kBlue);
        histDY -> SetFillColor(kGreen);

//    hs->GetXaxis()->SetTitle("m_{ee}");;

        hs -> Add(histDY);
        hs -> Add(histWW);
        hs -> Add(histTT);

        canvasForEE.cd();
        hs -> Draw("hist");
        canvasForEE.SaveAs(runType+".png");

        hs -> RecursiveRemove(histDY);
        hs -> RecursiveRemove(histWW);
        hs -> RecursiveRemove(histTT);
        curdir -> cd();
    }

    delete hs;

    inDY -> Close();
    inWW -> Close();
    inTT -> Close();
}