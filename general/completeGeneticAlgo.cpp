#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

struct Individual {
    std::vector<int> chromosome;
    double fitness;
};

double evaluateFitness(const std::vector<int>& chromosome) {
    double score = 0;
    for (int gene : chromosome) score += gene;
    return score; // Max possible score is the size of the chromosome
}

Individual GeneticAlgorithmComplete(std::vector<Individual> population, int generations, double mutation_rate) {
    std::random_device rd;
    std::mt19937 gen(rd());
    
    for (int g = 0; g < generations; ++g) {
        for (auto& ind : population) {
            ind.fitness = evaluateFitness(ind.chromosome);
        }
        
        std::sort(population.begin(), population.end(), 
                 [](const Individual& a, const Individual& b) { return a.fitness > b.fitness; });
                 
        std::vector<Individual> new_population;
        
        int elitism_count = std::max(1, (int)(population.size() * 0.1));
        for (int i = 0; i < elitism_count; ++i) {
            new_population.push_back(population[i]);
        }
        
        std::uniform_int_distribution<> pop_dist(0, population.size() / 2); 
        std::uniform_real_distribution<> prob_dist(0.0, 1.0);
        
        while (new_population.size() < population.size()) {
            Individual parent1 = population[pop_dist(gen)];
            Individual parent2 = population[pop_dist(gen)];
            
            std::uniform_int_distribution<> cross_dist(1, parent1.chromosome.size() - 1);
            int crossover_point = cross_dist(gen);
            
            Individual child = {std::vector<int>(parent1.chromosome.size()), 0.0};
            for (size_t i = 0; i < child.chromosome.size(); ++i) {
                child.chromosome[i] = (i < crossover_point) ? parent1.chromosome[i] : parent2.chromosome[i];
                if (prob_dist(gen) < mutation_rate) {
                    child.chromosome[i] = 1 - child.chromosome[i]; 
                }
            }
            new_population.push_back(child);
        }
        population = new_population;
    }
    
    // Final evaluation
    for (auto& ind : population) ind.fitness = evaluateFitness(ind.chromosome);
    std::sort(population.begin(), population.end(), 
             [](const Individual& a, const Individual& b) { return a.fitness > b.fitness; });
             
    return population.front();
}

int main() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> bit_dist(0, 1);
    
    int pop_size = 20;
    int chromosome_length = 10;
    std::vector<Individual> initial_population;
    
    // Generate random initial population
    for (int i = 0; i < pop_size; ++i) {
        std::vector<int> chromo(chromosome_length);
        for (int j = 0; j < chromosome_length; ++j) chromo[j] = bit_dist(gen);
        initial_population.push_back({chromo, 0.0});
    }
    
    Individual best = GeneticAlgorithmComplete(initial_population, 50, 0.05);
    
    std::cout << "Best GA Fitness: " << best.fitness << "\nChromosome: ";
    for (int gene : best.chromosome) std::cout << gene << " ";
    std::cout << std::endl;
    
    return 0;
}