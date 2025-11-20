#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <ctime>
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
void displayTopicMenu(int taskIndex);
void listTopics(int taskIndex, string action);
void toggleTopicStatus(int taskIndex);
void addNewTopic(int taskIndex);
void deleteTopic(int taskIndex);

void displayMainMenu() {
    cout << "____________________________________________________________" << endl;
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
            cout << "taskIndex: " << taskIndex << endl;
            if (taskIndex + 1 != 0) {
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
        return -1;
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
    displayTopicMenu(taskIndex);
    cout << "Status: " << tasks[taskIndex].status << endl;
    cout << "Press ENTER to skip, or enter new status (In Progress/Completed/Canceled): ";
    string newStatus;
    getline(cin.ignore(), newStatus);
    while (newStatus.empty() || newStatus != "In Progress" || newStatus != "Completed" || newStatus != "Canceled") {
        if (newStatus.empty()) {
            cout << "Status is required. Please enter a status: ";
            getline(cin, newStatus);
        }
        else if (newStatus != "In Progress" && newStatus != "Completed" && newStatus != "Canceled") {
            cout << "Invalid status. Must be: In Progress, Completed, or Canceled" << endl;
            cout << "Please enter a valid status: ";
            getline(cin, newStatus);
        }
        else {
            break;
        }
    }
    tasks[taskIndex].status = newStatus;
    tasks[taskIndex].updatedAt = getCurrentDateTime();
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
void listTopics(int taskIndex, string action) {
    cout << "Topics:" << endl;
    for (int i = 0; i < tasks[taskIndex].topicCount; i++) {
        if (tasks[taskIndex].topicCompleted[i] == true) {
            if (action == "list") {
                cout << "- " << tasks[taskIndex].topics[i] << ": y" << endl;
            }
            else {
                cout << i + 1 << ": " << tasks[taskIndex].topics[i] << ": y" << endl;
            }
        }
        else {
            if (action == "list") {
                cout << "- " << tasks[taskIndex].topics[i] << ": n" << endl;
            }
            else {
                cout << i + 1 << ": " << tasks[taskIndex].topics[i] << ": n" << endl;
            }
        }
    }
    if (action == "delete") {
        cout << "0. Cancel Delete Topic" << endl;
    }
    else if (action == "toggle") {
        cout << "0. Cancel Toggle Topic Status" << endl;
    }
}
void displayTopicMenu(int taskIndex) {
    listTopics(taskIndex, "list");
    cout << "1. Toggle status of topic" << endl;
    cout << "2. Add new topic" << endl;
    cout << "3. Delete topic" << endl;
    cout << "0. Continue" << endl;
    cout << "Enter choice: ";
    int choice;
    cin >> choice;
    if (choice == 0) {
        return;
    }
    else if (choice == 1) {
        toggleTopicStatus(taskIndex);
    }
    else if (choice == 2) {
        addNewTopic(taskIndex);
    }
    else if (choice == 3) {
        deleteTopic(taskIndex);
    }
}
void toggleTopicStatus(int taskIndex) {
    listTopics(taskIndex, "toggle");
    cout << "Enter topic number to toggle status: ";
    int topicIndex;
    while (1==1) {
        cin >> topicIndex;
        if (topicIndex > 0 && topicIndex <= tasks[taskIndex].topicCount) {
            tasks[taskIndex].topicCompleted[topicIndex - 1] = !tasks[taskIndex].topicCompleted[topicIndex - 1];
            tasks[taskIndex].updatedAt = getCurrentDateTime();
            break;
        } else if (topicIndex == 0) {
            return;
        }
        else {
            cout << "Invalid topic number. Please enter a valid topic number: ";
        }
    }
    displayTopicMenu(taskIndex);
}
void addNewTopic(int taskIndex) {
    cout << "Enter new topic: ";
    string newTopic;
    getline(cin.ignore(), newTopic);
    while (newTopic.empty()) {
        cout << "Topic is required. Please enter a topic: ";
        getline(cin.ignore(), newTopic);
    }
    tasks[taskIndex].topics[tasks[taskIndex].topicCount] = newTopic;
    tasks[taskIndex].topicCompleted[tasks[taskIndex].topicCount] = false;
    tasks[taskIndex].topicCount++;
    displayTopicMenu(taskIndex);
}
void deleteTopic(int taskIndex) {
    listTopics(taskIndex, "delete");
    cout << "Enter topic number to delete: ";
    int topicIndex;
    while (1==1) {
        cin >> topicIndex;
        if (topicIndex > 0 && topicIndex <= tasks[taskIndex].topicCount) {
            break;
        }
        else if (topicIndex == 0) {
            return;
        }
        else {
            cout << "Invalid topic number. Please enter a valid topic number: ";
        }
    }
    topicIndex--;
    for (int i = topicIndex; i < tasks[taskIndex].topicCount - 1; i++) {
        tasks[taskIndex].topics[i] = tasks[taskIndex].topics[i + 1];
        tasks[taskIndex].topicCompleted[i] = tasks[taskIndex].topicCompleted[i + 1];
    }
    tasks[taskIndex].topicCount--;
    tasks[taskIndex].updatedAt = getCurrentDateTime();
    if (tasks[taskIndex].topicCount == 0) {
        cout << "No topics found. Please add at least one topic." << endl;
        addNewTopic(taskIndex);
        return;
    }
    displayTopicMenu(taskIndex);
}