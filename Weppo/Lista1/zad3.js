function isPrime(num){
    for(var i = 2; i <= Math.sqrt(num); i+=1){
        if(num % i == 0) return false;
    }
    return true;
}

for(var i = 2; i <= 100000; i+=1){
    if(isPrime(i)) console.log(i);
}