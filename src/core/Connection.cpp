//
// Created by Tommy Ludwig on 23.09.23.
//

#include "Connection.h"
#include <cstdlib>
#include <cmath>

Connection::Connection()  {
    weight = randomWeight();
    deltaWeight = 0; // deltaWeight is the change in weight of the connection
}

double Connection::randomWeight() {
    // Small centered weights to avoid tanh saturation (was [0,1] -> all outputs went to 1)
    return (rand() / double(RAND_MAX) - 0.5) * 2.0 / sqrt(2.0);  // ~[-0.7, 0.7]
}
