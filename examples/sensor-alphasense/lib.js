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

var cpt = 0;
var sensor_handler = function(methods) {
    if(methods != coap.method.GET) {
        return new Error();
    }

    print("Methods are " + methods);
    var particulates = saul.get_by_name("particulates").read();
    var sensors = {
        Idx: cpt++,
        Part: particulates
    };
    var response = {
        reply: JSON.stringify(sensors),
        code: coap.code.CONTENT,
        format: coap.format.TEXT
    };

    return response;
};


coap.register_handler("/riot/js", coap.method.GET, sensor_handler);
