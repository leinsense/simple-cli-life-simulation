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
    string 
        name,
        status = "Idle";

    double
        energy = 100,
        hunger = 100,
        thirst = 100,
        money;
    
    int 
        day = 1,
        hour = 8,
        age;
        
    JobType job;


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
    string getName()    const { return name; }
    string getStatus()  const { return status; }
    JobType getJob()    const { return job; }
    int getDay()        const { return day; }
    int getHour()       const { return hour; }
    int getAge()        const { return age; }
    double getEnergy()  const { return energy; }
    double getHunger()  const { return hunger; }
    double getThirst()  const { return thirst; }
    double getMoney()   const { return money; }

    // Setter
    void setName(const string& newName) { name = newName; }
    void setStatus(const string& newStatus) {status = newStatus; }
    void setDay(int newDay) { age = newDay; }
    void setHour(int newHour) { age = newHour; }
    void setAge(int newAge) { age = newAge; }
    void setJob(JobType newJob) { job = newJob; }
    void setEnergy(double newEnergy) { hunger = newEnergy; }
    void setHunger(double newHunger) { hunger = newHunger; }
    void setThirst(double newThirst) { thirst = newThirst; }
    void setMoney(double newMoney) { money = newMoney; }

    // Logic
    void sleep(){
        while(energy < 100){
            energy+=15;
            if(energy > 100) energy = 100;
            passHour(1);
        }
    }

    void eat() { hunger = 100; }
    void drink() { thirst = 100; }
    void pay(double amount) { money -= amount; }
    void earn(double amount) { money += amount; }

    bool canAfford(double amount) const { return money >= amount; }

    bool work() {
        if (energy < 50 || hunger < 50 || thirst < 50) return false;

        switch (job) {
            case JobType::Programmer: earn(1000); break;
            case JobType::Designer:   earn(900);  break;
            case JobType::Artist:     earn(800);  break;
            case JobType::Barista:    earn(650);  break;
            default:                  earn(300);  break;
        }

        energy -= 50;
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

    void passHour(int h) {
        hour += h;
        if(hour >= 24) {
            hour %= 24;
            day++;
        }
    }
};

class Programmer : public Human{

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

    while (choice != 6) {
        std::system("cls");
        std::cout << " | Name: " << p->getName() << " | Age: " << p->getAge()
                  << "\n | Job: " << p->getJobString() << " | Money: $" << p->getMoney()
                  << "\n | Energy: " << p->getEnergy() << " | Hunger: " << p->getHunger()<< " | Thirst: " << p->getThirst()
                  << "\n | Status: " << p->getStatus()
                  << "\n | Day: " << p->getDay() << " | Hour: " << p->getHour() << '\n'
                  << "----------------------------------------\n"
                  << "1. Work (-50 Energy, Hunger & Thirst)\n"
                  << "2. Eat  ($130, +100 Hunger)\n"
                  << "3. Drink ($70, +100 Thirst)\n"
                  << "4. Night Club ($500, +100 Hunger & Thirst)\n"
                  << "5. Sleep (restore energy 15/hour)\n"
                  << "6. Exit\n"
                  << "> Option: "; std::cin >> choice;

        switch (choice) {
            case 1:
                if (!p->work()) p->setStatus("Too tired to work.");
                else p->setStatus("Worked succesfully");
                break;
            case 2:
                if (!p->canAfford(130)) p->setStatus("Not enough money for food.");
                else {
                    p->pay(130); p->eat(); p->setStatus("Ate food.");
                }
                break;
            case 3:
                if (!p->canAfford(70)) p->setStatus("Not enough money for drink.");
                else {
                    p->pay(70); p->drink(); p->setStatus("Drank water.");
                }
                break;
            case 4:
                if (!p->canAfford(500)) p->setStatus("Can't afford the club.");
                else {
                    p->pay(500); p->eat(); p->drink(); p->setStatus("Had fun at the club.");
                }
                break;
            case 5:
                p->sleep(); p->setStatus("Waking up from the sleep.");
                break;
            default:
                p->setStatus("Idle");
                break;
        }
    }
}