#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

#include "class.h"

using namespace std;

#define MAIN_MEMORY_SIZE 22

vector<Dept> main_memory1;
vector<Emp> main_memory2;

// To compare manager_id value
bool compare_id(const Dept& n1, const Dept& n2) {
    return n1.get_manager_id() < n2.get_manager_id();
}

// To compare eid value
bool compare_eid(const Emp& n1, const Emp& n2) {
    return n1.get_eid() < n2.get_eid();
}

/***
name: read_csv
parameter:  vector<Dept> dept_list / string file_name
function:   Read csv file "Dept.csv" and make Dept object using csv data
            Save to vector<Dept> dept_list
return: vector<Dept> dept_list
***/
vector<Dept> read_csv(vector<Dept> i_list, string file_name)
{
    int did;
    string dname;
    int manager_id;
    double budget;

    string str_buf;         
    fstream fs; 
    fs.open(file_name,ios::in);
    while(1){

        getline(fs,str_buf,',');
        if(str_buf.empty())
        {
            break;
        }
        did = stoi(str_buf);

        getline(fs,str_buf,',');
        dname = str_buf;

        getline(fs,str_buf,',');
        budget = stod(str_buf);

        getline(fs,str_buf,'\n');
        manager_id = stoi(str_buf);

        Dept *data1 = new Dept(did,dname,budget,manager_id);
        i_list.push_back(*data1);
        delete(data1);
    }

    fs.close();

    return i_list;

}


/***
name: Dept_temp_sort
parameter:  vector<Dept> dept_list / vector<Dept> main_memory1 / string filename
function:   By using dept_list, make temp sorted csv file, named "Dept_temp.csv"
            Sorted by memory size
return: the number of blocks
***/
int Dept_temp_sort(vector<Dept> dept_list, vector<Dept> main_memory1,string filename)
{
    int block_n = (dept_list.size()/22) + 1; // Assume the number of block
    Dept empty; // Make empty object

    vector<Dept>::iterator iter2 = dept_list.begin(); // dept_list Iterator

    ofstream fs (filename); // Define output file stream
    fs.precision(10); // To increase precision when file is written

    while(dept_list.size()!=0)  // Loop until dept_list is empty
    {
        if(fs.is_open()) // If file is opened
        {
            if(dept_list.size() > MAIN_MEMORY_SIZE) // If the number of dept_list is more than MAIN_MEMORY_SIZE
            {

                main_memory1.clear(); // Main memory clear
                main_memory1.assign(dept_list.begin(), dept_list.begin()+ MAIN_MEMORY_SIZE); // The number of MAIN_MEMORY_SIZE of data are assigned to main memory 
                sort(main_memory1.begin(),main_memory1.end(),compare_id); // Sort main memory

                dept_list.erase(iter2+0,iter2+22); // Delete used data
                vector<Dept>::iterator iterEnd = main_memory1.end(); // Define iterator
                for(vector<Dept>::iterator iter_p = main_memory1.begin();iter_p!=iterEnd;++iter_p) // Save sorted data to file
                {
                    fs << iter_p->get_did() << "," << iter_p->get_dname() << "," << iter_p->get_budget() << "," << iter_p->get_manager_id() << "\n";
                }
            }
            else // The number of dept_list is less than MAIN_MEMORY_SIZE
            {
                main_memory1.clear(); // Main memory clear
                main_memory1.assign(dept_list.begin(), dept_list.begin()+dept_list.size()); // The number of dept_list.size() of data are assigned to main memory
                sort(main_memory1.begin(),main_memory1.end(),compare_id); // Sort main memory

                dept_list.erase(iter2+0,iter2+dept_list.size()); // Delete used data
                vector<Dept>::iterator iterEnd = main_memory1.end(); // Define iterator
                for(vector<Dept>::iterator iter_p = main_memory1.begin();iter_p!=iterEnd;++iter_p) // Save sorted data to file
                {
                    fs << iter_p->get_did() << "," << iter_p->get_dname() << "," << iter_p->get_budget() << "," << iter_p->get_manager_id() << "\n";
                }
            }
        }
    }
    fs.close(); // File close
    return block_n;
}

