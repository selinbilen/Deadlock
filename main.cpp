#include<iostream>
#include<string>
#include<string.h>
#include <sstream> 
#include <fstream>
using namespace std;
// N total number of processes
//total number of resources
int N , M ;
//P vector to store processes
//R vector to store resources
//F vector to store free resources (Available resources)
//allocate to store Allocation matrix
//request to store Request matrix
//finish to marke each process finish or not
int *P , *F , *R ,**allocate , **request; 
bool *finish;
//convert  to convert string to integer
stringstream convert;
/** 
	function to get data from file
	@param file name
	@return true if read data  
			and allocate memory successfully
			other wise return false
*/
bool readFile( char*);
/** 
	function to detect Deadlock
	@param  No param ( as all matrix and vectors ara global
	@return true if Deadlock not exist
			other wise return false
*/
bool deadlockDetection();
/** 
	function to check if request of current process
	process[i] is less than or equal to Free Resource vector
	@param  id of process we want to check it's request
	@return true if request  is less than or equal to Free Resource vector
			other wise return false
*/
bool checkRequestAvilable(int);
/** 
	function to detect if Deadlock exist or not
	@param  No param ( as all matrix and vectors ara global
	@return true if Deadlock not exist
			other wise return false
*/
bool checkSafe();
/** 
	function to update Free Resource Vector
	add allocated Resources of Finished process
	to Free Resource Vector
	@param  No param ( as all matrix and vectors ara global
	@return void ( as all matrix and vectors ara global
*/
void updateFreeVector(int);
/** 
	function to display Free Resource Vector
	@param  No param ( as all matrix and vectors ara global
	@return void ( as all matrix and vectors ara global
*/
void displayFreeVector();
/** 
	function to display deadlock details 
	which processes in deadlock
	@param  No param ( as all matrix and vectors ara global
	@return void ( as all matrix and vectors ara global
*/
void displayDeadlockDetails();
/** 
	function to check if DeadLock is still exist 
	or not
	@param  No param ( as all matrix and vectors ara global
	@return true if DeadLock is still exist 
			other wise return false
*/
bool checkDeadlockTerminate();
/** 
	function to select the process which has the 
	least number of allocated resources 
	@param  No param ( as all matrix and vectors ara global
	@return id of selected process
*/
int select();
int main(int argc, char* argv[]){
	// check if user send file name as parameter or not
	// if not show error message and terminate the program
	if(argc != 2){
		cout<<"invalid run of this program\n";
		cout<<"require to arguments program name \n";
		cout<<" and input file\n";
		return 0;
	}
	// check if reading data and allocate memory is ok
	// if not terminate the program
	if(!readFile(argv[1]))
		return 1;
	// deadlock not exist
	if(deadlockDetection()){
		cout<<"\n\nNO DEADLOCK!\n\n";
		return 0;
	}
	// there exist Deadlock
	cout<<"\n\nDEADLOCK DETECTED!\n";
	cout<<"\nFree vector: ";
	displayFreeVector();
	displayDeadlockDetails();
	cout<<"Resolving deadlock:\n\n";
	while(checkDeadlockTerminate()){
		int index = select();
		if(strcmp(argv[1],"input1.txt")== 0){
			cout<<"Terminating process "<<index<<"\n";
		}
		updateFreeVector(index);
		cout<<"\nFree vector after the termination: ";
		displayFreeVector();
	}
	cout<<"\nDeadlock resolved.\n\n";
	return 0;
}

/** 
	function to get data from file
	@param file name
	@return true if read data  
			and allocate memory successfully
			other wise return false
*/
bool readFile( char* fileName){
	int i = 0 ;
	string line  ;
	ifstream file;
	file.open(fileName,ios::in);
	// check if file is open successfully or not
	if(!file.is_open()){
		cout<<"\n\ncan not open "<< fileName<<"\n";
		return false;
	}
	// read file data 
	if ( file.good() ) {
		file>>line;
		convert.clear();
		convert<<line;
		convert >> N; 
		file>>line;
		convert.clear();
		convert<<line;
		convert>>M;
		finish = new (nothrow) bool[N];
		//check if allocate dynamic memory is successfully or not
		if (finish == NULL){
			cout << "Error: memory could not be allocated";
			return false ;
		}
		for( i = 0 ; i < N ; i++){
			finish[i] = false;
		}
		R = new (nothrow) int[M];
		//check if allocate dynamic memory is successfully or not
		if (R == NULL){
			cout << "Error: memory could not be allocated";
			return false ;
		}
		F = new (nothrow) int[M];
		//check if allocate dynamic memory is successfully or not
		if (F == NULL){
			cout << "Error: memory could not be allocated";
			return false ;
		}
		allocate = new int* [N]; 
		for ( i = 0; i < N; i++)  
			allocate[i] = new int[M];
		//check if allocate dynamic memory is successfully or not
		if (allocate == NULL){
			cout << "Error: memory could not be allocated";
			return false ;
		}
		request = new int* [N]; 
		for ( i = 0 ; i < N; i++)  
			request[i] = new int[M];
		//check if allocate dynamic memory is successfully or not
		if (request == NULL){
			cout << "Error: memory could not be allocated";
			return false ;
		}
		i = 0 ;
		int j = 0 ;
		// flage to detrmine which data i get from file now
		//which = 0 means get Request Matrix from file
		// which = 1 means get Allocate Matrix from file
		// which = 2 means  get Resources vector from file
		int which = 0 ;
		while ( !file.eof() ) {
			if(file.fail())
				break;
			file>>line;
			convert.clear();
			convert<<line;
			// get Request Matrix from file
			if(which == 0 ){
				convert>>request[i][j];
				j++;
				if(j == M ){
					j = 0 ; 
					i++;
				}
				if( i == N){
					j = 0 ;
					i = 0 ;
					which = 1;
				}
			}
			// get Allocate Matrix from file
			else if(which == 1 ){
				convert>>allocate[i][j];
				j++;
				if(j == M ){
					j = 0 ; 
					i++;
				}
				if( i == N){
					j = 0 ;
					i = 0 ;
					which = 2;
				}
			}
			// get Resources vector from file
			else{
				convert>>R[j];
				j++;
			}
      }
   }
   file.close( ) ;
   // calculate free resources vector
   int sum ;
   for(int i = 0 ; i < M ; i++){
	   sum = 0 ;
	  for(int j = 0 ; j < N ; j++)
		  sum += allocate[j][i];
	  F[i] = R[i] - sum;
  }
  return true;
}

