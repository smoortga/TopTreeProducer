#include "../interface/TopTreeProducer.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/METReco/interface/HcalNoiseSummary.h"

using namespace std;
using namespace TopTree;
using namespace reco;
using namespace edm;



TopTreeProducer::TopTreeProducer(const edm::ParameterSet& iConfig)
{
    myConfig_ = iConfig.getParameter<ParameterSet>("myConfig");
    producersNames_ = iConfig.getParameter<ParameterSet>("producersNames");
    valuesForConsumeCommand = iConfig.getParameter<ParameterSet>("producerNamesBookkeepingThreads");
    
    // new for CMSSW76X and higher: all classes that are read from the event need to be registered in the constructor!
    // supposedly this is necessary in the case that the code is run on machines that use multi-threading.
    // It also allows the CMSSW compiler to optimise/speed up the code (or so the documentation says), by accessing collections that are used a lot or rarely with the consumesOften() and mayConsume() fuctions but consumes() will always work.
    // in the TopTreeProducer .py file these are stored in either the producerNamesBookkeepingThreads parameter set or the producersNames parameter set (which contains vstrings used by the various analysers, so please add new objects there if you need them.
    
	  vector<string> defaultVec;
    vMuonProducer = valuesForConsumeCommand.getUntrackedParameter<vector<string> >("vmuonProducer",defaultVec);
    vElectronProducer = valuesForConsumeCommand.getUntrackedParameter<vector<string> >("velectronProducer",defaultVec);
    vPhotonProducer = valuesForConsumeCommand.getUntrackedParameter<vector<string> >("vphotonProducer",defaultVec);
    vPFJetProducer = valuesForConsumeCommand.getUntrackedParameter<vector<string> >("vpfJetProducer",defaultVec);
    vPFmetProducer = valuesForConsumeCommand.getUntrackedParameter<vector<string> >("vpfmetProducer",defaultVec);
    vFatJetProducer = valuesForConsumeCommand.getUntrackedParameter<vector<string> >("vfatJetProducer",defaultVec);
    vGenJetProducer = valuesForConsumeCommand.getUntrackedParameter<vector<string> >("vgenJetProducer",defaultVec);
	
    for(unsigned int s=0; s<vMuonProducer.size(); s++)
    {
		  vmuonToken_.push_back(consumes<pat::MuonCollection>(edm::InputTag(vMuonProducer[s])));
    }
    for(unsigned int s=0; s<vElectronProducer.size(); s++)
    {
		  velectronToken_.push_back(consumes<pat::ElectronCollection>(edm::InputTag(vElectronProducer[s])));
    }
    for(unsigned int s=0; s<vPhotonProducer.size(); s++)
    {
		  vphotonToken_.push_back(consumes<pat::PhotonCollection>(edm::InputTag(vPhotonProducer[s])));
    }
    for(unsigned int s=0; s<vPFJetProducer.size(); s++)
    {
		  vjetToken_.push_back(consumes<pat::JetCollection>(edm::InputTag(vPFJetProducer[s])));
    }
    for(unsigned int s=0; s<vPFmetProducer.size(); s++)
    {
		  vmetToken_.push_back(consumes<pat::METCollection>(edm::InputTag(vPFmetProducer[s])));
    }
    for(unsigned int s=0; s<vFatJetProducer.size(); s++)
    {
		  vfatjetToken_.push_back(consumes<pat::JetCollection>(edm::InputTag(vFatJetProducer[s])));
    }
    for(unsigned int s=0; s<vGenJetProducer.size(); s++)
    {
		  vgenjetToken_.push_back(consumes<std::vector<reco::GenJet>>(edm::InputTag(vGenJetProducer[s])));
    }

    vtxToken_ = consumes<reco::VertexCollection>(valuesForConsumeCommand.getUntrackedParameter<edm::InputTag>("primaryVertexProducer"));
    triggerToken1_ = consumes<edm::TriggerResults>(valuesForConsumeCommand.getParameter<edm::InputTag>("hltProducer1st"));
    triggerToken2_ = consumes<edm::TriggerResults>(valuesForConsumeCommand.getParameter<edm::InputTag>("hltProducer2nd"));
    triggerToken3_ = consumes<edm::TriggerResults>(valuesForConsumeCommand.getParameter<edm::InputTag>("hltProducer3rd"));
    triggerToken4_ = consumes<edm::TriggerResults>(valuesForConsumeCommand.getParameter<edm::InputTag>("hltProducer4th"));
    pileUpProducerToken_ = consumes<std::vector< PileupSummaryInfo > >(valuesForConsumeCommand.getUntrackedParameter<edm::InputTag>("pileUpProducer"));
    metfilterToken_ = consumes<edm::TriggerResults>(valuesForConsumeCommand.getUntrackedParameter<edm::InputTag>("metfilterProducer"));
    hcalNoiseSummaryToken_ = consumes<HcalNoiseSummary>(valuesForConsumeCommand.getUntrackedParameter<edm::InputTag>("summaryHBHENoise"));
    fixedGridRhoAllToken_ = consumes<double>(valuesForConsumeCommand.getUntrackedParameter<edm::InputTag>("fixedGridRhoAll"));
    fixedGridRhoFastjetAllToken_ = consumes<double>(valuesForConsumeCommand.getUntrackedParameter<edm::InputTag>("fixedGridRhoFastjetAll"));
    fixedGridRhoFastjetAllCaloToken_ = consumes<double>(valuesForConsumeCommand.getUntrackedParameter<edm::InputTag>("fixedGridRhoFastjetAllCalo"));
    fixedGridRhoFastjetCentralCaloToken_ = consumes<double>(valuesForConsumeCommand.getUntrackedParameter<edm::InputTag>("fixedGridRhoFastjetCentralCalo"));
    fixedGridRhoFastjetCentralChargedPileUpToken_ = consumes<double>(valuesForConsumeCommand.getUntrackedParameter<edm::InputTag>("fixedGridRhoFastjetCentralChargedPileUp"));
    fixedGridRhoFastjetCentralNeutralToken_ = consumes<double>(valuesForConsumeCommand.getUntrackedParameter<edm::InputTag>("fixedGridRhoFastjetCentralNeutral"));
	genEventInfoProductToken_ = consumes<GenEventInfoProduct>(valuesForConsumeCommand.getUntrackedParameter<edm::InputTag>("genEventInfoProduct"));
    genParticlesToken_ = consumes<std::vector<reco::GenParticle> >(valuesForConsumeCommand.getUntrackedParameter<edm::InputTag>("prunedGenParticles"));
    lheproductToken_  = consumes<LHEEventProduct>(valuesForConsumeCommand.getUntrackedParameter<edm::InputTag>("lheproduct"));
    offlineBSToken_ = consumes<reco::BeamSpot>(valuesForConsumeCommand.getParameter<edm::InputTag>("offlineBeamSpot"));
    
}


