#include <iostream>
#include <vector>
#include <memory>

using std::string;
using std::vector;
using std::unique_ptr;
using std::make_unique;

enum class JobType { Programmer = 1, Designer, Artist, Barista, Unemployed };

class Human {
private:
    string name;
    JobType job;
    int age;
    double money;
    double hunger = 100;
    double thirst = 100;

public:
    Human(const string& name, JobType job, int age, double money)
        : name(name), age(age), money(money) 
    {
        if (age < 15 || age > 60) this->age = 18;
        if (job == JobType::Programmer || job == JobType::Designer ||
            job == JobType::Artist || job == JobType::Barista)
        {
            this->job = job;
        } else {
            this->job = JobType::Unemployed;
        }

        if (money < 500) this->money = 500;
        else if (money > 10000) this->money = 10000;
    }

    // Getter
    string getName() const { return name; }
    JobType getJob() const { return job; }
    int getAge() const { return age; }
    double getMoney() const { return money; }
    double getHunger() const { return hunger; }
    double getThirst() const { return thirst; }

    // Setter
    void setName(const string& newName) { name = newName; }
    void setAge(int newAge) { age = newAge; }
    void setJob(JobType newJob) { job = newJob; }
    void setMoney(double newMoney) { money = newMoney; }
    void setHunger(double newHunger) { hunger = newHunger; }
    void setThirst(double newThirst) { thirst = newThirst; }

    // Logic
    void eat() { hunger = 100; }
    void drink() { thirst = 100; }
    void pay(double amount) { money -= amount; }
    void earn(double amount) { money += amount; }

    bool canAfford(double amount) const { return money >= amount; }

    bool work() {
        if (hunger < 50 || thirst < 50) return false;

        switch (job) {
            case JobType::Programmer: earn(1000); break;
            case JobType::Designer:   earn(900);  break;
            case JobType::Artist:     earn(800);  break;
            case JobType::Barista:    earn(650);  break;
            default:                  earn(300);  break;
        }

        hunger -= 50;
        thirst -= 50;
        return true;
    }

    string getJobString() const {
        switch (job) {
            case JobType::Programmer: return "Programmer";
            case JobType::Designer:   return "Designer";
            case JobType::Artist:     return "Artist";
            case JobType::Barista:    return "Barista";
            case JobType::Unemployed: return "Unemployed";
            default:                  return "Unknown";
        }
    }
};

// Global variable and declaration
vector<unique_ptr<Human>> players;

bool mainMenu();
int mainMenuResponse(int response);
void createPlayer();
void selectPlayer();
void deletePlayer();
void playerMenu(Human* player);

// Main
int main() {
    while (mainMenu());
    return 0;
}

// Main menu
bool mainMenu() {
    int input = 0;
    while (input < 1 || input > 4) {
        std::system("cls");
        std::cout << "> 1. Create Player\n";
        std::cout << "> 2. Select Player\n";
        std::cout << "> 3. Delete Player\n";
        std::cout << "> 4. Quit Game\n";
        std::cout << "> #: "; std::cin >> input;
    }
    return mainMenuResponse(input);
}

int mainMenuResponse(int response) {
    switch (response) {
        case 1: createPlayer(); break;
        case 2: selectPlayer(); break;
        case 3: deletePlayer(); break;
        case 4: std::cout << "Thanks for playing!\n"; return false;
    }
    return true;
}

// Create Player
void createPlayer() {
    string name;
    int age, jobInput;
    double money;

    std::system("cls");
    std::cout << "#Insert Name: "; std::cin >> name;
    std::cout << "#Insert Age: "; std::cin >> age;

    std::cout << "List available jobs:\n"
              << "1. Programmer ($1000)\n"
              << "2. Designer  ($900)\n"
              << "3. Artist    ($800)\n"
              << "4. Barista   ($650)\n"
              << "#Insert Job #: "; std::cin >> jobInput;

    std::cout << "#Insert Starting Money: "; std::cin >> money;

    JobType job = static_cast<JobType>(jobInput);
    players.push_back(make_unique<Human>(name, job, age, money));
}

// Select Player
void selectPlayer() {
    if (players.empty()) {
        std::cout << "No players created.\n";
        std::cin.ignore(); std::cin.get();
        return;
    }

    int input;
    std::system("cls");
    std::cout << "> 0. Exit to main menu\n";
    for (size_t i = 0; i < players.size(); ++i) {
        auto& p = players[i];
        std::cout << "> " << i+1 << ". " << p->getName() << " | " << p->getAge()
                  << " | " << p->getJobString() << " | $" << p->getMoney() << "\n";
    }

    std::cout << "> Select Player #: "; std::cin >> input;
    if (input == 0) return;
    if (input < 1 || input > players.size()) return;

    playerMenu(players[input - 1].get());
}

// Delete Player
void deletePlayer() {
    if (players.empty()) return;

    int input;
    std::system("cls");
    std::cout << "> 0. Exit to main menu\n";
    for (size_t i = 0; i < players.size(); ++i) {
        auto& p = players[i];
        std::cout << "> " << i+1 << ". " << p->getName() << " | " << p->getAge()
                  << " | " << p->getJobString() << " | $" << p->getMoney() << "\n";
    }

    std::cout << "> Select Player #: "; std::cin >> input;
    if (input == 0) return;
    if (input < 1 || input > players.size()) return;

    players.erase(players.begin() + (input - 1));
}

// Player Menu
void playerMenu(Human* p) {
    int choice = 0;
    string state = "Idle";

    while (choice != 5) {
        std::system("cls");
        std::cout << " | Name: " << p->getName()
                  << "\n | Age: " << p->getAge()
                  << "\n | Job: " << p->getJobString()
                  << "\n | Money: $" << p->getMoney()
                  << "\n | State: " << state
                  << "\n | Hunger: " << p->getHunger()
                  << "\n | Thirst: " << p->getThirst() << "\n"
                  << "----------------------------------------\n"
                  << "1. Work (-50 Hunger & Thirst)\n"
                  << "2. Eat  ($130, +100 Hunger)\n"
                  << "3. Drink ($70, +100 Thirst)\n"
                  << "4. Night Club ($500, +100 Hunger & Thirst)\n"
                  << "5. Exit\n"
                  << "> Option: "; std::cin >> choice;

        switch (choice) {
            case 1:
                if (!p->work()) state = "Too tired to work.";
                else state = "Worked successfully.";
                break;
            case 2:
                if (!p->canAfford(130)) state = "Not enough money for food.";
                else {
                    p->pay(130); p->eat(); state = "Ate food.";
                }
                break;
            case 3:
                if (!p->canAfford(70)) state = "Not enough money for drink.";
                else {
                    p->pay(70); p->drink(); state = "Drank water.";
                }
                break;
            case 4:
                if (!p->canAfford(500)) state = "Can't afford the club.";
                else {
                    p->pay(500); p->eat(); p->drink(); state = "Had fun at the club.";
                }
                break;
            default:
                state = "Idle";
                break;
        }
    }
}