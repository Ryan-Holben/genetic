# genetic//core

This package contains basic/core definitions and objects for the genetic project.

**Types**
* Number - The underlying numerical type used throughout the project, defined at a single point.  E.g. we can swap from `double` to `float` to see about an 8% decrease in neural network compute time.
* Tuple - A vector of Numbers.
* Datum - An <input, output> pair of Tuples, representing a single dataset entry.
* Dataset - A set of Datums.

**Objects**
* Neuron - A simple node representing a neuron as well all of its outgoing connections
* NeuralNetwork - Duh