const express = require("express");
const app = express();
var CryptoJS = require("crypto-js");
var cors = require('cors')
app.use(cors())

app.use(express.json());

var data = "IoT Temperature and Humidity data sensor";


app.get('/', (req, res) => {
    res.send({
        "data": data
    });
})


app.post('/', (request, respond) => {
    var text = request.body;
    //result = base64.decode(text)
    var esp8266_msg = text.data;
    var esp8266_iv = text.iv;

    // The AES encryption/decryption key to be used.
    var AESKey = '2B7E151628AED2A6ABF7158809CF4F3C';

    var plain_iv = new Buffer(esp8266_iv, 'base64').toString('hex');
    var iv = CryptoJS.enc.Hex.parse(plain_iv);
    var key = CryptoJS.enc.Hex.parse(AESKey);

    // Decrypt
    var bytes = CryptoJS.AES.decrypt(esp8266_msg, key, { iv: iv });
    var plaintext = bytes.toString(CryptoJS.enc.Base64);
    var decoded_b64msg = new Buffer(plaintext, 'base64').toString('ascii');
    var decoded_msg = new Buffer(decoded_b64msg, 'base64').toString('ascii');

    data = decoded_msg
    console.log(data)
    respond.sendStatus(200);
});

app.listen(5000, (req, res) => {
    console.log("start at port 5000");
})