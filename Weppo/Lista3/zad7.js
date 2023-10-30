function fibRecMemo(n) {
    let memo = new Array(n).fill(-1);
    memo[0] = 0;
    memo[1] = 1;
    function aux(n) {
        if (memo[n] > -1) return memo[n];
        return memo[n] = aux(n - 1) + aux(n - 2);
    }
    return aux(n);
}

function* fib_yield() {
    var i = 0;
    while(true){
        yield fibRecMemo(i);
        i++;
    }
}

function* take(it, top) {
    for(var i = 0; i < top; i++) yield it.next().value;
}

// zwróć dokładnie 10 wartości z potencjalnie
// "nieskończonego" iteratora/generatora

for (let num of take( fib_yield(), 10 ) ) {
    console.log(num);
}