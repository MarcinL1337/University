function createGenerator() {
    var _state = 0;
    return {
        next: function () {
            return {
                value: _state,
                done: _state++ >= 10
            }
        }
    }
}

var foo = {
    [Symbol.iterator]: createGenerator
};
for (var f of foo){
    console.log(f);
}


function createGeneratorParam(param) {
    return function createGenerator(){
        var _state = 0;
        return {
            next: function () {
                return {
                    value: _state,
                    done: _state++ >= param
                }
            }
        }
    }
}

var foo1 = {
    [Symbol.iterator]: createGeneratorParam(420)
}

var foo2 = {
    [Symbol.iterator]: createGeneratorParam(13)
}

for (var f of foo1){
    console.log(f);
}

for (var f of foo2){
    console.log(f);
}