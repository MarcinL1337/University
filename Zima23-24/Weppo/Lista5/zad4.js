const fs = require('fs');

function readFile(path, f) {
    fs.readFile(path, 'utf8', (error, data) => {
        if (error) {
            console.error('Error while reading file: ', error);
            return;
        }
        f(data);
    });
}

const filePath = 'Weppo/Lista3/zad6.js';

readFile(filePath, console.log);