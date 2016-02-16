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

namespace larlite {
  /**
     \class FirstTrigger
     User custom analysis class made by SHELL_USER_NAME
   */
  class FirstTrigger : public ana_base{

  private:

    // Declare histograms
    TH1I  *h_HITS; 
    std::string name;
    double T;

    std::vector<double> hitNo, uhitNo, vhitNo, yhitNo, ADCvec, UADCvec, VADCvec, YADCvec, intvec, Uintvec, Vintvec, Yintvec;
    std::vector<int> eventNo, TDCvec, UTDCvec, VTDCvec, YTDCvec;


  public:
    /// Default constructor
    FirstTrigger(double Tolerance, std::string nm){ _name="exampleAnaunit"; _fout=0 ; T=Tolerance; name=nm; };

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

    std::string GetName() {
      return name;
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
