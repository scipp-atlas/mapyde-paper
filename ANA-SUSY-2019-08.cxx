#include "SimpleAnalysisFramework/AnalysisClass.h"

DefineAnalysis(EwkOneLeptonTwoBjets2018)
// Wh->1l+bb+met analysis (Run2 data)
// https://arxiv.org/pdf/1909.09226.pdf

void EwkOneLeptonTwoBjets2018::Init()
{
	// Define signal/control regions
	addRegions({"SR_h_Low", "SR_h_Med", "SR_h_High"} );
	addRegions({"SR_h_Low_Incl", "SR_h_Med_Incl", "SR_h_High_Incl"} );
	addRegions({"SR_h_Low_bin1", "SR_h_Low_bin2", "SR_h_Low_bin3"});
	addRegions({"SR_h_Med_bin1", "SR_h_Med_bin2", "SR_h_Med_bin3"});
	addRegions({"SR_h_High_bin1", "SR_h_High_bin2", "SR_h_High_bin3"});
	addRegions({"CR_tt_Low", "CR_tt_Med", "CR_tt_High"});
	addRegions({"WCR", "STCR"});
	addRegions({"VR_off_Low", "VR_off_Med", "VR_off_High"});
	addRegions({"VR_on_Low", "VR_on_Med", "VR_on_High"});

	// Preselection for debugging
	addRegions({"presel_1L", "presel_2J", "presel_bb", "presel_met", "presel_mbb"});
}