TopTreeProducer::~TopTreeProducer()
{
}


// ------------ method called once each job just before starting event loop  ------------
void TopTreeProducer::beginJob()
{

    // Load Config parameters
    verbosity = myConfig_.getUntrackedParameter<int>("verbosity", 0);
    rootFileName_ = myConfig_.getUntrackedParameter<string>("RootFileName","noname.root");
    doHLT = myConfig_.getUntrackedParameter<bool>("doHLT",false);
	doMCAssociation = myConfig_.getUntrackedParameter<bool>("doMCAssociation",false);
    doPDFInfo = myConfig_.getUntrackedParameter<bool>("doPDFInfo",false);
    doPrimaryVertex = myConfig_.getUntrackedParameter<bool>("doPrimaryVertex",false);
    doGenJet = myConfig_.getUntrackedParameter<bool>("doGenJet",false);
    doPFJet = myConfig_.getUntrackedParameter<bool>("doPFJet",false);
    doFatJet = myConfig_.getUntrackedParameter<bool>("doFatJet",false);
    doMuon = myConfig_.getUntrackedParameter<bool>("doMuon",false);
    doElectron = myConfig_.getUntrackedParameter<bool>("doElectron",false);
    doPhoton = myConfig_.getUntrackedParameter<bool>("doPhoton",false);
    doPhotonMC = myConfig_.getUntrackedParameter<bool>("doPhotonMC",false);
    doPFMET = myConfig_.getUntrackedParameter<bool>("doPFMET",false);
    drawMCTree = myConfig_.getUntrackedParameter<bool>("drawMCTree",false);
    doNPGenEvent = myConfig_.getUntrackedParameter<bool>("doNPGenEvent",false);
	doLHEEventProd  = myConfig_.getUntrackedParameter<bool>("doLHEEventProd",true);
	doEventCleaningInfo  = myConfig_.getUntrackedParameter<bool>("doEventCleaningInfo",true);
    useEventCounter_ = myConfig_.getUntrackedParameter<bool>("useEventCounter",true);

    vector<string> defaultVec;
    filters_ = myConfig_.getUntrackedParameter<std::vector<std::string> >("filters",defaultVec);

    for(unsigned int s=0; s<vGenJetProducer.size(); s++)
    {
        TClonesArray* a;
        vgenJets.push_back(a);
    }

    for(unsigned int s=0; s<vPFJetProducer.size(); s++)
    {
        TClonesArray* a;
        vpfJets.push_back(a);
    }

    for(unsigned int s=0; s<vFatJetProducer.size(); s++)
    {
        TClonesArray* a;
        vfatJets.push_back(a);
    }

    for(unsigned int s=0; s<vMuonProducer.size(); s++)
    {
        TClonesArray* a;
        vmuons.push_back(a);
    }

    for(unsigned int s=0; s<vElectronProducer.size(); s++)
    {
        TClonesArray* a;
        velectrons.push_back(a);
    }

    for(unsigned int s=0; s<vPhotonProducer.size(); s++)
    {
        TClonesArray* a;
        vphotons.push_back(a);
    }

    for(unsigned int s=0; s<vPFmetProducer.size(); s++)
    {
        TClonesArray* a;
        vPFmets.push_back(a);
    }


    nTotEvt_ = 0;

    // initialize root output file
    rootFile_ = new TFile(rootFileName_.c_str(), "recreate");
    rootFile_->cd();
    if(verbosity>0) cout << "New RootFile " << rootFileName_.c_str() << " is created" << endl;

    tmp_ = new TH1F("EventSummary","EventSummary", filters_.size(),0,filters_.size());

    runInfos_ = new TRootRun();
    runTree_ = new TTree("runTree", "Global Run Infos");
    runTree_->Branch ("runInfos", "TopTree::TRootRun", &runInfos_);
    if(verbosity>0) cout << "RunTree is created" << endl;

    rootEvent = new TRootEvent();
    eventTree_ = new TTree("eventTree", "Event Infos");
    eventTree_->Branch ("Event", "TopTree::TRootEvent", &rootEvent);
    if(verbosity>0) cout << "EventTree is created" << endl;

    if(doHLT)
    {
        if(verbosity>0) cout << "HLT info will be added to rootuple" << endl;
        hltAnalyzer_ = new HLTAnalyzer(triggerToken1_, triggerToken2_, triggerToken3_, triggerToken4_, myConfig_, verbosity);
    }


    if(doLHEEventProd)
    {
        if(verbosity>0) cout << "LHE info will be added to rootuple" << endl;
        lheEventProductAnalyzer_ = new LHEEventProductAnalyzer(verbosity);
        //lheEventProduct = new TClonesArray("LHEEventProduct", 1000);

        //eventTree_->Branch ("LHEEventProd", "LHEEventProduct",&lheEventProduct);
        //lheEventProductAnalyzer_->setVerbosity(verbosity);



    }



    //		if(!isRealData_)
    //	{
    if(verbosity>0) cout << "MC Particles info will be added to rootuple" << endl;
    mcParticles = new TClonesArray("TopTree::TRootMCParticle", 1000);
    eventTree_->Branch ("MCParticles", "TClonesArray", &mcParticles);
    //     	}

    if(doGenJet)
    {
        if(verbosity>0) cout << "GenJets info will be added to rootuple (for GenJetStudy)" << endl;
        for(unsigned int s=0; s<vGenJetProducer.size(); s++)
        {
            vgenJets[s] = new TClonesArray("TopTree::TRootGenJet", 2000);
            char name[100];
            sprintf(name,"GenJets_%s",vGenJetProducer[s].c_str());
            eventTree_->Branch (name, "TClonesArray", &vgenJets[s]);
        }
    }

    if(doPFJet)
    {
        if(verbosity>0) cout << "PFJets info will be added to rootuple" << endl;
        for(unsigned int s=0; s<vPFJetProducer.size(); s++)
        {
            vpfJets[s] = new TClonesArray("TopTree::TRootPFJet", 1000);
            char name[100];
            sprintf(name,"PFJets_%s",vPFJetProducer[s].c_str());
            eventTree_->Branch (name, "TClonesArray", &vpfJets[s]);
        }
    }


    if(doFatJet)
    {
        if(verbosity>0) cout << "FatJets info will be added to rootuple" << endl;
        for(unsigned int s=0; s<vFatJetProducer.size(); s++)
        {
            vfatJets[s] = new TClonesArray("TopTree::TRootSubstructureJet", 1000);
            char name[100];
            sprintf(name,"FatJets_%s",vFatJetProducer[s].c_str());
            eventTree_->Branch (name, "TClonesArray", &vfatJets[s]);
        }
    }

    if(doNPGenEvent)
    {
        if(verbosity>0) cout << "NPGenEvent info will be added to rootuple" << endl;
        NPgenEvent = new TClonesArray("TopTree::TRootNPGenEvent", 1000);
        eventTree_->Branch ("NPGenEvent", "TClonesArray", &NPgenEvent);
    }

    if(doMuon)
    {
        if(verbosity>0) cout << "Muons info will be added to rootuple" << endl;
        for(unsigned int s=0; s<vMuonProducer.size(); s++)
        {
            vmuons[s] = new TClonesArray("TopTree::TRootMuon", 1000);
            char name[100];
            sprintf(name,"Muons_%s",vMuonProducer[s].c_str());
            eventTree_->Branch (name, "TClonesArray", &vmuons[s]);
        }
    }

    if(doElectron)
    {
        if(verbosity>0) cout << "Electrons info will be added to rootuple" << endl;
        for(unsigned int s=0; s<vElectronProducer.size(); s++)
        {
            velectrons[s] = new TClonesArray("TopTree::TRootElectron", 1000);
            char name[100];
            sprintf(name,"Electrons_%s",vElectronProducer[s].c_str());
            eventTree_->Branch (name, "TClonesArray", &velectrons[s]);
        }
    }

    if(doPhoton)
    {
        if(verbosity>0) cout << "Photons info will be added to rootuple" <<endl;
        for(unsigned int s=0; s<vPhotonProducer.size(); s++)
        {
            vphotons[s] = new TClonesArray("TopTree::TRootPhoton", 1000);
            char name[100];
            sprintf(name,"Photons_%s",vPhotonProducer[s].c_str());
            eventTree_->Branch (name, "TClonesArray", &vphotons[s]);
        }
    }

    if(doPFMET)
    {
        if(verbosity>0) cout << "ParticleFlowMET info will be added to rootuple" << endl;
        for(unsigned int s=0; s<vPFmetProducer.size(); s++)
        {
            vPFmets[s] = new TClonesArray("TopTree::TRootPFMET", 1000);
            char name[100];
            sprintf(name,"PFMET_%s",vPFmetProducer[s].c_str());
            eventTree_->Branch (name, "TClonesArray", &vPFmets[s]);
        }
    }


    if(doPrimaryVertex)
    {
        if(verbosity>0) cout << "Primary Vertex info will be added to rootuple" << endl;
        primaryVertex = new TClonesArray("TopTree::TRootVertex", 1000);
        eventTree_->Branch ("PrimaryVertex", "TClonesArray", &primaryVertex);
    }
	if (doEventCleaningInfo){
		if (verbosity > 0 ) cout << "Event cleaning information will be added to roottuple" << endl;
		
		
	}
	
	

}


