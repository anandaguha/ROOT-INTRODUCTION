//
// Created by ananda on 2/21/23.
//
#define _USE_MATH_DEFINES

#include "TROOT.h"
#include "TLorentzVector.h"
#include "TFile.h"
#include "TTree.h"
#include "TBrowser.h"
#include "TChain.h"
#include "TH2.h"
#include "TH1.h"
#include "TNamed.h"
#include "TRandom.h"
#include "string"
#include "TDirectory.h"
#include "TCanvas.h"
#include "THStack.h"
#include "TStyle.h"

#include <Math/GenVector/LorentzVector.h>
#include <Math/Vector4D.h>
#include <vector>
#include <cmath>
#include <iostream>
void runningForLoop(const char *);

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

//    ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiMVector> Particles [50];
    std::vector<TLorentzVector> Leptons;
    TString typeRun = "bJetCounting"; //this should be changed when you do another run ie MET or Bjet etc.
    TDirectory* curDir= gDirectory;

    //file stuff
    const char* filePathInput = "/home/ananda/Documents/UCSB/Research_Particle_Phys/afiles/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8/dy_0.root";
    TString typeProcess;
    TString constantString = "/home/ananda/Documents/UCSB/Research_Particle_Phys/afiles/";

//    TFile *result = TFile::Open(constantString + folderName + "-Result/jetHist.root", "recreate"); //not sure
    TFile *fileIn = TFile::Open(filePathInput,"read");


    TH1F *histJets = new TH1F("hpxJet","",100,-2,100);
    TH1F *histMET =  new TH1F("hpxMet", "", 100,0,10);
//    TCanvas can ("JetFromDy");


    for (int q =0 ; q < 3; q++)
    {
        switch (q) {
            case 0:
                typeProcess = "DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8";
                histJets ->SetName(typeRun + "Jet");
                histMET -> SetName(typeRun + "MET");
                break;
            case 1:
                typeProcess = "TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8";
                histJets ->SetName(typeRun + "Jet");
                histMET -> SetName(typeRun + "MET");
                break;
            case 2:
                typeProcess = "WWTo2L2Nu_TuneCP5_13TeV-powheg-pythia8";
                histJets ->SetName(typeRun + "Jet");
                histMET -> SetName(typeRun + "MET");
                break;

        }
        TFile *resultFile = TFile::Open("/home/ananda/Documents/UCSB/Research_Particle_Phys/afiles/" + typeProcess + "Jet.root", "recreate"); //opens a file for the results of 1 process
        TFile *resultFileMET = TFile::Open("/home/ananda/Documents/UCSB/Research_Particle_Phys/afiles/" + typeProcess + "MET.root", "recreate");
        TChain *mainTree = new TChain("Events"); //creates the chain that will hold all the branches from 1 process
        mainTree -> Add(constantString + typeProcess + "/*.root"); //adds all the files for 1 process (ie dy*.root)


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
        std::cout << "Events: " << events << std::endl;
        for (UInt_t event = 0; event < events; event++){
            std::cout << "Got here"<< std::endl;
            mainTree ->GetEntry(event);
            if (numberOfElec + numberOfMunons >= 2){
                countLeptons = 0;
                countJets = 0;
                for (UInt_t i = 0; i < numberOfMunons; i++){
                    if (pT_mu[i] > 25 && id_mu[i] && abs(eta_mu[i]) < 2.4 && relEnergy_mu[i] < 0.15){
                        countLeptons++;
                        TLorentzVector v;
                        v.SetPtEtaPhiM(pT_mu[i],eta_mu[i],phi_mu[i],mass_mu[i]);
                        Leptons.push_back(v);
                    }

                }
                for (UInt_t i = 0; i < numberOfElec; i++){
                    if (pT_elec[i] > 25 && id_elec[i] && abs(eta_elec[i]) < 2.4 && relEnergy_elec[i] < 0.15){
                        countLeptons++;
                        TLorentzVector v;
                        v.SetPtEtaPhiM(pT_elec[i],eta_elec[i],phi_elec[i],mass_elec[i]);
                        Leptons.push_back(v);
                    }
                }
                if(countLeptons == 2)
                {
                    Float_t CountedTransE = 0;
                    for (UInt_t i = 0; i < numberOfJets; i++){
                        while (abs(jet_Phi[i]) > M_PI)
                        {
                            if (jet_Phi[i] < 0)
                                jet_Phi[i] += 2*M_PI;
                            else
                                jet_Phi[i] -= 2*M_PI;
                        }
                        if(jetId[i]>>1 & 1 && abs(jet_Eta[i]) <= 2.5 && jet_Pt[i] >= 30 && sqrt(pow(jet_Phi[i],2) + pow(jet_Eta[i],2) ) >= 0.4 && jet_btagDeepFlavB[i] > 0.2783  ){
                            countJets++;
                        }
                    }
                    if (countJets == 2){
                        for (auto i : Leptons){
                            CountedTransE += i.Et();
//                        histMET -> Fill();
                        }

                        histMET -> Fill(13000-CountedTransE);
                    }
                    histJets ->  Fill(countJets);
                }
            }//end of looking at good event
            Leptons.clear();
        }//end looking at all events
//        std::cout << "Priting" << typeProcess << std::endl;
        resultFile ->WriteTObject(histJets);
        resultFileMET ->WriteTObject(histMET);
        delete mainTree;
    }//end all three proceeses
    delete histJets;
    delete histMET;
    fileIn ->Close();

}//end main
void runningForLoop(const char* typeRun){

}