module.exports = { fun_b };
let a = require('./zad1a');
function fun_b(n) {
    if (n > 0) {
        console.log(`b: ${n}`);
        a.fun_a(n - 1);
    }
}