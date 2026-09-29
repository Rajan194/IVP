#include <bits/stdc++.h>
using namespace std;

int x[] = {2, 5, 6, 8, 1, 7, 3};
int y[] = {6, 2, 7, 3, 4, 6, 1};

random_device rd;
mt19937 gen(rd());

char name(int n)
{
    return 'A' + n;
}

double distanceBetween(int a, int b)
{
    return sqrt(
        (x[a] - x[b]) * (x[a] - x[b]) +
        (y[a] - y[b]) * (y[a] - y[b])
    );
}

double cost(vector<int> route)
{
    double total = 0;

    total += sqrt(
        x[route[0]] * x[route[0]] +
        y[route[0]] * y[route[0]]
    );

    for(int i = 0; i < 6; i++)
        total += distanceBetween(route[i], route[i + 1]);

    total += sqrt(
        x[route[6]] * x[route[6]] +
        y[route[6]] * y[route[6]]
    );

    return total;
}

void printRoute(vector<int> route)
{
    cout << "W -> ";
    for(int i = 0; i < 7; i++)
    {
        cout << name(route[i]);
        if(i != 6)
            cout << " -> ";
    }
    cout << " -> W";
}

vector<int> randomRoute()
{
    vector<int> route = {0, 1, 2, 3, 4, 5, 6};
    shuffle(route.begin(), route.end(), gen);
    return route;
}

double hillClimbing(vector<int> &finalRoute, int &iterations, bool showOutput = true)
{
    vector<int> current = randomRoute();
    double currentCost = cost(current);
    iterations = 0;

    if(showOutput)
    {
        cout << "\nInitial Route : ";
        printRoute(current);
        cout << "\nInitial Cost  : " << currentCost << "\n\n";

        cout << left << setw(12) << "Iteration"
             << setw(45) << "Route"
             << setw(12) << "Cost" << "\n";
        cout << string(69, '-') << "\n";

        cout << left << setw(12) << 0;
        printRoute(current);
        cout << "  " << currentCost << "\n";
    }

    while(true)
    {
        vector<int> best = current;
        double bestCost = currentCost;

        for(int i = 0; i < 7; i++)
        {
            for(int j = i + 1; j < 7; j++)
            {
                vector<int> neighbour = current;
                swap(neighbour[i], neighbour[j]);

                double neighbourCost = cost(neighbour);

                if(neighbourCost < bestCost)
                {
                    best = neighbour;
                    bestCost = neighbourCost;
                }
            }
        }

        if(bestCost >= currentCost)
            break;

        current = best;
        currentCost = bestCost;
        iterations++;

        if(showOutput)
        {
            cout << left << setw(12) << iterations;
            printRoute(current);
            cout << "  " << currentCost << "\n";
        }
    }

    finalRoute = current;
    return currentCost;
}

double simulatedAnnealing(vector<int> &bestRoute, int &iterations, bool showOutput = true)
{
    double T = 100;
    double alpha = 0.95;
    double Tmin = 0.1;

    vector<int> current = randomRoute();
    double currentCost = cost(current);

    bestRoute = current;
    double bestCost = currentCost;
    iterations = 0;

    if(showOutput)
    {
        cout << "\nInitial Route : ";
        printRoute(current);
        cout << "\nInitial Cost  : " << currentCost << "\n\n";

        cout << left
             << setw(10) << "Iteration"
             << setw(12) << "Temp."
             << setw(15) << "Current Cost"
             << setw(17) << "Neighbour Cost"
             << setw(12) << "Delta E"
             << setw(12) << "Decision" << "\n";

        cout << string(78, '-') << "\n";
    }

    uniform_real_distribution<double> random01(0.0, 1.0);

    while(T >= Tmin)
    {
        vector<int> neighbour = current;

        int i = gen() % 7;
        int j = gen() % 7;

        while(i == j)
            j = gen() % 7;

        swap(neighbour[i], neighbour[j]);

        double newCost = cost(neighbour);
        double deltaE = newCost - currentCost;
        bool accepted = false;

        if(deltaE < 0)
        {
            accepted = true;
        }
        else
        {
            double P = exp(-deltaE / T);
            double r = random01(gen);

            if(r < P)
                accepted = true;
        }

        if(accepted)
        {
            current = neighbour;
            currentCost = newCost;

            if(currentCost < bestCost)
            {
                bestRoute = current;
                bestCost = currentCost;
            }
        }

        iterations++;

        if(showOutput)
        {
            cout << left
                 << setw(10) << iterations
                 << setw(12) << T
                 << setw(15) << currentCost
                 << setw(17) << newCost
                 << setw(12) << deltaE
                 << setw(12)
                 << (accepted ? "Accepted" : "Rejected")
                 << "\n";
        }

        T = alpha * T;
    }

    if(showOutput)
    {
        cout << "\nBest Route : ";
        printRoute(bestRoute);
        cout << "\nBest Cost  : " << bestCost << "\n";
    }

    return bestCost;
}

