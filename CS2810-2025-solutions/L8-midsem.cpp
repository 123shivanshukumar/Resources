#include <bits/stdc++.h>
#include <chrono>

using namespace std;

class Employee{
    public:
        long long int ID, age, experience, bossID;
        long long int depth, experienceSum, avgExp, subordinateCount;
        vector<Employee> subordinates;
        vector<int> childrenIDs;

        Employee(int ID, int age, int experience, int bossID){
            this->ID = ID;
            this->age = age;
            this->experience = experience;
            this->bossID = bossID;
            this->depth = 0; // will be set later
            this->experienceSum = experience; // experience of subordinates will be added later
            this->subordinateCount = 1; // count of subordinates will be added later
        }

        Employee(){
            this->ID = 0;
            this->age = 0;
            this->experience = 0;
            this->bossID = 0;
            this->depth = 0;
            this->experienceSum = 0;
            this->subordinateCount = 0;
        }
};

// Count number of non dominatable employees
// sortingFn: sorts employees in descending order of first criteria and breaks any ties by descending order of second criteria
// secondDomArg: returns the second argument value for the employee
int countNonDomEmployees(vector<Employee> &employees, function<bool(Employee &a, Employee &b)> sortingFn, function<int(Employee &a)> secondDomArg){
    sort(employees.begin(), employees.end(), sortingFn);

    int ans = 0, minSecondDomArg = INT_MAX;
    for (int i=0; i<(int)employees.size(); i++){
        ans += (minSecondDomArg >= secondDomArg(employees[i]));
        minSecondDomArg = min(minSecondDomArg, secondDomArg(employees[i]));
    }

    return ans;
}

// Set depth, experienceSum, subordinateCount, avgExp
void dfs(int currID, map<int, Employee> &IDtoEmployee){
    Employee &currEmp = IDtoEmployee[currID];
    for(auto childID : currEmp.childrenIDs){
        Employee &childEmp = IDtoEmployee[childID];

        childEmp.depth = currEmp.depth + 1;
        dfs(childID, IDtoEmployee);
        currEmp.experienceSum += childEmp.experienceSum;
        currEmp.subordinateCount += childEmp.subordinateCount;
    }
    currEmp.avgExp = currEmp.experienceSum / currEmp.subordinateCount;
}

// Merge two sorted lists of employees
vector<Employee> mergeLists(vector<Employee> & list1, vector<Employee> & list2){
    vector<Employee> mergedList;
    int i = 0, j = 0;
    while(i < (int)list1.size() && j < (int)list2.size()){
        if (list1[i].age < list2[j].age){
            mergedList.push_back(list1[i]);
            i++;
        }
        else if (list1[i].age > list2[j].age){
            mergedList.push_back(list2[j]);
            j++;
        }
        else {
            if (list1[i].ID < list2[j].ID){
                mergedList.push_back(list1[i]);
                i++;
            }
            else {
                mergedList.push_back(list2[j]);
                j++;
            }
        }
    }

    while(i < (int)list1.size()){
        mergedList.push_back(list1[i]);
        i++;
    }

    while(j < (int)list2.size()){
        mergedList.push_back(list2[j]);
        j++;
    }

    return mergedList;
}

// Set subordinates sorted by (age, ID)
void setSubordinates(int currID, map<int, Employee> &IDtoEmployee){
    Employee &currEmp = IDtoEmployee[currID];
    vector<Employee> subordinates;
    subordinates.push_back(currEmp);

    for(auto childID : currEmp.childrenIDs){
        setSubordinates(childID, IDtoEmployee);
        subordinates = mergeLists(subordinates, IDtoEmployee[childID].subordinates);
        // Alternate: (extra log n time factor)
        // subordinates.insert(subordinates.end(), IDtoEmployee[childID].subordinates.begin(), IDtoEmployee[childID].subordinates.end());
    }

    // needed for alternate:
    // sort(subordinates.begin(), subordinates.end(), [](Employee &a, Employee &b){
    //     if (a.age == b.age) return a.ID < b.ID;
    //     return a.age < b.age;
    // });
    currEmp.subordinates = subordinates;
}

void processInput(map<int, Employee> &IDtoEmployee, int &rootID){
    int n;
    cin >> n;
    // taking input
    for(int i=0; i<n; i++){
        int ID, age, experience, bossID;
        cin >> ID >> age >> experience >> bossID;
        Employee emp(ID, age, experience, bossID);
        IDtoEmployee[ID] = emp;
        if (ID == bossID) rootID = ID;
    }

    // set childrenIDs
    for(auto &IDEmployeePair : IDtoEmployee){
        int ID = IDEmployeePair.first;
        int bossID = IDEmployeePair.second.bossID;
        if (ID != bossID){
            IDtoEmployee[bossID].childrenIDs.push_back(ID);
        }
    }
}

int main(){
    map<int, Employee> IDtoEmployee;
    vector<Employee> employees;
    int q, rootID;
    bool encounteredType3Queries = false;

    processInput(IDtoEmployee, rootID);        

    dfs(rootID, IDtoEmployee); // sets depth, experienceSum, subordinateCount

    cin >> q;
    employees.clear();
    for(auto &u : IDtoEmployee){
        employees.push_back(u.second);
    }

    for(int i=0; i<q; i++){
        int queryType;
        cin >> queryType;

        if (queryType == 1){
            int domCriteria;
            cin >> domCriteria;

            if (domCriteria == 1) {
                cout << countNonDomEmployees(employees, [](Employee &a, Employee &b){
                    if (a.experience == b.experience) return a.depth > b.depth;
                    return a.experience > b.experience;
                }, [](Employee &a){ return a.depth; }) << endl;
            }
            else if (domCriteria == 2){
                cout << countNonDomEmployees(employees, [](Employee &a, Employee &b){
                    if (a.age == b.age) return a.depth > b.depth;
                    return a.age > b.age;
                }, [](Employee &a){ return a.depth; }) << endl;
            }
            else {
                cout << countNonDomEmployees(employees, [](Employee &a, Employee &b){
                    if (a.experience == b.experience) return a.age > b.age;
                    return a.experience > b.experience;
                }, [](Employee &a){ return a.age; }) << endl;
            }
        }
        else if (queryType == 2){
            int ID;
            cin >> ID;
            cout << IDtoEmployee[ID].avgExp << endl;
        }
        else {
            int ID, k;
            cin >> ID >> k;

            // subordinates vector is set only once if type 3 queries are encountered -> precomputation
            if (!encounteredType3Queries) {
                encounteredType3Queries = true; 
                setSubordinates(rootID, IDtoEmployee); 
            }

            Employee &emp = IDtoEmployee[ID];
            if (emp.subordinateCount < k) cout << -1 << endl;
            else cout << emp.subordinates[k-1].ID << endl;
        }
    }
}