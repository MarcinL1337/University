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

function fib_gen() {
    var _state = 0;
    return {
        next: function () {
            return {
                value: fibRecMemo(_state++),
                done: false
            }
        }
    }
}


function* fib_yield() {
    var i = 0;
    while(true){
        yield fibRecMemo(i);
        i++;
    }
}


function foo (){
    var _it = fib_gen();
    var aux = 1, max_aux = 15;
    for ( var _result; _result = _it.next(), !_result.done; ) {
        console.log( _result.value );
        if(aux++ > max_aux) break;
    }

    aux = 1;
    _it = fib_yield();
    for ( var _result; _result = _it.next(), !_result.done; ) {
        console.log( _result.value );
        if(aux++ > max_aux) break;
    }

    // TypeError: fib_gen is not a function or its return value is not iterable
    // aux = 1;
    // for(let i of fib_gen()){
    //     console.log(i);
    //     if(aux++ > max_aux) break;
    // }

    aux = 1;
    for(let i of fib_yield()){
        console.log(i);
        if(aux++ > max_aux) break;
    }
}

foo()