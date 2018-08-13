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

/* TODO : Maybe move these to the C code ? */

/*
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
    }
    return res;
}
*/
var sensor_handler = function(methods) {
    if(methods == coap.code.GET) {
        print("I should be there !")
        return new Error();
    }
    print("Methods are " + methods);
    //var temperature = saul.get_by_name("temperature").read();
    //var pressure = saul.get_by_name("pressure").read();
    var sensors = {
        Temp: 2,
        Pres: 3
    };
    var response = {
        reply: JSON.stringify(sensors),
        code: coap.code.CONTENT,
        format: coap.format.TEXT
    };

    return response;
};

/* TODO: Renvoyer l'object contenant la réponse : reply, code */

coap.register_handler("/riot/js", coap.method.GET, sensor_handler);
