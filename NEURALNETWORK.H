#ifndef NEURAL_H
#define NEURAL_H

#include <vector>
#include <iostream>
#include <cfloat>
#include <climits>
#include <algorithm>
#include <cmath>

// Implementation of a small evolving neural network system

#define DEFAULT_INPUT 4
#define DEFAULT_LAYERS 16
#define DEFAULT_HIDDEN 16
#define INF 100000000;

class NeuralNetwork {
    public:
    
    // The mechanism of a neural network is actually fairly simple. 
    // There are input nodes, hidden nodes, and output nodes. 
    // Hidden and output nodes simply receive data in the form of a linear combination of the data from their parents.
    // The node then puts this linear combination (input data) into an activation function to "accentuate" the value.
    // Nodes then send the activated data to the child nodes as part of the linear combinations of those child nodes.
    // Each hidden layer and output layer node also takes in a bias coefficient. The bias coefficient is represented as a node in each layer that always outputs one.
    
    // The output layer has only one node.
    // The activation function for each layer is a sigmoid.
    
    // How coefficients are encoded? weights[L][a][b] is the scale the data from node a in layer L 
    // is multiplied when inserted into node b in layer L + 1.
    
    int INPUT_SIZE = DEFAULT_INPUT;
    int HIDDEN_LAYERS = DEFAULT_LAYERS;
    int NODES_PER_HIDDEN = DEFAULT_HIDDEN;
    std::vector<std::vector<std::vector<double>>> weights;
    
    bool operator<(const NeuralNetwork& other) { return weights < other.weights; }
    
    NeuralNetwork(const NeuralNetwork& other) {
        INPUT_SIZE = other.INPUT_SIZE;
        HIDDEN_LAYERS = other.HIDDEN_LAYERS;
        NODES_PER_HIDDEN = other.NODES_PER_HIDDEN;
        weights = std::vector<std::vector<std::vector<double>>>();
        for (int i = 0; i < other.weights.size(); i++) {
            weights.push_back(std::vector<std::vector<double>>());
            for (int j = 0; j < other.weights[i].size(); j++) {
                weights[i].push_back(std::vector<double>());
                for (int k = 0; k < other.weights[i][j].size(); k++) weights[i][j].push_back(other.weights[i][j][k]);
            }
        }
    }
    
    NeuralNetwork() {
        if (HIDDEN_LAYERS == 0) {
            weights.push_back(std::vector<std::vector<double>>(INPUT_SIZE + 1, std::vector<double>(1, 1)));
            return;
        }
        weights = std::vector<std::vector<std::vector<double>>>(1, std::vector<std::vector<double>>(INPUT_SIZE + 1, std::vector<double>(NODES_PER_HIDDEN, 1)));
        for (int i = 0; i < HIDDEN_LAYERS; i++) {
            weights.push_back(std::vector<std::vector<double>>(NODES_PER_HIDDEN + 1, std::vector<double>(NODES_PER_HIDDEN, 1)));
        }
        weights.push_back(std::vector<std::vector<double>>(NODES_PER_HIDDEN + 1, std::vector<double>(1, 1)));
    }
    
    NeuralNetwork(int protogen, int primagen, int primogenitor) {
        INPUT_SIZE = protogen;
        HIDDEN_LAYERS = primagen;
        NODES_PER_HIDDEN = primogenitor;
        
        if (HIDDEN_LAYERS == 0) {
            weights.push_back(std::vector<std::vector<double>>(INPUT_SIZE + 1, std::vector<double>(1, 1)));
            return;
        }
        weights = std::vector<std::vector<std::vector<double>>>(1, std::vector<std::vector<double>>(INPUT_SIZE + 1, std::vector<double>(NODES_PER_HIDDEN, 1)));
        for (int i = 0; i < HIDDEN_LAYERS; i++) {
            weights.push_back(std::vector<std::vector<double>>(NODES_PER_HIDDEN + 1, std::vector<double>(NODES_PER_HIDDEN, 1)));
        }
        weights.push_back(std::vector<std::vector<double>>(NODES_PER_HIDDEN + 1, std::vector<double>(1, 1)));
    }
    
