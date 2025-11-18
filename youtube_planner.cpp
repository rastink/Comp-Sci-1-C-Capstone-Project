#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <ctime>
#include <chrono>
#include <vector>
#include <algorithm>
#include <random>
#include <sstream>
#include <istream>

using namespace std;

// ============================================================================
// CONSTANTS
// ============================================================================
const int MAX_TASKS = 100;
const int MAX_TOPICS = 50;
const string FILENAME = "youtube_tasks.txt";

// ============================================================================
// STRUCTURE DEFINITION
// ============================================================================
class Task {
    public:
        string title;
        string description;
        string status;
        string createdAt;
        string updatedAt;
        string topics[MAX_TOPICS];
        bool topicCompleted[MAX_TOPICS];
        int topicCount;
    };

Task tasks[MAX_TASKS];
int taskCount = 0;
string filename = FILENAME;
// ============================================================================
// FUNCTION DECLARATIONS
// ============================================================================
void displayMainMenu();
int listTasks(string action);
void displayTaskDetails(int taskIndex);
void editTasks();
void editTask(int taskIndex);
void createTask();
void saveToFile();
void loadFromFile();
void deleteTask(int taskIndex);
void quit();
int calculatePercentComplete(Task task);
string getCurrentDateTime();
void validateStatus(string status);
void displayMainMenu();

void displayMainMenu() {
    cout << "1. View Tasks" << endl;
    cout << "2. Edit Tasks" << endl;
    cout << "3. Create Task" << endl;
    cout << "4. Delete Task" << endl;
    cout << "5. Save" << endl;
    cout << "6. Quit" << endl;
    cout << "Enter choice: ";
}


// ============================================================================
// MAIN FUNCTION
// ============================================================================
int main() {
    loadFromFile();
    while (true) {
        // cout << "displayMainMenu()" << endl;
        displayMainMenu();
        int choice;
        cin >> choice;
        if (choice == 1) {
            // cout << "viewTasks()" << endl;
            int taskIndex = listTasks("view");
            if (taskIndex != 0) {
                displayTaskDetails(taskIndex);
            }
        } else if (choice == 2) {
                // cout << "editTasks()" << endl;
                int taskIndex = listTasks("edit");
                if (taskIndex >= 0) {
                    editTask(taskIndex);
                }
        } else if (choice == 3) {
            // cout << "createTask()" << endl;
            createTask();
        } else if (choice == 4) {
            // cout << "deleteTask()" << endl;
            int taskIndex = listTasks("delete");
            if (taskIndex >= 0) {
                deleteTask(taskIndex);
            }
        } else if (choice == 5) {
            // cout << "saveToFile()" << endl;
            saveToFile();
        } else if (choice == 6) {
            // cout << "quit()" << endl;
            quit();
            break;
        } else {
            cout << "Invalid choice. Please enter 1-5." << endl;
            }
    }
    cout << "Goodbye!" << endl;
    return 0;
}


// ============================================================================
// VIEW TASKS
// ============================================================================
int listTasks(string action) {
    // cout << "In viewTasks() function" << endl;
    if (taskCount == 0) {
        cout << "No tasks found." << endl;
        return 0;
    }
    cout << "Select task to " << action << " (0 to return):" << endl;
    for (int i = 0; i < taskCount; i++) {
        cout << (i+1) << ". " << tasks[i].title << endl;
    }
    cout << "0. Return to main menu" << endl;
    int taskSelection;
    cin >> taskSelection;
    if (taskSelection == 0) {
        return 0;
    } else if (taskSelection < 1 || taskSelection > taskCount) {
        cout << "Invalid task number." << endl;
        return 0;
    }
    return taskSelection - 1;
}

// ============================================================================
// DISPLAY TASK DETAILS
// ============================================================================
void displayTaskDetails(int taskIndex) {
    // cout << "In displayTaskDetails() function" << endl;
    cout << "Title: " << tasks[taskIndex].title << endl;
    cout << "Description: " << tasks[taskIndex].description << endl;
    cout << "Status: " << tasks[taskIndex].status << endl;
    cout << "Created At: " << tasks[taskIndex].createdAt << endl;
    cout << "Updated At: " << tasks[taskIndex].updatedAt << endl;
    int percent = calculatePercentComplete(tasks[taskIndex]);
    cout << "Percent Complete: " << percent << "%" << endl;
    cout << "Topics:" << endl;
    for (int i = 0; i < tasks[taskIndex].topicCount; i++) {
        cout << "- " << tasks[taskIndex].topics[i] << ": " << (tasks[taskIndex].topicCompleted[i] ? "y" : "n") << endl;
    }
}

