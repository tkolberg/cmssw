#ifndef HLTrigger_JetMET_HLTAlphaTFilter_h
#define HLTrigger_JetMET_HLTAlphaTFilter_h

/** \class HLTAlphaTFilter
 *
 *  \author Bryn Mathias
 *
 */

#include "HLTrigger/HLTcore/interface/HLTFilter.h"

namespace edm {
   class ConfigurationDescriptions;
}


//
// class declaration
//
template<typename T>
class HLTAlphaTFilter : public HLTFilter {

   public:
      explicit HLTAlphaTFilter(const edm::ParameterSet&);
      ~HLTAlphaTFilter();
      static void fillDescriptions(edm::ConfigurationDescriptions & descriptions);
      virtual bool hltFilter(edm::Event&, const edm::EventSetup&, trigger::TriggerFilterObjectWithRefs & filterproduct);
      
   private:
      edm::InputTag inputJetTag_;           // input tag identifying jets
      edm::InputTag inputJetTagFastJet_;    // input tag identifying a second collection of jets
      std::vector<double> minPtJet_;
      std::vector<double> etaJet_;
      unsigned int maxNJets_;
      double minHt_;
      double minAlphaT_;
      int triggerType_;
};

#endif // HLTrigger_JetMET_HLTAlphaTFilter_h
