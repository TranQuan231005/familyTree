#include <iostream>
#include <string>
#include <queue>
using namespace std;

// create General tree node structure
struct node
{
    string id;
    int age;
    string name;
    string gender;
    string relationTo;
    node *siblingNext;
    node *child;

    // constructor
    node(string id, int age, string name, string gender = "", string relationTo = "")
    {
        this->id = id;
        this->age = age;
        this->name = name;
        this->gender = gender;
        this->relationTo = relationTo;
        this->siblingNext = nullptr;
        this->child = nullptr;
    }
};

node *grandParent = nullptr;

// create a family tree
node *createFamilyTree(node *grandParent)
{
    node *parent = new node("1", 48, "Parent", "Male");
    node *uncle1 = new node("2", 46, "Uncle1", "Male");
    node *child1 = new node("1.1", 25, "Child1", "Female");
    node *child2 = new node("1.2", 20, "Child2", "Male");

    grandParent->child = parent;
    parent->siblingNext = uncle1;
    parent->child = child1;
    child1->siblingNext = child2;

    return grandParent;
}

// Add member
node *addMember(node *grandParent, string id, int age, string name, string gender, string relationTo)
{
    node *newMember = new node(id, age, name, gender, relationTo);
    node *parentCurr = grandParent->child;

    if (parentCurr == nullptr)
    {
        cout << "No parents found in the family tree.\n";
        return grandParent;
    }

    // Find correct parent
    while (parentCurr != nullptr && parentCurr->name != relationTo)
    {
        parentCurr = parentCurr->siblingNext;
    }

    if (parentCurr == nullptr)
    {
        cout << "Parent not found.\n";
        return grandParent;
    }

    // Insert child sorted by age (descending = old → young)
    node *child = parentCurr->child;

    // Case 1: no children yet
    if (child == nullptr)
    {
        parentCurr->child = newMember;
        return grandParent;
    }

    // Case 2: insert at head (new older)
    if (newMember->age > child->age)
    {
        newMember->siblingNext = child;
        parentCurr->child = newMember;
        return grandParent;
    }

    // Case 3: insert in middle or end
    node *prev = nullptr;
    node *curr = child;

    while (curr != nullptr && curr->age >= newMember->age)
    {
        prev = curr;
        curr = curr->siblingNext;
    }

    prev->siblingNext = newMember;
    newMember->siblingNext = curr;

    return grandParent;
}

// Find member by ID by queue
node *findMember(node *grandParent, const string &id)
{
    if (!grandParent)
        return nullptr;
    queue<node *> q;
    q.push(grandParent);

    while (!q.empty())
    {
        node *curr = q.front();
        q.pop();
        if (curr->id == id)
            return curr;
        for (node *prev = curr->child; prev != nullptr; prev = prev->siblingNext)
        {
            q.push(prev);
        }
    }
    return nullptr;
}
// display family tree
void displayFamilyTree(node *grandParent)
{
    if (grandParent == nullptr)
        return;

    // Display grandparent
    cout << grandParent->name << " (Id: " << grandParent->id << ", age: " << grandParent->age << ")\n";
    node *parent = grandParent->child;
    // Display parents and their children
    while (parent != nullptr)
    {
        cout << "  " << parent->name << " (Id: " << parent->id
             << ", age: " << parent->age << ", gender: " << parent->gender << ")\n";

        node *child = parent->child;
        while (child != nullptr)
        {
            cout << "     " << child->name << " (Id: " << child->id
                 << ", age: " << child->age << ", gender: " << child->gender << ")\n";
            child = child->siblingNext;
        }
        parent = parent->siblingNext;
    }
}

int main()
{
    // Initialize root of family tree
    grandParent = new node("0", 70, "Grandparent", " ");
    createFamilyTree(grandParent);

    // User menu
    string option;
    cout << "1. Add Member\n";
    cout << "2. Display Family Tree\n";
    cout << "3. Search Member by ID\n";
    cout << "Type 'exit' to quit.\n\n";
    cout << "==============================\n";
    cout << "Available family tree initialized.\n";
    displayFamilyTree(grandParent);
    cout << "==============================\n";
    // Option loop
    while (true)
    {
        cout << "Enter option: ";
        getline(cin, option);

        if (option == "exit")
            break;
        // Add Member
        if (option == "1")
        {
            string id, name, gender, relationTo, ageStr;
            int age;

            cout << "Enter ID: ";
            getline(cin, id);

            cout << "Enter Name: ";
            getline(cin, name);

            cout << "Enter Age: ";
            cin >> age;
            cin.ignore();

            cout << "Enter gender: ";
            getline(cin, gender);

            cout << "Enter relation to (parent name): ";
            getline(cin, relationTo);

            addMember(grandParent, id, age, name, gender, relationTo);
        }
        // Display Family Tree
        else if (option == "2")
        {
            cout << "==============================\n";
            cout << "Family Tree:\n";
            displayFamilyTree(grandParent);
            cout << "==============================\n";
        }
        // Search Member by ID
        else if (option == "3")
        {
            string searchId;
            cout << "Enter  ID to search:";
            getline(cin, searchId);
            node *exist = findMember(grandParent, searchId);
            if (exist)
            {
                cout << "Member found: " << exist->name << ", Age: " << exist->age << "\n";
            }
            else
            {
                cout << "Member with ID " << searchId << " not found.\n";
            }
        }
    }
    return 0;
}
