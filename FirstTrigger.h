/**
 * \file FirstTrigger.h
 *
 * \ingroup ShowerTrig
 * 
 * \brief Class def header for a class FirstTrigger
 *
 * @author luke
 */

/** \addtogroup ShowerTrig

    @{*/

#ifndef LARLITE_FIRSTTRIGGER_H
#define LARLITE_FIRSTTRIGGER_H

#include "Analysis/ana_base.h"
#include <map>
#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <cmath>
#include "DataFormat/rawdigit.h"
#include "DataFormat/mctruth.h"
#include <TROOT.h>
#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TFrame.h>
#include <TAxis.h>
#include <TAttLine.h>
#include <TChain.h>
#include <TFile.h>
#include <string>
#include <fstream>
#include <map>
#include <time.h>

namespace larlite {
  /**
     \class FirstTrigger
     User custom analysis class made by SHELL_USER_NAME
   */
  class FirstTrigger : public ana_base{

  private:

    int _evtN;
    int _hitNo, _hitNoU, _hitNoV, _hitNoY; 
    float _TDCstd, _TDCstdU, _TDCstdV, _TDCstdY;
    float _TDCiqr, _TDCiqrU, _TDCiqrV, _TDCiqrY;
    float _ADCamp, _ADCampU, _ADCampV, _ADCampY;
    float _WFint, _WFintU, _WFintV, _WFintY;
    
    std::map <int,int> _HitMap;
    std::map <int,int> _ClusterWires;
    std::map <int,int> _ClusterWiresTwo;

    // TDC standard deviations
    float stdTDC, UstdTDC, VstdTDC, YstdTDC;
    // Mean amplitudes
    double MampADC, UMampADC, VMampADC, YMampADC, SDampADC, USDampADC, VSDampADC, YSDampADC;
    // TDC interquartile ranges
    int iqrTDC, UiqrTDC, ViqrTDC, YiqrTDC;
    // Numbers of hits
    int _isHit, uHit ,vHit, yHit;
    // integrated ADC waveforms
    double intADC,UintADC,VintADC,YintADC;

    // wire and event number
    int wire, event;    

    double Tolerance;
    // file names for use in simplewfana.py
    std::string nm, fnm;

    // Vectors for TDC times of hits
    std::vector<int> TDCvec, UTDCvec, VTDCvec, YTDCvec;
    // Vectors for ADC amplitudes of hits
    std::vector<double> ADCvec, UADCvec, VADCvec, YADCvec;

    std::vector<int> WireNumbers, ClusterSize, WireNumbersTwo, ClusterSizeTwo;
 
    TTree* _t_ch;


  public:
    /// Default constructor
    FirstTrigger(double T, std::string name){ _name="exampleAnaunit"; _fout=0 ; Tolerance=T; nm=name; };

    /// Default destructor
    virtual ~FirstTrigger(){}

    /** IMPLEMENT in FirstTrigger.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in FirstTrigger.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in FirstTrigger.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    // Accessor function for tolerance
    double GetT() {
      return Tolerance;
    }
    // Accessor functions for file names
    std::string GetName() {
      return nm;
    }

   
  };
}
#endif

//**************************************************************************
// 
// For Analysis framework documentation, read Manual.pdf here:
//
// http://microboone-docdb.fnal.gov:8080/cgi-bin/ShowDocument?docid=3183
//
//**************************************************************************

/** @} */ // end of doxygen group 