/***
name: Dept_sort
parameter:  int block_n / vector<Dept> dept_list / vector<Dept> main_memory1 / string filename
function:   Read "Dept_temp.csv" and save to 2 dimention array named arr
            Sort all of data and save to dept_list
return: dept_list
***/
vector<Dept> Dept_sort(int block_n, vector<Dept> dept_list, vector<Dept> main_memory1,string filename)
{
    // To use save value
    int did;
    string dname;
    int manager_id;
    double budget;
    
    // Used variables
    int current_num = 0;
    int count = 0;
    int comp;
    int index;       

    // Define 2 Dimension array
    Dept empty;
    vector< vector<Dept> >arr(block_n, vector<Dept>(MAIN_MEMORY_SIZE,empty));

    // file open
    string str_buf;         
    fstream fs; 
    fs.open(filename,ios::in);

    arr[current_num].clear(); // To clear array

    while(1){

        getline(fs,str_buf,','); // getline from file
        if(str_buf.empty())
        {
            break;
        }
        did = stoi(str_buf);

        getline(fs,str_buf,',');
        dname = str_buf;

        getline(fs,str_buf,',');
        budget = stod(str_buf);

        getline(fs,str_buf,'\n');
        manager_id = stoi(str_buf);

        Dept *data1 = new Dept(did,dname,budget,manager_id); // Define temporary Dept object data

        if(count < MAIN_MEMORY_SIZE) // arr[current_num] can contain maximum MAIN_MEMORY_SIZE values
        {
            arr[current_num].insert( arr[current_num].end(), *data1); // Save to array
            count++;
        }
        else // if arr[current_num] is full
        {
            current_num += 1;
            arr[current_num].clear();
            arr[current_num].insert( arr[current_num].end(), *data1); // Save to array
            count = 1;
        }
        delete(data1);
    }
    fs.close(); // To close file

    if(block_n > (MAIN_MEMORY_SIZE-1)) // If the number of block is over than (MAIN_MEMORY_SIZE-1)
    {
        throw out_of_range("Exceed maximum block number"); // Error
    }   
    else
    {
        main_memory1.clear(); // To clear main memory

        for(int j=0 ; j<block_n;j++) // Save to main memory from the each block of array vector's first value
        {
            main_memory1.push_back(arr[j][0]); // Save to main memory
            arr[j].erase(arr[j].begin()); // Delete data that is saved in main memory
        }

        comp = main_memory1[0].get_manager_id(); // Set first data
        while(!main_memory1.empty()) // Loop until main memory is empty
        {
            index = 0;
            vector<Dept>::iterator iterEnd = main_memory1.end(); // Define iterator
            for(vector<Dept>::iterator iter_p = main_memory1.begin();iter_p!=iterEnd;++iter_p) // Compare values
            {
                if(iter_p->get_manager_id() < comp )
                {
                    index = distance(main_memory1.begin(), iter_p); // Return lowest value's index
                }
            }

            dept_list.push_back(main_memory1[index]); // Save lowest value in the dept_list

            if(arr[index].empty()) // If arr[index] is empty
            {
                main_memory1.erase(main_memory1.begin()+index); // Delete from main memory to ignore comparison
            }
            else
            {
                if(index == 0) // If first value is lowest value
                {
                    comp = arr[index][0].get_manager_id(); // Reset compare value
                }
                main_memory1[index] = arr[index][0]; // Get from array
                arr[index].erase(arr[index].begin()); // Erase used data
            }
        }
    }
    return dept_list;
}


/***
name: read_csv
parameter:  vector<Emp> emp_list / string file_name
function:   Read csv file "Emp.csv" and make Emp object using csv data
            Save to vector<Emp> emp_list
return: vector<Emp> emp_list
***/
vector<Emp> read_csv(vector<Emp> i_list, string file_name)
{
    string str_buf;         
    fstream fs; 

    int eid;
    string ename;
    int age;
    double salary;
    
    fs.open(file_name,ios::in);
    while(1){

        getline(fs,str_buf,',');
        if(str_buf.empty())
        {
            break;
        }
        eid = stoi(str_buf);

        getline(fs,str_buf,',');
        ename = str_buf;

        getline(fs,str_buf,',');
        age = stoi(str_buf);

        getline(fs,str_buf,'\n');
        salary = stod(str_buf);

        Emp *data2 = new Emp(eid,ename,age,salary);
        i_list.push_back(*data2);
        delete(data2);
    }
    fs.close();

    return i_list;
}


