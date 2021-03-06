#ifndef TopTreeProducer_h
#define TopTreeProducer_h

// system include files
#include <memory>
#include <string>
#include <iostream>

// user include files

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Run.h"
#include "DataFormats/Provenance/interface/EventID.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Utilities/interface/EDMException.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/FileInPath.h"
#include "FWCore/Framework/interface/LuminosityBlock.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/Common/interface/MergeableCounter.h"
#include "DataFormats/METReco/interface/HcalNoiseSummary.h"

#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"

#include "../interface/HLTAnalyzer.h"
#include "../interface/MCAnalyzer.h"
#include "../interface/MCAssociator.h"
#include "../interface/VertexAnalyzer.h"
#include "../interface/JetAnalyzer.h"
#include "../interface/GenJetAnalyzer.h"
#include "../interface/PFJetAnalyzer.h"
#include "../interface/FatJetAnalyzer.h"
#include "../interface/MuonAnalyzer.h"
#include "../interface/ElectronAnalyzer.h"
#include "../interface/PhotonAnalyzer.h"
#include "../interface/METAnalyzer.h"
#include "../interface/PFMETAnalyzer.h"
#include "../interface/NPGenEventAnalyzer.h"
#include "../interface/LHEEventProductAnalyzer.h"



#include "../interface/TRootRun.h"
#include "../interface/TRootEvent.h"
#include "../interface/TRootParticle.h"
#include "../interface/TRootMCParticle.h"
#include "../interface/TRootJet.h"
#include "../interface/TRootGenJet.h"
#include "../interface/TRootPFJet.h"
#include "../interface/TRootSubstructureJet.h"
#include "../interface/TRootLepton.h"
#include "../interface/TRootMuon.h"
#include "../interface/TRootElectron.h"
#include "../interface/TRootMET.h"
#include "../interface/TRootPFMET.h"
#include "../interface/TRootNPGenEvent.h"
#include "../interface/TRootVertex.h"

#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TClonesArray.h"

//using namespace TopTree;

class TopTreeProducer : public edm::EDAnalyzer {
public:
	explicit TopTreeProducer(const edm::ParameterSet&);
	~TopTreeProducer();


private:
	virtual void beginJob() ;
	virtual void beginRun(const edm::Run&, const edm::EventSetup&) ;
	virtual void endRun(const edm::Run&, const edm::EventSetup&, const edm::ParameterSet&) ;
	virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endLuminosityBlock(const edm::LuminosityBlock&, const EventSetup&);
	virtual void endJob() ;

	edm::ParameterSet myConfig_;
	edm::ParameterSet producersNames_;
	edm::ParameterSet valuesForConsumeCommand;

	int verbosity;
	std::string rootFileName_ ;
	TFile* rootFile_ ;
	TTree* eventTree_;
	TTree* runTree_;
  TH1F* tmp_;
	bool doHLT;
	bool doMC;
	bool doMCAssociation;
	bool doPDFInfo;
	bool doSignalMuMuGamma;
	bool doSignalTopTop;
	bool doPrimaryVertex;
	bool doGenJet;
	bool doPFJet;
	bool doFatJet;
	bool doMuon;
	bool doElectron;
	bool doPhoton;
	bool doPhotonMC;
	bool doPFMET;
	bool doNPGenEvent;
	bool doLHEEventProd;
	bool drawMCTree;
	bool doEventCleaningInfo;
	std::vector<std::string> vGenJetProducer;
	std::vector<std::string> vPFJetProducer;
	std::vector<std::string> vFatJetProducer;
	std::vector<std::string> vMuonProducer;
	std::vector<std::string> vElectronProducer;
	std::vector<std::string> vPhotonProducer;
        std::vector<std::string> vPFmetProducer;
	int nTotEvt_;
	HLTAnalyzer* hltAnalyzer_;
    LHEEventProductAnalyzer* lheEventProductAnalyzer_;
    LHEEventProductAnalyzer* RunlheEventProductAnalyzer_;
	TRootRun* runInfos_;
	TRootEvent* rootEvent;
	TClonesArray* mcParticles;
	TClonesArray* tracks;
	std::vector<TClonesArray*> vgenJets;
	std::vector<TClonesArray*> vpfJets;
	std::vector<TClonesArray*> vfatJets;
	std::vector<TClonesArray*> vmuons;
	std::vector<TClonesArray*> velectrons;
	std::vector<TClonesArray*> vphotons;
	std::vector<TClonesArray*> vPFmets;
	//	TClonesArray* lheEventProduct;
	TClonesArray* NPgenEvent;
	TClonesArray* primaryVertex;

    bool useEventCounter_;
    std::vector<std::string> filters_;
    bool isRealData_;


	// handles necessary for accessing objects in 76X or higher:

      // ---------- tokens to CMSSW member classes to be accessed in analysis ------------------
    // good practice is to use these instead of getbylabel. However getbylabel is still supported...
    edm::EDGetTokenT<reco::VertexCollection> vtxToken_;
    std::vector<edm::EDGetTokenT<pat::MuonCollection> > vmuonToken_;
    std::vector<edm::EDGetTokenT<pat::ElectronCollection> > velectronToken_;
    std::vector<edm::EDGetTokenT<pat::PhotonCollection> > vphotonToken_;
    std::vector<edm::EDGetTokenT<pat::JetCollection> > vjetToken_;
    std::vector<edm::EDGetTokenT<pat::JetCollection> > vfatjetToken_;
    std::vector<edm::EDGetTokenT<pat::METCollection> > vmetToken_;
    std::vector<edm::EDGetTokenT<std::vector<reco::GenJet> > >vgenjetToken_;
    edm::EDGetTokenT<edm::TriggerResults> triggerToken1_;
    edm::EDGetTokenT<edm::TriggerResults> triggerToken2_;
    edm::EDGetTokenT<edm::TriggerResults> triggerToken3_;
    edm::EDGetTokenT<edm::TriggerResults> triggerToken4_;
    edm::EDGetTokenT<edm::TriggerResults> metfilterToken_;
    edm::EDGetTokenT<HcalNoiseSummary> hcalNoiseSummaryToken_;
    edm::EDGetTokenT<std::vector< PileupSummaryInfo > > pileUpProducerToken_;
    edm::EDGetTokenT<double> fixedGridRhoAllToken_;
    edm::EDGetTokenT<double> fixedGridRhoFastjetAllToken_;
    edm::EDGetTokenT<double> fixedGridRhoFastjetAllCaloToken_;
    edm::EDGetTokenT<double> fixedGridRhoFastjetCentralCaloToken_;
    edm::EDGetTokenT<double> fixedGridRhoFastjetCentralChargedPileUpToken_;
    edm::EDGetTokenT<double> fixedGridRhoFastjetCentralNeutralToken_;
	  edm::EDGetTokenT<GenEventInfoProduct> genEventInfoProductToken_;
    edm::EDGetTokenT<std::vector<reco::GenParticle> > genParticlesToken_;
    edm::EDGetTokenT<LHEEventProduct> lheproductToken_;
	  edm::EDGetTokenT<LHERunInfoProduct> lheRunInfoproductToken_;
    
    edm::EDGetTokenT<reco::BeamSpot> offlineBSToken_;


    
};


#endif