int main()
{
    cout << fixed << setprecision(3);

    cout << "\nHILL CLIMBING\n";

    vector<int> hcRoute;
    int hcIterations;

    double hcCost = hillClimbing(hcRoute, hcIterations);

    cout << "\nFinal Route : ";
    printRoute(hcRoute);
    cout << "\nFinal Cost  : " << hcCost;
    cout << "\nIterations  : " << hcIterations << "\n";

    cout << "\nSIMULATED ANNEALING\n";

    vector<int> saRoute;
    int saIterations;

    double saCost = simulatedAnnealing(saRoute, saIterations);

    cout << "\nIterations : " << saIterations << "\n";

    vector<double> hcCosts, saCosts;
    vector<int> hcIter, saIter;

    cout << "\n5 RUN COMPARISON\n\n";

    cout << left
         << setw(8) << "Run"
         << setw(15) << "HC Cost"
         << setw(15) << "SA Cost"
         << setw(15) << "HC Iter."
         << setw(15) << "SA Iter."
         << "\n";

    cout << string(68, '-') << "\n";

    for(int run = 1; run <= 5; run++)
    {
        vector<int> route1, route2;
        int iterations1, iterations2;

        double resultHC = hillClimbing(route1, iterations1, false);
        double resultSA = simulatedAnnealing(route2, iterations2, false);

        hcCosts.push_back(resultHC);
        saCosts.push_back(resultSA);
        hcIter.push_back(iterations1);
        saIter.push_back(iterations2);

        cout << left
             << setw(8) << run
             << setw(15) << resultHC
             << setw(15) << resultSA
             << setw(15) << iterations1
             << setw(15) << iterations2
             << "\n";
    }

    double hcBest = *min_element(hcCosts.begin(), hcCosts.end());
    double hcWorst = *max_element(hcCosts.begin(), hcCosts.end());
    double saBest = *min_element(saCosts.begin(), saCosts.end());
    double saWorst = *max_element(saCosts.begin(), saCosts.end());

    double hcAverage = 0, saAverage = 0;
    double hcAverageIter = 0, saAverageIter = 0;

    for(int i = 0; i < 5; i++)
    {
        hcAverage += hcCosts[i];
        saAverage += saCosts[i];
        hcAverageIter += hcIter[i];
        saAverageIter += saIter[i];
    }

    hcAverage /= 5;
    saAverage /= 5;
    hcAverageIter /= 5;
    saAverageIter /= 5;

    cout << "\nFINAL STATISTICS\n\n";

    cout << left
         << setw(25) << "Statistic"
         << setw(20) << "Hill Climbing"
         << setw(20) << "Simulated Annealing"
         << "\n";

    cout << string(65, '-') << "\n";

    cout << setw(25) << "Best Cost"
         << setw(20) << hcBest
         << setw(20) << saBest << "\n";

    cout << setw(25) << "Worst Cost"
         << setw(20) << hcWorst
         << setw(20) << saWorst << "\n";

    cout << setw(25) << "Average Cost"
         << setw(20) << hcAverage
         << setw(20) << saAverage << "\n";

    cout << setw(25) << "Average Iterations"
         << setw(20) << hcAverageIter
         << setw(20) << saAverageIter << "\n";

    return 0;
}

/*
    Equation hill climbing
    #include <iostream>
#include <iomanip>
#include <random>

using namespace std;

#define fast_io ios_base::sync_with_stdio(false); cin.tie(NULL);

// The continuous function to maximize
double f(double x) {
    return -(x * x) + (4 * x) + 10;
}

void solveHillClimbing() {
    // Random number generator for initial state
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(-10.0, 10.0); // Start somewhere between -10 and 10

    double current_x = dis(gen);
    double current_val = f(current_x);
    double step_size = 0.01; // How far to look left and right
    
    cout << "--- Hill Climbing ---\n";
    cout << fixed << setprecision(4);
    cout << "Starting x: " << current_x << " | Initial f(x): " << current_val << "\n";

    while (true) {
        // Generate continuous neighbors
        double left_x = current_x - step_size;
        double right_x = current_x + step_size;

        double left_val = f(left_x);
        double right_val = f(right_x);

        // Find the steepest ascent
        if (left_val > current_val && left_val >= right_val) {
            current_x = left_x;
            current_val = left_val;
        } 
        else if (right_val > current_val && right_val > left_val) {
            current_x = right_x;
            current_val = right_val;
        } 
        else {
            // Neither left nor right improves the value; we are at the peak
            break;
        }
    }

    cout << "Maximum found at x: " << current_x << "\n";
    cout << "Maximum value f(x): " << current_val << "\n\n";
}
*/