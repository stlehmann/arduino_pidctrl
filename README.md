# pidctlr Library

* **version**: 1.0.1
* **license**: MIT License
* **author**: Stefan Lehmann, Stefan.St.Lehmann@gmail.com

The purpose of this library is to provide a basic PID controller algorithm.

## Features
* pid controller class where each instance is fully configurable via
  properties and functions
* antiwindup
* overflow protection at runtime
* use of 32 bit integers
* cycletime measurement, so no need to provide constant cycletimes

## Examples

### controllertest

This example illustrates the functionality of the algorithm by providing a
binding to the Serial Port. You can configure the control parameters and input
variable and watch the effects on the output variable.