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

# Links
[Previous project](https://github.com/Lydrin/RIOT/tree/anthony/examples/anthony)
[Old middleware](https://github.com/Lydrin/RIOT/tree/anthony/examples/middleware)
