export function map<T, G>(a : Array<T>, f : (arg : T) => G) : Array<G>{
    let res : Array<G> = [];
    res.length = a.length;
    for(let i = 0; i < a.length; i++) {
        res[i] = f(a[i]);
    }
    return res;
}

function filter<T>(a : Array<T>, f : (arg : T) => boolean) : Array<T>{
    let res : Array<T> = [];
    for(let i = 0; i < a.length; i++) {
        const x = a[i]
        if (f(x)) res.push(x);
    }
    return res;
}

function forEach<T>(a : Array<T>, f : (arg : T) => void) : void{
    for(let i = 0; i < a.length; i++) {
        f(a[i]);
    }
    return;
}

let x = [1,2,3,4];
forEach( x, _ => { console.log( _ ); } );
console.log(filter( x, _ => _ < 3 ));
console.log(map( x, _ => _ * 2 ));