/***
name: Emp_temp_sort
parameter:  vector<Emp> emp_list / vector<Emp> main_memory2 / string filename
function:   By using emp_list, make temp sorted csv file, named "Emp_temp.csv"
            Sorted by memory size
return: the number of blocks
***/
int Emp_temp_sort(vector<Emp> emp_list, vector<Emp> main_memory2,string filename)
{
    int block_n = (emp_list.size()/22) + 1; // Assume the number of block
    Emp empty; // Make empty object

    vector<Emp>::iterator iter2 = emp_list.begin(); // emp_list Iterator

    ofstream fs (filename); // Define output file stream
    fs.precision(10); // To increase precision when file is written

    while(emp_list.size()!=0) // Loop until emp_list is empty
    {
        if(fs.is_open()) // If file is opened
        {       
            if(emp_list.size() > MAIN_MEMORY_SIZE) // If the number of emp_list is more than MAIN_MEMORY_SIZE
            {

                main_memory2.clear(); // Main memory clear
                main_memory2.assign(emp_list.begin(), emp_list.begin()+ MAIN_MEMORY_SIZE); // The number of MAIN_MEMORY_SIZE of data are assigned to main memory 
                sort(main_memory2.begin(),main_memory2.end(),compare_eid); // Sort main memory

                emp_list.erase(iter2+0,iter2+22); // Delete used data
                vector<Emp>::iterator iterEnd = main_memory2.end(); // Define iterator
                for(vector<Emp>::iterator iter_p = main_memory2.begin();iter_p!=iterEnd;++iter_p) // Save sorted data to file
                {
                    fs << iter_p->get_eid() << "," << iter_p->get_ename() << "," << iter_p->get_age() << "," << iter_p->get_salary() << "\n";
                }

            }
            else // The number of emp_list is less than MAIN_MEMORY_SIZE
            {
                main_memory2.clear(); // Main memory clear
                main_memory2.assign(emp_list.begin(), emp_list.begin()+emp_list.size()); // The number of dept_list.size() of data are assigned to main memory
                sort(main_memory2.begin(),main_memory2.end(),compare_eid); // Sort main memory

                emp_list.erase(iter2+0,iter2+emp_list.size()); // Delete used data
                vector<Emp>::iterator iterEnd = main_memory2.end(); // Define iterator
                for(vector<Emp>::iterator iter_p = main_memory2.begin();iter_p!=iterEnd;++iter_p) // Save sorted data to file
                {
                    fs << iter_p->get_eid() << "," << iter_p->get_ename() << "," << iter_p->get_age() << "," << iter_p->get_salary() << "\n";
                }
            }
        }
    }
    fs.close(); // File close
    return block_n;
}


