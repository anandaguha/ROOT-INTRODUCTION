//
// Created by ananda on 8/10/22.
//
#include "TROOT.h"
#include "TFile.h"
#include "TChain.h"
#include "TTree.h"
#include "TBrowser.h"
#include "TH2.h"
#include "TRandom.h"
#include "string"
#include "TCanvas.h"
#include "TStyle.h"

#include <iostream>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <vector>

void stackHist(const TString& folderName)
{
    //setBranches to these vars
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
    UInt_t numberOfMunons;
    Float_t genWeight;
    Float_t genWeightSum;

    //cross-sectional areas that might change
    Float_t CrossSection;
//    UInt_t DYCross = 6225.4;
//    UInt_t TTCross = 87.3348;
//    UInt_t WWCross = 11.08;


    std::vector<Long64_t> Energy;
    std::vector<Long64_t> px;
    std::vector<Long64_t> py;
    std::vector<Long64_t> pz;

    int countelec = 0;
    int countmu = 0;

    TString constantString = "/home/ananda/Documents/UCSB/Research_Particle_Phys/afiles/";

    TH1F *histEE = new TH1F ("hpxEE","",100,0,100);
    TH1F *histEMu = new TH1F ("hpxEMu","",100,0,100);
    TH1F *histMuMu = new TH1F ("hpxMuMu","",100,0,100);

    TFile *result = TFile::Open(constantString + folderName + "-Result/hist.root", "recreate"); //not sure


    TChain *mainTree = new TChain("Events");

    mainTree -> Add(constantString + folderName + "/*.root");

    if (folderName == "DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8" ){
        CrossSection = 6225.4;
    }
    else if (folderName == "TTTo2L2Nu_TuneCP5_13TeV-powheg-pythia8"){
        CrossSection = 87.3348;
    }
    else if (folderName == "WWTo2L2Nu_TuneCP5_13TeV-powheg-pythia8"){
        CrossSection = 11.08;
    }


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

    for (UInt_t event = 0; event < events; event++){
        mainTree ->GetEntry(event);
        if (numberOfElec + numberOfMunons >= 2){
            countelec = 0;
            countmu =0;
            for (UInt_t i = 0; i < numberOfMunons; i++){
                if (pT_mu[i] > 25 && id_mu && abs(eta_mu[i]) < 2.4 && relEnergy_mu[i] < 0.15){

                    px.push_back(pT_mu[i] * cos(phi_mu[i]));
                    py.push_back(pT_mu[i] * sin(phi_mu[i]));
                    pz.push_back(pT_mu[i] * sinh(eta_mu[i]));
                    Energy.push_back(sqrt(pow(px.back(), 2) + pow(py.back(), 2) + pow(pz.back(), 2) + pow(mass_mu[i], 2)));
                    countmu++;
                }

            }
            for (UInt_t i = 0; i < numberOfElec; i++){
                if (pT_elec[i] > 25 && id_elec && abs(eta_elec[i]) < 2.4 && relEnergy_elec[i] < 0.15){

                    px.push_back(pT_elec[i] * cos(phi_elec[i]));
                    py.push_back(pT_elec[i] * sin(phi_elec[i]));
                    pz.push_back(pT_elec[i] * sinh(eta_elec[i]));
                    Energy.push_back(sqrt(pow(px.back(), 2) + pow(py.back(), 2) + pow(pz.back(), 2) + pow(mass_elec[i], 2)));
                    countelec++;
                }
            }
            if(countelec + countmu == 2)
            {
                if (countelec == 1){
                    //write to elec mu hist
                    std::cout <<  sqrt(pow( accumulate(Energy.begin(), Energy.end(), 0.0),2 ) - ( pow(accumulate(px.begin(), px.end(),0.0),2) + pow(accumulate(py.begin(), py.end(), 0.0),2) + pow(accumulate(pz.begin(), pz.end(), 0.0),2)) )  << std::endl;
                    histEMu -> Fill( sqrt(pow( accumulate(Energy.begin(), Energy.end(), 0.0),2 ) - ( pow(accumulate(px.begin(), px.end(),0.0),2) + pow(accumulate(py.begin(), py.end(), 0.0),2) + pow(accumulate(pz.begin(), pz.end(), 0.0),2)) ), genWeight );
                }
                else if (countmu == 2){
                    //write to mumu hist
                    std::cout <<  sqrt(pow( accumulate(Energy.begin(), Energy.end(), 0.0),2 ) - ( pow(accumulate(px.begin(), px.end(),0.0),2) + pow(accumulate(py.begin(), py.end(), 0.0),2) + pow(accumulate(pz.begin(), pz.end(), 0.0),2)) )  << std::endl;
                    histMuMu -> Fill( sqrt(pow( accumulate(Energy.begin(), Energy.end(), 0.0),2 ) - ( pow(accumulate(px.begin(), px.end(),0.0),2) + pow(accumulate(py.begin(), py.end(), 0.0),2) + pow(accumulate(pz.begin(), pz.end(), 0.0),2)) ),genWeight );
                }
                else
                {
                    //write to ee hist
                    std::cout <<  sqrt(pow( accumulate(Energy.begin(), Energy.end(), 0.0),2 ) - ( pow(accumulate(px.begin(), px.end(),0.0),2) + pow(accumulate(py.begin(), py.end(), 0.0),2) + pow(accumulate(pz.begin(), pz.end(), 0.0),2)) )  << std::endl;
                    histEE -> Fill( sqrt(pow( accumulate(Energy.begin(), Energy.end(), 0.0),2 ) - ( pow(accumulate(px.begin(), px.end(),0.0),2) + pow(accumulate(py.begin(), py.end(), 0.0),2) + pow(accumulate(pz.begin(), pz.end(), 0.0),2)) ),genWeight );
                }
                genWeightSum += genWeight;
            }
        px.clear();
        py.clear();
        pz.clear();
        Energy.clear();
        }//end of looking at good event
    }//end looking at all events


    histEE -> Scale(CrossSection/genWeightSum);
    histEMu -> Scale(CrossSection/genWeightSum);
    histMuMu -> Scale(CrossSection/genWeightSum);

    result ->WriteTObject(histEE);
    result ->WriteTObject(histEMu);
    result ->WriteTObject(histMuMu);

    delete histMuMu;
    delete histEMu;
    delete histEE;
    delete mainTree;
    result -> Close();

} //end read tree