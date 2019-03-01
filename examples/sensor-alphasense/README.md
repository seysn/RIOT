# Sensor-Alphasense

This project is meant to be used on a system with an alphasense sensor.

## Example RIOT COAP server

(From https://www.iot-lab.info/tutorials/riot-coap-m3/)

Start an experiment with two M3 nodes.

Available IPv6 /64 subnets for M3 nodes for the site *Lille* is from `2001:660:4403:0480::/64` to `2001:660:4403:04ff::/64`.

```sh
# On first shell
$ iotlab-node --update ~/gnrc_border_router.elf -l lille,m3,102
$ sudo ethos_uhcpd.py m3-102 tap10 2001:660:4403:0480::1/64

# On a second shell
$ iotlab-node --update ~/nanocoap_server.elf -l lille,m3,103

# When previous command is finished, on the first shell (ethos_uhcpd.py)
> nib neigh
fe80::1 dev #7 lladdr fa:ce:74:d8:5b:eb  STALE GC
fe80::f8ce:74ff:fed8:5beb dev #7 lladdr fa:ce:74:d8:5b:eb  STALE GC
2001:660:4403:480:1711:6b10:65fb:841a dev #6 lladdr 15:11:6b:10:65:fb:84:1a  REACHABLE REGISTERED
```

Here `2001:660:4403:480:1711:6b10:65fb:841a` is the ipv6 address of  *nanocoap_server*.

Now we can :
```sh
$ coap get coap://[2001:660:4403:480:1711:6b10:65fb:841a]/riot/board
(2.05)	iotlab-m3
```

## Sensor-alphasense firmware

Same as previous example, just replace `nanocoap_server.elf` with `sensor_alphasense.elf`.

# Notes

## Send JSON to alphasense api

Right now, JerryScript doesn't support the XMLHttpRequest object as any other would do.
So it's pretty difficult to send a GET HTTP Request because of that.
The first solution would be to use the [iot.js](http://iotjs.net/) that has a http object.
But I haven't figured out how to use this library with RIOT.

Creating a module that implements the http request using TCP socket would be a good solution, but it seems there is no implementations of TCP sockets at the moment as we can see on [this issue](https://github.com/RIOT-OS/RIOT/issues/10664).

The best solution would be to use [gcoap](https://riot-os.org/api/group__net__gcoap.html) as it is supported by SensorThings API and the [JS module already exists](../../sys/js/coap.c).

## Undefined behaviour : Choosing nodes and sub-networks on IoT-Lab

I've figured out that sometimes, [gnrc_border_router](../gnrc_border_router) doesn't link with the other node.
This undefined behaviour seems to be related about the nodes and/or sub-networks you're choosing.
I don't know exactly what's the problem, but I am pretty sure it's from their end.

## SensorThings API


The object `Observation` is used as a result you send everytime you want to save a value in the database, here's a example :

```
{
  "phenomenonTime": "2017-02-07T18:02:00.000Z",
  "resultTime" : "2017-02-07T18:02:05.000Z",
  "result" : 137,
  "Datastream":{"@iot.id":1}
}
```

But the SensorThings API Database needs to be initialized before sending data.
Here are a list of the things you need to initialize with an exemple associated.
You can find more details on [the documentation](https://developers.sensorup.com/docs/).

### Things

```
{
  "name": "Compteur de particules",
  "description": "Capteur de pollution",
  "encodingType": "application/vnd.geo+json",
  "location": {
    "type": "Point",
    "coordinates": [-114.133, 51.08]
  }
}
```

### ObservedProperty

```
{
  "name": "Particules",
  "description": "Atmospheric particulate matter – also known as particulate matter (PM) or particulates – are microscopic solid or liquid matter suspended in the Earth's atmosphere.",
  "definition": "http://dbpedia.org/page/Particulates"
}
```

### Sensor

```
{
  "encodingType": "http://schema.org/description",
  "metadata": "compteur de particules",
  "description": "compteur de particules"
}
```

### Datastream

```
{
  "name": "Particules",
  "description": "Atmospheric particulate matter – also known as particulate matter (PM) or particulates – are microscopic solid or liquid matter suspended in the Earth's atmosphere.",
  "observationType": "http://www.opengis.net/def/observationType/OGC-OM/2.0/OM_Measurement",
  "unitOfMeasurement": {
    "name": "Particules",
    "symbol": "PM"
  },
  "Thing":{"@iot.id":1},
  "ObservedProperty":{"@iot.id":1},
  "Sensor":{"@iot.id":1}
}
```

# Links
[Previous project](https://github.com/Lydrin/RIOT/tree/anthony/examples/anthony)
[Old middleware](https://github.com/Lydrin/RIOT/tree/anthony/examples/middleware)
