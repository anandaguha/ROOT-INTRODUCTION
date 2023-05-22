//
// Created by ananda on 5/1/23.
//
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
#include "TLegend.h"


#include <iostream>
#include <cmath>
#include <vector>


void plotStackedHistGenLoop (){
    TDirectory *curdir = gDirectory;
    TString runType;

    THStack *hs = new THStack("hs", "");
    TH1F *histDY;
    TH1F *histWW;
    TH1F *histTT;

    TLegend* legend = new TLegend(0.7,0.7,0.9,0.9);

    TFile *inDY = TFile::Open("/home/ananda/Documents/UCSB/Research_Particle_Phys/afiles/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8Jet.root","read");
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
                runType = "Jet";
                legend->SetHeader("Number of Good BJets","C"); // option "C" allows to center the header
                break;
            case 1:
                runType = "MET";
                legend->SetHeader("Missing Transverse Energy","C"); // option "C" allows to center the header
                break;
            case 2:
                runType = "MLB";
                legend->SetHeader("Minimum Lepton BJet Pairing","C"); // option "C" allows to center the header
                break;
            default:
                break;
        }
        TCanvas canvas (runType+"Canvas");
        TFile *inDY = TFile::Open("/home/ananda/Documents/UCSB/Research_Particle_Phys/afiles/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8"+runType+".root","read");
        TFile *inWW = TFile::Open("/home/ananda/Documents/UCSB/Research_Particle_Phys/afiles/WWTo2L2Nu_TuneCP5_13TeV-powheg-pythia8"+runType+".root","read");
        TFile *inTT = TFile::Open("/home/ananda/Documents/UCSB/Research_Particle_Phys/afiles/TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8"+runType+".root","read");
        inDY -> cd();
        histDY = (TH1F*)inDY -> Get("bJetCounting" + runType);
        if (!histDY)
            std::cout << "Dy hist DNE" << std::endl;
        curdir -> cd ();

        inWW -> cd();
        histWW = (TH1F*)inWW -> Get("bJetCounting" + runType);
        if (!histWW)
            std::cout << "WW hist DNE" << std::endl;
        curdir -> cd();

        inTT -> cd();
        histTT = (TH1F*)inTT -> Get("bJetCounting" + runType);
        if (!histTT)
            std::cout << "TT hist DNE" << std::endl;
        curdir -> cd();



//    hs->GetXaxis()->SetTitle("m_{ee}");;


        hs -> Add(histDY);
        hs -> Add(histWW);
        hs -> Add(histTT);

        canvas.cd();

        canvas.SetLogy();
        hs -> Draw("hist");
        histTT ->SetFillColor(kRed);
        histWW ->SetFillColor(kBlue);
        histDY -> SetFillColor(kGreen);
        if(i == 0)
        {
            legend->AddEntry(histDY,"Drell Yan","f");
            legend->AddEntry(histWW,"WW","f");
            legend->AddEntry(histTT,"TT","f");
        }

        legend->Draw();
        canvas.SaveAs(runType+"Log.png");

        hs -> RecursiveRemove(histDY);
        hs -> RecursiveRemove(histWW);
        hs -> RecursiveRemove(histTT);
        curdir -> cd();
    }

    delete hs;
    delete legend;

    inDY -> Close();
    inWW -> Close();
    inTT -> Close();
}