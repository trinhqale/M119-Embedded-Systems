// based on the example on https://www.npmjs.com/package/@abandonware/noble

const noble = require('@abandonware/noble');

const uuid_service = "1101"
const ax_uuid_value = "2101"
const ay_uuid_value = "2102"
const az_uuid_value = "2103"

let sensorValue = NaN
let x, y, z = NaN

noble.on('stateChange', async (state) => {
    if (state === 'poweredOn') {
        console.log("start scanning")
        await noble.startScanningAsync([uuid_service], false);
    }
});

noble.on('discover', async (peripheral) => {
    await noble.stopScanningAsync();
    await peripheral.connectAsync();
    const {
        characteristics
    } = await peripheral.discoverSomeServicesAndCharacteristicsAsync([uuid_service], [ax_uuid_value, ay_uuid_value, az_uuid_value]);
    readDataX(characteristics[0]);
    readDataY(characteristics[1]);
    readDataZ(characteristics[2]);
});

//
// read data periodically
//
let readDataX = async (characteristic) => {
    const value = (await characteristic.readAsync());
    x = value.readFloatLE(0);
    console.log("x: " + x);

    // read data again in t milliseconds
    setTimeout(() => {
        readDataX(characteristic)
    }, 10);
}

let readDataY = async (characteristic) => {
    const value = (await characteristic.readAsync());
    y = value.readFloatLE(0);
    console.log("y: " + y);

    // read data again in t milliseconds
    setTimeout(() => {
        readDataY(characteristic)
    }, 10);
}

let readDataZ = async (characteristic) => {
    const value = (await characteristic.readAsync());
    z = value.readFloatLE(0);
    console.log("z: " + z);

    // read data again in t milliseconds
    setTimeout(() => {
        readDataZ(characteristic)
    }, 10);
}

//
// hosting a web-based front-end and respond requests with sensor data
// based on example code on https://expressjs.com/
//
const express = require('express')
const app = express()
const port = 3000

app.set('view engine', 'ejs');

app.get('/', (req, res) => {
    res.render('index')
})

app.post('/', (req, res) => {
    res.writeHead(200, {
        'Content-Type': 'application/json'
    });
    res.end(JSON.stringify({
        x : x,
        y : y,
        z : z
    }))
})

app.listen(port, () => {
    console.log(`Example app listening on port ${port}`)
})
