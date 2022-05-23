# RunAt

RunAt is a small library for the Arduino Platform to run a function at a given
timestamp. But it does'nt uses threads, instead it uses the main thread of the
microcontroller to run. Therefore you need to call `run_at_loop()` in the loop
of you're code. This function iterates over all scheduled functions and runs
them if the timestamp is reached.

### Example

```ino
#include <RunAt.h>

void print_1() {
  Serial.println("print 1");
}

void print_2() {
  Serial.println("print 2");
}

void print_3() {
  Serial.println("print 3");
}

void setup() {
  Serial.begin(115200);

  run_at_function_in_millisec(print_1, 1000);
  run_at_function_in_millisec(print_3, 3000);
  run_at_function_in_millisec(print_2, 2000);
}

void loop() {
  run_at_loop();
}
```
