1#include "TFile.h"
#include "TTree.h"
#include "TGraph.h"
#include "TCanvas.h"

void PlotData() {
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
    Int_t time, channel, fileNum;
    Double_t current;
    tree->SetBranchAddress("channel", &channel);
    tree->SetBranchAddress("time", &time);
    tree->SetBranchAddress("current", &current);
    tree->SetBranchAddress("fileNum",&fileNum);

    // Loop over entries in the tree and fill the arrays
    Long64_t nEntries = tree->GetEntries();
    for (Long64_t i = 0; i < nEntries; ++i) {
        tree->GetEntry(i);
        time_vec.push_back(double(time));
	channel_vec.push_back(double(channel));
	//std::cout<<time<<std::endl;
        current_vec.push_back(current);
    }

    // Create a TGraph from the data
    TGraph *graph = new TGraph(nEntries, &time_vec[0], &current_vec[0]);
    // Create a canvas to draw the graph
    TCanvas *canvas = new TCanvas("canvas", "Current Versus Time Scatter Plot", 800, 600);
    // Draw the graph on the canvas
    graph->SetMarkerStyle(20);
    graph->SetMarkerSize(0.5);
    graph->SetTitle("Current Versus Time Scatter Plot");
    graph->GetXaxis()->SetTitle("Time");
    graph->GetYaxis()->SetTitle("Current");
    graph->Draw("AP");
    // Save the canvas as an image (optional)
    canvas->SaveAs("CurentlVsTimeScatterPlot.png");

    TGraph *graph2 = new TGraph(nEntries, &channel_vec[0], &current_vec[0]);
    // Create a canvas to draw the graph
    canvas->SetTitle("Current Versus Channel Scatter Plot");
    // Draw the graph on the canvas
    graph2->SetMarkerStyle(20);
    graph2->SetMarkerSize(0.5);
    graph2->SetTitle("Current Versus Channel Scatter Plot");
    graph2->GetXaxis()->SetTitle("Channel");
    graph2->GetYaxis()->SetTitle("Current");
    graph2->Draw("AP");
    // Save the canvas as an image (optional)
    canvas->SaveAs("CurrentVersusTimeScatterPlot.png");

    TGraph *graph3 = new TGraph(nEntries, &time_vec[0], &channel_vec[0]);
    // Create a canvas to draw the graph
    canvas->SetTitle("Current Versus Channel Scatter Plot");
    // Draw the graph on the canvas
    graph3->SetMarkerStyle(20);
    graph3->SetMarkerSize(0.5);
    graph3->SetTitle("Current Versus Channel Scatter Plot");
    graph3->GetXaxis()->SetTitle("Channel");
    graph3->GetYaxis()->SetTitle("Current");
    graph3->Draw("AP");
    // Save the canvas as an image (optional)
    canvas->SaveAs("CurrentVersusChannelScatterPlot.png");

    // Clean up
    //delete graph;
    //delete canvas;
    //file->Close();
}
