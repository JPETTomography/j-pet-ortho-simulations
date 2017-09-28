#ifndef PARTICLEGENERATOR_H
#define PARTICLEGENERATOR_H

#include <TLorentzVector.h>
#include <TRandom3.h>
#include "TGenPhaseSpace.h"
#include "event.h"
#include "parammanager.h"
#include <vector>
#include <iostream>
#include <typeinfo>

TLorentzVector* generateSingleGamma(double energy)
{
    if(energy==0)
        return nullptr;
    double theta = TMath::ACos(gRandom->Uniform(-1.0, 1.0));
    double phi = gRandom->Uniform(0.0, 2*TMath::Pi());
    double P = energy/1000.0; //GeV
    return new TLorentzVector(P*TMath::Sin(theta)*TMath::Cos(phi), P*TMath::Sin(theta)*TMath::Sin(phi), P*TMath::Cos(theta), P);
}



std::string recognizeType(const DecayType type, int& noOfGammas)
{
    std::string type_string;
    if(type==ONE)
    {
        type_string = "1";
        noOfGammas = 1;
    }
    else if(type==TWO)
    {
        type_string = "2";
        noOfGammas = 2;
    }
    else if(type==THREE)
    {
        type_string = "3";
        noOfGammas = 3;
    }
    else if(type==TWOandONE)
    {
        type_string = "2&1";
        noOfGammas = 2;
    }
    else if(type==TWOandN)
    {
        type_string = "2&N";
        noOfGammas = 2;
    }
    else
    {
        type_string = "WRONG TYPE!";
        noOfGammas = 0;
    }
    return type_string;
}

Event* generateEvent(TGenPhaseSpace& phaseSpaceGen, const TLorentzVector& source, const ParamManager& pManag, const DecayType type)
{
       //Generation of a decay
       double weight;
       std::vector<TLorentzVector*> fourMomenta;// = new std::vector<TLorentzVector*>();
       std::vector<TLorentzVector*> sourcePar;// = new std::vector<TLorentzVector*>();
       if(type == ONE)
       {
           if(pManag.GetE()<=0.0)
               throw("[ERROR] When gamma has no energy there is no gamma!");
           weight = 1.0;
           fourMomenta.push_back(generateSingleGamma(pManag.GetE()/1000.0));
       }
       else
       {
           weight = phaseSpaceGen.Generate();
           //have to make copies here, because the objects are held by the instance of TGenPhaseSpace
           fourMomenta.push_back(new TLorentzVector(*phaseSpaceGen.GetDecay(0)));
           fourMomenta.push_back(new TLorentzVector(*phaseSpaceGen.GetDecay(1)));
       }

       //Generating emission point inside a ball
       if(source.T() != 0)
            sourcePar.push_back(new TLorentzVector(source.X()+gRandom->Uniform(-1.0,1.0)*source.T(), source.Y()+gRandom->Uniform(-1.0,1.0)*source.T(),\
                                                      source.Z()+gRandom->Uniform(-1.0,1.0)*source.T(), 0.0));
       else
           sourcePar.push_back(new TLorentzVector(source.X(), source.Y(), source.Z(), 0.0));

       if(type != ONE)
           sourcePar.push_back(new TLorentzVector(*sourcePar.at(0))); //making copies

       if(type == THREE)
       {
           fourMomenta.push_back(new TLorentzVector(*phaseSpaceGen.GetDecay(2)));
           sourcePar.push_back(new TLorentzVector(*sourcePar.at(0)));
       }
       else if(type == TWOandONE && gRandom->Uniform() < pManag.GetP() && pManag.GetE()>0.0)
       {
           fourMomenta.push_back(generateSingleGamma(pManag.GetE()/1000.0)); //E in [MeV]
           sourcePar.push_back(new TLorentzVector(*sourcePar.at(0)));
       }
       else if(type == TWOandN)
       {
           //loop over all beta decay branches
           for(int ii=0; ii< pManag.GetNumberOfDecayBranches(); ii++)
           {
               //check if a beta decay occurs
               if(gRandom->Uniform() < pManag.GetDecayBranchProbabilityAt(ii))
               {
                   //loop over all possible gamma emissions
                   for(int jj=0; jj<pManag.GetBranchSize(ii); jj++)
                   {
                       fourMomenta.push_back(generateSingleGamma(pManag.GetGammaEnergyAt(ii, jj)/1000.0)); //E in [MeV]
                       sourcePar.push_back(new TLorentzVector(*sourcePar.at(0)));
                   }
               }
           }
       }

       //Packing everything to EVENT object
       Event* eventDecay = new Event(&sourcePar, &fourMomenta, weight, type);

       //Releasing memory
       for(std::vector<TLorentzVector*>::iterator it = fourMomenta.begin(); it != fourMomenta.end(); ++it)
       {
           delete *it;
       }
       for(std::vector<TLorentzVector*>::iterator it = sourcePar.begin(); it != sourcePar.end(); ++it)
            delete *it;
       fourMomenta.clear();
       sourcePar.clear();

       return eventDecay;
}

#endif
