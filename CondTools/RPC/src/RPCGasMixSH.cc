/*
 *  See headers for a description
 *
 *  \author D. Pagano - Dip. Fis. Nucl. e Teo. & INFN Pavia
 */

#include "CondTools/RPC/interface/RPCGasMixSH.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CondCore/DBOutputService/interface/PoolDBOutputService.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include<iostream>

popcon::RpcDataGasMix::RpcDataGasMix(const edm::ParameterSet& pset) :
  m_name(pset.getUntrackedParameter<std::string>("name","RpcData")),
  host(pset.getUntrackedParameter<std::string>("host", "source db host")),
  user(pset.getUntrackedParameter<std::string>("user", "source username")),
  passw(pset.getUntrackedParameter<std::string>("passw", "source password")),
  m_since(pset.getUntrackedParameter<unsigned long long>("since",5)),
  m_till(pset.getUntrackedParameter<unsigned long long>("till",0)){
}

popcon::RpcDataGasMix::~RpcDataGasMix()
{
}

void popcon::RpcDataGasMix::getNewObjects() {

  std::cout << "------- " << m_name << " - > getNewObjects\n" 
	    << "got offlineInfo"<< tagInfo().name 
	    << ", size " << tagInfo().size << ", last object valid since " 
	    << tagInfo().lastInterval.first << " token "   
            << tagInfo().lastPayloadToken << std::endl;

  std::cout << " ------ last entry info regarding the payload (if existing): " 
	    << logDBEntry().usertext << "last record with the correct tag has been written in the db: "
	    << logDBEntry().destinationDB << std::endl; 
  
  //  snc = tagInfo().lastInterval.first;


   std::cout << std::endl << "=============================================" << std::endl;
   std::cout << std::endl << "===================  MIX   ==================" << std::endl;
   std::cout << std::endl << "=============================================" << std::endl << std::endl;
   snc = m_since;
   std::cout << ">> Range mode [" << snc << ", " << m_till << "]" << std::endl;
   std::cout << std::endl << "=============================================" << std::endl << std::endl;
   

   
   RPCFw caen ( host, user, passw );
   std::vector<RPCObGasMix::Item> GasMixcheck;
   GasMixcheck = caen.createMix(snc, m_till);
   GasMixdata = new RPCObGasMix();
   RPCObGasMix::Item GasMixfill;
   std::vector<RPCObGasMix::Item>::iterator GasMixit;
   for(GasMixit = GasMixcheck.begin(); GasMixit != GasMixcheck.end(); GasMixit++)
     {
       GasMixfill = *(GasMixit);
       GasMixdata->ObGasMix_rpc.push_back(GasMixfill);
     }
   std::cout << " >> Final object size: " << GasMixdata->ObGasMix_rpc.size() << std::endl;  

   if (GasMixdata->ObGasMix_rpc.size() > 0) {
     niov = snc;
   } else {
     niov = snc;
     std::cout << "NO DATA TO BE STORED" << std::endl;
   }


   ::timeval tv;
   tv.tv_sec = niov;
   tv.tv_usec = 0;
   edm::Timestamp tmstamp((unsigned long long)tv.tv_sec*1000000+(unsigned long long)tv.tv_usec);
   std::cout << "UNIX time = " << tmstamp.value() << std::endl;

   edm::TimeValue_t daqtime=0LL;
   daqtime=tv.tv_sec;
   daqtime=(daqtime<<32)+tv.tv_usec;
   edm::Timestamp daqstamp(daqtime);
   edm::TimeValue_t dtime = daqstamp.value();
   std::cout<<"DAQ time = " << dtime <<std::endl;

   niov = dtime;

   std::cout << "===> New IOV: since is = " << niov << std::endl;
   m_to_transfer.push_back(std::make_pair((RPCObGasMix*)GasMixdata,niov));
}

