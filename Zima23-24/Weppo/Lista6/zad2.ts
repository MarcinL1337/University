const fibRec = (x: number) : number => {
    if (x == 1 || x == 2) return 1;
    else return fibRec(x-2) + fibRec(x-1);
}

export function fib_memo() : (x : number) => number {
  let memo : { [idx: number] : number; }= {
    0: 1,
    1: 1,
  };
  return function fib(n: number) : number {
    if (n in memo) {
      return memo[n];
    } else {
      const result = fib(n - 2) + fib(n - 1);
      memo[n] = result;
      return result;
    }
  };
}

const fib = fib_memo();
// console.log(fib(15));