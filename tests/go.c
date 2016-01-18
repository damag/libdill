/*

  Copyright (c) 2015 Martin Sustrik

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"),
  to deal in the Software without restriction, including without limitation
  the rights to use, copy, modify, merge, publish, distribute, sublicense,
  and/or sell copies of the Software, and to permit persons to whom
  the Software is furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included
  in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
  IN THE SOFTWARE.

*/

#include <assert.h>
#include <errno.h>
#include <stdio.h>

#include "../libdill.h"

int sum = 0;

coroutine void worker(int count, int n) {
    int i;
    for(i = 0; i != count; ++i) {
        sum += n;
        int rc = yield();
        assert(rc == 0);
    }
}

coroutine void dummy(void) {
    int rc = msleep(now() + 50);
    assert(rc == 0);
}

int main() {
    assert(errno == 0);
    go(worker(3, 7));
    go(worker(1, 11));
    go(worker(2, 5));
    int rc = msleep(100);
    assert(rc == 0);
    assert(sum == 42);

    /* Test whether stack deallocation works. */
    int i;
    for(i = 0; i != 20; ++i)
        go(dummy());
    rc = msleep(now() + 100);
    assert(rc == 0);

    return 0;
}

