//***************************************************************************************************
// CPSC 5110
// Term Project for Computational Optimization
// File: Heuristic.cpp
// Purpose: Program to produce a feasible class schedule using Greedy Heuristic
// Written by: Mir Tafseer Nayeem
//****************************************************************************************************

#include <iostream>
#include <iomanip>
#include <fstream>
 
using namespace std;

//Constant Declarations
const int COLS=8;
const int ROWS=10;
const int conflictingTimeslots[] = {5,6,7};

//Define structure
struct course
{
   double rating;
   int row;
   int col;
};

//Function Prototypes
void loadData(double **);

void writeToFile(double **);

void writeToConsole(double **);

void deallocateArray(double **);

void chooseCompulsoryCourse(double **, int);

void chooseOptionalCourse(double **, int, int, int);

int loadCourses(double **, int, int);

void sortCourses(course [], int);

void crossOutSlots(double **, int, int);

double determineHeuristic(double **);

//***************************************************************************************
//Function: main function
//Purpose: Controling the overall problem flow.
//         This Program has been designed in a way that in can be easily reconfigarable
//         This can be more dynamic easily using user inputs
//***************************************************************************************

int main() {
	
	double** dataArray = new double*[ROWS];
	for(int i = 0; i < ROWS; ++i) {
		dataArray[i] = new double[COLS];
	}
 
	loadData(dataArray);
	
	
	chooseCompulsoryCourse(dataArray,0);
	
	chooseCompulsoryCourse(dataArray,1);
	
	chooseOptionalCourse(dataArray, 2,3,1);
	
	chooseOptionalCourse(dataArray, 4,7,2);
	
	
	writeToFile(dataArray);

	deallocateArray(dataArray);
	
}

//******************************************************************
//Function: loadData
//Purpose: To load data from an input to data Array
//******************************************************************

void loadData(double **dataArray){
	
	ifstream fin;
	fin.open("input.txt");
	
	while (!fin.eof()){
	for(int i = 0; i < ROWS; ++i) {
		for(int j = 0; j < COLS; ++j) {
			fin >> dataArray[i][j];
		}
	}
	fin.close();
   }
}

//******************************************************************
//Function: writeToFile
//Purpose: To write into a CSV file according to a readable froamt 
//******************************************************************

void writeToFile(double **dataArray){
	
	ofstream fout;
	fout.open("output.CSV");
	double heuristicRating=0.0;
	
	char coursesCodes[][50] = {"MGT 490","FIN 358","CIS 102T", 
	                            "CIS 102W", "FIN 325","FIN 352",
						        "FIN 356","FIN 359"};
	
	char coursesTitles[][50] = {"Business Strategy","International Finance","Intergenerational Computing", 
	                            "Web Design for Nonprofit Organizations", "Data Analysis in Finance","Risk Management",
						        "Options Futures and Swaps","Fixed Instruments and Markets"};
	
	char timeSlots[][50] = {"M Eve","T Eve","W Eve", "Th Eve", "F Eve", "M 1:25-3:15 p.m. & W 1:25-2:20 p.m.", 
							"M 1:25-2:20 p.m. & W 1:25-3:15 p.m.","W 2:30-5:15 p.m.", "T 1:25-3:15 p.m. & Th 1:25-2:20 p.m.", 
							"Th 2:30-5:15 p.m."};


	fout <<""<< ","
		 <<"Course Schedule"<< ","
		 <<"(Greedy Heuristic)"<< ","
		 <<""<<","
		 <<endl;

    fout << "Course"<< ","
		 << "Titles"<< ","
		 <<"Meeting Time(s)"<< ","
		 <<"Rating";
	
	fout << endl;

    for(int i = 0; i < ROWS; ++i) 
		for(int j = 0; j < COLS; ++j) {
			if(dataArray[i][j] > 0){
				fout << coursesCodes[j]<< ",";
				fout << coursesTitles[j]<< ",";
				fout << timeSlots[i]<< ",";
				fout << dataArray[i][j];
				fout << endl;		
			}
		}
	
	heuristicRating = determineHeuristic(dataArray);
	
	fout << endl;
	
	fout << ""<< ","
	     << ""<< ","
	     << "Greedy Heuristic Rating:"<< ","
	     <<heuristicRating;
	
	fout.close();
}

//******************************************************************
//Function: determineHeuristic
//Purpose: Determine the Total Heuristic Rating
//******************************************************************

