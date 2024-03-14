#include "TFile.h"
#include "TTree.h"
#include "TGraph.h"
#include "TCanvas.h"

void PositionPlots() {
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
    vector<double> time_vec1;
    vector<double> channel_vec1;
    vector<double> current_vec1;

    vector<double> time_vec2;
    vector<double> channel_vec2;
    vector<double> current_vec2;


    // Fill the arrays from the tree
    Int_t time, channel, fileNum;
    Double_t current;
    tree->SetBranchAddress("channel", &channel);
    tree->SetBranchAddress("time", &time);
    tree->SetBranchAddress("current", &current);
    tree->SetBranchAddress("fileNum",&fileNum);

    TH1D *h_x =new TH1D("h_x", "h_x",128,1-0.5,128+0.5);
    TH1D *h_y =new TH1D("h_y", "h_y",128,1-0.5,128+0.5);

    
    // Loop over entries in the tree and fill the arrays
    Long64_t nEntries = tree->GetEntries();
    for (Long64_t i = 0; i < nEntries; ++i) {
        tree->GetEntry(i);
        if(fileNum <2 && channel<10 && time<10){
	  std::cout<<channel<<" "<<time<<" "<<current<<" "<<fileNum<<std::endl;
	}

	if(time == 1 && fileNum ==0){
	  
	  if(channel<=128){
	    time_vec1.push_back(double(time));
	    channel_vec1.push_back(double(channel));
	    //std::cout<<time<<std::endl;
	    current_vec1.push_back(current);
	    h_x->SetBinContent(channel, current);
	  }
	  if(channel>128){
	    time_vec1.push_back(double(time));
	    channel_vec1.push_back(double(channel));
	    //std::cout<<time<<std::endl;
	    current_vec1.push_back(current);
	    h_y->SetBinContent(channel-128,current);
	  }
	}
    }

    TH2D* h2D = new TH2D("h2D","h2D",h_x->GetNbinsX(),0,h_x->GetNbinsX(),h_y->GetNbinsX(),0,h_y->GetNbinsX());
    for (int i = 1; i<=128;i++){
      for(int j =1; j<=128;j++){
	h2D->Fill(i,j, h_x->GetBinContent(i)+h_y->GetBinContent(j));

      }
    }


    h2D->SetStats(0);
    TCanvas * canvas = new TCanvas("canvas", "Pixels", 800,600);
    h2D->Draw("colz");
    canvas->SaveAs("2D_ChannelVersusCurrent.png");

    // Clean up
    //delete graph;
    //delete canvas;
    //file->Close();
}
