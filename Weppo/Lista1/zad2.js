function check_if_perfect(num){
    var num_copy = num;
    var digit;
    var digit_sum = 0;
    while(num_copy != 0){
        digit = num_copy % 10;
        // console.log(digit);
        num_copy = Math.floor(num_copy / 10);
        // console.log(num_copy);
        if(num % digit != 0) return false;
        digit_sum += digit;
    }
    // console.log(digit_sum);
    if(num % digit_sum != 0) return false;
    return true;
}

for(var i = 0; i <= 100000; i+=1){
    if(check_if_perfect(i)) console.log(i);
}

// check_if_perfect(12345);