/// @file MyGateOutput.cpp
/// @author Rafal Maselek <rafalmaselek@gmail.com>
/// @date 15.02.2018
/// @version 1.0
///
/// @section DESCRIPTION
/// User defined class that wraps automatically generated GateOutput class.
///

#include "MyGateOutput.h"
#include <math.h>       /* atan2 */
#include <iostream>
///
/// \brief MyGateOutput::Loop Function that loop over data and fills fields of the MyGateOutput class.
///
void MyGateOutput::Loop()
{
	if (fChain == 0) return;

   	Long64_t nentries = fChain->GetEntriesFast();
   	entries = nentries;
   	Long64_t nbytes = 0, nb = 0;

        // define bufors and initial values
   	std::vector<TLorentzVector> buforSource;
   	std::vector<TLorentzVector> buforMomentum;
   	std::vector<TLorentzVector> buforHits;
   	std::vector<bool> buforPrimaries;
   	std::vector<double> buforEdep;
        std::vector<double> buforPhi;
        std::vector<double> buforTheta;
   	int lastEventID = -1;
        events = 0;
        // loop over entries in the ROOT tree in data

        std::cout<<"[LOADING DATA]"<<std::endl;
   	for (Long64_t jentry=0; jentry<nentries;jentry++) 
   	{
            
            Long64_t ientry = LoadTree(jentry);
            if (ientry < 0) break;
            nb = fChain->GetEntry(jentry);   nbytes += nb;
            // if (Cut(ientry) < 0) continue;
            if(jentry==0) lastEventID=eventID;
            if(!(strcmp("Compton", processName) == 0 || strcmp("compt", processName) == 0))
                    continue;

//            std::cout<<((double)jentry/(double)nentries * 100.0 - (int)((double)jentry/(double)nentries * 100.0))<<std::endl;
            // print progress to std::out
            if( ((double)jentry/(double)nentries * 100.0 - (int)((double)jentry/(double)nentries * 100.0)) < 100.0/nentries)
                std::cout<<"["<<jentry*100/nentries<<"\% LOADED]"<<std::endl;

            // event ID changed or end of file, move data from bufors
            if(eventID != lastEventID || jentry==nentries)
            {
                emissionPoints.push_back(buforSource);
                fourMomenta.push_back(buforMomentum);
                hits.push_back(buforHits);
                primaryPhotons.push_back(buforPrimaries);
                edepVec.push_back(buforEdep);
                edepSmearVec.push_back(buforEdep);
                hitTheta.push_back(buforTheta);
                hitPhi.push_back(buforPhi);

                // clear content of bufors
                buforSource.clear();
                buforMomentum.clear();
                buforHits.clear();
                buforPrimaries.clear();
                buforEdep.clear();
                buforTheta.clear();
                buforPhi.clear();
                events++;
            }

            // add data to bufors
            buforSource.push_back(TLorentzVector(sourcePosX, sourcePosY, sourcePosZ, 0.0));
            buforHits.push_back(TLorentzVector(posX, posY, posZ, time));
            buforMomentum.push_back(TLorentzVector(0.0, 0.0, 0.0, edep));
            if(nPhantomCompton==0 && nCrystalCompton==0)
                buforPrimaries.push_back(true);
            else
                buforPrimaries.push_back(false);
            buforEdep.push_back(edep);
            buforPhi.push_back(atan2(posY, posX));
            buforTheta.push_back(atan(sqrt(posY*posY+posX*posX)/posZ));
            ids.push_back(runID);
            lastEventID = eventID;
        }
        std::cout<<"[DATA LOADED]"<<std::endl;
}
