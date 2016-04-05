
var client = require('bindings')('tpcall'),
    _ = require('lodash');

module.exports = function tpcall(options, callback) {

    var metaValueFields,
        metaArrayFields,
        serviceName,
        fml;

    metaValueFields = options.metaValueFields || [];
    metaArrayFields = options.metaArrayFields || [];
    serviceName = options.serviceName;
    fml = options.fml;

    client.tpcall(serviceName, fml, function(err, data) {

        var metaArrays,
            metaValues,
            errorData,
            rowArrays,
            rowCount,
            response,
            errorObj,
            rows,
            i;

        if (err) {

            errorData = err.message.match(/.*{(.*)}.*{(.*)}.*{(.*)}.*{(.*)}/);

            if (errorData) {

                errorData.splice(0, 1);

                errorObj = {

                    comment: errorData[0],
                    type: errorData[1],
                    code: errorData[2],
                    message: errorData[3],
                };
            }
            else {

                errorObj = {

                    comment: 'unexpected error',
                    type: 'I',
                    code: 'UIE',
                    message: err.message
                };
            }

            callback(errorObj);
            return;
        }


        // Return the UBF data.

        response = data;

        callback(null, response);
    });
}