// ------------ method called once each job just after ending the event loop  ------------
void TopTreeProducer::endJob()
{

    // Trigger Summary Tables
    if(doHLT)
    {
        cout << "Trigger Summary Tables" << endl;
        hltAnalyzer_->copySummary(runInfos_);
        hltAnalyzer_->printStats();
    }




    runInfos_->setPrePathCounter(tmp_->GetBinContent(1));
    runInfos_->setPostPathCounter(tmp_->GetBinContent(2));

    runTree_->Fill();
    //tmp_->Write(); // not saving histogram for consistency, instead we save these numbers in runTree (TJ)
    std::cout << "Initial number of events: " << tmp_->GetBinContent(1) << std::endl;
    //Initial number of events could be different from nTotEvt_ if there is any filter in the PAT sequence
    std::cout << "Total number of events: " << nTotEvt_ << std::endl;
    std::cout << "Closing rootfile " << rootFile_->GetName() << std::endl;
    rootFile_->Write();
    rootFile_->Close();

}

void TopTreeProducer::endLuminosityBlock(const edm::LuminosityBlock & lumi, const edm::EventSetup & setup)
{
    if(useEventCounter_)
    {
        for(unsigned int i=0; i<filters_.size(); ++i)
        {
            std::string name = filters_[i];
            edm::Handle<edm::MergeableCounter> numEventsCounter;
            lumi.getByLabel(name, numEventsCounter);
            if( numEventsCounter.isValid())
            {
                tmp_->AddBinContent(i+1, numEventsCounter->value);
                tmp_->GetXaxis()->SetBinLabel(i+1,filters_[i].c_str());
            }
        }
    }
}

