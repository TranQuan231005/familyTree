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
// ===== Helper: Find a node anywhere in the tree by name =====
node *findRootMember(node *grandParent, const string &name)
{
    if (grandParent == nullptr)
        return nullptr;

    // Match by name
    if (grandParent->name == name)
        return grandParent;

    // Search children
    node *result = findRootMember(grandParent->child, name);
    if (result != nullptr)
        return result;

    // Search siblings
    return findRootMember(grandParent->siblingNext, name);
}

// ===== Add member at ANY level =====
string capitalizeName(string s)
{
    if (s.empty())
        return s;

    s[0] = toupper(s[0]);
    for (int i = 1; i < s.length(); i++)
        s[i] = tolower(s[i]);

    return s;
}
node *addMember(node *grandParent, string id, int age, string name, string gender, string relationTo)
{
    // Convert to lowercase for case-insensitive comparison
    name = capitalizeName(name);
    relationTo = capitalizeName(relationTo);
    gender = capitalizeName(gender);

    node *newMember = new node(id, age, name, gender, relationTo);

    // -------- 1. Find parent anywhere in the tree --------
    node *parentCurr = findRootMember(grandParent, relationTo);
    if (parentCurr == nullptr)
    {
        cout << "Parent not found.\n";
        return grandParent;
    }

    // -------- 2. Insert child sorted by age (descending) --------
    node *child = parentCurr->child;

    // Case 1: no child yet
    if (child == nullptr)
    {
        parentCurr->child = newMember;
        return grandParent;
    }

    // Case 2: insert at head
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
void displayFamilyTree(node *Grandparent, int level = 0)
{
    if (Grandparent == nullptr)
        return;

    // Print indent based on level
    for (int i = 0; i < level; i++)
        cout << "     ";
    if (Grandparent->name == "(Deleted)")
    {
        cout << Grandparent->name << "\n";
    }
    else
    {
        cout << Grandparent->name
             << " (Id: " << Grandparent->id
             << ", age: " << Grandparent->age
             << ", gender: " << Grandparent->gender << ")\n";
    }

    // Print this member's children (one level deeper)
    node *child = Grandparent->child;
    displayFamilyTree(child, level + 1);

    // Print siblings at the same level
    displayFamilyTree(Grandparent->siblingNext, level);
}

void deleteMember(node *grandParent, const string &id)
{
    node *target = findMember(grandParent, id);
    if (!target)
        return;

    // 1. delete the target member
    if (target->child != nullptr)
    {
        target->name = "(Deleted)";
    }
    else
    {
        // use BFS to find parent of target (traverse tree level by level)
        queue<node *> q;
        q.push(grandParent);

        while (!q.empty())
        {
            node *curr = q.front();
            q.pop();

            node *child = curr->child;

            // Case 1: target is the first child
            if (child == target)
            {
                curr->child = target->siblingNext;
                delete target;
                cout << "Node deleted successfully\n";
                return;
            }

            // Case 2: target is not the first child
            while (child != nullptr && child->siblingNext != nullptr)
            {
                if (child->siblingNext == target)
                {
                    child->siblingNext = target->siblingNext;
                    delete target;
                    cout << "Node deleted successfully\n";
                    return;
                }
                child = child->siblingNext;
            }

            // Continue BFS
            child = curr->child;
            while (child != nullptr)
            {
                q.push(child);
                child = child->siblingNext;
            }
        }

        cout << "Delete failed (parent not found)\n";
    }
}
void updateMemberInfo(node *grandParent, const string &id)
{
    node *target = findMember(grandParent, id);
    if (!target)
    {
        cout << "Invalid id" << id << endl;
        return;
    }
    // check if deleteled
    if (target->name == "(Deleted)")
    {
        cout << "It's deleted.\n";
        return;
    }

    string input;

    // update name
    cout << "Enter new name (Enter to continue): ";
    getline(cin, input);
    if (!input.empty())
    {
        target->name = input;
    }

    // update age
    cout << "Enter new age (Enter to continue): ";
    getline(cin, input);
    if (!input.empty())
    {
        try
        {
            int newAge = stoi(input); // Try to convert

            // Validation Logic
            if (newAge < 0)
            {
                while (true)
                {
                    cout << "Age must be non-negative. Please re-enter Age: ";
                    cin >> newAge;
                    cin.ignore();

                    if (newAge >= 0)
                    {
                        target->age = newAge;
                        break;
                    }
                }
            }
            else
            {
                target->age = newAge;
            }
        }
        catch (...)
        { // Catch any error (like user typing "abc")
            cout << "Invalid number format! Age not updated.\n";
        }
    }

    // update gender
    cout << "Enter new gender (Enter to continue): ";
    getline(cin, input);
    if (!input.empty())
    {
        target->gender = input;
    }

    cout << "Update successfully\n";
}

int main()
{
    // Initialize root of family tree
    grandParent = new node("0", 70, "Grandparent", " ");
    createFamilyTree(grandParent);

    // User menu
    string option;
    cout << "Type '1' to Add Member\n";
    cout << "Type '2' to Search Member by ID\n";
    cout << "Type '3' to delete Member by ID\n";
    cout << "Type '4' to Update Member Info by ID\n";
    cout << "Type '5' to Display Family Tree\n";
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
            // validate age input
            while (true)
            {
                if (age < 0)
                {
                    cout << "Age must be non-negative. Please re-enter Age: ";
                    cin >> age;
                    cin.ignore();
                }
                else
                {
                    break;
                }
            }

            cout << "Enter gender: ";
            getline(cin, gender);

            cout << "Enter relation to (parent name): ";
            getline(cin, relationTo);

            addMember(grandParent, id, age, name, gender, relationTo);
        }
        // Display Family Tree

        // Search Member by ID
        else if (option == "2")
        {
            string searchId;
            cout << "Enter ID to search: ";
            getline(cin, searchId);
            node *exist = findMember(grandParent, searchId);
            if (exist && exist->name != "(Deleted)")
            {
                cout << "Member found: " << exist->name << ", Age: " << exist->age << " gender: " << exist->gender << "\n";
            }
            else
            {
                cout << "Member with ID " << searchId << " not found.\n";
            }
        }
        // delete member
        else if (option == "3")
        {
            string deleteId;
            cout << "Enter ID to delete: ";
            getline(cin, deleteId);
            deleteMember(grandParent, deleteId);
        }
        // update member info
        else if (option == "4")
        {
            string updateId;
            cout << "Enter ID to update: ";
            getline(cin, updateId);
            updateMemberInfo(grandParent, updateId);
        }
        // Display Family Tree
        else if (option == "5")
        {
            cout << "==============================\n";
            cout << "Family Tree:\n";
            displayFamilyTree(grandParent);
            cout << "==============================\n";
        }
        else
        {
            cout << "Invalid option. Please try again.\n";
        }
    }
    return 0;
}
