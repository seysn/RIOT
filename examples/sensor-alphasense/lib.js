print("Registering Javascript function to coap");

coap.method = {
    GET     : 0x1,
    POST    : 0x2,
    PUT     : 0x4
};

coap.code = {
    CREATED : ((2<<5) | 1),
    DELETED : ((2<<5) | 2),
    VALID   : ((2<<5) | 3),
    CHANGED : ((2<<5) | 4),
    CONTENT : ((2<<5) | 5)
};

coap.format = {
    TEXT : 0,
    LINK : 40,
    OCTET : 42,
    //JSON : 50,
    CBOR : 60,
    NONE : 65535
};

saul.get_by_name = function (name) {
    var res;
    switch (name) {
    case "led":
        res = saul._find_name("LED(red)");
        break;
    case "pressure":
        res = saul._find_name("lps331ap");
        break;
    case "temperature":
        res = saul._get_one(0x82);
        break;
    case "accelerometer":
        res = saul._find_name("lsm303dlhc");
        break;
    case "magnetometer":
        res = saul._get_one(0x86);
        break;
    case "brightness":
        res = saul._find_name("isl29020");
        break;
    case "gyrometer":
        res = saul._find_name("l3g4200d");
        break;
    case "OPC-R1":
    case "particulates":
        res = saul._find_name("opc-r1");
    case "OPC-N3":
        res = saul._find_name("opc-n3");
        break;
    }
    return res;
};

// SensorThings : Database address
var addr = "127.0.0.1";

// SensorThings : Datasteam ID
var datastream = 1;

// Interval of update (in seconds)
var interval = 60;

// Function that get particulates value
// Returns JSON that fit in a SensorThings API
var get_particulates = function(name) {
    var particulates = saul.get_by_name(name).read();

    // This is an Observation object for Sensorthings API.
    // Before you use it, be sure that the datastream is correctly
    // created and the datastream ID is correctly set.
    var value = {
        "phenomenonTime": new Date().toISOString(),
        "resultTime" : new Date().toISOString(),
        "result" : particulates,
        "Datastream": {
            "@iot.id": datastream
        }
    };

    return value;
};

// COAP Handler that return particulates value
// Doesn't push anything in a database
// Returns JSON that fit in a SensorThings API
var sensor_handler = function(methods) {
    if(methods != coap.method.GET) {
        return new Error();
    }

    print("Methods are " + methods);

    var sensor = get_particulates("particulates");

    var response = {
        reply: JSON.stringify(sensors),
        code: coap.code.CONTENT,
        format: coap.format.TEXT
    };

    return response;
};

// TODO : needs to be tested after setting a SensorThings API
//var sensor_routine = function() {
//    var value = get_particulates("particulates");

//    // see coap_request in ../../sys/js/coap.c
//    coap.request_sync(addr, 0, JSON.stringify(value));
//};
//setInterval(sensor_handler, interval * 1000);


// TODO : Data needs to be sent to a Sensorthings API instead of returned
coap.register_handler("/riot/js", coap.method.GET, sensor_handler);

// Modify the Datastream value
var setDatastream = function(value) {
    datastream = value;
};
coap.register_handler("/riot/datastream", coap.method.PUT, setDatastream);
