#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TCanvas.h"
#include <vector>
#include <map>

using namespace std;

void AllPositionPlots() {
    // Open the ROOT file containing the tree
    TFile *file = TFile::Open("GEMdata.root");
    if (!file || file->IsZombie()) {
        cerr << "Error: Unable to open ROOT file!" << endl;
        return;
    }

    // Get the tree from the file
    TTree *tree;
    file->GetObject("data_tree", tree);
    if (!tree) {
        cerr << "Error: Unable to retrieve tree from ROOT file!" << endl;
        file->Close();
        return;
    }

    // Create arrays to hold the data
    vector<double> time_vec;
    vector<double> channel_vec;
    vector<double> current_vec;

    // Fill the arrays from the tree
    Int_t time, channel, NumFile;
    Double_t current;
    tree->SetBranchAddress("channel", &channel);
    tree->SetBranchAddress("time", &time);
    tree->SetBranchAddress("current", &current);
    tree->SetBranchAddress("NumFile",&NumFile);

    TH1D * hx0 = new TH1D("hx0", "hx0", 128,0.5,128.5);
    TH1D * hy0 = new TH1D("hy0", "hy0", 128,0.5,128.5);

    // Map to store histograms for different file numbers and times
    map<pair<int, int>, pair<TH1D*,TH1D*>> histogramsMap;

    // Loop over entries in the tree and fill the arrays
    Long64_t nEntries = tree->GetEntries();
    for (Long64_t i = 0; i < nEntries; ++i) {
        tree->GetEntry(i);
	int SomeNum = int(i/512/256);
	
	//cout<< time<<" "<<channel<<" "<<NumFile<<" "<<SomeNum<<" "<<current<<endl;
	//Create historgrams for every time file number pair.
	pair<int, int> fileTimePair = make_pair(SomeNum, time);
	if(histogramsMap.find(fileTimePair) == histogramsMap.end()){
	  string xh_name = Form("h1d_x_%d_%d",SomeNum,time);
	  string yh_name = Form("h1d_y_%d_%d",SomeNum,time);
	  
	  TH1D * hx = new TH1D(xh_name.c_str(), xh_name.c_str(), 128,0.5,128.5);
	  TH1D * hy = new TH1D(yh_name.c_str(), yh_name.c_str(), 128,0.5,128.5);
	  histogramsMap[fileTimePair] = make_pair(hx, hy);  
	}
	if ( SomeNum ==0){
	  if ( channel <= 128) {
	    histogramsMap[fileTimePair].first->SetBinContent(channel, 0);
	    hx0->SetBinContent(channel, current-4.5);
	  } else{
	    histogramsMap[fileTimePair].second->SetBinContent(channel-128, 0);
	    hy0->SetBinContent(channel-128, current-4.5);
	  }
	} else{
	  if ( channel <= 128) {
	    histogramsMap[fileTimePair].first->SetBinContent(channel, current-4.5-hx0->GetBinContent(channel));
	  } else{
	    histogramsMap[fileTimePair].second->SetBinContent(channel-128, current-4.5-hy0->GetBinContent(channel-128));
	  }
	}

        
    }

    
    

    cout << "Number of histograms: " <<histogramsMap.size() << endl;
    TH2D* h2D = new TH2D("h2D","h2D",128,0.5,128.5,128,0.5,128.5);
    TH1D* h1Dx = new TH1D("h1DXX","h1DXX",128,0.5,128.5);
    TH1D* h1Dy = new TH1D("h1DYY","h1DYY",128,0.5,128.5);
    TFile * outputFile = new TFile("histograms.root","RECREATE");
    double cut = 1;
    double xVoltage, yVoltage;
    for(auto &entry : histogramsMap){
      entry.second.first->SetLineColor(kRed);
      entry.second.second->SetLineColor(kBlue);  
     
      for (int k = 1; k<=128;k++){
	for(int j =1; j<=128;j++){
	  xVoltage = entry.second.first->GetBinContent(k);
	  yVoltage = entry.second.second->GetBinContent(j);
	  if (xVoltage <-cut && yVoltage<-cut){
	    h2D->Fill(k,j, xVoltage+yVoltage);

	  }
	}
      }
      
      entry.second.first->Write();
      entry.second.second->Write();
    }
    h2D->Write();
    TCanvas *canvas = new TCanvas("canvas", "Value Histogram", 800, 600);

    // Draw the histogram on the canvas
    h2D->GetXaxis()->SetTitle("Channels 1-128");
    h2D->GetYaxis()->SetTitle("Channels 129-256 (-128)");
    h2D->Write();
    h2D->Draw("colz");

    // Save the canvas as an image (optional)
    canvas->SaveAs("Reconstuction.png");

    outputFile->Close();
    


    // Clean up
    file->Close();
}