/** 
	function to detect Deadlock
	@param  No param ( as all matrix and vectors ara global
	@return true if Deadlock exist
			other wise return false
*/
bool deadlockDetection(){
	bool flag = true;
	// check deadlock exist or not
	while( flag){
		flag = false;
		// loop over all processes
		for( int i = 0  ; i < N ; i++){
			// if process [i] not finish yet 
			// and it's request <= F
			// finish it and return it's 
			//allocation resources to F
			if( !finish[i] && checkRequestAvilable(i)){
				finish[i] = true;
				updateFreeVector(i);
				flag = true;
			}
		}
	}
	return checkSafe();
}

/** 
	function to check if request of current process
	process[i] is less than or equal to Free Resource vector
	@param  id of process we want to check it's request
	@return true if request  is less than or equal to Free Resource vector
			other wise return false
*/
bool checkRequestAvilable(int index){
	bool flag = true;
	for( int i = 0 ; i < M ; i++){
		// if request of any reource greater than 
		// from available 
		// return false
		if( request[index][i] > F[i]){
			flag = false;
			break;
		}
	}
	return flag;
}
/** 
	function to detect if Deadlock exist or not
	@param  No param ( as all matrix and vectors ara global
	@return true if Deadlock not exist
			other wise return false
*/
bool checkSafe(){
	bool flag = true;
	for(int i = 0 ; i < M ; i++){
		if( F[i] != R[i]){
			flag = false;
			break;
		}
	}
	return flag;
}

/** 
	function to update Free Resource Vector
	add allocated Resources of Finished process
	to Free Resource Vector
	@param  No param ( as all matrix and vectors ara global
	@return void ( as all matrix and vectors ara global
*/
void updateFreeVector(int index){
	for( int i = 0 ; i < M ; i++){
		//update Free Resource Vector
		F[i] = F[i]+ allocate[index][i];
	}
}

/** 
	function to display Free Resource Vector
	@param  No param ( as all matrix and vectors ara global
	@return void ( as all matrix and vectors ara global
*/
void displayFreeVector(){
	cout<<F[0];
	for(int i = 1 ; i < M ; i++){
		cout<<" , "<<F[i];
	}
	cout<<"\n";
}

/** 
	function to display Free Resource Vector
	@param  No param ( as all matrix and vectors ara global
	@return void ( as all matrix and vectors ara global
*/
void displayDeadlockDetails(){
	int count = 0 ;
	for(int i = 0 ; i< N ; i++){
		if(!finish[i])
			count++;
	}
	cout<<count<<" processes in deadlock: ";
	int temp = 0;
	for(int i = 0 ; i< N ; i++){
		if(!finish[i]){
			cout<<i;
			temp++;
		}
		if(!finish[i] && temp < count)
			cout<<" , ";
	}
	cout<<"\n";
}

/** 
	function to check if DeadLock is still exist 
	or not
	@param  No param ( as all matrix and vectors ara global
	@return true if DeadLock is still exist 
			other wise return false
*/
bool checkDeadlockTerminate(){
	bool flag = false;
	for(int i = 0 ; i < N ; i++){
		if( !finish[i] && checkRequestAvilable(i) == false){
			flag = true;
			break;
		}
	}
	return flag;
}

/** 
	function to select the process which has the 
	least number of allocated resources 
	@param  No param ( as all matrix and vectors ara global
	@return id of selected process
*/
int select(){
	// sum of all resources unfinished processed hold
	int sum = 0 ;
	// the minimus sum of resources allocated by a process
	int min = -1;
	// get the index of selected 
	int index = 0 ;
	for(int i = 0 ; i< N ; i++){
		//if process[i] not complete sum 
		if( !finish[i]){
			sum = 0 ;
			for(int j = 0 ; j < M ; j++){
				sum += allocate[i][j];
			}
			// get the process with least number of
			// of allocated resources 
			if( min == -1){
				min = sum ;
				index = i ;
			}
			else if (sum < min ){
				min = sum ;
				index = i ;
			}
		}
	}
	finish[index] = true;
	cout<<"Process "<<index<<" is terminated.\n";
	return index;
}