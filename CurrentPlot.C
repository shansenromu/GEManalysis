#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TCanvas.h"

void CurrentPlot() {
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

    // Create a histogram for the value
    TH1F *hist = new TH1F("hist", "Value Histogram", 100, 0, 10); // Adjust the range and binning as needed

    // Fill the histogram from the tree
    Double_t current;
    tree->SetBranchAddress("current", &current);
    Long64_t nEntries = tree->GetEntries();
    for (Long64_t i = 0; i < nEntries; ++i){
        tree->GetEntry(i);
        hist->Fill(current);
    }

    // Create a canvas to draw the histogram
    TCanvas *canvas = new TCanvas("canvas", "Value Histogram", 800, 600);

    // Draw the histogram on the canvas
    hist->GetXaxis()->SetTitle("Current");
    hist->GetYaxis()->SetTitle("Frequency");
    hist->Draw();

    // Save the canvas as an image (optional)
    canvas->SaveAs("Current.png");

    // Clean up
    delete hist;
    delete canvas;
    file->Close();
}
