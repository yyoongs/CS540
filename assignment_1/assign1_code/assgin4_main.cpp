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

bool compare_id(const Dept& n1, const Dept& n2) {
    return n1.get_manager_id() < n2.get_manager_id();
}

bool compare_eid(const Emp& n1, const Emp& n2) {
    return n1.get_eid() < n2.get_eid();
}


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
    }

    fs.close();

    return i_list;

}


vector<Dept> Dept_sort(vector<Dept> dept_list, vector<Dept> main_memory1)
{
    int block_n = dept_list.size()/22; // 1
    int current_num = 0;

    Dept empty;
    vector<Dept>::iterator iter2 = dept_list.begin();
    vector< vector<Dept> >arr(block_n+1, vector<Dept>(MAIN_MEMORY_SIZE,empty));
    // cout << "sdfsdfsdfdsf : " << arr[1][0].get_eid() << endl;


// 여기서 가능하다면 파일에 쓰고 다시 읽어와서 저장하는것도 방법 i/o 표현위해
    while(dept_list.size()!=0)
    {
        if(dept_list.size() > MAIN_MEMORY_SIZE)
        {

            main_memory1.clear();
            main_memory1.assign(dept_list.begin(), dept_list.begin()+ MAIN_MEMORY_SIZE);
            sort(main_memory1.begin(),main_memory1.end(),compare_id);

            dept_list.erase(iter2+0,iter2+22);

            arr[current_num].clear();
            arr[current_num].insert( arr[current_num].end(), main_memory1.begin(), main_memory1.end());
            current_num++;
        }
        else
        {
            main_memory1.clear();
            main_memory1.assign(dept_list.begin(), dept_list.begin()+dept_list.size());
            sort(main_memory1.begin(),main_memory1.end(),compare_id);

            dept_list.erase(iter2+0,iter2+dept_list.size());

            arr[current_num].clear();
            arr[current_num].insert(arr[current_num].end(), main_memory1.begin(), main_memory1.end());
            current_num++;
        }
    }


    if(block_n > (MAIN_MEMORY_SIZE-1))
    {
        throw out_of_range("Exceed maximum block number");
    }   
    else
    {
        main_memory1.clear();
        // main_memory2.push_back(emp_list_temp[0]);
        // main_memory2.push_back(emp_list_temp[MAIN_MEMORY_SIZE]);
        for(int j=0 ; j<=block_n;j++)
        {
            main_memory1.push_back(arr[j][0]);
            arr[j].erase(arr[j].begin());
        }
        // 반복 * num
        // 데이터 옮겼으면 지우고 빈공간으로 남겨둠 

        int comp;
        comp = main_memory1[0].get_manager_id();
        int index;       


        while(!main_memory1.empty())
        {
            index = 0;
            // main memory가 empty일때 종료
            vector<Dept>::iterator iterEnd = main_memory1.end();
            for(vector<Dept>::iterator iter_p = main_memory1.begin();iter_p!=iterEnd;++iter_p)
            {
                if(iter_p->get_manager_id() < comp )
                {
                    // comp = iter_p->get_eid();
                    index = distance(main_memory1.begin(), iter_p);
                }
            }

            dept_list.push_back(main_memory1[index]);
            // 가장 작은거 emp_list에 저장

            // arr[index]가 empty면 main memory 삭제
            if(arr[index].empty())
            {
                main_memory1.erase(main_memory1.begin()+index);
            }
            else
            {
                if(index == 0)
                {
                    comp = arr[index][0].get_manager_id();
                }
                main_memory1[index] = arr[index][0];
                arr[index].erase(arr[index].begin());
            }
            // 해당 블록 다음 원소 가져오기

            // cout << "index : " << index << endl;
        }
        
    }

    return dept_list;
    // cout << " sssss : "<< emp_list_temp.size() / MAIN_MEMORY_SIZE << endl;
}


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
    }
    fs.close();

    return i_list;
}

