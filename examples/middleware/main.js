/*
 * Copyright (C) 2018 Inria
 *               2017 Kaspar Schleiser <kaspar@schleiser.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

var state = 0;

print("IoT-Lab-M3 RIOT javascript demo");

print("Debut test hoisting : appel de la fonction en brut");

fonction(coap.method.GET);

print("Appel de coap.register_handler :");

coap.register_handler("/riot/test", coap.method.GET | coap.method.PUT, fonction);

function fonction(methods){
    if(methods === coap.method.GET){
        print("C'est une méthode GET");
    }

    return true;
}
