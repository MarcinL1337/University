const readline = require('readline');
var cl = readline.createInterface( process.stdin, process.stdout );
var question = function(q) {
    return new Promise( (res, rej) => {
        cl.question( q, answer => {
            res(answer);
        })
    });
};

(async function main() {
    var answer = '';
    answer = await question('Whats your name?\n');
    console.log( 'Hello ' + answer);
    cl.close();
})();