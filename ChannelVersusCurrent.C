#include "TFile.h"
#include "TTree.h"
#include "TH2F.h"
#include "TCanvas.h"

void ChannelVersusCurrent() {
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

    // Create a canvas to draw the histogram
    TCanvas *canvas = new TCanvas("canvas", "Channel vs Current Histogram", 800, 600);

    // Create a 2D histogram
    TH2F *histogram = new TH2F("histogram", "Channel vs Currnet", 256, -0.5, 256.5, 60, 0, 12);

    // Fill the histogram from the tree
    tree->Draw("value:channel>>histogram", "", "colz");

    // Draw the histogram on the canvas
    histogram->Draw("colz");
    
    // Adjust the display
    canvas->SetRightMargin(0.15);
    gPad->Update();

    // Save the canvas as an image (optional)
    canvas->SaveAs("ChannelVsValueHistogram.png");

    // Clean up
    //delete canvas;
    //file->Close();
}