//------------- method called for each run -------------
void TopTreeProducer::beginRun(const edm::Run& iRun, const edm::EventSetup& iSetup)
{
	//edm::ParameterSet valuesForConsumeCommand = iConfig.getParameter<ParameterSet>("producerNamesBookkeepingThreads");
	//lheRunInfoproductToken_  = consumes<LHERunInfoProduct>(valuesForConsumeCommand.getUntrackedParameter<edm::InputTag>("lheproduct"));
    //RunlheEventProductAnalyzer_ = new LHEEventProductAnalyzer(verbosity);
    //RunlheEventProductAnalyzer_->PrintWeightNamesList(iRun,lheRunInfoproductToken_);
}
void TopTreeProducer::endRun(const edm::Run& iRun, const edm::EventSetup& iSetup,const edm::ParameterSet& iConfig)
{
    edm::ParameterSet valuesForConsumeCommand = iConfig.getParameter<ParameterSet>("producerNamesBookkeepingThreads");
	lheRunInfoproductToken_  = consumes<LHERunInfoProduct>(valuesForConsumeCommand.getUntrackedParameter<edm::InputTag>("lheproduct"));
	RunlheEventProductAnalyzer_ = new LHEEventProductAnalyzer(verbosity);
    RunlheEventProductAnalyzer_->CopyWeightNames(iRun, runInfos_,lheRunInfoproductToken_);
    if(verbosity > 1 ) RunlheEventProductAnalyzer_->PrintWeightNamesList(iRun,lheRunInfoproductToken_);
}
// ------------ method called to for each event  ------------
void TopTreeProducer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
    isRealData_ = iEvent.isRealData();

    rootFile_->cd();
    nTotEvt_++;
    if( (verbosity>1) || (verbosity>0 && nTotEvt_%10==0 && nTotEvt_<=100)  || (verbosity>0 && nTotEvt_%100==0 && nTotEvt_>100) )
        cout << endl << endl
             << "####### TopTreeProducer - Cumulated Events " << nTotEvt_
             << " - Run " << iEvent.id().run()
             << " - Event " << iEvent.id().event()
             << " #######" << endl;


    // Global Event Infos

    rootEvent = new TRootEvent();

    rootEvent->setNb(nTotEvt_);
    rootEvent->setEventId(iEvent.id().event());
    rootEvent->setRunId(iEvent.id().run());
    rootEvent->setLumiBlockId(iEvent.luminosityBlock());
	
	// event cleaning. Particularly important for analyses using MET!
	// Information is stored in TRootEvent as bool flags. false = bad events.
	if (doEventCleaningInfo){
		
		// get TriggerResults information for the csc and ee bad supercluster filters. In principle the same thing could be done for the HBHE and HBHEIso filters but those currently need to be re-run
	  try{
		edm::Handle<edm::TriggerResults> metTrigResults; //our trigger result object
		iEvent.getByToken(metfilterToken_,metTrigResults);
		Bool_t theresult=true;
		if(metTrigResults.isValid()){
			const edm::TriggerNames& metTrigNames = iEvent.triggerNames(*metTrigResults);
			theresult=metTrigResults->accept(metTrigNames.triggerIndex("Flag_CSCTightHaloFilter"));
			rootEvent->setCSCTightHaloFilter(theresult);
			theresult=true;
			theresult=metTrigResults->accept(metTrigNames.triggerIndex("Flag_eeBadScFilter"));
			rootEvent->setEEBadScFilter(theresult);
		}
		theresult=true;
		edm::Handle<HcalNoiseSummary> hSummary;
		iEvent.getByLabel("hcalnoise", hSummary);
        if( hSummary.isValid()){
			if( hSummary->numIsolatedNoiseChannels() >=10 )  theresult = false;
			if( hSummary->isolatedNoiseSumE() >=50        )  theresult = false;
			if( hSummary->isolatedNoiseSumEt() >=25       )  theresult = false;
		}
		rootEvent->setHCalIsoNoise(theresult);
		
		edm::Handle<bool> hNoiseResult;
          
        iEvent.getByLabel("HBHENoiseFilterResultProducer", "HBHENoiseFilterResult", hNoiseResult);
        theresult= true;
		if(hNoiseResult.isValid())
			theresult = *hNoiseResult;
		rootEvent->setHBHENoise(theresult);
		
		if (verbosity > 2){
			cout << "************************************" << endl;
			cout << "Filling event cleaning information: " << endl;

			cout << " HCAL Isolation noise filter value: " << rootEvent->getHCalIsoNoise() << endl;
			cout << " HBHE noise filter value: " << rootEvent->getHBHENoise() << endl;
			cout << " CSS halo filter value: " << rootEvent->getCSCTightHaloFilter() << endl;
			cout << " EE bad SuperCluster filter value: " << rootEvent->getEEBadScFilter() << endl;
			cout << "************************************" << endl;

		}
	  }  
	  catch (...){
	    std::cerr << "tried to retrieve MET noise filter information and failed! " << std::endl;
	  }
	}
	// end of event cleaning code

    if(verbosity>4)
    {
        cout << "Root Event things set." << endl;
    }

    // do PileUp info

    Handle<std::vector< PileupSummaryInfo > >  PupInfo;
    iEvent.getByToken(pileUpProducerToken_, PupInfo);

    if (PupInfo.isValid())
    {
        std::vector<PileupSummaryInfo>::const_iterator PVI;

        // (then, for example, you can do)
        for(PVI = PupInfo->begin(); PVI != PupInfo->end(); ++PVI)
        {
            if(verbosity>2)
            {
                std::cout << " Pileup Information: bunchXing, nvtx: " << PVI->getBunchCrossing() << " " << PVI->getPU_NumInteractions() << std::endl;
            }

            rootEvent->setNPu(PVI->getBunchCrossing(), PVI->getPU_NumInteractions());
            if(verbosity>4)
            {
                cout << "Root Event PU Stuff 1." << endl;
            }
            if(PVI->getBunchCrossing() == 0) rootEvent->setNTruePU(PVI->getTrueNumInteractions());
        }
        if(verbosity>4)
        {
            cout << "Root Event PU Stuff 2." << endl;
        }
        //cout << rootEvent->nPu(0) << endl;
        //cout << rootEvent->nPu(1) << endl;
    }


    // we need to store some triggerFilter info to be able to emulate triggers on older data
