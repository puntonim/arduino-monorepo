/**
 * NAMESPACE
 * =========
 * A namespace defines a scope: a way to group and isolate related identifiers
 *  such as variables, functions, and classes under a single name.
 * Vars in a namespace are always reachable by their fully qualified name.
 *
 * Namespaces are used to organize code into logical groups and to prevent
 *  name collisions that can occur especially when your code base includes
 *  multiple libraries.
 *
 * Docs:
 *  - https://learn.microsoft.com/en-us/cpp/cpp/namespaces-cpp
 *  - https://www.geeksforgeeks.org/namespace-in-c/
 *
 * Libs:
 *  - TaskManagerIO by davetcc
 *    https://github.com/TcMenu/TaskManagerIO
 */

#include <Arduino.h>

// Namespace definition.
namespace cherry {
void greet(char* name) { Serial.println((String) "Hello " + name); }
void greetAgain(char* name) { Serial.println((String) "Hello " + name); }
}  // namespace cherry

void setup() {
  Serial.begin(57600);
  unsigned short i = 30;
  while (!Serial && i--) delay(1);
  Serial.println("START " __FILE__ " compiled on " __DATE__);
}  // namespace void setup()

void loop() {
  // greet(); // Compile error.

  // A. explicit full namespace.
  cherry::greet("explicit");

  // B. `using` directive per var.
  using cherry::greet;
  greet("`using` directive");
  // greetAgain("using directive"); // Compile error.

  // C. `using namespace` directive and default namespace.
  using namespace cherry;
  greetAgain("`using namespace` directive");

  delay(1 * 1000);
}
