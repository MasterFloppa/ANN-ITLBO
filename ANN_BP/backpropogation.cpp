#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

// Define the sigmoid activation function
double sigmoid(double x) {
    return 1.0 / (1.0 + exp(-x));
}

// Define the derivative of the sigmoid activation function
double sigmoid_derivative(double x) {
    return x * (1.0 - x);
}

class NeuralNetwork {
private:
    vector<vector<double>> weights_input_hidden;
    vector<double> weights_hidden_output;
    vector<double> hidden_layer;
    double output;

    double learning_rate;

public:
    NeuralNetwork(double learning_rate) : learning_rate(learning_rate) {
        // Initialize weights randomly
        weights_input_hidden = {
            {0.2, 0.3, 0.5},  // weights for input 1 to hidden layer
            {0.4, -0.1, -0.2} // weights for input 2 to hidden layer
        };
        weights_hidden_output = {0.7, -0.6, 0.3}; // weights for hidden layer to output
    }

    // Feedforward pass
    double feedforward(vector<double> inputs) {
        double sum_hidden = 0.0;

        // Calculate output for hidden layer
        for (int i = 0; i < weights_input_hidden.size(); ++i) {
            double hidden_neuron = 0.0;
            for (int j = 0; j < inputs.size(); ++j) {
                hidden_neuron += inputs[j] * weights_input_hidden[j][i];
            }
            hidden_neuron = sigmoid(hidden_neuron);
            hidden_layer.push_back(hidden_neuron);
        }

        // Calculate output for output layer
        for (int i = 0; i < hidden_layer.size(); ++i) {
            sum_hidden += hidden_layer[i] * weights_hidden_output[i];
        }
        output = sigmoid(sum_hidden);

        return output;
    }

    // Backpropagation
    void backpropagate(double expected_output, vector<double> inputs) {
        // Calculate output layer errors
        double output_error = expected_output - output;
        double output_delta = output_error * sigmoid_derivative(output);

        // Update weights between hidden and output layers
        for (int i = 0; i < hidden_layer.size(); ++i) {
            weights_hidden_output[i] += learning_rate * output_delta * hidden_layer[i];
        }

        // Calculate hidden layer errors
        vector<double> hidden_errors;
        for (int i = 0; i < hidden_layer.size(); ++i) {
            double error = output_delta * weights_hidden_output[i] * sigmoid_derivative(hidden_layer[i]);
            hidden_errors.push_back(error);
        }

        // Update weights between input and hidden layers
        for (int i = 0; i < inputs.size(); ++i) {
            for (int j = 0; j < hidden_layer.size(); ++j) {
                weights_input_hidden[i][j] += learning_rate * hidden_errors[j] * inputs[i];
            }
        }

        // Clear hidden layer for next iteration
        hidden_layer.clear();
    }
};

int main() {
    NeuralNetwork nn(0.5);

    // Training data
    vector<vector<double>> training_inputs = {{0.5, -0.3},  // 1
            {-0.1, 0.8},  // 1
            {0.9, 0.1},   // 1
            {-0.5, -0.7}, // 1
            {0.2, 0.4},   // 1
            {-0.6, 0.9},  // 1
            {0.3, -0.8},  // 1
            {-0.9, 0.6}};
    vector<double> training_outputs = {0.999, 0.999, 0.999, 0.999, 0.999, 0.999, 0.999, 0.999};

    // Training the neural network
    for (int epoch = 0; epoch < 25; ++epoch) {
        for (int i = 0; i < training_inputs.size(); ++i) {
            double output = nn.feedforward(training_inputs[i]);
            nn.backpropagate(training_outputs[i], training_inputs[i]);
        }
    }

    // Testing the trained neural network
    cout << "Testing the trained neural network:" << endl;
    double mse=0;
    for (int i = 0; i < training_inputs.size(); ++i) {
        double output = nn.feedforward(training_inputs[i]);
        cout << "Input: " << training_inputs[i][0] << ", " << training_inputs[i][1] << " Output: " << output << endl;
        mse=mse+(output-training_outputs[i])*(output-training_outputs[i]);
    }
    
    cout<<"mse: "<<mse/training_inputs.size()<<endl;

    return 0;
}