    double activation(double x) {
        double res = 1 + exp(-1 * x);
        return (1.0 / res) - 0.5;
    }
    
    double eval(std::vector<double> input) {
        if (input.size() < INPUT_SIZE) return DBL_MIN;
        if (HIDDEN_LAYERS == 0) {
            double res = 0;
            for (int i = 0; i < INPUT_SIZE; i++) res += input[i] * weights[0][i][0];
            res += weights[0][INPUT_SIZE][0];
            return activation(res);
        }
        std::vector<double> data(NODES_PER_HIDDEN, 0);
        for (int i = 0; i < NODES_PER_HIDDEN; i++) {
            data[i] = weights[0][INPUT_SIZE][i];
            for (int j = 0; j < INPUT_SIZE; j++) data[i] += weights[0][j][i] * input[j];
            data[i] = activation(data[i]);
        }
        
        // for (auto i : data) std::cout << i << " ";
        // std::cout << "\n";
        
        std::vector<double> newdata(NODES_PER_HIDDEN, 0);
        
        for (int layer = 1; layer <= HIDDEN_LAYERS; layer++) {
            for (int i = 0; i < NODES_PER_HIDDEN; i++) {
                newdata[i] = weights[layer][NODES_PER_HIDDEN][i];
                for (int j = 0; j < NODES_PER_HIDDEN; j++) newdata[i] += weights[layer][j][i] * data[j];
                newdata[i] = activation(newdata[i]);
            }
            
            data = newdata;
            // for (auto i : data) std::cout << i << " ";
            // std::cout << "\n";
            newdata = std::vector<double>(NODES_PER_HIDDEN, 0);
        }
        
        double res = 0;
        for (int i = 0; i < NODES_PER_HIDDEN; i++) res += data[i] * weights[HIDDEN_LAYERS + 1][i][0];
        res += weights[HIDDEN_LAYERS + 1][NODES_PER_HIDDEN][0];
        // std::cout << res << std::endl;
        return activation(res);
    }
};


bool operator<(const NeuralNetwork& nn, const NeuralNetwork& other) { return nn.weights < other.weights; }

namespace Genetic {

double randf() {
    return (double)(rand()) / (double)(RAND_MAX);
}

double randrad() {
    return 2 * (randf() - 0.5);
}

NeuralNetwork randomAI(double radius = 16, int protogen = DEFAULT_INPUT, int primagen = DEFAULT_LAYERS, int primogenitor = DEFAULT_HIDDEN) {
    NeuralNetwork nn(protogen, primagen, primogenitor);
    for (int i = 0; i < nn.weights.size(); i++) {
        for (int j = 0; j < nn.weights[i].size(); j++) {
            for (int k = 0; k < nn.weights[i][j].size(); k++) nn.weights[i][j][k] = radius * randrad();
        }
    }
    return nn;
}

NeuralNetwork cross(NeuralNetwork n1, NeuralNetwork n2) {
    NeuralNetwork res(n1);
    for (int i = 0; i < n1.weights.size(); i++) {
        for (int j = 0; j < n1.weights[i].size(); j++) {
            for (int k = 0; k < n1.weights[i][j].size(); k++) if (rand() % 2 == 0) res.weights[i][j][k] = n2.weights[i][j][k];
        }
    }
    return res;
}

NeuralNetwork mutate(NeuralNetwork nn, double radius = 16) {
    NeuralNetwork res(nn);
    for (int i = 0; i < nn.weights.size(); i++) {
        for (int j = 0; j < nn.weights[i].size(); j++) {
            for (int k = 0; k < nn.weights[i][j].size(); k++) {
				if (rand() % (nn.HIDDEN_LAYERS * nn.NODES_PER_HIDDEN * 4) == 0) res.weights[i][j][k] = radius * randrad();
			}
        }
    }
    return res;
}

}

#endif
