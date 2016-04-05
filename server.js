
var bodyParser = require('body-parser'),
    express = require('express'),
    config = require('./config'),
    tpcall = require('./tpcall');

var app = express();

app.use(bodyParser.json());

app.get('/tpcall', function(req, res) {

    tpcall({serviceName:"UNIXINFO",fml:{}}, function(err, data) {

        if (err) {

            res.status(500).json(err);
            return;
        }

        res.json(data);
    });
});

app.get('/ping', function(req, res) {
    res.send('PONG');
});

app.listen(config.port, function() {
    console.log("listening on port " + config.port);
});
