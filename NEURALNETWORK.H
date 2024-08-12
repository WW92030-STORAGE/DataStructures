#ifndef NEURAL_H
#define NEURAL_H

#include <vector>
#include <iostream>
#include <cfloat>
#include <climits>
#include <algorithm>
#include <cmath>
#include <omp.h>
// Implementation of a small evolving neural network system

#define DEFAULT_INPUT 2
#define DEFAULT_LAYERS 1
#define DEFAULT_HIDDEN 2
#define INF (100000000)
#define REFRESH_RATE (1.0 / 60.0)

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
    
    // This implementation has 2 activation functions: one for the output layer and one for everything else.
    
    int INPUT_SIZE = DEFAULT_INPUT;
    int HIDDEN_LAYERS = DEFAULT_LAYERS;
    int NODES_PER_HIDDEN = DEFAULT_HIDDEN;
    
    double WEIGHTLIMIT = (1<<16);
    
    int edges = 0;
    std::vector<std::vector<std::vector<double>>> weights;
    std::vector<std::vector<double>> values;
    
    bool operator<(const NeuralNetwork& other) { return weights < other.weights; }
    
    void init() {
        edges = 0;
        for (auto i : weights) {
            for (auto j : i) {
                for (auto k : j) edges++;
            }
        }
        
        values = std::vector<std::vector<double>>();
        values.push_back(std::vector<double>(INPUT_SIZE + 1, 1));
        for (int i = 0; i < HIDDEN_LAYERS; i++) values.push_back(std::vector<double>(NODES_PER_HIDDEN + 1, 1));
        values.push_back(std::vector<double>(1, 0));
        
        for (int i = 0; i <= HIDDEN_LAYERS; i++) values[i][values[i].size() - 1] = 1;
    }
    
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
        
        init();
    }
    
    NeuralNetwork() {
        if (HIDDEN_LAYERS == 0) {
            weights.push_back(std::vector<std::vector<double>>(INPUT_SIZE + 1, std::vector<double>(1, 1)));
            return;
        }
        weights = std::vector<std::vector<std::vector<double>>>(1, std::vector<std::vector<double>>(INPUT_SIZE + 1, std::vector<double>(NODES_PER_HIDDEN, 1)));
        for (int i = 1; i < HIDDEN_LAYERS; i++) {
            weights.push_back(std::vector<std::vector<double>>(NODES_PER_HIDDEN + 1, std::vector<double>(NODES_PER_HIDDEN, 1)));
        }
        weights.push_back(std::vector<std::vector<double>>(NODES_PER_HIDDEN + 1, std::vector<double>(1, 1)));
        
        init();
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
        for (int i = 1; i < HIDDEN_LAYERS; i++) {
            weights.push_back(std::vector<std::vector<double>>(NODES_PER_HIDDEN + 1, std::vector<double>(NODES_PER_HIDDEN, 1)));
        }
        weights.push_back(std::vector<std::vector<double>>(NODES_PER_HIDDEN + 1, std::vector<double>(1, 1)));
        
        init();
    }
    
    double sigmoid(double x) {
        return std::tanh(x);
    }
    
    double sigd(double y) {
        return 1 - y * y;
    }
    
    double activation(double x) {
        // return x;
        return sigmoid(x);
    }
    
    
    double activd(double y) {
        // return 1;
        return sigd(y);
    }
    
    double finalactivation(double x) {
        return sigmoid(x);
        return x;
    }
    
    double finalad(double y) {
        return sigd(y);
        return 1;
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
        
        values[0][INPUT_SIZE] = 1;
        
        for (int i = 0; i < INPUT_SIZE; i++) values[0][i] = input[i];
        for (int i = 0; i < NODES_PER_HIDDEN; i++) {
            data[i] = weights[0][INPUT_SIZE][i];
            for (int j = 0; j < INPUT_SIZE; j++) data[i] += weights[0][j][i] * input[j];
            data[i] = activation(data[i]);
            values[1][i] = data[i];
        }
        
        // for (auto i : data) std::cout << i << " ";
        // std::cout << "\n";
        
        std::vector<double> newdata(NODES_PER_HIDDEN, 0);
        
        for (int layer = 1; layer < HIDDEN_LAYERS; layer++) {
            for (int i = 0; i < NODES_PER_HIDDEN; i++) { // next node
                newdata[i] = weights[layer][NODES_PER_HIDDEN][i];
                for (int j = 0; j < NODES_PER_HIDDEN; j++) newdata[i] += weights[layer][j][i] * data[j];
                newdata[i] = activation(newdata[i]);
                values[layer + 1][i] = newdata[i];
            }
            
            data = std::vector<double>(newdata);
            // for (auto i : data) std::cout << i << " ";
            // std::cout << "\n";
            newdata = std::vector<double>(NODES_PER_HIDDEN, 0);
        }
        
        double res = 0;
        for (int i = 0; i < NODES_PER_HIDDEN; i++) res += data[i] * weights[HIDDEN_LAYERS][i][0];
        res += weights[HIDDEN_LAYERS][NODES_PER_HIDDEN][0];
        // std::cout << res << std::endl;
        
        values[HIDDEN_LAYERS + 1][0] = finalactivation(res); // store the final value for consistency
        return finalactivation(res);
    }
    
    std::string toString() {
        std::string res = "[" + std::to_string(INPUT_SIZE) + " " + std::to_string(HIDDEN_LAYERS) + " " + std::to_string(NODES_PER_HIDDEN) + "]\n";
        
        for (int i = 0; i < weights.size(); i++) {
            res = res + "\nLAYER " + std::to_string(i) + ":\n";
            for (int j = 0; j < weights[i].size(); j++) {
                for (int k = 0; k < weights[i][j].size(); k++) res = res + "" + std::to_string(weights[i][j][k]) + " ";
                res = res + "\n";
            }
        }
        
        return res;
    }
    
    void backprop(double yhat, double y, double alpha, bool verbose = false) {
        if (verbose) {
            std::cout << "NN\n";
            std::cout << toString() << "\n";
            std::cout << "NN VALUES\n";
            for (auto i : values) {
                for (auto j : i) std::cout << j << " ";
                std::cout << "\n";
            }
        }
        
        
        double Eprime = yhat - y; // d(Squared error) / d(yhat) = dE / dY'
        
        // denote N as the input value to a node (weighted sum) and N' the corresponding output (activation(N))
        
        // If a node N outputs to some outputs [x1 ... xk] and inputs from some inputs [z1 ... zj]
        // then the gradient d(squared error) / dN' is simply the sum of the following:
        // d(squared error) / d(z') * dz' / dz * dz / dN' over all z in [z1 ... zj]
        // You can get the values of N' from the values vector which stores all input and intermediate values
        
        // The neural network has N + 2 layers. 1 input layer, N hidden layers, and 1 output layer.
        // There are N + 1 layers of weights. Layer i (weights[i]) forms a matrix of weights from Layer i to Layer i + 1
        // weights[i][j][k] is the weight connecting node j in layer i to node k in layer i + 1
        
        std::vector<std::vector<double>> nodegrads(1, std::vector<double>(INPUT_SIZE + 1, 0)); // Gradients d(squared error) / dN'
        for (int i = 0; i < HIDDEN_LAYERS; i++) nodegrads.push_back(std::vector<double>(NODES_PER_HIDDEN + 1, 0));
        nodegrads.push_back(std::vector<double>(1, 0));
        
        // nodegrads[i][j] is d(squared error) / d(value' of node j in layer i so after the sigmoid)
        
        NeuralNetwork gradients(*this); // weight gradients. 
        // gradients.weights[l][i][j] is the gradient of the edge starting on node i on layer l leading into node j on layer l + 1
        
        // For the output layer it simply has d(sqerror) / d(output')
        nodegrads[HIDDEN_LAYERS + 1][0] = Eprime;
        // Now for successive layers:
        for (int i = HIDDEN_LAYERS; i >= 0; i--) {
            for (int k = 0; k < values[i].size(); k++) { // node k on layer i
                double sum = 0;
                for (int j = 0; j < nodegrads[i + 1].size(); j++) {
                    if (i == HIDDEN_LAYERS) sum += nodegrads[i + 1][j] * finalad(values[i + 1][j]) * weights[i][k][j];
                    else sum += nodegrads[i + 1][j] * activd(values[i + 1][j]) * weights[i][k][j];
                }
                nodegrads[i][k] = sum;
            }
        }
        
        if (verbose) {
            std::cout << "NODE GRADIENTS\n";
            for (auto i : nodegrads) {
                for (auto j : i) std::cout << j << " ";
                std::cout << "\n";
            }
        }
        
        // Now to look at the weights
        
        for (int i = HIDDEN_LAYERS; i >= 0; i--) { // layer i --> layer (i + 1)
            for (int j = 0; j < weights[i].size(); j++) {
                for (int k = 0; k < weights[i][j].size(); k++) {
                    // d(sqerror) / d(weight[i][j][k]) = d(sqerror) / d(v'[i + 1][k]) * d(v'[i + 1][k]) / d(v[i + 1][k]) * d(v[i + 1][k]) / d(w)
                    if (i == HIDDEN_LAYERS) gradients.weights[i][j][k] = nodegrads[i + 1][k] * finalad(values[i + 1][k]) * values[i][j];
                    else gradients.weights[i][j][k] = nodegrads[i + 1][k] * activd(values[i + 1][k]) * values[i][j];
                }
            }
        }
        
        if (verbose) std::cout << "GRADIENTS " << "\n" << gradients.toString() << "\n";
        
        for (int i = 0; i < weights.size(); i++) {
            for (int j = 0; j < weights[i].size(); j++) {
                for (int k = 0; k < weights[i][j].size(); k++) {
                    weights[i][j][k] -= gradients.weights[i][j][k] * alpha;
                    if (weights[i][j][k] < -1 * WEIGHTLIMIT) weights[i][j][k] = -1 * WEIGHTLIMIT;
                    if (weights[i][j][k] > WEIGHTLIMIT) weights[i][j][k] = WEIGHTLIMIT;
                }
            }
        }
    }
    
    private:

    static int find(std::string value, char c, int start = 0) {
        for (int i = start; i < value.length(); i++) {
            if (value[i] == c) return i;
        }
        return value.length();
    }

    static std::string substring(std::string data, int a, int b) { // [a, b)
        return data.substr(a, b - a);
    }
    
    // Generates a neural network based on the toString readout of another.
    
    public:
    
    static NeuralNetwork readIn(std::string data) {
        int space = find(data, ' ');
        int input = std::stoi(substring(data, find(data, '[') + 1, space));
        int space2 = find(data, ' ', space + 1);
        int layers = std::stoi(substring(data, space + 1, space2));
        int hidden = std::stoi(substring(data, space2 + 1, find(data, ']')));
    
        NeuralNetwork nn(input, layers, hidden);
    
        int start = find(data, ':') + 1;
        int previouslayer = start;
        for (int in = 0; in <= input; in++) {
            for (int out = 0; out < hidden; out++) {
                int end = find(data, ' ', start);
                nn.weights[0][in][out] = std::stod(substring(data, start, end));
                start = end + 1;
            }
        }
    
        for (int layer = 1; layer < layers; layer++) {
            start = find(data, ':', previouslayer) + 1;
            previouslayer = start;
            for (int in = 0; in <= hidden; in++) {
                for (int out = 0; out < hidden; out++) {
                    int end = find(data, ' ', start);
                    nn.weights[layer][in][out] = std::stod(substring(data, start, end));
                    start = end + 1;
                }
            }
        }
    
    
        start = find(data, ':', previouslayer) + 1;
        for (int in = 0; in <= hidden; in++) {
            int end = find(data, ' ', start);
            nn.weights[layers][in][0] = std::stod(substring(data, start, end));
            start = end + 1;
        }
    
        return nn;
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

NeuralNetwork randomAI(double radius = 1, int protogen = DEFAULT_INPUT, int primagen = DEFAULT_LAYERS, int primogenitor = DEFAULT_HIDDEN) {
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

NeuralNetwork mutate(NeuralNetwork nn, double radius = 64) {
    int threshold = (int)(nn.edges);
    
    NeuralNetwork res(nn);
    int beep = rand() % threshold;
    int count = 0;
    for (int i = 0; i < nn.weights.size(); i++) {
        for (int j = 0; j < nn.weights[i].size(); j++) {
            for (int k = 0; k < nn.weights[i][j].size(); k++) {
                if (rand() % threshold == 0) res.weights[i][j][k] = radius * randrad();
				if (count == beep) res.weights[i][j][k] = radius * randrad();
				count++;
			}
        }
    }
    return res;
}

}

#endif
