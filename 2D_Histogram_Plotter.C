#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <math.h>

{
  //INSTRUCTIONS:
  //1. In the same directory as this Macro (2D_Histogram_Plotter.C) there must exist a folder named "CSV_Files".
  //2. Inside the "CSV_Files" folder must contain all csv files of the plots you want to make.
  //3. For each "_mmt" "_ntd" or "_g4" csv files, there must be a corrosponding "_all" for the efficency calculations.
  //4. The code will automatically plot all the csv files inside folder and create a new folder folder containing 8 PNG plots under the same naming convention.
  //5. Make sure ROOT is started inside the same directory as this Macro and the folder.
  
  vector<string> filelist;
  const char *dirname="./CSV_Files";
  const char *ext=".csv";                    //EXTENTION of the Files WE Want
  const char *extall ="_all.csv";            // EXTENTION of the FILES WE DONT WANT
  TSystemDirectory dir(dirname, dirname); 
  TList *files = dir.GetListOfFiles();
  if (files)
    {
      TSystemFile *file; TString fname;
      TIter next(files);
      while ((file=(TSystemFile*)next()))
	{
	  fname = file->GetName();
	  if (!file->IsDirectory() && fname.EndsWith(ext))
	    {
	      cout << fname.Data() << endl; //PRINT OUT TO CONSLE OF ALL THE FILES
	      if (!fname.EndsWith(extall))
		filelist.push_back(fname.Data()); //THE FILES WE WANT TO PLOT (WITHOUT _all.csv Files)
	    }
	}
    }

 

  for (int j = 0; j<filelist.size(); j++) //start of the for loop file cycling
    {
  //DELCARE the file name Mass_Charge_Detector
      string file = filelist[j];  //the program will be looped and 'file' will be changed loop

  //REMOVING THE .CSV ending for naming use    
  string filename = "";
  for (int i = 0; i<file.size(); i++){
     if (file[i] != '.')
       {
	filename += file[i];
       }
     else
       break;
    }
  

  
  //Try to take information from the title
  int i1 = 0; //2 indexes for sandwiching terms I need i1 i2
  float mass = 0;
  float charge = 0; 
  int tracker = 1; //1 for mass, 2 for charge
  string detector_name;
  string filechar; //used to convert char into string
  for(int i2 = 0; i2<file.length();i2++)
    {
      filechar = file[i2]; //picking out the strings inside file
      if (filechar == "_")
	if (tracker == 1)
	  for (int i = i1; i <i2; i++)
	    {
	      filechar = file[i];
	      mass +=  stof(filechar)*pow(10,(i2-i-1)); //acount for the correct 10s 100s 1s
	      i1 = i2+1; //now we only look past the mass numbers
	      tracker = 2; //move on to charge
	    }
	else if(tracker == 2)
	  for (int i = i1; i < i2; i++)
	      {
		filechar = file[i];
		charge += stof(filechar)*pow(10,(i2-i-1)); //power for the correct decimal point
		i1 = i2 +1; //now move on to the name
	      }
      if (filechar == ".")
	for (int i = i1; i<i2; i++)
	  {
	    char filecharacter = file[i];
	      detector_name += filecharacter;
	  }
    }
  
  //REFORMATING OF THE DETECTOR NAMES
  if (detector_name == "g4"){
    detector_name = "g4_NTD";}
  else if (detector_name == "mmt"){
    detector_name = "MMT";}
  else if (detector_name =="ntd"){
    detector_name = "NTD";}
  else{
    cout<< "Unidentifiable Dectector Format" <<endl;
    std::exit(0); }
 

  // PART 1 THE ALL Data Analysis -- INDEPENDANT OF THE IMPORT FILE
  //declare the strings for placeholder in _all files
  string col1_all, col2_all, col3_all, col4_all, col5_all, col6_all, col7_all,
    col8_all, col9_all, col10_all, col11_all, col12_all, col13_all, line;

  //Declare Variables We Need To Track
  vector<double> Event_all;

  //automatically declare the corropsonding _all file
  ostringstream filename_all;
  filename_all << (int)mass << "_" << (int)charge << "_all.csv";
  string filelocation_all = "CSV_Files/" + filename_all.str();

  cout << "Charge (gD): " << charge << endl;
  cout << "Mass (GeV): " << mass << endl;
  
  //Open File
  ifstream myFile_all(filelocation_all.c_str());
  //Discard First Row
  getline(myFile_all,line);
  
  //Track Rows in _all file
  int row_total_all = 0;

  //Begin Reading Sequence
  while (!myFile_all.eof())
    {
      getline(myFile_all,col1_all,',');
      if (col1_all == ""){
	break; }
      Event_all.push_back(stof(col1_all));

      getline(myFile_all,col2_all,',');
      getline(myFile_all,col3_all,',');
      getline(myFile_all,col4_all,',');
      getline(myFile_all,col5_all,',');
      getline(myFile_all,col6_all,',');
      getline(myFile_all,col7_all,',');
      getline(myFile_all,col8_all,',');
      getline(myFile_all,col9_all,',');
      getline(myFile_all,col10_all,',');
      getline(myFile_all,col11_all,',');
      getline(myFile_all,col12_all,',');
      getline(myFile_all,col13_all,'\n');
      row_total_all += 1;
    }
  double total = Event_all.size();
  cout << "Number of Events: " << total << endl;
  
  // PART 2 THE ACTUAL PLOTTING DATA
  //declare strings for placeholders- 19 columns
  string col1,col2,col3,col4,col5,col6,col7,col8,col9,
    col10,col11,col12,col13,col14,col15,col16,col17,col18,col19,col20,col21,col22,col23,col24,
    col25,col26,col27,col28,col29,col30,col31,col32,col33,col34,col35,col36,col37,col38,col39;

  //declare the Variables to match CSV file
  vector<double> ntd_foil_number, d_hit_x, d_hit_y, d_hit_z;


  //Open File
  string filelocation = "CSV_Files/" + file;
  ifstream myFile(filelocation.c_str());

  //discard first line,
  getline(myFile,line);

  //Tracking Rows in a file
  int row_total = 0;

  if (detector_name == "g4_NTD"){
      //FOR G4
      //Here comes the appending arrays into variables
      while (!myFile.eof())
	{
	  getline(myFile,col1,','); //set placeholder to a float

	  if (col1 == ""){
	    break; } // while loops with eof goes 1 more than the end so this breaks it. 

	  getline(myFile,col2,',');
	  ntd_foil_number.push_back(stof(col2));

	  getline(myFile,col3,',');
	  getline(myFile,col4,',');
	  getline(myFile,col5,',');
	  getline(myFile,col6,',');
	  getline(myFile,col7,',');

	  getline(myFile,col8,',');
	  d_hit_x.push_back(stof(col8)); //d for detector

	  getline(myFile,col9,',');
	  d_hit_y.push_back(stof(col9));

	  getline(myFile,col10,',');
	  d_hit_z.push_back(stof(col10));

	  getline(myFile,col11,',');
	  getline(myFile,col12,',');
	  getline(myFile,col13,',');
	  getline(myFile,col14,',');
	  getline(myFile,col15,',');
	  getline(myFile,col16,',');
	  getline(myFile,col17,',');
	  getline(myFile,col18,',');
	  getline(myFile,col19,',');
	  getline(myFile,col20,',');
	  getline(myFile,col21,',');
	  getline(myFile,col22,',');
	  getline(myFile,col23,',');
	  getline(myFile,col24,',');
	  getline(myFile,col25,',');
	  getline(myFile,col26,',');
	  getline(myFile,col27,',');
	  getline(myFile,col28,',');
	  getline(myFile,col29,',');
	  getline(myFile,col30,',');
	  getline(myFile,col31,',');
	  getline(myFile,col32,',');
	  getline(myFile,col33,',');
	  getline(myFile,col34,',');
	  getline(myFile,col35,',');
	  getline(myFile,col36,',');
	  getline(myFile,col37,',');
	  getline(myFile,col38,',');
	  getline(myFile,col39,'\n');

	  row_total += 1;
	}
      //Now all the data is stored in these Vector variables

      //Now we pull out the ntd foil number = 1 arrays
      vector<double> d_hit_x_1, d_hit_y_1, d_hit_z_1;
      int row_total_1 = 0;
      for (int i = 0; i<row_total; i++)
	{
	  if (ntd_foil_number[i] == 1)
	    {
	      d_hit_x_1.push_back(d_hit_x[i]);
	      d_hit_y_1.push_back(d_hit_y[i]);
	      d_hit_z_1.push_back(d_hit_z[i]);

	      row_total_1 += 1;
	    }
	}
	 
      //Since we only care about the Foil1's lets redefine d_hit_x into these new values
      d_hit_x = d_hit_x_1;
      d_hit_y = d_hit_y_1;
      d_hit_z = d_hit_z_1;
      row_total = row_total_1;
      //END OF G4
  }
  
  else if (detector_name == "NTD")
    {
      //FOR NTD
      //Here comes the appending arrays into variables
      while (!myFile.eof())
	{
	  getline(myFile,col1,','); //set placeholder to a float

	  if (col1 == ""){
	    break; } // while loops with eof goes 1 more than the end so this breaks it. 

	  getline(myFile,col2,',');
	  getline(myFile,col3,',');
	  getline(myFile,col4,',');
	  getline(myFile,col5,',');
	  getline(myFile,col6,',');
	  getline(myFile,col7,',');
	  getline(myFile,col8,',');
	  getline(myFile,col9,',');
	  getline(myFile,col10,',');
	  getline(myFile,col11,',');
	  getline(myFile,col12,',');
	  getline(myFile,col13,',');
	  getline(myFile,col14,',');
	  
	  getline(myFile,col15,',');
	  d_hit_x.push_back(stof(col15)); //d for detector

	  getline(myFile,col16,',');
	  d_hit_y.push_back(stof(col16));
		  
	  getline(myFile,col17,',');
	  d_hit_z.push_back(stof(col17));
		  
	  getline(myFile,col18,',');
	  getline(myFile,col19,',');
	  getline(myFile,col20,',');
	  getline(myFile,col21,',');
	  getline(myFile,col22,',');
	  getline(myFile,col23,',');
	  getline(myFile,col24,'\n');

	  row_total += 1;
	}
      //END of NTD
    }
  
  else if (detector_name == "MMT")
    {
      //FOR MMT
      //Here comes the appending arrays into variables
      while (!myFile.eof())
	{
	  getline(myFile,col1,','); //set placeholder to a float

	  if (col1 == ""){
	    break; } // while loops with eof goes 1 more than the end so this breaks it. 

	  getline(myFile,col2,',');
	  getline(myFile,col3,',');
	  getline(myFile,col4,',');
	  getline(myFile,col5,',');
	  getline(myFile,col6,',');
	  getline(myFile,col7,',');
	  getline(myFile,col8,',');
	  getline(myFile,col9,',');
	  getline(myFile,col10,',');
	  getline(myFile,col11,',');
	  getline(myFile,col12,',');
	  getline(myFile,col13,',');
	  getline(myFile,col14,',');
	  
	  getline(myFile,col15,',');
	  d_hit_x.push_back(stof(col15)); //d for detector

	  getline(myFile,col16,',');
	  d_hit_y.push_back(stof(col16));
		  
	  getline(myFile,col17,',');
	  d_hit_z.push_back(stof(col17));
		  
	  getline(myFile,col18,'\n');
	  row_total += 1;
	}
      //END of MMT
    }
  
  
  //Detector hit and Detector effciency
  float d_hits = row_total; //d stands for detector
  float d_acc = d_hits/total;

  //NO IDEA WHAT IM DOING... CONVERTING TO 3SF
  char buffer[32];
  memset(buffer, 0, sizeof(buffer));
  snprintf(buffer, sizeof(buffer), "%0.2e", d_acc);
 
  string strObj4(buffer);
  string d_acc_3sf = strObj4;

  //Converting to Spherical Coordinates
  vector<double> Radial, Theta, Phi; // Assuming Phi is the AZIMUTHAL Angle

  for(int i = 0; i<row_total; i++) //Using Initial x y z data as a example...
    {
      Radial.push_back(sqrt(
			   pow(d_hit_x[i],2) +
			   pow(d_hit_y[i],2) +
			   pow(d_hit_z[i],2)));

      Theta.push_back(acos(
			   d_hit_z[i] / Radial[i])); //Since Radial is already calcuated

      Phi.push_back(atan2(
			  d_hit_y[i] , d_hit_x[i])) + M_PI); //go from -pi tp pi to MoDEALs convention of 0 to 2pi
      
	}
  //Calcuating the Lowest and Highest Values of Each Array
  double smallx = d_hit_x[0];
  double largex = d_hit_x[0];
  for (int i=1; i<row_total; i++)
    {
      if (d_hit_x[i] < smallx)
	smallx = d_hit_x[i];
      if (d_hit_x[i] >largex)
	largex = d_hit_x[i];
    }
  double smally = d_hit_y[0];
  double largey = d_hit_y[0];
  for (int i=1; i<row_total; i++)
    {
      if (d_hit_y[i] < smally)
	smally = d_hit_y[i];
      if (d_hit_x[i] >largex)
	largey = d_hit_y[i];
    }
  double smallz = d_hit_z[0];
  double largez = d_hit_z[0];
  for (int i=1; i<row_total; i++)
    {
      if (d_hit_z[i] < smallz)
	smallz = d_hit_z[i];
      if (d_hit_z[i] >largez)
	largez = d_hit_z[i];
    }
  
  double smalltheta = Theta[0];
  double largetheta = Theta[0];
  for (int i=1; i<row_total; i++)
    {
      if (Theta[i] < smalltheta)
	smalltheta = Theta[i];
      if (Theta[i] > largetheta)
	largetheta = Theta[i];
    }

  double smallphi = Phi[0];
  double largephi = Phi[0];
  for (int i=1; i<row_total; i++)
    {
      if (Phi[i] < smallphi)
	smallphi = Phi[i];
      if (Phi[i] >largephi)
	largephi = Phi[i];
    }

  //MAKE A FOLDER FOR MY PLOTS
  string foldername = filename + "_plots";
  string folder_create = "mkdir " + foldername;
  
  gSystem -> Exec(folder_create.c_str());

  TStyle* MyStyle = new TStyle("MyStyle", "MyStyle");
  MyStyle -> SetTitleH(.09);
  MyStyle -> SetTitleW(1.0);
  MyStyle -> SetTitleBorderSize(0.1);
  gROOT -> SetStyle("MyStyle");
  TCanvas* c1 = new TCanvas("c1", "Histograms",1200,800);
  c1 -> SetLeftMargin(0.14);
  c1 -> SetRightMargin(0.14);
  c1 -> SetBottomMargin(0.14);
  gStyle -> SetOptStat(0);

  string plotname;

  //Change the g4_ntd to NTD since the difference no longer matters in this code
  if (detector_name == "g4_NTD")
    {
      detector_name = "NTD";
    }

  ostringstream plotlabelXvsY;
  plotlabelXvsY << "2D Histogram of " << detector_name << " Hits X vs Y for Monopole of Mass = " << (int)mass << " GeV and Charge = " << (int)charge << " gD, Number of Events = " << total << ", Number of " << detector_name << "  Hits = " << d_hits << ", " << detector_name << "  Efficiency = " << d_acc_3sf;
  TH2D* XvsY2D = new TH2D("XvsY2D"," ;Y (mm);X (mm)", 40,smally,largey, 40,smallx,largex);
  XvsY2D -> SetTitle(plotlabelXvsY.str().c_str());
  for (int i = 0; i<row_total; i++)
    {
      XvsY2D->Fill(d_hit_y[i],d_hit_x[i]);
    }
  XvsY2D -> Draw("COLZ");
  plotname = foldername + "/" +  filename + "_X_Y.png";
  c1 -> SaveAs(plotname.c_str());
  
  ostringstream plotlabelYvsX;
  plotlabelYvsX << "2D Histogram of " << detector_name << " Hits Y vs X for Monopole of Mass = " << (int)mass << " GeV and Charge = " << (int)charge << " gD, Number of Events = " << total << ", Number of " <<detector_name << " Hits = " << d_hits << ", " << detector_name << " Efficiency = " << d_acc_3sf; 
  TH2D* YvsX2D = new TH2D("YvsX2D"," ;X (mm);Y (mm)", 40,smallx,largex, 40,smally,largey);
  YvsX2D -> SetTitle(plotlabelYvsX.str().c_str());
  for (int i = 0; i<row_total; i++)
    {
      YvsX2D->Fill(d_hit_x[i],d_hit_y[i]);
    }
  YvsX2D -> Draw("COLZ");
  plotname = foldername + "/" + filename + "_Y_X.png";
  c1 -> SaveAs(plotname.c_str());

  ostringstream plotlabelXvsZ;
  plotlabelXvsZ << "2D Histogram of " << detector_name << " Hits X vs Z for Monopole of Mass = " << (int)mass << " GeV and Charge = " << (int)charge << " gD, Number of Events = " << total << ", Number of " << detector_name << " Hits = " << d_hits << ", " << detector_name << " Efficiency = " << d_acc_3sf;
  TH2D* XvsZ2D = new TH2D("XvsZ2D"," ;Z (mm);X (mm)", 40,smallz,largez, 40,smallx,largex);
  XvsZ2D -> SetTitle(plotlabelXvsZ.str().c_str());
  for (int i = 0; i<row_total; i++)
    {
      XvsZ2D->Fill(d_hit_z[i],d_hit_x[i]);
    }
  XvsZ2D -> Draw("COLZ");
  plotname = foldername + "/" + filename + "_X_Z.png";
  c1 -> SaveAs(plotname.c_str());
  
  ostringstream plotlabelZvsX;
  plotlabelZvsX << "2D Histogram of " << detector_name << " Hits Z vs X for Monopole of Mass = " << (int)mass << " GeV and Charge = " << (int)charge << " gD, Number of Events = " << total << ", Number of " << detector_name << " Hits = " << d_hits << ", " << detector_name << " Efficiency = " << d_acc_3sf;
  TH2D* ZvsX2D = new TH2D("ZvsX2D"," ;X (mm);Z (mm)", 40,smallx,largex, 40,smallz,largez);
  ZvsX2D -> SetTitle(plotlabelZvsX.str().c_str());
    for (int i = 0; i<row_total; i++)
    {
      ZvsX2D->Fill(d_hit_x[i],d_hit_z[i]);
    }
  ZvsX2D -> Draw("COLZ");
  plotname = foldername + "/" + filename + "_Z_X.png";
  c1 -> SaveAs(plotname.c_str());
  
  ostringstream plotlabelYvsZ;
  plotlabelYvsZ << "2D Histogram of " << detector_name << " Hits Y vs Z for Monopole of Mass = " << (int)mass << " GeV and Charge = " << (int)charge << " gD, Number of Events = " << total << ", Number of " << detector_name << " Hits = " << d_hits << ", " << detector_name << " Efficiency = " << d_acc_3sf;
  TH2D* YvsZ2D = new TH2D("YvsZ2D"," ;Z (mm);Y (mm)", 40,smallz,largez, 40,smally,largey);
  YvsZ2D -> SetTitle(plotlabelYvsZ.str().c_str());
  for (int i = 0; i<row_total; i++)
    {
      YvsZ2D->Fill(d_hit_z[i],d_hit_y[i]);
    }
  YvsZ2D -> Draw("COLZ");
  plotname = foldername + "/" + filename + "_Y_Z.png";
  c1 -> SaveAs(plotname.c_str());
  
  ostringstream plotlabelZvsY;
  plotlabelZvsY << "2D Histogram of " << detector_name << " Hits Z vs Y for Monopole of Mass = " << (int)mass << " GeV and Charge = " << (int)charge << " gD, Number of Events = " << total << ", Number of " << detector_name << " Hits = " << d_hits << ", " << detector_name << " Efficiency = " << d_acc_3sf;
  TH2D* ZvsY2D = new TH2D("ZvsY2D"," ;Y (mm);Z (mm)", 40,smally,largey, 40,smallz,largez);
  ZvsY2D -> SetTitle(plotlabelZvsY.str().c_str());
  for (int i = 0; i<row_total; i++)
    {
      ZvsY2D->Fill(d_hit_y[i],d_hit_z[i]);
    }
  ZvsY2D -> Draw("COLZ");
  plotname = foldername + "/" + filename + "_Z_Y.png";
  c1 -> SaveAs(plotname.c_str());

  ostringstream plotlabelThetavsPhi;
  plotlabelThetavsPhi << "2D Histogram of " << detector_name << " Hits #theta vs #phi for Monopole of Mass = " << (int)mass << " GeV and Charge = " << (int)charge << " gD, Number of Events = " << total << ", Number of " << detector_name << " Hits = " << d_hits << ", " << detector_name << " Efficiency = " << d_acc_3sf;
  TH2D* ThetavsPhi2D = new TH2D("ThetavsPhi2D"," ;#phi (rad);#theta (rad)", 40,smallphi,largephi, 40,smalltheta,largetheta);
  ThetavsPhi2D -> SetTitle(plotlabelThetavsPhi.str().c_str());
  for (int i = 0; i<row_total; i++)
    {
      ThetavsPhi2D->Fill(Phi[i],Theta[i]);
    }
  ThetavsPhi2D -> Draw("COLZ");
  plotname = foldername + "/" +  filename + "_Theta_Phi.png";
  c1 -> SaveAs(plotname.c_str());

  ostringstream plotlabelPhivsTheta;
  plotlabelPhivsTheta << "2D Histogram of " << detector_name << " Hits #phi vs #theta for Monopole of Mass = " << (int)mass << " GeV and Charge = " << (int)charge << " gD, Number of Events = " << total << ", Number of " << detector_name << " Hits = " << d_hits << ", " << detector_name << " Efficiency = " << d_acc_3sf;
  TH2D* PhivsTheta2D = new TH2D("PhivsTheta2D"," ;#theta (rad);#phi (rad)", 40,smalltheta,largetheta, 40,smallphi,largephi);
  PhivsTheta2D -> SetTitle(plotlabelPhivsTheta.str().c_str());
  for (int i = 0; i<row_total; i++)
    {
      PhivsTheta2D->Fill(Theta[i],Phi[i]);
    }
  PhivsTheta2D -> Draw("COLZ");
  plotname = foldername + "/" + filename + "_Phi_Theta.png";
  c1 -> SaveAs(plotname.c_str());


    } //end of for loop

}
