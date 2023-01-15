#include <iostream>
#include <fstream>
#include <vector>
#include <functional>
#include <random>

// Klasa reprezentująca problem podzbioru sumy
class SubsetSumProblem {
public:
// Konstruktor przyjmujący wektor elementów oraz sumę jakiej ma dany podzbiór elementów
    SubsetSumProblem(const std::vector<int>& set, int targetSum) : set_(set), targetSum_(targetSum) {}

    std::vector<int> randomSample() {
        std::vector<int> subset;
        int currentSum = 0;
        while (currentSum != targetSum_) {
            subset.clear();
            currentSum = 0;
            for (int i = 0; i < set_.size(); i++) {
                if (std::rand() % 2 == 1) { // losowanie czy element ma zostać dodany do podzbioru
                    subset.push_back(set_[i]);
                    currentSum += set_[i];
                }
            }
        }
        return subset;
    }

    // Metoda rozwiązująca problem podzbioru sumy za pomocą algorytmu wspinaczkowego
    std::vector<int> solveWithClimbingAlgorithm() {
        // Tworzenie początkowego rozwiązania - pustego podzbioru
        std::vector<int> currentSolution;
        // Tworzenie najlepszego dotychczasowego rozwiązania - pustego podzbioru
        std::vector<int> bestSolution;
        // Obliczenie aktualnej sumy
        int currentSum = 0;
        // Obliczenie najlepszej dotychczasowej sumy
        int bestSum = 0;
        // Pętla while, która będzie działać, dopóki nie zostanie znalezione rozwiązanie
        while (bestSum != targetSum_) {
            // Przeglądanie elementów wektora
            for (int i = 0; i < set_.size(); i++) {
                // Jeśli element nie znajduje się w aktualnym rozwiązaniu
                if (std::find(currentSolution.begin(), currentSolution.end(), set_[i]) == currentSolution.end()) {
                    // Dodanie elementu do aktualnego rozwiązania
                    currentSolution.push_back(set_[i]);
                    // Obliczenie aktualnej sumy
                    currentSum += set_[i];
                    // Jeśli aktualna suma jest większa niż najlepsza dotychczas suma
                    if (currentSum > bestSum) {
                        bestSum = currentSum;
                        bestSolution = currentSolution;
                    }
                }
            }
// Jeśli nie udało się znaleźć lepszego rozwiązania zwracamy najlepsze dotychczasowe
            if (bestSum == currentSum) {
                return bestSolution;
            }
// Usuwanie ostatniego elementu z aktualnego rozwiązania
            currentSum -= currentSolution.back();
            currentSolution.pop_back();
        }
        return bestSolution;
    }

// Metoda ustawiająca wartość target sum
    void setTargetSum(int targetSum) { targetSum_ = targetSum; }

// Przeciążony operator wyjścia, który pozwala na wypisanie problemu
    friend std::ostream& operator<<(std::ostream& os, const SubsetSumProblem& problem) {
        os << "Target sum: " << problem.targetSum_ << std::endl;
        os << "Set: [ ";
        for (const auto& element : problem.set_) {
            os << element << " ";
        }
        os << "]" << std::endl;

        return os;
    }

// Metoda rozwiązująca problem podzbioru sumy
    std::vector<int> solve() {
        std::vector<int> currentSolution;
        int currentSum = 0;
        // Tworzenie generatora liczb losowych
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, set_.size() - 1);

        while (currentSum != targetSum_) {
            // Losowanie indeksu elementu
            int randomIndex = dis(gen);
            // Jeśli element nie znajduje się w aktualnym rozwiązaniu
            if (std::find(currentSolution.begin(), currentSolution.end(), set_[randomIndex]) == currentSolution.end()) {
                // Dodanie elementu do aktualnego rozwiązania
                currentSolution.push_back(set_[randomIndex]);
                currentSum += set_[randomIndex];
            }
        }
        return currentSolution;
    }

    int targetSum_;// suma jakiej ma dany podzbiór elementów

private:
    std::vector<int> set_; // wektor elementów
};

