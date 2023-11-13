//g++ -std=c++20 main.cpp 
//flags: -n maximum number of retryes, -o number of operations in each scenario, -s scenarios count
//example command: /.a.out -n 3 -o 3 -s 2 50 -100 400 -400 50 60
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>
#include <random>
#include <atomic>
#include <queue>
#include <getopt.h>

class BankAccount {
public:
    BankAccount(double initialBalance) : balance(initialBalance) {}

    void deposit(double amount) {
        std::lock_guard<std::mutex> lock(accountMutex);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        balance += amount;
        std::cout << "было=" << balance - amount << ", операция +" << amount << ", стало=" << balance<< std::endl;
        return;
    }

    bool withdraw(double amount, std::queue<int> *pending) {
        std::lock_guard<std::mutex> lock(accountMutex);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
            if (balance >= amount) {
                balance -= amount;
                std::cout << "было=" << balance + amount << ", операция -" << amount << ", стало=" << balance << std::endl;
                return true;
            } else {
                std::cout<<"Недостаточно средств. Операция -"<<amount<<", Баланс "<<balance<<std::endl;
                pending->push(amount);
                return false;
            }
    }

    double checkBalance() {
        std::lock_guard<std::mutex> lock(accountMutex);
        return balance;
    }

    double balance;
    std::mutex accountMutex;

};
void check_balance_thread(BankAccount &account) {
    while (true) {
        char command;
        std::cin >> command;
        if (command == 'c') {
            std::cout << "Текущий баланс: " << account.checkBalance() << std::endl;
        }
    }
}

void scenario(BankAccount &account, const std::vector<double> &operations, int maxRetries) {
    int attemptsCounter = 0;
    std::queue<int> pending;
    for (double amount : operations) {
        if (amount >= 0) {
            account.deposit(amount);
        } else {
            account.withdraw(-amount, &pending);
        }
        if(!pending.empty()){
            if(attemptsCounter < maxRetries){
                if(account.balance>pending.front()){
                    account.withdraw(pending.front(), &pending);
                    pending.pop();
                    attemptsCounter = 0;
                    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                }else{
                    attemptsCounter++;
                }
            }else{
                pending.pop();
                attemptsCounter = 0;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

}

int main(int argc, char* argv[]) {
    int maxRetries = 1;
    int numOperations = 1;
    int numScenarios = 1;
    int opt;
    while ((opt = getopt(argc, argv, "n:o:s:")) != -1) {
        switch (opt) {
            case 'n':
                maxRetries = std::atoi(optarg);
                break;
            case 'o':
                numOperations = std::atoi(optarg);
                break;
            case 's':
                numScenarios = std::atoi(optarg);
                break;
            default:
                std::cerr << "Использование: " << argv[0] << " -n <максимальное количество попыток> -o <количество операций>" << std::endl;
                return 1;
        }
    }

    BankAccount account(0);

    std::vector<std::thread> threads;

    int k = optind;
    while(numScenarios--){
        std::vector<double> _scenario;
        threads.emplace_back([&account]{ check_balance_thread(account);});
        for (int i = k; i < k + numOperations; i++) {
            _scenario.push_back(std::stod(argv[i]));
        }
        k += numOperations;
        threads.emplace_back([_scenario, &account, maxRetries] {
            scenario(account, _scenario, maxRetries);
        });
    }
    for (auto &thread : threads) {
        thread.join();
    }
    std::cout << "Остаток на счете: " << account.checkBalance() << std::endl;

    return 0;
}