void EwkOneLeptonTwoBjets2018::ProcessEvent(AnalysisEvent *event)
{
	//
	// Soft Objects
	//
	auto baseEle = event->getElectrons(7,2.47, ELooseBLLH);
	auto baseMuon = event->getMuons(6,2.70, MuMedium | MuNotCosmic | MuZ05mm | MuQoPSignificance);
	auto baseJets = event->getJets(20.,4.5);
	auto met_Vect  = event->getMET();
	float met       = met_Vect.Pt();
	auto weights        = event->getMCWeights();

	//
	// Baseline objects
	//
	auto radiusCalcLep = [] (const AnalysisObject& lep,const AnalysisObject&) {
							 return (0.04 + 10/lep.Pt()) > 0.4 ? 0.4 : 0.04 + 10/lep.Pt();
						 };

	// ele-mu OR
	baseEle = overlapRemoval(baseEle, baseMuon, 0.01);
	// ele-jet OR
	baseJets = overlapRemoval(baseJets, baseEle, 0.2);
	baseEle = overlapRemoval(baseEle, baseJets, radiusCalcLep);
	// mu-jet OR
	baseJets = overlapRemoval(baseJets, baseMuon, 0.2, LessThan3Tracks);
	baseMuon = overlapRemoval(baseMuon, baseJets, radiusCalcLep);

	// Get signal electrons with FCLoose for pT < 200 GeV and FCHighPtCaloOnly for pT > 200
	// filterObjects() only allows lower pT cut, so work around that
	auto signalEle = filterObjects(baseEle,7., 2.47, ETightLH | ED0Sigma5  | EZ05mm  | EIsoFCLoose);
	AnalysisObjects signalEleLowPt;
    for (const auto& ele : signalEle) {
      if ((ele.Pt() < 200.)) signalEleLowPt.push_back(ele);
  	}
	auto signalEleHighPt = filterObjects(baseEle,200., 2.47, ETightLH | ED0Sigma5  | EZ05mm  | EIsoFCHighPtCaloOnly);
	signalEle = signalEleLowPt + signalEleHighPt;

	auto signalMuon = filterObjects(baseMuon,6., 2.7, MuD0Sigma3 | MuZ05mm | MuIsoFCLoose);
	auto signalLept =  signalEle + signalMuon;

	auto signalJets = filterObjects(baseJets, 30., 2.80, JVT120Jet);
	auto signalBJets = filterObjects(signalJets, 30., 2.8, BTag77MV2c10);

    unsigned int N_baseEle = baseEle.size();
	unsigned int N_baseMuon = baseMuon.size();
	unsigned int N_baseLept = N_baseEle + N_baseMuon;

	unsigned int N_signalEle = signalEle.size();
	unsigned int N_signalMuon = signalMuon.size();
	unsigned int N_signalLept = N_signalEle + N_signalMuon;
	unsigned int N_signalJets = signalJets.size();
	unsigned int N_signalBJets = signalBJets.size();

	float mt=0, m_CT=0,  mbb=0, mlb1=0;
	if (signalLept.size()==1   && signalBJets.size()==2 ) {
		mt   = calcMT(signalLept[0], met_Vect);
		m_CT = calcMCT(signalBJets[0],signalBJets[1],met_Vect);
		mbb= (signalBJets[0]+signalBJets[1]).M();
		mlb1= (signalBJets[0]+signalLept[0]).M();
	}

	if(N_signalLept == 1 && N_baseLept == 1) accept("presel_1L");
	if(N_signalJets<=3 && N_signalJets >= 2) accept("presel_2J");
	if(N_signalBJets == 2) accept("presel_bb");
	if(met> 220. && mt> 50.) accept("presel_met");
	if(mbb>100 && mbb<=140) accept("presel_mbb");

	// Preselection
	if(N_baseLept != 1) return;
	if(N_signalLept != 1) return;
	if(N_signalJets>3 || N_signalJets < 2) return;
	if(N_signalBJets != 2) return;
	if(mt< 50.) return;
	if(met< 220.) return;

	ntupVar("AnalysisType", (baseMuon.size() == 1) ? 2 : 1);
	ntupVar("met",met);
	ntupVar("mct2",m_CT);
	ntupVar("mbb",mbb);
	ntupVar("mlb1",mlb1);
	ntupVar("mt",mt);
	ntupVar("nBJet",met);
	ntupVar("nJet30",(int)signalJets.size());
    ntupVar("nBJet30_MV2c10",(int)signalBJets.size());

	ntupVar("lep1Pt", signalLept[0].Pt());
    ntupVar("lep1Eta", signalLept[0].Eta());
    ntupVar("lep1Phi", signalLept[0].Phi());

	ntupVar("jet1Pt", signalJets[0].Pt());
    ntupVar("jet1Eta", signalJets[0].Eta());
    ntupVar("jet1Phi", signalJets[0].Phi());

	ntupVar("nLep_signal", (int)signalLept.size());
    ntupVar("nLep_base", (int)N_baseLept);

	// Signal regions
	if(met>240 && mbb>100 && mbb<=140 && m_CT>180) {
		if(mt>100 && mt<160) {
			accept("SR_h_Low");
			if(m_CT > 180 && m_CT <= 230) {
				accept("SR_h_Low_bin1");
			}else if(m_CT > 230 && m_CT <= 280) {
				accept("SR_h_Low_bin2");
			}else{
				accept("SR_h_Low_bin3");
			}
		}
		else if(mt>160 && mt<240) {
			accept("SR_h_Med");
			if(m_CT > 180 && m_CT <= 230) {
				accept("SR_h_Med_bin1");
			}else if(m_CT > 230 && m_CT <= 280) {
				accept("SR_h_Med_bin2");
			}else{
				accept("SR_h_Med_bin3");
			}
		}
		else if(mt>240 && mlb1 > 120) {
			accept("SR_h_High");
			if(m_CT > 180 && m_CT <= 230) {
				accept("SR_h_High_bin1");
			}else if(m_CT > 230 && m_CT <= 280) {
				accept("SR_h_High_bin2");
			}else{
				accept("SR_h_High_bin3");
			}
		}
	}
	if(met>240 && mbb>100 && mbb<=140 && m_CT>180) {
		if(mt > 100) {
			accept("SR_h_Low_Incl");
		}
		if(mt >= 160) {
			accept("SR_h_Med_Incl");
		}
		if(mt > 240 && mlb1 > 120) {
			accept("SR_h_High_Incl");
		}
	}

	// Control  regions W
	if(met>240 && mbb<80 && mbb > 50 && mt<100 && mt>50 && m_CT>180) {
		accept("WCR");
	}

	// Control  regions tt
	if(met>240 && mt>100 && m_CT<180 && ((mbb < 100 && mbb > 50)|| mbb > 140)) {
		if(mt < 160) {
			accept("CR_tt_Low");
		}else if(mt < 240) {
			accept("CR_tt_Med");
		}else if(mlb1 > 120) {
			accept("CR_tt_High");
		}
	}

	// Control  regions ST
	if(met>240 && m_CT>180 && mt > 100 && mbb > 195) {
		accept("STCR");
	}

	// validation regions on
	if(mbb>100 && mbb<140 && m_CT<180 && mt > 100) {
		if(mt < 160 && met > 240) {
			accept("VR_on_Low");
		}else if(met > 240 && mt < 240 && mt > 160) {
			accept("VR_on_Med");
		}else if(mt > 240) {
			accept("VR_on_High");
		}
	}

	// validation regions off
	if(((mbb > 50 && mbb<80) ||( mbb>160  && mbb<195))&& m_CT>180 && mt > 100) {
		if(mt<160) {
			accept("VR_off_Low");
		}else if(met > 240 && mt < 240 && mt > 160) {
			accept("VR_off_Med");
		}else if(mt > 240 && ((mbb > 50 && mbb < 75)||(mbb > 165 && mbb < 195))) {
			accept("VR_off_High");
		}
	}

	return;
}
