#ifndef LARLITE_FIRSTTRIGGER_CXX
#define LARLITE_FIRSTTRIGGER_CXX

#include "FirstTrigger.h"
#include "DataFormat/mcshower.h"

 /*
  // Function to determine of wire is hit or not
  bool hitCount(const std::vector<short int> adcs, double T, double offset){
    // Loop over entire wire
    double x(0);
    for(size_t l(0); l<adcs.size(); l ++){
        x = adcs[l];
        if((x-offset)>T||(x-offset)<-T) return true;
    }
    return false;
  }
 
  // Function that calculates number of hits on a wire assuming the beginning of one hit and end of another are separated by 150 TDCs
  int hitPerWire(const std::vector<short int> adcs, double T, double offset){
    int hits = 0; double x(0);
    // Determine if wire is hit or not
    for(size_t l(0); l<adcs.size(); l ++){
        x = adcs[l];
        if((x-offset)>T||(x-offset)<-T){
          hits ++; // add one to hit counter
          l += 150; // skip ahead by 150 TDCs
        }
    }
    return hits;
  }
*/

namespace larlite {

  // Function that returns the TDC of the start of each hit
  std::vector<int> hitTDC(const std::vector<short int> adcs, double T, double offset) {
    // Initialize TDC variable and vector to store
    std::vector<int> v_TDCs; double x(0);
    // Determine if wire is hit or not
    for(size_t l(0); l<adcs.size(); l++){
        x = adcs[l];
        if((x-offset)>T||(x-offset)<-T){
            // if wire is hit record TDC time of when it occurs
            v_TDCs.push_back((int)l);
            l += 150;
        }
    }
    return v_TDCs;
  }


  // Function that returns maximum height of each hit in ADC units
  std::vector<double> ADCamp(const std::vector<short int> adcs, double T, double offset) {
    std::vector<double> v_ADCamp;
    double max(0), y(0), yp(0);
    // Determine if wire is hit or not
    for(size_t l; l<adcs.size(); l ++){
      y = pow(pow(adcs[l]-offset,2),0.5);
      if( y>T ){
        max = y; // If threshold is passed set max as this point
        // Loop over max size of peak and find the most extremal point
        for(size_t i(1); i<150; i++){
          yp = pow(pow(adcs[l+i]-offset,2),0.5);
          if(yp>max) max = yp;
        }
        // Push on to vector of amplitudes
        v_ADCamp.push_back(max);
        l += 150;
      }
    }
    return v_ADCamp;
  }

  // calculate mean ADC amplitude
  double ampMean(std::vector<double> ADCvec) {
    double meanADC(0);
    for(size_t f=0; f<ADCvec.size(); ++f) meanADC += ADCvec[f];
    meanADC /= ((double)ADCvec.size());
    return meanADC;
  }

  // Function to return standard deviation of TDC
  float TDCstd(std::vector<int> TDCvec){

    float stdTDC(0), meanTDC(0);

    for(size_t i(0); i<TDCvec.size(); ++i) meanTDC += ((float)TDCvec[i]); // Calculate mean
 
    meanTDC /= ((float)TDCvec.size());

    // Calculate standard deviation
    for(size_t i=0; i<TDCvec.size(); ++i) stdTDC += (TDCvec[i]-meanTDC)*(TDCvec[i]-meanTDC);
    stdTDC = sqrt( stdTDC / ((float)TDCvec.size()));
    return stdTDC;
  }

  //
  // This function is called in the beggining of event loop
  // Do all variable initialization you wish to do here.
  // If you have a histogram to fill in the event loop, for example,
  // here is a good place to create one on the heap (i.e. "new TH1D"). 
  //
  bool FirstTrigger::initialize() {
    int tnbins(0), tmax(0);
    // double T = SimpleWFAna::GetT(); // Get tolerance from python script
    h_HITS = new TH1I("h_HITS","",tnbins,0,tmax);
    T = 10;

    return true;
    
  }
  
    //
    // Do your event-by-event analysis here. This function is called for 
    // each event in the loop. You have "storage" pointer which contains 
    // event-wise data. To see what is available, check the "Manual.pdf":
    //
    // http://microboone-docdb.fnal.gov:8080/cgi-bin/ShowDocument?docid=3183
    // 
    // Or you can refer to Base/DataFormatConstants.hh for available data type
    // enum values. Here is one example of getting PMT waveform collection.
    //
    // event_fifo* my_pmtfifo_v = (event_fifo*)(storage->get_data(DATA::PMFIFO));
    //
    // if( event_fifo )
    //
    //   std::cout << "Event ID: " << my_pmtfifo_v->event_id() << std::endl;
    //
    