// ============================================================================
// EDIT TASK
// ============================================================================
void editTask(int taskIndex) {
    // cout << "In editTask() function" << endl;
    if (taskIndex < 0 || taskIndex >= taskCount) {
        cout << "Invalid task index." << endl;
        return;
    }
    cin.ignore(); // Clear the newline left in buffer from previous cin >>
    cout << "Title: " << tasks[taskIndex].title << endl;
    cout << "Press ENTER to skip, or type new title: ";
    string newTitle;
    getline(cin, newTitle);
    if (!newTitle.empty()) {
        tasks[taskIndex].title = newTitle;
        tasks[taskIndex].updatedAt = getCurrentDateTime();
    }
    cout << "Description: " << tasks[taskIndex].description << endl;
    cout << "Press ENTER to skip, or type new description: ";
    string newDescription;
    getline(cin, newDescription);
    if (!newDescription.empty()) {
        tasks[taskIndex].description = newDescription;
        tasks[taskIndex].updatedAt = getCurrentDateTime();
    }
    // TODO: Add code to edit completed and incomplete topics
    cout << "Status: " << tasks[taskIndex].status << endl;
    cout << "Press ENTER to skip, or enter new status (In Progress/Completed/Canceled): ";
    string newStatus;
    getline(cin, newStatus);
    if (!newStatus.empty()) {
        if (newStatus == "In Progress" || newStatus == "Completed" || newStatus == "Canceled") {
            tasks[taskIndex].status = newStatus;
            tasks[taskIndex].updatedAt = getCurrentDateTime();
        }
        else {
            cout << "Invalid status. Must be: In Progress, Completed, or Canceled" << endl;
            cout << "Please enter a valid status: ";
            getline(cin, newStatus);
            tasks[taskIndex].status = newStatus;
            tasks[taskIndex].updatedAt = getCurrentDateTime();
        }
    }
    cout << "Task updated successfully." << endl;
}   

// ============================================================================
// DELETE TASK
// ============================================================================
void deleteTask(int taskIndex) {
    // cout << "In deleteTask() function" << endl;
    if (taskIndex < 0 || taskIndex >= taskCount) {
        cout << "Invalid task index." << endl;
        return;
    }
    for (int i = taskIndex; i < taskCount - 1; i++) {
        // cout << "tasks[i]: " << tasks[i].title << endl;
        // cout << "tasks[i + 1]: " << tasks[i + 1].title << endl;
        tasks[i] = tasks[i + 1];
    }
    taskCount--;
    cout << "Task deleted successfully." << endl;
}
// ============================================================================
// CREATE TASK
// ============================================================================
void createTask() {
    // cout << "In createTask() function" << endl;
    if (taskCount >= MAX_TASKS) {
        cout << "Maximum number of tasks reached." << endl;
        return;
    }
    Task newTask;
    newTask.topicCount = 0;
    newTask.createdAt = getCurrentDateTime();
    newTask.updatedAt = getCurrentDateTime();
    cout << "Enter title (required): ";
    getline(cin.ignore(), newTask.title);
    // cout << "newTask.title: " << newTask.title << endl;
    while (newTask.title.empty()) {
        cout << "Title is required. Please enter a title: ";
        getline(cin, newTask.title);
        // cout << "newTask.title: " << newTask.title << endl;
    }
    cout << "Enter description (press ENTER to skip): ";
    getline(cin, newTask.description);
    // cout << "newTask.description: " << newTask.description << endl;
    newTask.status = "Planning";
    cout << "Enter first topic (at least one topic is required): ";
    string topicInput;

    while (1==1) {
        getline(cin, newTask.topics[0]);
        // cout << "newTask.topics[0]: " << newTask.topics[0] << endl;
        if(!newTask.topics[0].empty()) {
            newTask.topicCompleted[0] = false;
            newTask.topicCount++;
            break;
        }
        else{
            cout << "Atleast one topic is required. Please enter atleast one topic: ";
        }
    }
    for (int i = 1; i<5; ++i) {
        cout << "Enter next topic or DONE to finish: ";
        getline(cin, topicInput);
        // cout << "topicInput: " << topicInput << endl;
        if (topicInput == "DONE") {
           break;
        }
        // handle new topic
        newTask.topics[i] = topicInput;
        newTask.topicCompleted[i] = false;
        newTask.topicCount++;
    }
    // user finished creating five topics, create new task
    tasks[taskCount] = newTask;
    taskCount++;
    cout << "Task created successfully." << endl;
}
// ============================================================================
// CALCULATE PERCENT COMPLETE
// ============================================================================
int calculatePercentComplete(Task task) {
    // cout << "In calculatePercentComplete() function" << endl;
    int completedCount = 0;
    for (int i = 0; i < task.topicCount; i++) {
        if (task.topicCompleted[i] == true) {
            completedCount++;
        }
    }
    int percent = (completedCount * 100) / task.topicCount;
    return percent;
}