//    if (doHLT)
//    {
//        if (verbosity > 1) cout << "should do HLT now..." << endl;
//
//        // get Trigger summary from Event
//        edm::Handle<trigger::TriggerEvent> summary, summary1st, summary2nd, summary3rd, summary4th;
//        edm::InputTag summaryTag1st_("hltTriggerSummaryAOD","",(producersNames_.getParameter < edm::InputTag > ("hltProducer1st")).process());
//        edm::InputTag summaryTag2nd_("hltTriggerSummaryAOD","",(producersNames_.getParameter < edm::InputTag > ("hltProducer2nd")).process());
//        edm::InputTag summaryTag3rd_("hltTriggerSummaryAOD","",(producersNames_.getParameter < edm::InputTag > ("hltProducer3rd")).process());
//        edm::InputTag summaryTag4th_("hltTriggerSummaryAOD","",(producersNames_.getParameter < edm::InputTag > ("hltProducer4th")).process());
//
//        try
//        {
//            iEvent.getByLabel(summaryTag1st_,summary1st);
//        }
//        catch (...)
//        {
//            ;
//        }
//        try
//        {
//            iEvent.getByLabel(summaryTag2nd_,summary2nd);
//        }
//        catch (...)
//        {
//            ;
//        }
//        try
//        {
//            iEvent.getByLabel(summaryTag3rd_,summary3rd);
//        }
//        catch (...)
//        {
//            ;
//        }
//        try
//        {
//            iEvent.getByLabel(summaryTag4th_,summary4th);
//        }
//        catch (...)
//        {
//            ;
//        }
//
//        //cout << summaryTag1st_ << " " << summaryTag2nd_  << " " << summaryTag3rd_  << " " << summaryTag4th_ << endl;
//        //cout << summaryTag1st_.process() << " " << summaryTag2nd_.process()<< " " << summaryTag3rd_.process()<< " " << summaryTag4th_.process() << endl;
//        //cout << summary1st.isValid() << " " << summary2nd.isValid() << " " << summary3rd.isValid() << " " << summary4th.isValid()<< endl;
//
//        if (summary1st.isValid())
//            summary = summary1st;
//        else if (summary2nd.isValid())
//            summary = summary2nd;
//        else if (summary3rd.isValid())
//            summary = summary3rd;
//        else if (summary4th.isValid())
//            summary = summary4th;
//        else
//            cout << "TopTreeProducer::Analyze ERROR: Could not store info for trigger emulation: provided HLTproducerNames are null" << endl;
//
//        //cout << "summary " << summary << endl;
//
//        if (summary.isValid())
//        {
//            for (unsigned int i=0; i<summary->sizeFilters(); i++)
//            {
//                if (verbosity > 1) cout << i << " -> " << summary->filterTag(i).label() << endl;
//
//                // get all trigger objects corresponding to this module.
//                // loop through them and see how many objects match the selection
//                const trigger::Keys& KEYS (summary->filterKeys(i));
//                const int n1(KEYS.size());
//
//                for (int j=0; j!=n1; ++j)
//                {
//                    const trigger::TriggerObject& triggerObject( summary-> getObjects().at(KEYS[j]) );
//                    //cout << "j: " << j << " -> id " << triggerObject.id() << endl;
//                    //cout << "j: " << j << " -> pt " << triggerObject.pt() << endl;
//                    //cout << "j: " << j << " -> eta " << triggerObject.eta() << endl;
//                    //cout << "j: " << j << " -> phi " << triggerObject.phi() << endl;
//                    rootEvent->AddTriggerObject(string(summary->filterTag(i).label()), triggerObject.id(),triggerObject.pt(),triggerObject.eta(),triggerObject.phi());
//                }
//            }
//        }
//    }

    //fastjet density rho
    //commenting out this rho for 7_0_X as it seems to be missing from miniAOD
    edm::Handle<double> rhoAll;
    iEvent.getByToken(fixedGridRhoAllToken_,rhoAll);
    rootEvent->setfixedGridRhoAll(*rhoAll);

    edm::Handle<double> rhoFSAll;
    iEvent.getByToken(fixedGridRhoFastjetAllToken_,rhoFSAll);
    rootEvent->setfixedGridRhoFastjetAll(*rhoFSAll);

    edm::Handle<double> rhoFSAC;
    iEvent.getByToken(fixedGridRhoFastjetAllCaloToken_,rhoFSAC);
    rootEvent->setfixedGridRhoFastjetAllCalo(*rhoFSAC);

    edm::Handle<double> rhoFJCC;
    iEvent.getByToken(fixedGridRhoFastjetCentralCaloToken_,rhoFJCC);
    rootEvent->setfixedGridRhoFastjetCentralCalo(*rhoFJCC);

    edm::Handle<double> rhoFJCCPU;
    iEvent.getByToken(fixedGridRhoFastjetCentralChargedPileUpToken_,rhoFJCCPU);
    rootEvent->setfixedGridRhoFastjetCentralChargedPileUp(*rhoFJCCPU);

    edm::Handle<double> rhoFJCN;
    iEvent.getByToken(fixedGridRhoFastjetCentralNeutralToken_,rhoFJCN);
    rootEvent->setfixedGridRhoFastjetCentralNeutral(*rhoFJCN);

    //density rho for electron isolation (effective area stuff)
