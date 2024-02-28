const http = require('http');

function getContent(url) {
    return new Promise((resolve, reject) => {
        http.get(url, (response) => {
            let data = '';

            response.on('data', (chunk) => {
                data += chunk;
            });

            response.on('end', () => {
                resolve(data);
            });

            response.on('error', (blad) => {
                reject(blad);
            });
        });
    });
}

const url = 'http://users.pja.edu.pl/~s3452/prezentacja/html/http.html';

getContent(url)
    .then(console.log)
    .catch(console.error);