  bool FirstTrigger::analyze(storage_manager* storage) {

    // --------------------- INITIALISE VARIABLES ---------------------    
    int _evtN(0), Hits(0), uHits(0), vHits(0), yHits(0);
    double intADC(0), UintADC(0), VintADC(0), YintADC(0), MampADC(0), UMampADC(0), VMampADC(0), YMampADC(0);


    // --------------------- SETUP & ERROR CHECK ---------------------
    // Get mctruth data
    /*
    auto ev_mct = storage->get_data<event_mctruth>("generator");
    // auto ev_mcs = storage->get_data<event_mcshower>("mcreco");
    // Set truth data flag to 1 if data is present
    if ( (ev_mct) ){
      // for (auto const& shower : *ev_mcs)
    } 
*/
    // Get rawdigit data
    auto wfs = storage->get_data<event_rawdigit>("daq");
    // Display error if rawdigit data not present
    if ( (!wfs) || (!wfs->size())){
    	print (msg::kERROR,__FUNCTION__,"RawDigit data product not found!");
    	return false;
    }


    // --------------------- GET WIRE WAVEFORMS ---------------------
    for (size_t i=0; i < wfs->size(); i++){
      
      auto const& wf = (*wfs).at(i); // get waveform from wire i
      auto const& adcs = wf.ADCs(); // Convert from analogue to digital
 

      // Calculate Mean
      double offset(0);
      for(size_t j=0; j<adcs.size(); ++j) offset += adcs[j];
      offset /= ((double)adcs.size());

      // Calculate total number of hits and add to hit counter for event
      // Change to hitCount() for number of hit wires
      std::vector <int> Temp = hitTDC(adcs,T,offset); int TempVal = Temp.size();

      Hits += TempVal; 
      // Separate into wire planes
      if(i<2400){uHits += TempVal;}
      if(i>=2400&&i<4800){vHits += TempVal;}
      if(i>=4800&&i<8256){yHits += TempVal;}
    

      // --------------------- CALCULATE WFINT --------------------- 
      // Loop over adc vector
      for(size_t j=0; j<adcs.size()-1 ; ++j ){
        // Get offset adjusted modulus of two points
        double x1 = pow(pow(adcs[j]-offset,2),0.5);
        double x2 = pow(pow(adcs[j+1]-offset,2),0.5);
        // If both above tolerance add up area between them
        if( x1>T && x2>T ){
          intADC = intADC + 0.5*x1 + 0.5*x2;
          if(i<2400) UintADC = UintADC + 0.5*x1 + 0.5*x2; 
          if(i>=2400&&i<4800) VintADC = VintADC + 0.5*x1 + 0.5*x2;
          if(i>=4800&&i<8256) YintADC = YintADC + 0.5*x1 + 0.5*x2;
        }
      }

      // --------------------- CALCULATE TDC SPREAD --------------------- 
      // Get TDCs of hits and sort into planes
      std::vector<int> v_TDCs = hitTDC(adcs,T,offset);
      for (size_t j=0; j<v_TDCs.size(); ++j) {
        TDCvec.push_back(v_TDCs[j]);
        if(i<2400) UTDCvec.push_back(v_TDCs[j]);
        if(i>=2400&&i<4800) VTDCvec.push_back(v_TDCs[j]);
        if(i>=4800&&i<8256) YTDCvec.push_back(v_TDCs[j]);
      }

      // --------------------- CALCULATE TDC STDDEV --------------------- 
      // Calculate standard deviations of TDCs of hits in each plane
      float stdTDC = TDCstd(TDCvec); float UstdTDC = TDCstd(UTDCvec); float VstdTDC = TDCstd(VTDCvec); float YstdTDC = TDCstd(YTDCvec);
      // If not a number set to zero
      if(isnan(stdTDC)==1) stdTDC=0;
      if(isnan(UstdTDC)==1) UstdTDC=0;
      if(isnan(VstdTDC)==1) VstdTDC=0;
      if(isnan(YstdTDC)==1) YstdTDC=0;

      // --------------------- CALCULATE ADC AMPLITUDE --------------------- 
   
      // Get ADC amplitudes of hits and sort into planes
      std::vector<double> v_ADCamp = ADCamp(adcs,T,offset);
      for (size_t j=0; j<v_ADCamp.size(); ++j) {
        ADCvec.push_back(v_ADCamp[j]);
        if(i<2400){UADCvec.push_back(v_ADCamp[j]);}
        if(i>=2400&&i<4800){VADCvec.push_back(v_ADCamp[j]);}
        if(i>=4800&&i<8256){YADCvec.push_back(v_ADCamp[j]);}
      }

      // Calculate mean and standard deviation of amplitudes
      if(Hits!=0){MampADC = ampMean(ADCvec);}else{MampADC=0;}
      if(uHits!=0){UMampADC = ampMean(UADCvec);}else{UMampADC=0;}
      if(vHits!=0){VMampADC = ampMean(VADCvec);}else{VMampADC=0;}
      if(yHits!=0){YMampADC = ampMean(YADCvec);}else{YMampADC=0;}

      // Fill vector of integers with event number
      eventNo.push_back(_evtN);

      _evtN += 1;   
      hitNo.push_back(Hits); uhitNo.push_back(uHits); vhitNo.push_back(vHits); yhitNo.push_back(yHits);
      ADCvec.push_back(MampADC); UADCvec.push_back(UMampADC); YADCvec.push_back(YMampADC); VADCvec.push_back(VMampADC); }
 
    return true;
  }

    // This function is called at the end of event loop.
    // Do all variable finalization you wish to do here.
    // If you need, you can store your ROOT class instance in the output
    // file. You have an access to the output file through "_fout" pointer.
    //
    // Say you made a histogram pointer h1 to store. You can do this:
    //
    // if(_fout) { _fout->cd(); h1->Write(); }
    //
    // else 
    //   print(MSG::ERROR,__FUNCTION__,"Did not find an output file pointer!!! File not opened?");
    //
 

  bool FirstTrigger::finalize() {

    // Get file names from python script
    std::string fileName = larlite::FirstTrigger::GetName();

    // Save histograms to .root file
    TFile* outfile = new TFile(fileName.c_str() , "RECREATE");
    h_HITS->Write();
    delete h_HITS;
    outfile->Close();

    return true;
  }

}
#endif
