#include <iostream>
#include <list>
#include <vector>
#include <cmath>
#include <string>
#include <fstream>

// // for C
#include <string.h>

using namespace std;

#define MAX_BLOCK 4096
#define MAX_DATA_NUM 400

int result_pow();
int result_pow2();

int i = 2; // for use pow(2,i)
int s = 0; // check block
int buc_num = 4; // now bucket number

struct data // data structure
{
    data(int Id, char* Name, char* Bio, int Manager_id)
    {
        id = Id;
        name = Name;
        bio = Bio;
        manager_id = Manager_id;
        size = 8 + name.length() + bio.length();
    }
    int id;
    string name;
    string bio;
    int manager_id;
    int size;
};

struct search_set // key set structure
{
    search_set(int Id, int Buc)
    {
        id = Id;
        bucket_id = Buc;
    }
    int id;
    int bucket_id;
};

class Linear_Hash // class define
{
    private:
    int BUCKET_num;    // No. of buckets 
    int BUCKET_usage; // usage of bucket
    vector <data> data_table; 

    public: 
        Linear_Hash(int V);  // Constructor 
        int get_num(); // return bucket number
        vector <data> get_table(); // return table
        bool check_usage(int size); // usage check
        void insert_data(data input); // to insert data to data table
        vector<data> delete_data(); // to delete data from data table
        void display(); // to show status
}; 

Linear_Hash::Linear_Hash(int input) // constructor
{ 
    BUCKET_num = input;
    this->BUCKET_usage = 0;
} 


// ************ global variable ************
vector <Linear_Hash> vec_hash;
vector <search_set> key_set;


int Linear_Hash::get_num()
{
    return BUCKET_num;
}

vector <data> Linear_Hash::get_table()
{
    return data_table;
}

bool Linear_Hash::check_usage(int size)
{
    if( ((BUCKET_usage + size) / MAX_BLOCK)*100 >= 80) // if usage over 80%
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Linear_Hash::insert_data(data input)
{
    BUCKET_usage += input.size; // change bucket size
    data_table.push_back(input);  // insert data
    search_set *set = new search_set(input.id,BUCKET_num);

    // key_set managing
    for(vector<search_set>::iterator iter_set = key_set.begin();iter_set!=key_set.end();)
    {
        if(iter_set->id == input.id)
        {
            key_set.erase(iter_set);
        }
        else
        {
            ++iter_set;
        }
    }
    key_set.push_back(*set);
    delete(set);
}

// delete data from data table
// return vector of deleted data
vector<data> Linear_Hash::delete_data()
{
    vector<data> changed_vec;

    for(vector<data>::iterator iter_p = data_table.begin();iter_p!=data_table.end();)
    {

        if(iter_p->id % result_pow2() != s)
        {
            BUCKET_usage -= iter_p->size;
            changed_vec.push_back(*iter_p);
            iter_p = data_table.erase(iter_p);
        }
        else
        {
            ++iter_p;
        }
    }
    return changed_vec;
}
  
// to show data table
void Linear_Hash::display() { 

    vector<data>::iterator iterE = data_table.end();
        for(vector<data>::iterator iter_p = data_table.begin();iter_p!=iterE;++iter_p)
        {
            cout << " --> " << iter_p->name;
        }
    cout << endl;
} 


// divide procedure
// vec_hash[index] -> check and divide
void divide(data input, int index)
{

    // make new block if the bucket usage is over 80%
    if(vec_hash[index].check_usage(input.size) == true)
    {
        vec_hash[index].insert_data(input);
        Linear_Hash *hash1 = new Linear_Hash(buc_num);
        vec_hash.push_back(*hash1);
        delete(hash1);

        buc_num += 1;

        vector <data> changed;
        changed = vec_hash[s].delete_data();

        vector<data>::iterator iterEnd = changed.end();
        for(vector<data>::iterator iter_p = changed.begin();iter_p!=iterEnd;++iter_p)
        {
            int re_index = iter_p->id % result_pow2();
            vec_hash[re_index].insert_data(*iter_p);
        } // calculate new index and insert data to corrected data table

        // change check variables
        s += 1;
        if(s == result_pow())
        {
            s = 0;
            i +=1;
        }
    }
    else
    {
        vec_hash[index].insert_data(input);

    }    
}


void insertItem(data input)
{
    int index = input.id % result_pow();
    if(index < s && s != 0 )
    {
        index = input.id % (result_pow()*2);
        divide(input,index);
    }
    else
    {
        divide(input,index);
    }
}

// to calculate 2^i
int result_pow()
{
    int result = pow(2,i);
    return result;
}

// to calculate 2^(i+1)
int result_pow2()
{
    int result = pow(2,i+1);
    return result;
}


list<data> read_csv(list<data> i_list)
{
    int id;
    int manager_id;
    char *name = new char[200];
    char *bio = new char[500];

    FILE *sample = fopen("Employee.csv", "r");

    char *currLine = new char[1024];
    size_t len = 0;
    ssize_t nread;
    while ((nread = getline(&currLine, &len, sample)) != -1)
    {

        char *saveptr; //to use strtok_r
        char *token;
        token = strtok_r(currLine, ",", &saveptr); 
        id = stoi(token);

        token = strtok_r(NULL, ",", &saveptr);
        strcpy(name,token);

        token = strtok_r(NULL, ",", &saveptr);
        strcpy(bio,token);

        token = strtok_r(NULL, "\r", &saveptr);
        manager_id = stoi(token);

        data *data1 = new data(id,name,bio,manager_id);

        i_list.push_back(*data1);
        delete(data1);

    }

    delete[](name);
    delete[](bio);
    delete[](currLine);

    return i_list;
}

int main(int argc, const char* argv[])
{
    list<data> datalist;
    datalist = read_csv(datalist);
    list<Linear_Hash*> todelete;
    // read data and make data list

    for(int init = 0;init<4;init++)
    {
        Linear_Hash *hash = new Linear_Hash(init);
        vec_hash.push_back(*hash);
        todelete.push_back(hash);
    } // to initialize first 4 bucket

    list<data>::iterator iterEnd = datalist.end();
    for(list<data>::iterator iter_p = datalist.begin();iter_p!=iterEnd;++iter_p)
    {
        insertItem(*iter_p);  
    }
    // insert data to vec_hash table from datalist

    // display the Hash table 
    for(int j=0 ; j<buc_num ; j++)
    {
        cout << j << " : ";
        vec_hash[j].display();
    }


    ofstream myfile;
    myfile.open ("EmployeeIndex.txt");

    for(int j=0 ; j< datalist.size() ; j++)
    {
        myfile << "key set : { " << key_set.back().id<< " , " << key_set.back().bucket_id << " }" << endl;
        key_set.pop_back();
    }
    myfile.close();

    while (todelete.size()!=0)
    {
        Linear_Hash* tode = todelete.front();
        delete(tode);
        todelete.pop_front();
    }

    return 0; 
}

