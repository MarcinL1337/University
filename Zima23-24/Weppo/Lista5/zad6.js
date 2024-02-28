const fs = require('fs');
const readline = require('readline');

const filePath = 'Weppo/Lista5/logi_https.txt'; 
const fileStream = fs.createReadStream(filePath, 'utf8');
const readInterface = readline.createInterface({
    input: fileStream,
    crlfDelay: Infinity,
});

const clientStats = new Map();

readInterface.on('line', (line) => {
    const columns = line.split(' ');
    const IPAddress = columns[1];
    
    if (clientStats.has(IPAddress)) {
        clientStats.set(IPAddress, clientStats.get(IPAddress) + 1);
    } else {
        clientStats.set(IPAddress, 1);
    }
});
readInterface.on('close', () => {
    console.log(clientStats.entries())
    const sortedStats = [...clientStats.entries()].sort((a, b) => b[1] - a[1]);

    for (let i = 0; i < 3 && i < sortedStats.length; i++) {
        let [IPAddress, taskCount] = sortedStats[i];
        console.log(`${IPAddress} ${taskCount}`);
    }
});

readInterface.on('error', (blad) => {
    console.error('Wystąpił błąd podczas odczytu pliku:', blad);
});