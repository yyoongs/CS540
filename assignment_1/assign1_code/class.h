#ifndef __CLASS_H__
#define __CLASS_H__

#include <iostream>
#include <string>

using namespace std;

// Dept (did (integer), dname (string), budget (double), managerid (integer))
// Fields of types integer,double, and string occupy 4, 8, and 40 bytes, respectively.

class Dept // data structure
{
    private:
        int did;
        string dname;
        double budget;
        int manager_id;

    public:

        Dept(int Id, string Name, double Budget, int Manager_id)
        {
            did = Id;
            dname = Name;
            budget = Budget;
            manager_id = Manager_id;
            
        }

        Dept()
        {
        }

        int get_did() { return did; }
        string get_dname() { return dname; }
        double get_budget() { return budget; }
        int get_manager_id() const { return manager_id; }

};

// Emp (eid (integer), ename (string), age (integer), salary (double))

class Emp // data structure
{
    private:
        int eid;
        string ename;
        int age;
        double salary;

    public:

        Emp(int Id, string Name, int Age, double Salary)
        {
            eid = Id;
            ename = Name;
            age = Age;
            salary = Salary;
        }

        Emp()
        {
        }

        int get_eid() const { return eid; }
        string get_ename() { return ename; }
        int get_age() { return age; }
        double get_salary() { return salary; }
};

// did, dname, budget, manager id , eid, ename, age, salary
#endif