/***
name: Emp_sort_join(
parameter:  int block_number /  vector<Dept> dept_list / vector<Emp> emp_list / vector<Emp> main_memory2 / string filename / fstream &fout
function:   Read "Emp_temp.csv" and save to 2 dimention array named arr
            Sort all of data and join with dept_list at the same time
            Save joined table to Join.csv file
return: x
***/
void Emp_sort_join(int block_n2, vector<Dept> dept_list, vector<Emp> emp_list, vector<Emp> main_memory2, string filename, fstream &fout)
{
    // To use save value
    int eid;
    string ename;
    int age;
    double salary;

    // Used variables
    int count = 0;
    int current_num = 0;
    int comp;
    int index;       
    int dept_idx = 0;
    int add = 0;
    bool flag = true;

    // Define 2 Dimension array
    Emp empty;
    vector<Emp>::iterator iter2 = emp_list.begin();
    vector< vector<Emp> >arr(block_n2, vector<Emp>(MAIN_MEMORY_SIZE,empty));

    // file open
    string str_buf;         
    fstream fs; 
    fs.open(filename,ios::in);

    fout.precision(10); // To increase precision when file is written

    arr[current_num].clear(); // To clear array

    while(1){

        getline(fs,str_buf,','); // getline from "Emp_temp.csv"
        if(str_buf.empty())
        {
            break;
        }
        eid = stoi(str_buf);

        getline(fs,str_buf,',');
        ename = str_buf;

        getline(fs,str_buf,',');
        age = stoi(str_buf);

        getline(fs,str_buf,'\n');
        salary = stod(str_buf);

        Emp *data1 = new Emp(eid,ename,age,salary); // Define temporary Dept object data

        if(count < MAIN_MEMORY_SIZE) // arr[current_num] can contain maximum MAIN_MEMORY_SIZE values
        {
            arr[current_num].insert( arr[current_num].end(), *data1); // Save to array
            count++;
        }
        else // if arr[current_num] is full
        {
            current_num += 1;
            arr[current_num].clear(); // To clear array
            arr[current_num].insert( arr[current_num].end(), *data1); // Save to array
            count = 1;
        }
    }
    fs.close(); // To close file

    if(block_n2 > (MAIN_MEMORY_SIZE-1)) // If the number of block is over than (MAIN_MEMORY_SIZE-1)
    {
        throw out_of_range("Exceed maximum block number"); // Error
    }   
    else
    {
        main_memory2.clear(); // To clear main memory

        for(int j=0 ; j<block_n2;j++) // Save to main memory from the each block of array vector's first value
        {
            main_memory2.push_back(arr[j][0]); // Save to main memory
            arr[j].erase(arr[j].begin()); // Delete data that is saved in main memory
        }

        comp = main_memory2[0].get_eid(); // Set first data
        while(!main_memory2.empty()) // Loop until main memory is empty
        {
            index = 0;
            vector<Emp>::iterator iterEnd = main_memory2.end(); // Define iterator
            for(vector<Emp>::iterator iter_p = main_memory2.begin();iter_p!=iterEnd;++iter_p) // Compare values
            {
                if(iter_p->get_eid() < comp )
                {
                    index = distance(main_memory2.begin(), iter_p); // Return lowest value's index
                }
            }

            emp_list.push_back(main_memory2[index]); // Save lowest value in the dept_list

            add = 0;
            flag = true;
            while(flag)
            {
                if(dept_idx >= dept_list.size()-1) // If dept_list is finished, end loop
                {
                    flag = false;
                }
                if(emp_list.back().get_eid() > dept_list[dept_idx].get_manager_id()) // If eid value is bigger than manager_id value
                {
                    dept_idx++; // Increase index of dept_list 
                }
                if(emp_list.back().get_eid() < dept_list[dept_idx].get_manager_id()) // If manager_id is bigger than eid
                {
                    flag = false; // Change flag "false" and finish loop to get next eid
                }
                if(emp_list.back().get_eid() == dept_list[dept_idx].get_manager_id()) // If manager_id is same as eid ( this case makes join tuple )
                {
                    //join with emp_list[x] - dept_list[y]
                    fout << dept_list[dept_idx].get_did() << ',' << dept_list[dept_idx].get_dname() << ',' << dept_list[dept_idx].get_budget() 
                    << ',' << dept_list[dept_idx].get_manager_id() << ',' << emp_list.back().get_eid() << ',' 
                    << emp_list.back().get_ename() << ',' << emp_list.back().get_age() << ',' << emp_list.back().get_salary() << "\n";
                    
                    while(1) // To check duplicate
                    {
                        if(dept_list[dept_idx+add].get_manager_id() == dept_list[dept_idx+add+1].get_manager_id()) // If next manager_id value is same as eid
                        {
                            //join with emp_list[x] - dept_list[dept_idx+add+1]
                            fout << dept_list[dept_idx+add+1].get_did() << ',' << dept_list[dept_idx+add+1].get_dname() << ',' << dept_list[dept_idx+add+1].get_budget() 
                            << ',' << dept_list[dept_idx+add+1].get_manager_id() << ',' << emp_list.back().get_eid() << ',' 
                            << emp_list.back().get_ename() << ',' << emp_list.back().get_age() << ',' << emp_list.back().get_salary() << "\n";
                            
                            add++;
                        }
                        else
                        {
                            break;
                        }
                    }
                    flag = false; // To finish infinite loop
                }
            }

            if(arr[index].empty()) // If arr[index] is empty
            {
                main_memory2.erase(main_memory2.begin()+index); // Delete from main memory to ignore comparison
            }
            else
            {
                if(index == 0) // If first value is lowest value
                {
                    comp = arr[index][0].get_eid(); // Reset compare value
                }
                main_memory2[index] = arr[index][0]; // Get from array
                arr[index].erase(arr[index].begin()); // Erase used data
            }
        }
    }
}


int main(int argc, const char* argv[])
{
    vector<Dept> dept_list; // Define dept_list
    vector<Emp> emp_list; // Define emp_list

    // File stream
    fstream joinout;
    joinout.open("Join_test3.csv", ios::out | ios::app);

    // Used variables
    int block_n, block_n2;

    // Read csv files and save to vector list
    dept_list = read_csv(dept_list, "Dept.csv");
    emp_list = read_csv(emp_list, "Emp.csv");

    block_n = Dept_temp_sort(dept_list,main_memory1,"Dept_temp.csv"); // Dept_list temporary sort and save to "Dept_temp.csv"
    dept_list.clear();
    dept_list = Dept_sort(block_n, dept_list,main_memory1,"Dept_temp.csv"); // Dept_list is finally sorted

    block_n2 = Emp_temp_sort(emp_list,main_memory2,"Emp_temp.csv"); // Emp_list temporary sort and save to "Emp_temp.csv"
    emp_list.clear();

    Emp_sort_join(block_n2, dept_list, emp_list, main_memory2, "Emp_temp.csv", joinout); // Emp_list is sorted and joined with dept during the merge phase

    return 0; 
}
