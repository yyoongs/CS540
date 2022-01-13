#include <iostream>
#include <list>
#include <vector>
#include <cmath>
#include <string>
#include <stdio.h>
#include <string.h>
#include <fstream>


using namespace std;



class data // data structure
{
    public:
    int eid;
    string ename;
    int age;
    int salary;
    data(int EId, char* EName, int Age, int Salary);
    void operator=(const data& other);

   
};

void data::operator=(const data& other) {
        if (this!= &other)  {
           this->eid = other.eid;
	   this->ename = other.ename;
	   this->age = other.age;
	   this->salary = other.salary;
	}
}


data::data(int EId, char* EName, int Age, int Salary)
{
        this->eid = EId;
        this->ename = EName;
        this->age = Age;
        this->salary = Salary;
}



void merge (vector <data> &arr, int low, int mid, int high);
void MergeSort(vector<data> &arr, int low, int high);

void merge (vector <data> &arr, int low, int mid, int high) {
   int n1 = mid - low+1;
   int n2 = high - mid;
   // create a vector data for temp arr
   vector <data> L1;
   vector <data> R1;
   // copy data to temp array L[] and R[]
   for (int i = 0; i < n1; i++)  {
      data copy_data = arr[low+i];
      L1.push_back(copy_data);
   }

   for (int j = 0; j < n2; j++) {
      data copy_data = arr[mid+1+j];
      R1.push_back(copy_data);

   }

   int ii = 0;

   int jj = 0;

   int kk = low;

   while (ii < n1 && jj < n2)  {
     if (L1[ii].eid <= R1[jj].eid ) {
        arr[kk] = L1[ii];
	ii++;
     } else {
        arr[kk] = R1[jj];
	jj++;
     }
     kk++;


   }
   // copying the remaining element of the L[] if there is any

   while  (ii < n1) {
      arr[kk] = L1[ii];
      ii++;
      kk++;

   }
   
  // copying the remaining element of the R[] if there is any
  
  while (jj < n2)  {
     arr[kk] = R1[jj];
     jj++;
     kk++;
  }




}

void MergeSort (vector<data> &arr, int low, int high) {
   if (low>=high) {
      return;
   }
   int mid = low + (high-low)/2;
   MergeSort(arr,low,mid);
   MergeSort(arr, mid+1, high);
   merge(arr,low,mid,high);
   
}

vector<data> read_csv(vector<data> i_list)
{
    int EId;
    int Salary;
    char *ename = new char[200];
    int Age;

    FILE *sample = fopen("Emp.csv", "r");

    char *currLine = new char[1024];
    size_t len = 0;
    ssize_t nread;
    while ((getline(&currLine, &len, sample)) != -1)
    {

        char *saveptr; //to use strtok_r
        char *token;
        token = strtok_r(currLine, ",", &saveptr); 
        EId = stoi(token);

        token = strtok_r(NULL, ",", &saveptr);
        strcpy(ename,token);

        token = strtok_r(NULL, ",", &saveptr);
        Age = stoi(token);

        token = strtok_r(NULL, "\r", &saveptr);
        Salary = stoi(token);

        data *data1 = new data(EId,ename,Age,Salary);

        i_list.push_back(*data1);
    }

    delete[](ename);
    delete[](currLine);
    return i_list;
}

int main(int argc, const char* argv[])
{   
    vector<data> datalist;
    datalist = read_csv(datalist);
    /*
    for (int i = 0; i < datalist.size(); i++)
    {
       cout << datalist[i].eid << "\n";
    }
    */
    int memory_size = 22;
    int block = 2;
    int block_data = datalist.size();
    int ind = memory_size;
    while (1)  {
         //cout << ind << "\n";
       int sec_ind = ind;
       int start = 0;
       while (sec_ind <= block_data) {
	//  cout << start << "\n";
         // cout << sec_ind << "\n";
	  MergeSort(datalist,0, sec_ind-1);
	  if (sec_ind == block_data) {
             break;
	  }
          if (sec_ind+ind > block_data) {
	     start = sec_ind;
             sec_ind = sec_ind + (block_data % ind);
          } else {
	     start = sec_ind;
             sec_ind = ind + sec_ind; 
          }

       }
  
       if (ind==block_data) {
          break;
       }

       if (ind*2 > block_data) {
          ind = block_data;
       } else {
          ind = ind * 2;
       }

      
    }
    /*
    for (int i = 0; i < datalist.size(); i++)
    {
        cout << datalist[i].eid << "\n";
    }
    */
  
    ofstream fileout ("EmpSorted.csv");
    string out_data = "";
    string eid;
    string ename;
    string age;
    string salary;
    for (int j = 0; j < datalist.size(); j++) {
        eid = to_string(datalist[j].eid);
	ename = datalist[j].ename;
	age = to_string(datalist[j].age);
	salary = to_string(datalist[j].salary);
	out_data = eid + "," + ename + "," + age + "," + salary + "\n";
	fileout << out_data;
    }

    fileout.close();

       
    return 0; 
}
