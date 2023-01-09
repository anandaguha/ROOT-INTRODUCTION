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


void plotStackedHist (){
    TDirectory *curdir = gDirectory;

//    THStack *hs = new THStack("hs", "");
    TH1F *histDY;
    TH1F *histWW;
    TH1F *histTT;
    TCanvas canvasForEE ("Canvas for elec elec");
//    TCanvas canvasForMuE ("Canvas for mu elec");
//    TCanvas canvasForMuMu ("Canvas for mu mu");


    TFile *inDY = TFile::Open("/home/ananda/Documents/UCSB/Research_Particle_Phys/afiles/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8-Result/hist.root","read");
    TFile *inWW = TFile::Open("/home/ananda/Documents/UCSB/Research_Particle_Phys/afiles/WWTo2L2Nu_TuneCP5_13TeV-powheg-pythia8-Result/hist.root","read");
    TFile *inTT = TFile::Open("/home/ananda/Documents/UCSB/Research_Particle_Phys/afiles/TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8-Result/hist.root","read");

    gStyle -> SetOptStat(0);

//    hs ->SetTitle("");
//    hs->GetYaxis()->SetTitle("Events");
    /******************************************************************************************************************/
    //for electron
    inDY -> cd();
    histDY = (TH1F*)inDY -> Get("hpxEE");
    if (!histDY)
        std::cout << "Dy hist DNE" << std::endl;
    curdir -> cd ();

    inWW -> cd();
    histWW = (TH1F*)inWW -> Get("hpxEE");
    if (!histWW)
        std::cout << "WW hist DNE" << std::endl;
    curdir -> cd();

    inTT -> cd();
    histTT = (TH1F*)inTT -> Get("hpxEE");
    if (!histTT)
        std::cout << "TT hist DNE" << std::endl;
    curdir -> cd();

    histTT ->SetFillColor(kRed);
    histWW ->SetFillColor(kBlue);
    histDY -> SetFillColor(kGreen);

//    hs->GetXaxis()->SetTitle("m_{ee}");;

//    hs -> Add(histDY);
//    hs -> Add(histWW);
//    hs -> Add(histTT);

    canvasForEE.cd();
    histDY -> Draw();
//    hs -> Draw();
//    hs -> RecursiveRemove(histDY);
//    hs -> RecursiveRemove(histWW);
//    hs -> RecursiveRemove(histTT);
    curdir -> cd();

    /******************************************************************************************************************/
 /*   //for mu e
    inDY -> cd();
    histDY = (TH1F*)inDY -> Get("hpxEMu");
    curdir -> cd ();

    inWW -> cd();
    histWW = (TH1F*)inWW -> Get("hpxEMu");
    curdir -> cd();

    inTT -> cd();
    histTT = (TH1F*)inTT -> Get("hpxEMu");
    curdir -> cd();

    hs->GetXaxis()->SetTitle("m_{/mu e}");
    histTT ->SetFillColor(kRed);
    histWW ->SetFillColor(kBlue);
    histDY -> SetFillColor(kGreen);

    hs -> Add(histDY);
    hs -> Add(histWW);
    hs -> Add(histTT);

    canvasForEE.cd();
    hs -> Draw();
    hs -> RecursiveRemove(histDY);
    hs -> RecursiveRemove(histWW);
    hs -> RecursiveRemove(histTT);
    curdir -> cd();
    /******************************************************************************************************************/
    //for mu mu
  /*  inDY -> cd();
    histDY = (TH1F*)inDY -> Get("hpxMuMu");
    curdir -> cd ();

    inWW -> cd();
    histWW = (TH1F*)inWW -> Get("hpxMuMu");
    curdir -> cd();

    inTT -> cd();
    histTT = (TH1F*)inTT -> Get("hpxMuMu");
    curdir -> cd();

    hs->GetXaxis()->SetTitle("m_{/mu e}");
    histTT ->SetFillColor(kRed);
    histWW ->SetFillColor(kBlue);
    histDY -> SetFillColor(kGreen);

    hs -> Add(histDY);
    hs -> Add(histWW);
    hs -> Add(histTT);

    canvasForEE.cd();
    hs -> Draw();
    hs -> RecursiveRemove(histDY);
    hs -> RecursiveRemove(histWW);
    hs -> RecursiveRemove(histTT);
    curdir -> cd();
    /******************************************************************************************************************/
    //should be implemented in a loop

    canvasForEE.SaveAs("EEStack.png");
//    canvasForMuE.SaveAs("MuEStack.png");
//    canvasForMuMu.SaveAs("MuMuStack.png");


//    delete hs;

    inDY -> Close();
    inWW -> Close();
    inTT -> Close();
}