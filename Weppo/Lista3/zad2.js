function forEach(a, f){
    for(var i = 0; i < a.length; i++){
        f(a[i]);
    }
}

function map(a, f){
    for(var i = 0; i < a.length; i++){
        a[i] = f(a[i]);
    }
}

function filter(a, f){
    var b = new Array();
    for(var i = 0; i < a.length; i++){
        if(f(a[i])) b.push(a[i]);
    }
    return b;
}

var a = [1,2,3,4];

forEach( a, _ => { console.log( _ ); } );
// [1,2,3,4]
console.log(filter( a, _ => _ < 3 ));
// [1,2]
map( a, _ => _ * 2 );
console.log(a);
// [2,4,6,8]