double determineHeuristic(double **dataArray){
	
	double heuristicRating=0.0;
	
	for(int i = 0; i < ROWS; ++i) 
		for(int j = 0; j < COLS; ++j)
			if(dataArray[i][j] > 0.0)
				heuristicRating = heuristicRating + dataArray[i][j];
	
	return heuristicRating;
}

//******************************************************************
//Function: deallocateArray
//Purpose: Deallocates the array used in this program
//******************************************************************

void deallocateArray(double **dataArray){
	
	for(int i = 0; i < ROWS; ++i) {
		delete [] dataArray[i];
	}
	delete [] dataArray;	
}

//******************************************************************
//Function: chooseCompulsoryCourse
//Purpose: Choose compulsory course with Highest Rating
//******************************************************************

void chooseCompulsoryCourse(double **dataArray, int j){
	
	double maxRating=0.0;
	int timeSlotIndex;
	
	for(int i = 0; i < ROWS; ++i) {
		if(dataArray[i][j] > maxRating ){
			maxRating = dataArray[i][j];
			timeSlotIndex = i;	
		}
	}    
	
	crossOutSlots(dataArray, timeSlotIndex, j);		
}

//******************************************************************
//Function: loadCourses
//Purpose: Loads Optional courses in a Structure
//******************************************************************

int loadCourses(double **dataArray, course optionalCourse[], int startCourseIndex, int endCourseIndex){
	
	int k=0;
	
	for(int i = 0; i < ROWS; ++i) 
		for(int j = startCourseIndex; j <= endCourseIndex; ++j) 
			if(dataArray[i][j] > 0){
				optionalCourse[k].rating=dataArray[i][j];
				optionalCourse[k].row=i;
				optionalCourse[k].col=j;
				k++;	
		   }
     return k;	
}

//******************************************************************
//Function: chooseOptionalCourse
//Purpose: Choose Optional courses with Highest Rating
//******************************************************************

void chooseOptionalCourse(double **dataArray, int startCourseIndex, int endCourseIndex, int numberOfCourses){
	
	int length = ((endCourseIndex - startCourseIndex) + 1 ) * ROWS;
	course optionalCourse[length];
	
	int k;
	
    k = loadCourses(dataArray, optionalCourse, startCourseIndex, endCourseIndex);
	sortCourses(optionalCourse,k);
	
	for(int i=0; i<numberOfCourses; ++i){	
		crossOutSlots(dataArray, optionalCourse[i].row, optionalCourse[i].col);	
	}
	
	for(int j=numberOfCourses; j<k; ++j){
	   if(optionalCourse[j].rating > 0){
	   	    int m=optionalCourse[j].row;
	   	    int n=optionalCourse[j].col;
			dataArray[m][n]=0;	   	
	   }			
	}
			
}

//******************************************************************
//Function: sortCourses
//Purpose: Sorts Optional courses according to Highest Rating
//******************************************************************
void sortCourses(course optionalCourse[], int numberOfCourses){
	
   course temp;
    
   for(int i=0; i<numberOfCourses; i++){
      for(int j=i+1; j<numberOfCourses; j++)
	 	if(optionalCourse[i].rating <  optionalCourse[j].rating){
	    	temp = optionalCourse[i];
	    	optionalCourse[i]= optionalCourse[j];
	    	optionalCourse[j] = temp;
		}
  }
}

//***********************************************************************************************
//Function: crossOutSlots
//Purpose: Crossing out Overlaping Schedules
//		   General Program used to overlaping schedule for both optional and compulsory courses
//***********************************************************************************************

void crossOutSlots(double **dataArray, int slotIndex, int courseIndex){
	
	int inConflictingSlots=0;
	int k;
	
	for(int i = 0; i < ROWS; ++i){	
		if(i ==slotIndex)
			continue;
		dataArray[i][courseIndex]=0;
	}
	
	
	for(int i = 0; i < ROWS; ++i) 	
		if(slotIndex == conflictingTimeslots[i]){
			inConflictingSlots=1;
			break;
		}
		
	if(inConflictingSlots){
	for(int i = 0; i < (sizeof conflictingTimeslots/sizeof(int)) ; ++i){
		k = conflictingTimeslots[i];
		for(int j = 0; j < COLS; ++j){
		    if(j == courseIndex)
			    continue; 
			dataArray[k][j] = 0;
		}
	}
	}
	else {
	for(int j =0; j < COLS; ++j){
		 if(j == courseIndex)
			    continue; 
		dataArray[slotIndex][j]=0;
	}	
	}
}

//end
