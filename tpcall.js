
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


        // parse FML to a more readable form

        response = {};

        metaValues = _.pick(data, metaValueFields);

        _.forOwn(metaValues, function(value, key) {
            response[key] = value[0];
        });

        metaArrays = _.pick(data, metaArrayFields);

        _.forOwn(metaArrays, function(value, key) {
            response[key] = value;
        });

        rowArrays = _.omit(data, metaValueFields.concat(metaArrayFields));


        // no rows if UI_ITEMS_TOT is zero

        if (!response.UI_ITEMS_TOT) {

            response.rows = [];

            callback(err, response);
            return;
        }

        rows = [];

        rowCount = (_.values(rowArrays)[0] || []).length;

        for (i = 0; i < rowCount; i++) {

            var item = {};

            _.forOwn(rowArrays, function(value, key) {
                item[key] = value[i];
            });

            rows.push(item);
        }

        response.rows = rows;

        callback(null, response);
    });
}
