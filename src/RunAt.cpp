/* RunAt.cpp
 *
 * Copyright 2022 Leonard Schwennesen <ungedummt@gmx.de>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * SPDX-License-Identifier: MIT
 */


#include <stdlib.h>
#include <Arduino.h>

typedef struct _run_at_func {
  void (*function) ();
  ulong timestamp;
  struct _run_at_func *next;
} run_at_function;

static run_at_function *run_list = NULL;


static run_at_function* run_at_init_struct(void (*func) (), ulong timestamp) {
  run_at_function *run = (run_at_function*) malloc(sizeof(run_at_function));
  run->function = func;
  run->timestamp = timestamp;
  run->next = NULL;
  return run;
}

static run_at_function* run_at_get_last() {
  run_at_function *last = run_list;
  while (last->next != NULL)
    last = last->next;

  return last;
}

void run_at_add_function(void (*func) (), ulong timestamp) {
  if (run_list == NULL)
    run_list = run_at_init_struct(func, timestamp);
  else
    run_at_get_last()->next = run_at_init_struct(func, timestamp);
}

static bool run_at_if_time(run_at_function *func_to_run) {
  if (millis() >= func_to_run->timestamp) {
    func_to_run->function();
    return true;
  }
  return false;
}

void run_at_loop() {
  run_at_function *current_run = run_list;
  run_at_function *last_run    = NULL;

  while (current_run != NULL) {
    if (run_at_if_time(current_run)) {
      run_at_function *temp_run = current_run->next;
      free(current_run);

      current_run = temp_run;

      if (last_run != NULL)
        last_run->next = current_run;
      else
        run_list = current_run;

    } else {
      last_run    = current_run;
      current_run = current_run->next;
    }
  }
}
