module.exports = { work_a };
let b = require('./zad1b');
function work_a(n) {
    if (n > 0) {
        console.log(`a: ${n}`);
        b.work_b(n - 1);
    }
}