// Klasa reprezentująca rozwiązanie problemu podzbioru sumy
class SubsetSumSolution {
public:
// Konstruktor przyjmujący podzbiór elementów oraz sumę jaką oni reprezentują
    SubsetSumSolution(const std::vector<int>& subset, int targetSum) : subset_(subset), targetSum_(targetSum) {}

// Przeciążony operator wyjścia, który pozwala na wypisanie rozwiązania
    friend std::ostream& operator<<(std::ostream& os, const SubsetSumSolution& solution) {
        os << "Subset: [ ";
        for (const auto& element : solution.subset_) {
            os << element << " ";
        }
        os << "], sum: " << solution.sum();
        return os;
    }

// Metoda zwracająca sumę elementów podzbioru
    int sum() const {
        int sum = 0;
        for (const auto& element : subset_) {
            sum += element;
        }
        return sum;
    }
private:
    std::vector<int> subset_; // wektor elementów podzbioru
    int targetSum_; // suma jaką reprezentują elementy podzbioru
};


int main() {
    std::ifstream inputFile("input.txt");
    if (!inputFile.is_open()) {
        std::cout << "Unable to open input.txt file" << std::endl;
        return 1;
    }

    int targetSum;
    inputFile >> targetSum;
    std::vector<int> set;
    int temp;
    // Wczytywanie elementów z pliku
    while (inputFile >> temp) {
        set.push_back(temp);
    }

    int iteration_count = 0;
    // Tworzenie obiektu reprezentującego problem podzbioru sumy
    SubsetSumProblem problem(set, targetSum);

    // Tworzenie funkcji, która iteruje po całej dziedzinie problemu
    auto iterate = [&problem, &iteration_count](std::function<void(const SubsetSumProblem&)> callback) {
        for (int i = 1; i <= problem.targetSum_; i++) {
            iteration_count++;
            problem.setTargetSum(i);
            callback(problem);
        }
    };

// Tworzenie wektora, do którego będą zapisywane rozwiązania
    std::vector<SubsetSumSolution> solutions;
// Iterowanie po wszystkich punktach dziedziny
    iterate([&solutions](SubsetSumProblem problem) {
        auto subset = problem.solve();
        if (!subset.empty()) {
            SubsetSumSolution solution(subset, problem.targetSum_);
            // Dodanie rozwiązania do wektora
            solutions.push_back(solution);
        }
    });

    std::cout << "Iteration count: " << iteration_count << std::endl;
// Tworzenie 2D tablicy z rozmiarem (targetSum + 1) x (set.size() + 1)
    std::vector<std::vector<bool>> dp(targetSum + 1, std::vector<bool>(set.size() + 1));

// Tworzenie tablicy dynamicznej, w której każdy wiersz odpowiada sumie, a każda kolumna odpowiada elementowi zbioru
// Wartości w tablicy reprezentują czy dana suma jest możliwa do osiągnięcia z danych elementów zbioru
    for (int i = 0; i <= set.size(); i++) {
        dp[0][i] = true;
    }

// Warunek początkowy - suma 0 jest możliwa z każdego podzbioru
    for (int i = 1; i <= targetSum; i++) {
        for (int j = 1; j <= set.size(); j++) {
            dp[i][j] = dp[i][j - 1];
            // Przepisywanie wartości z poprzedniego stanu
            if (i >= set[j - 1]) {
                dp[i][j] = dp[i][j] || dp[i - set[j - 1]][j - 1];
            }
        }
    }

// Sprawdz if target sum can be reached
    if (!dp[targetSum][set.size()]) {
        // Jeśli nie ma rozwiązania
        std::cout << "Target sum can't be reached with the given set" << std::endl;
        return 1;
    }

// Znajdz subset ktory osiaga target sum
    std::vector<int> subset;
    int i = targetSum, j = set.size();
    // Przeglądanie tablicy dynamicznej w celu znalezienia elementów podzbioru
    while (i > 0 && j > 0) {
        // Jezeli ten element wlaczony w subset
        if (dp[i][j] == dp[i][j - 1]) {
            j--;
        } else {
            // current element nie jest wlaczony w subset
            subset.push_back(set[j - 1]);
            i -= set[j - 1];
            j--;
        }
    }

    SubsetSumSolution solution(subset, targetSum);

    std::ofstream outputFile("output.txt");
    if (!outputFile.is_open()) {
        std::cout << "Unable to open output.txt file" << std::endl;
        return 1;
    }

    outputFile << solution << std::endl;

    return 0;
}