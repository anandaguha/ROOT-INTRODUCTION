//
// Created by ananda on 2/21/23.
//
#define _USE_MATH_DEFINES

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
#include <cmath>
#include <iostream>

void jetCounting (){
    //setBranches to these vars
    UInt_t numberOfMunons;
    Float_t phi_mu[50];
    Float_t eta_mu[50];
    Float_t pT_mu[50];
    Float_t mass_mu[50];
    Bool_t id_mu[50];
    Float_t relEnergy_mu[50];
    Int_t sign_mu[50];

    UInt_t numberOfElec;
    Float_t phi_elec[50];
    Float_t eta_elec[50];
    Float_t pT_elec[50];
    Float_t mass_elec[50];
    Bool_t id_elec[50];
    Float_t relEnergy_elec[50];
    Int_t sign_elec[50];

    UInt_t numberOfJets;
    Float_t jet_Eta[50];
    Float_t jet_Pt[50];
    Float_t jet_Phi[50];
    Float_t jet_btagDeepFlavB[50];
    Int_t jetId[50];

    Float_t genWeight;
    Float_t genWeightSum;

    //cross-sectional areas that might change
    Float_t CrossSection;
//    UInt_t DYCross = 6225.4;
//    UInt_t TTCross = 87.3348;
//    UInt_t WWCross = 11.08;

    Int_t countJets = 0;
    Int_t countLeptons = 0;
    Float_t delR = 0;

    TDirectory* curDir= gDirectory;

    //file stuff
    const char* filePathInput = "/home/ananda/Documents/UCSB/Research_Particle_Phys/afiles/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8/dy_0.root";
    TString constantString = "/home/ananda/Documents/UCSB/Research_Particle_Phys/afiles/";

//    TFile *result = TFile::Open(constantString + folderName + "-Result/jetHist.root", "recreate"); //not sure
    TFile *fileIn = TFile::Open(filePathInput,"read");


    TH1F *histJets = new TH1F("hpxJet","",100,-2,100);
    TCanvas can ("JetFromDy");


//    TChain *mainTree = new TChain("Events");
//
//    mainTree -> Add(constantString + folderName + "/*.root");

//    if (folderName == "DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8" ){
//        CrossSection = 6225.4;
//    }
//    else if (folderName == "TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8"){
//        CrossSection = 87.3348;
//    }
//    else if (folderName == "WWTo2L2Nu_TuneCP5_13TeV-powheg-pythia8"){
//        CrossSection = 11.08;
//    }

    TTree *mainTree = (TTree*)fileIn->Get("Events");
    UInt_t events = mainTree -> GetEntries();

    mainTree->SetBranchAddress("Muon_phi", phi_mu);                     //setting branch adresses for muon
    mainTree->SetBranchAddress("Muon_eta", eta_mu);                     //setting branch adresses for muon
    mainTree->SetBranchAddress("Muon_pt", pT_mu);                       //setting branch adresses for muon
    mainTree->SetBranchAddress("Muon_mass", mass_mu);                   //setting branch adresses for muon
    mainTree ->SetBranchAddress("Muon_mediumPromptId",id_mu);          //setting branch adresses for muon
    mainTree ->SetBranchAddress("Muon_pfRelIso03_all", relEnergy_mu);   //setting branch adresses for muon
    mainTree->SetBranchAddress("nMuon", &numberOfMunons);               //setting branch adresses for muon

    mainTree->SetBranchAddress("Electron_phi", phi_elec);               //setting branch adresses for electron
    mainTree->SetBranchAddress("Electron_eta", eta_elec);               //setting branch adresses for electron
    mainTree->SetBranchAddress("Electron_pt", pT_elec);                 //setting branch adresses for electron
    mainTree ->SetBranchAddress("Electron_mvaFall17V2noIso_WP90",id_elec);       //setting branch adresses for electron
    mainTree ->SetBranchAddress("Electron_pfRelIso03_all", relEnergy_elec); //setting branch adresses for electron
    mainTree->SetBranchAddress("Electron_mass", mass_elec);             //setting branch adresses for electron
    mainTree->SetBranchAddress("nElectron", &numberOfElec);                 //setting branch adresses for electron
    mainTree->SetBranchAddress("genWeight", &genWeight);

    mainTree -> SetBranchAddress("Jet_jetId", jetId);
    mainTree -> SetBranchAddress("Jet_eta", jet_Eta);
    mainTree -> SetBranchAddress("Jet_pt", jet_Pt);
    mainTree ->SetBranchAddress("Jet_phi",jet_Phi);
    mainTree ->SetBranchAddress("Jet_btagDeepFlavB",jet_btagDeepFlavB);
    mainTree -> SetBranchAddress("nJet", &numberOfJets);

    for (UInt_t event = 0; event < events; event++){
        mainTree ->GetEntry(event);
        if (numberOfElec + numberOfMunons >= 2){
            countLeptons = 0;
            countJets = 0;
            for (UInt_t i = 0; i < numberOfMunons; i++){
                if (pT_mu[i] > 25 && id_mu[i] && abs(eta_mu[i]) < 2.4 && relEnergy_mu[i] < 0.15){
                    countLeptons++;
                }

            }
            for (UInt_t i = 0; i < numberOfElec; i++){
                if (pT_elec[i] > 25 && id_elec[i] && abs(eta_elec[i]) < 2.4 && relEnergy_elec[i] < 0.15){
                    countLeptons++;
                }
            }
            if(countLeptons == 2)
            {
                for (UInt_t i = 0; i < numberOfJets; i++){
                    while (abs(jet_Phi[i]) > M_PI)
                    {
                        if (jet_Phi[i] < 0)
                            jet_Phi[i] += 2*M_PI;
                        else
                            jet_Phi[i] -= 2*M_PI;
                    }
                    if(jetId[i]>>1 & 1 && abs(jet_Eta[i]) <= 2.5 && jet_Pt[i] >= 30 && sqrt(pow(jet_Phi[i],2) + pow(jet_Eta[i],2) ) >= 0.4 ){
                        countJets++;
                    }
                }
                histJets ->  Fill(countJets);
            }
        }//end of looking at good event
    }//end looking at all events
    can.cd();
    histJets ->Draw("hist");
    can.SaveAs("JetFromDy.png");
    curDir -> cd();

//    histEE -> Scale(CrossSection/genWeightSum);
//    histEMu -> Scale(CrossSection/genWeightSum);
//    histMuMu -> Scale(CrossSection/genWeightSum);
//
//    result ->WriteTObject(histEE);
//    result ->WriteTObject(histEMu);
//    result ->WriteTObject(histMuMu);
//
//    delete histMuMu;
//    delete histEMu;
//    delete histEE;
    delete histJets;
    delete mainTree;
//    result -> Close();
}