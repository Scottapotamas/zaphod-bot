/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 bcard
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Modified to be lazily instantiated, rewritten in Typescript
 */

import { Movement } from './movements'

interface StackFrame {
  n: number
  index: number
}

function factorial(num: number) {
  var result = num
  while (num > 1) {
    num--
    result = result * num
  }
  return result
}

export class Permutor {
  private maxIterations: number
  private iterations: number = 0
  private x: Movement[]
  private n: number
  private stack: StackFrame[] = []
  private initialised = false

  constructor(arr: Movement[]) {
    this.x = arr
    this.n = arr.length
    this.maxIterations = factorial(arr.length)

    for (let i = this.n; i > 0; i--) {
      this.stack.push({ n: i, index: 0 })
    }

    this.next = this.next.bind(this)
    this.swap = this.swap.bind(this)
    this.hasNext = this.hasNext.bind(this)
    this.getIterations = this.getIterations.bind(this)
    this.getTotal = this.getTotal.bind(this)
  }

  next() {
    if (!this.hasNext())
      throw new Error(
        `Stack ran out at ${this.iterations}/ ${this.maxIterations}`,
      )

    this.iterations++

    let s = this.stack.pop()!
    let skipSwap = false

    while (s.n !== 1) {
      if (!skipSwap) {
        if (s.n % 2 === 0) {
          this.swap(s.index, s.n - 1)
        } else {
          this.swap(0, s.n - 1)
        }
        s.index++
      }

      if (s.index < s.n) {
        this.stack.push(s)
        this.stack.push({ n: s.n - 1, index: 0 })
        skipSwap = true
      }

      s = this.stack.pop()!
    }

    return this.x.slice(0)
  }

  swap(i: number, j: number) {
    const tmp = this.x[i]
    this.x[i] = this.x[j]
    this.x[j] = tmp
  }

  hasNext() {
    return this.iterations < this.maxIterations
  }

  getIterations() {
    return this.iterations
  }

  getTotal() {
    return this.maxIterations
  }
}
