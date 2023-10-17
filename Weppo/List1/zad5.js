function fibRec(n){
    if(n == 0) return 0;
    if(n == 1) return 1;
    return fibRec(n-1) + fibRec(n-2);
}

function fibIter(n){
    if(n == 0) return 0;
    if(n == 1) return 1;
    var fib1 = 1, fib2 = 0, res = 0;
    for(var i = 2; i <= n; i+=1){
        res = fib1 + fib2;
        fib2 = fib1;
        fib1 = res;
    }

    return res;
}

for(var i = 1; i <= 35; i+=1){
    console.log("Iteration number: " + i);
    console.time("rec");
    fibRec(i);
    console.timeEnd("rec");

    console.time("iter");
    fibIter(i);
    console.timeEnd("iter");
    console.log("--------------------");
}