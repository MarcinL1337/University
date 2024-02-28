function getRandomInt(max) {
    return Math.floor(Math.random() * max);
}
const readline = require('readline');
let cl = readline.createInterface(process.stdin, process.stdout);
let question = function (q) {
    return new Promise((res, rej) => {
        cl.question(q, answer => {
            res(answer);
        })
    });
};

(async function main() {
    let my_num = getRandomInt(100);
    console.log("Try to guess a value between 0 and 100");
    let answer = '-1';
    let guess_num = parseInt(answer);
    while(guess_num != my_num){
        answer = await question('Whats my number?\n');
        guess_num = parseInt(answer);
        if (guess_num > my_num){
            console.log("My number is smaller");
        }else{
            console.log("My number is bigger");
        }
    }
    console.log('You guessed correctly! The number was ' + my_num);
    cl.close();
})();