//    edm::Handle<double> rhoIso;
//    iEvent.getByToken(fixedGridRhoFastjetAllToken_,rhoIso);
//    rootEvent->setKt6PFJetsForIsolation_rho(*rhoIso);

    if(!isRealData_)
    {
        //flavorHistory path

        //turning off for 7_0_X as not present/renamed in miniAOD
        //	edm::Handle<unsigned int> flavHist;
        //	iEvent.getByLabel("flavorHistoryFilter","",flavHist);
        //	rootEvent->setflavorHistoryPath(*flavHist);
    }

    // Trigger
    rootEvent->setGlobalHLT(true);
    if(doHLT)
    {
        if(verbosity>1) std::cout << endl << "Get TriggerResults..." << std::endl;
        hltAnalyzer_->process(iEvent, rootEvent, valuesForConsumeCommand);
    }

    // MC Info
    if(!isRealData_)
    {
        if(verbosity>1) cout << endl << "Analysing MC info..." << endl;
        MCAnalyzer* myMCAnalyzer = new MCAnalyzer(myConfig_,verbosity);
        if (drawMCTree) myMCAnalyzer->DrawMCTree(iEvent, iSetup, myConfig_, producersNames_);
        if (doPDFInfo ) myMCAnalyzer->PDFInfo(iEvent, rootEvent, genEventInfoProductToken_);
        myMCAnalyzer->ProcessMCParticle(iEvent, mcParticles,genParticlesToken_);
        delete myMCAnalyzer;
    }


    if(doLHEEventProd)
    {

        lheEventProductAnalyzer_->Process(iEvent,  rootEvent, lheproductToken_);

    }

    // Get Primary Vertices
    if(doPrimaryVertex)
    {
        if(verbosity>1) cout << endl << "Analysing primary vertices collection..." << endl;
        VertexAnalyzer* myVertexAnalyzer = new VertexAnalyzer(verbosity);
        myVertexAnalyzer->Process(iEvent, primaryVertex, vtxToken_);
        delete myVertexAnalyzer;
    }

    // GenJet
    if(!isRealData_ && doGenJet)
    {
        if(verbosity>1) cout << endl << "Analysing GenJets collection ..." << endl;
        for(unsigned int s=0; s<vGenJetProducer.size(); s++)
        {
            GenJetAnalyzer* myGenJetAnalyzer = new GenJetAnalyzer(vGenJetProducer[s], myConfig_, verbosity);
            myGenJetAnalyzer->Process(iEvent, vgenJets[s], vgenjetToken_[s]);
            delete myGenJetAnalyzer;
        }
    }

    // PFJet
    if(doPFJet)
    {
        if(verbosity>1) cout << endl << "Analysing PFjets collection..." << endl;
        for(unsigned int s=0; s<vPFJetProducer.size(); s++)
        {
            PFJetAnalyzer* myPFJetAnalyzer = new PFJetAnalyzer(myConfig_, verbosity);
            myPFJetAnalyzer->Process(iEvent, vpfJets[s], iSetup, vjetToken_[s]);
            delete myPFJetAnalyzer;
        }
    }

    // FatJet
    if(doFatJet)
    {
        if(verbosity>1) cout << endl << "Analysing Fatjets collection..." << endl;
        for(unsigned int s=0; s<vFatJetProducer.size(); s++)
        {
            FatJetAnalyzer* myFatJetAnalyzer = new FatJetAnalyzer(myConfig_, verbosity);
            myFatJetAnalyzer->Process(iEvent, vfatJets[s], iSetup, vfatjetToken_[s]);
            delete myFatJetAnalyzer;
        }
    }

    // NPGenEvent
    if(doNPGenEvent)
    {
        if(verbosity>1) cout << endl << "Analysing NPGenEvent collection..." << endl;
        NPGenEventAnalyzer* myNPGenEventAnalyzer = new NPGenEventAnalyzer(verbosity);
        myNPGenEventAnalyzer->Process(iEvent, NPgenEvent, genParticlesToken_);
        delete myNPGenEventAnalyzer;
    }

    // Muons
    if(doMuon)
    {

        if(verbosity>1) cout << endl << "Analysing muon collection..." << endl;
        for(unsigned int s=0; s<vMuonProducer.size(); s++)
        {
            MuonAnalyzer* myMuonAnalyzer = new MuonAnalyzer(myConfig_,verbosity);
            myMuonAnalyzer->Process(iEvent, vmuons[s], offlineBSToken_, vmuonToken_[s], vtxToken_);
            delete myMuonAnalyzer;

        }
    }

    // Electrons
    if(doElectron)
    {
        if(verbosity>1) cout << endl << "Analysing electrons collection..." << endl;
        for(unsigned int s=0; s<vElectronProducer.size(); s++)
        {
            ElectronAnalyzer* myElectronAnalyzer = new ElectronAnalyzer(myConfig_, verbosity);
            myElectronAnalyzer->Process(iEvent, velectrons[s], iSetup, offlineBSToken_, velectronToken_[s], vtxToken_);
            delete myElectronAnalyzer;
        }
    }

    // Photons
    if(doPhoton)
    {
        for(unsigned int s=0; s<vPhotonProducer.size(); s++)
        {
            PhotonAnalyzer* myPhotonAnalyzer = new PhotonAnalyzer(myConfig_, verbosity);
            myPhotonAnalyzer->Process(iEvent, vphotons[s], iSetup, vphotonToken_[s]);
            delete myPhotonAnalyzer;
        }
    }


    // MET

    if(doPFMET)
    {

        if(verbosity>1) cout << endl << "Analysing ParticleFlow Missing Et..." << endl;
        for(unsigned int s=0; s<vPFmetProducer.size(); s++)
        {
            PFMETAnalyzer* myPFMETAnalyzer = new PFMETAnalyzer(myConfig_, verbosity);
            myPFMETAnalyzer->Process(iEvent, vPFmets[s], vmetToken_[s]);
            delete myPFMETAnalyzer;

        }
    }




    // Associate recoParticles to mcParticles
    if(!isRealData_ && doMCAssociation)
    {
      	    MCAssociator* myMCAssociator = new MCAssociator(verbosity);
            myMCAssociator->init(iEvent, mcParticles, genParticlesToken_);
        	if(doPFJet && vpfJets.size() > 0) myMCAssociator->process(vpfJets[0]);
        	if(doMuon && vmuons.size() > 0) myMCAssociator->process(vmuons[0]);
        	if(doElectron && velectrons.size() > 0) myMCAssociator->process(velectrons[0]);
        	//if(verbosity>2 && doMuon && vmuons.size() > 0) myMCAssociator->printParticleAssociation(vmuons[0]);
        	//if(verbosity>2 && doElectron && velectrons.size() > 0) myMCAssociator->printParticleAssociation(velectrons[0]);
        	//if(verbosity>2 && doPhoton) myMCAssociator->printParticleAssociation(photons);
        	delete myMCAssociator;

    }


    if(verbosity>1) cout << endl << "Filling rootuple..." << endl;
    eventTree_->Fill();


    if(verbosity>1) cout << endl << "Deleting objects..." << endl;
    delete rootEvent;

    if(!isRealData_) (*mcParticles).Delete();

    if(!isRealData_ && doGenJet)
    {
        for(unsigned int s=0; s<vGenJetProducer.size(); s++)
        {
            (*vgenJets[s]).Delete();
        }
    }
    if(doPFJet)
    {
        for(unsigned int s=0; s<vPFJetProducer.size(); s++)
        {
            (*vpfJets[s]).Delete();
        }
    }

    if(doFatJet)
    {
        for(unsigned int s=0; s<vFatJetProducer.size(); s++)
        {
            (*vfatJets[s]).Delete();
        }
    }

    if(doMuon)
    {
        for(unsigned int s=0; s<vMuonProducer.size(); s++)
        {
            (*vmuons[s]).Delete();
        }
    }
    if(doElectron)
    {
        for(unsigned int s=0; s<vElectronProducer.size(); s++)
        {
            (*velectrons[s]).Delete();
        }
    }
    if(doPhoton)
    {
        for(unsigned int s=0; s<vPhotonProducer.size(); s++)
        {
            (*vphotons[s]).Delete();
        }
    }
    if(doPFMET)
    {
        for(unsigned int s=0; s<vPFmetProducer.size(); s++)
        {
            (*vPFmets[s]).Delete();
        }
    }



// cout<<"in top tree producer.. end of method"<<endl;

    if(doNPGenEvent) (*NPgenEvent).Delete();
    if(doPrimaryVertex) (*primaryVertex).Delete();
//    if(verbosity>0) cout << endl;
}