void Emp_sort_join(vector<Dept> dept_list, vector<Emp> emp_list, vector<Emp> main_memory2, fstream &fout)
{
    int block_n = emp_list.size()/22; // 1
    int current_num = 0;
    int y = 0;
    int a = 0;
    bool flag = true;
    Emp empty;
    vector<Emp>::iterator iter2 = emp_list.begin();
    vector< vector<Emp> >arr(block_n+1, vector<Emp>(MAIN_MEMORY_SIZE,empty));
    fout.precision(10);
    // cout << "sdfsdfsdfdsf : " << arr[1][0].get_eid() << endl;


// 여기서 가능하다면 파일에 쓰고 다시 읽어와서 저장하는것도 방법 i/o 표현위해
    while(emp_list.size()!=0)
    {
        if(emp_list.size() > MAIN_MEMORY_SIZE)
        {

            main_memory2.clear();
            main_memory2.assign(emp_list.begin(), emp_list.begin()+ MAIN_MEMORY_SIZE);
            sort(main_memory2.begin(),main_memory2.end(),compare_eid);

            emp_list.erase(iter2+0,iter2+22);

            arr[current_num].clear();
            arr[current_num].insert( arr[current_num].end(), main_memory2.begin(), main_memory2.end());
            current_num++;
        }
        else
        {
            main_memory2.clear();
            main_memory2.assign(emp_list.begin(), emp_list.begin()+emp_list.size());
            sort(main_memory2.begin(),main_memory2.end(),compare_eid);

            emp_list.erase(iter2+0,iter2+emp_list.size());

            arr[current_num].clear();
            arr[current_num].insert(arr[current_num].end(), main_memory2.begin(), main_memory2.end());
            current_num++;
        }
    }


    if(block_n > (MAIN_MEMORY_SIZE-1))
    {
        throw out_of_range("Exceed maximum block number");
    }   
    else
    {
        main_memory2.clear();
        // main_memory2.push_back(emp_list_temp[0]);
        // main_memory2.push_back(emp_list_temp[MAIN_MEMORY_SIZE]);
        for(int j=0 ; j<=block_n;j++)
        {
            main_memory2.push_back(arr[j][0]);
            arr[j].erase(arr[j].begin());
        }
        // 반복 * num
        // 데이터 옮겼으면 지우고 빈공간으로 남겨둠 

        int comp;
        comp = main_memory2[0].get_eid();
        int index;       


        while(!main_memory2.empty())
        {
            index = 0;
            // main memory가 empty일때 종료
            vector<Emp>::iterator iterEnd = main_memory2.end();
            for(vector<Emp>::iterator iter_p = main_memory2.begin();iter_p!=iterEnd;++iter_p)
            {
                if(iter_p->get_eid() < comp )
                {
                    // comp = iter_p->get_eid();
                    index = distance(main_memory2.begin(), iter_p);
                }
            }

            emp_list.push_back(main_memory2[index]);

            // int y = 0;
            a = 0;
            flag = true;
            while(flag)
            {
                if(y >= dept_list.size()-1)
                {
                    flag = false;
                }

                if(emp_list.back().get_eid() > dept_list[y].get_manager_id())
                {
                    y++;
                }
                if(emp_list.back().get_eid() < dept_list[y].get_manager_id())
                {
                    flag = false;
                }
                if(emp_list.back().get_eid() == dept_list[y].get_manager_id())
                {
                    //join with emp_list[x] - dept_list[y]
                    fout << dept_list[y].get_did() << ',' << dept_list[y].get_dname() << ',' << dept_list[y].get_budget() 
                    << ',' << dept_list[y].get_manager_id() << ',' << emp_list.back().get_eid() << ',' 
                    << emp_list.back().get_ename() << ',' << emp_list.back().get_age() << ',' << emp_list.back().get_salary() << "\n";
                    
                    while(1)
                    {
                        if(dept_list[y+a].get_manager_id() == dept_list[y+a+1].get_manager_id())
                        {
                            //join with emp_list[x] - dept_list[y+a+1]
                            fout << dept_list[y+a+1].get_did() << ',' << dept_list[y+a+1].get_dname() << ',' << dept_list[y+a+1].get_budget() 
                            << ',' << dept_list[y+a+1].get_manager_id() << ',' << emp_list.back().get_eid() << ',' 
                            << emp_list.back().get_ename() << ',' << emp_list.back().get_age() << ',' << emp_list.back().get_salary() << "\n";
                            
                            a++;
                        }
                        else
                        {
                            break;
                        }
                    }
                    flag = false;
                }
            }
            
            // 가장 작은거 emp_list에 저장

            // arr[index]가 empty면 main memory 삭제
            if(arr[index].empty())
            {
                main_memory2.erase(main_memory2.begin()+index);
            }
            else
            {
                if(index == 0)
                {
                    comp = arr[index][0].get_eid();
                }
                main_memory2[index] = arr[index][0];
                arr[index].erase(arr[index].begin());
            }
            // 해당 블록 다음 원소 가져오기

            // cout << "index : " << index << endl;
        }
        
    }
    // cout << " sssss : "<< emp_list_temp.size() / MAIN_MEMORY_SIZE << endl;
}


int main(int argc, const char* argv[])
{
    vector<Dept> dept_list;
    vector<Emp> emp_list;

    dept_list = read_csv(dept_list, "Dept.csv");
    emp_list = read_csv(emp_list, "Emp.csv");

    fstream joinout;
    joinout.open("Join_test.csv", ios::out | ios::app);

    cout << "size : " <<dept_list.size() << endl;

//sort
    dept_list = Dept_sort(dept_list,main_memory1);
    Emp_sort_join(dept_list,emp_list,main_memory2, joinout);


    // int i=0;
    // vector<Dept>::iterator iterEnd = dept_list.end();
    // for(vector<Dept>::iterator iter_p = dept_list.begin();iter_p!=iterEnd;++iter_p)
    // {
    //     cout << i <<"manager _id : " << iter_p->get_manager_id() << endl;
    //     i++;
    // }

    // cout << endl;

    // i = 0;
    // vector<Emp>::iterator iterEnd2 = emp_list.end();
    // for(vector<Emp>::iterator iter_p = emp_list.begin();iter_p!=iterEnd2;++iter_p)
    // {
    //     cout << i <<" eid : " << iter_p->get_eid() << endl;
    //     i++;
    // }

    return 0; 
}