// ============================================================================
// SAVE TO FILE
// ============================================================================
// save tasks to file if file already exists, overwrite it, if not create new file
void saveToFile() {
    // cout << "In saveToFile() function" << endl;
    ofstream file(filename, ios::out | ios::trunc);
    if (!file.is_open()) {
        cout << "Error: Could not save to file." << endl;
        return;
    }
    cout << "Saving tasks to file: " << filename << endl;
    for (int i = 0; i < taskCount; i++) {
        file << "--" << endl;
        file << tasks[i].title << endl;
        file << tasks[i].description << endl;
        file << tasks[i].status << endl;
        file << tasks[i].createdAt << endl;
        file << tasks[i].updatedAt << endl;
        file << tasks[i].topicCount << endl;
        for (int j = 0; j < tasks[i].topicCount; j++) {
            file << tasks[i].topics[j] << endl;
            file << tasks[i].topicCompleted[j] << endl;
        }
    }
    file.close();
    cout << "Data saved to " << filename << endl;
}

// ============================================================================
// LOAD FROM FILE
// ============================================================================
void loadFromFile() {
    // cout << "In loadFromFile() function" << endl;
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Could not load from file." << endl;
        return;
    }

    taskCount = 0;
    string taskHeader;
    getline(file, taskHeader);
    cout << "Loading tasks from file: " << filename << endl;
    while (taskHeader == "--") { 
        cout << "Loading task: " << taskCount << endl;
        Task newTask;
        getline(file, newTask.title);
        getline(file, newTask.description);
        getline(file, newTask.status);
        getline(file, newTask.createdAt);
        getline(file, newTask.updatedAt);
        file >> newTask.topicCount;
        file.ignore(); // consume the newline after topicCount
        for (int i = 0; i < newTask.topicCount; i++) {
            getline(file, newTask.topics[i]);
            file >> newTask.topicCompleted[i];
            file.ignore(); // consume the newline after topicCompleted
        }
        tasks[taskCount] = newTask;
        taskCount++;
        getline(file, taskHeader); // read the next line which should be "--" or end of file
        // cout << "taskHeader end of loop: " << taskHeader << endl;
    }
    file.close();
    cout << "Tasks loaded from file: " << filename << endl;
}

// ============================================================================
// QUIT
// ============================================================================
void quit() {
    // cout << "In quit() function" << endl;
    cout << "Do you want to save before quitting? (y/n): ";
    string saveChoice;
    cin >> saveChoice;
    if (saveChoice == "y" || saveChoice == "Y") {
        saveToFile();
    }
    else if (saveChoice == "n" || saveChoice == "N") {
        cout << "Quitting without saving. Goodbye!" << endl;
    }
    else {
        cout << "Invalid choice. Quitting without saving." << endl;
    }
}
// ============================================================================
// HELPER FUNCTIONS
// ============================================================================
// Get current date and time as string
string getCurrentDateTime() {
    // Get current time
    time_t now = time(0);
    tm* localTime = localtime(&now);
    
    // Format as string (example: "3/22/2026 22:32")
    stringstream ss;
    ss << (localTime->tm_mon + 1) << "/" 
       << localTime->tm_mday << "/" 
       << (localTime->tm_year + 1900) << " "
       << setfill('0') << setw(2) << localTime->tm_hour << ":"
       << setfill('0') << setw(2) << localTime->tm_min;
    
    return ss.str();
}
